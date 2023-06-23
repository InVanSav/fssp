#include "txtdeserializer.h"

namespace fssp {

SignalData TxtDeserializer::operator()(const QString &absoluteFilePath) {
  QFile file;
  file.setFileName(absoluteFilePath);

  file.open(QIODevice::ReadOnly | QIODevice::Text);

  // дата начала.
  QDate start_date;
  // время начала
  QTime start_time;
  // продолжительность по времени в последний день измерений
  QTime dur;
  // дата + время начала
  QDateTime start;
  // дата + время конца
  QDateTime end;
  // длительность в днях
  qint64 dur_days = 0;
  // миллисекунды в дне
  qint64 MILLIS_IN_DAY = 86400000;

  // вспомогательные переменные.
  QString s_date;
  QString s_time;
  QString str;
  QString tmp = "";

  // число каналов
  qint64 channels_num;
  // количество записей
  qint64 data_num;
  // частота, время для одной записи(сек), полное время работы(сек).
  double rate, time_for_r, all_time;
  // вектор значений всех каналов.
  std::vector<std::vector<double>> data;

  // имена каналов.
  std::vector<QString> channels_names;

  // Для создания даты.
  int year = -1, month = -1, day = -1;
  // Для создания времени.
  int hour = -1, minute = -1, sec = -1, msec = 0;

  QTextStream in(&file);
  size_t i = 0;

  str = in.readLine();
  str = in.readLine();

  channels_num = str.toULongLong();

  str = in.readLine();
  str = in.readLine();
  data_num = str.toULongLong();

  data = std::vector<std::vector<double>>(channels_num,
                                          std::vector<double>(data_num));

  str = in.readLine();
  str = in.readLine();
  rate = str.toDouble();
  time_for_r = 1. / rate;
  all_time = time_for_r * data_num;

  str = in.readLine();
  str = in.readLine();
  while (i < str.size()) {
    if (str[i] != '-') {
      tmp += str[i];
    } else if (day == -1) {
      day = tmp.toInt();
      tmp = "";
    } else if (month == -1) {
      month = tmp.toInt();
      tmp = "";
    }
    s_date.append(str[i++]);
  }
  year = tmp.toInt();
  tmp = "";
  start_date = QDate(year, month, day);

  i = 0;

  str = in.readLine();
  str = in.readLine();
  while (i < str.size()) {
    if (str[i] != ':' && str[i] != '.') {
      tmp += str[i];
    } else if (hour == -1) {
      hour = tmp.toInt();
      tmp = "";
    } else if (minute == -1) {
      minute = tmp.toInt();
      tmp = "";
    } else if (sec == -1) {
      sec = tmp.toInt();
      tmp = "";
    }
    s_time.append(str[i++]);
  }
  if (sec == -1) {
    sec = tmp.toInt();
  } else {
    msec = tmp.toInt();
  }
  start_time = QTime(hour, minute, sec, msec);

  start = QDateTime(start_date, start_time);
  end = start.addMSecs((qint64)(all_time * 1000));
  dur = QTime(0, 0, 0);
  dur = dur.addMSecs((end - start).count() % MILLIS_IN_DAY);
  dur_days = ((end - start).count() / MILLIS_IN_DAY);

  str = in.readLine();
  str = in.readLine();
  QVector<QString> tmp_names = str.split(";");

  for (size_t i = 0; i < tmp_names.size(); ++i) {
    channels_names.push_back(tmp_names[i]);
  }

  if (channels_names.size() == channels_num) {
    QString tmp = "";
    for (size_t j = 0; j < channels_names[channels_num - 1].size(); ++j) {
      if (channels_names[channels_num - 1][j] != '\r' &&
          channels_names[channels_num - 1][j] != '\n')
        tmp += channels_names[channels_num - 1][j];
      else {
        break;
      }
    }
    channels_names[channels_num - 1] = tmp;
  }

  if (channels_names.size() > channels_num) {
    channels_names.pop_back();
  }

  for (size_t k = 0; k < data_num; ++k) {
    str = in.readLine();
    QStringList lineData = str.split(" ");

    for (size_t j = 0; j < channels_num; ++j) {
      data[j][k] = lineData[j].toDouble();
    }
  }

  file.close();

  return SignalData(start, end, rate, time_for_r, all_time,
                    std::move(channels_names), std::move(data));
}

}  // namespace fssp
