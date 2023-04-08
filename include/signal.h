#pragma once

#include <QVBoxLayout>
#include <QWidget>

#include "filereader.h"

class Signal : public QWidget {
	Q_OBJECT
public:
	explicit Signal(QString fileName, QWidget *parent = nullptr);

	void drawWaveform(const QVector<double> &channel, QImage &image);

	FileReader *reader;

	QVector<QImage> images;
	const int WIDTH = 1488, HEIGHT = 228;
	QVBoxLayout *layout;
};
