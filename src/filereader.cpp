#include "filereader.h"

FileReader::FileReader() {}

FileReader::FileReader(QString fileName) { file.setFileName(fileName); }

void FileReader::close() { file.close(); }

void FileReader::open() {
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "Ошибка при открытии файла";
	}
}

void FileReader::read() {
	QTextStream ts(stdout);
	this->open();
	// if (this->checkTXT(file_name)) {
	size_t i = 0;

	str = file.readLine();
	str = file.readLine();
	channels_num = str.toULongLong();
	ts << channels_num << "\n";

	str = file.readLine();
	str = file.readLine();
	data_num = str.toULongLong();
	ts << data_num << "\n";

	data = QVector<QVector<double>>(channels_num, QVector<double>(data_num));

	str = file.readLine();
	str = file.readLine();
	rate = str.toDouble();
	time_for_r = 1. / rate;
	all_time = time_for_r * data_num;
	ts << rate << "\n";
	ts << time_for_r << "\n";
	ts << all_time << "\n";

	str = file.readLine();
	str = file.readLine();
	while (str[i] != '\r') {
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
	ts << s_date << "\n";

	str = file.readLine();
	str = file.readLine();
	while (str[i] != '\r') {
		if (str[i] != ':') {
			tmp += str[i];
		} else if (hour == -1) {
			hour = tmp.toInt();
			tmp = "";
		} else if (minute == -1) {
			minute = tmp.toInt();
			tmp = "";
		} else if (str[i] == '.') {
			sec = tmp.toInt();
			tmp = "";
		}
		s_time.append(str[i++]);
	}
	if (sec == -1) {
		sec = tmp.toInt();
		start_time = QTime(hour, minute, sec);
	} else {
		msec = tmp.toInt();
		start_time = QTime(hour, minute, sec, msec);
	}

	i = 0;
	ts << start_time.toString() << "\n";
	ts << s_time << "\n";

	start = QDateTime(start_date, start_time);
	end = start.addMSecs((qint64)(all_time * 1000));
	durance = QTime(0, 0);
	durance = durance.addMSecs((end - start).count());
	ts << start.toString() << "\n";
	ts << end.toString() << "\n";
	ts << durance.toString() << "\n";

	str = file.readLine();
	str = file.readLine();
	channels_names = str.split(";");

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

	for (size_t j = 0; j < channels_names.size(); ++j) {
		ts << channels_names[j] << "\n";
	}

	for (size_t k = 0; k < data_num; ++k) {
		str = file.readLine();
		QStringList lineData = str.split(" ");

		for (size_t j = 0; j < channels_num; ++j) {
			data[j][k] = lineData[j].toDouble();
		}
	}
	//}
	this->close();
}

bool FileReader::checkTXT(QString str) {
	QString ::iterator itStr = str.end();

	if (*(itStr - 1) == 't' && *(itStr - 2) == 'x' && *(itStr - 3) == 't') {
		return true;
	}
	return false;
}
