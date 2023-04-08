#pragma once

#include <QDate>
#include <QDateTime>
#include <QFile>
#include <QTime>
#include <QVector>

class FileReader {
public:
	QDate start_date; // дата начала.
	QTime start_time, durance; // время начала, продолжительность.
	QDateTime start;
	QDateTime end; // дата + время начала, конца.

	QString file_name, s_date, s_time, str,
			tmp = ""; // вспомогательные переменные.
	unsigned long long channels_num,
			data_num; // число каналов, количество записей.
	int year = -1, month = -1, day = -1; // Для создания даты.
	int hour = -1, minute = -1, sec = -1, msec = -1; // Для создания времени.
	double rate, time_for_r, all_time; // частота, время для одной записи(сек),
									   // полное время работы(сек).
	QVector<QVector<double>> data; // вектор значений всех каналов.

	QVector<QString> channels_names; // имена каналов.

	QFile file;

	FileReader();
	FileReader(QString fileName);

	void open();
	void close();
	void read();
	bool checkTXT(QString str);
};
