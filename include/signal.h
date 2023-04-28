#pragma once

#include <QVBoxLayout>
#include <QWidget>

#include "filereader.h"

class Signal : public QWidget {
	Q_OBJECT
public:
	explicit Signal(QString fileName, QWidget *parent = nullptr);

	void drawWaveform(const QVector<double> &channel, QImage &image);
	void drawAxes(const QVector<double> &channel, QImage &image);
	/*void drawLine(int x1, int y1, int x2, int y2, uchar *pixels, int width);
	void drawText(int x, int y, const QString &text, uchar *pixels, int width);
	void drawRotatedText(int x, int y, const QString &text, uchar *pixels, int width);*/

	FileReader *reader;

	QVector<QImage> images;
	const int WIDTH = 600, HEIGHT = 300;
	QVBoxLayout *layout;
};
