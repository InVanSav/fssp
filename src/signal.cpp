#include "signal.h"

#include "customlabel.h"
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QVBoxLayout>

Signal::Signal(QString fileName, QWidget *parent) :
		QWidget{ parent } {
	layout = new QVBoxLayout(this);

	reader = new FileReader(fileName);
	reader->read();

	// Создаем изображение формата чб, красим в белый,
	// вызываем отрисовку осциллограммы поканально
	for (int i = 0; i < reader->data.size(); ++i) {
		QImage image(WIDTH, HEIGHT, QImage::Format_Mono);
		image.fill(1);
		drawAxes(reader->data[i], image);
		images.push_back(image);
	}

	for (int i = 0; i < images.size(); ++i) {
		QLabel *image = new CustomLabel();
		image->setPixmap(QPixmap::fromImage(images[i]));
		layout->addWidget(image);
		QLabel *label = new CustomLabel();
		label->setText(reader->channels_names[i]);
		layout->addWidget(label);
		layout->setAlignment(image, Qt::AlignCenter);
		layout->setAlignment(label, Qt::AlignCenter);
	}

	setWindowTitle(tr("Signals"));
}

void Signal::drawWaveform(const QVector<double> &channel, QImage &image) {
	int numSamples = channel.size();
	double minVal = *std::min_element(channel.begin(), channel.end());
	double maxVal = *std::max_element(channel.begin(), channel.end());
	double range = maxVal - minVal;
	double scale = HEIGHT / range;

	for (int i = 0; i < numSamples - 1; ++i) {
		int x1 = round(i * WIDTH / numSamples);
		int x2 = round((i + 1) * WIDTH / numSamples);
		int y1 = HEIGHT - round((channel[i] - minVal) * scale);
		int y2 = HEIGHT - round((channel[i + 1] - minVal) * scale);

		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);
		int sx = (x1 < x2) ? 1 : -1;
		int sy = (y1 < y2) ? 1 : -1;
		int err = dx - dy;

		while (true) {
			int x = x1;
			int y = HEIGHT - y1;

			int byteOffset = y * image.bytesPerLine() + (x / 8);
			int bitIndex = 7 - (x % 8);

			uchar *pixel = image.bits() + byteOffset;
			*pixel &= ~(1 << bitIndex);

			if (x == x2 && y == (HEIGHT - y2)) {
				break;
			}

			int err2 = 2 * err;

			if (err2 > -dy) {
				err -= dy;
				x1 += sx;
			}

			if (err2 < dx) {
				err += dx;
				y1 += sy;
			}
		}
	}
}

//void Signal::drawAxes(const QVector<double> &channel, QImage &image) {
//	const int width = image.width();
//	const int height = image.height();
//	uchar *pixels = image.bits();
//	//	memset(pixels, 0, width * height / 8);

//	// Определяем минимальное и максимальное значение по оси y
//	double minVal = *std::min_element(channel.begin(), channel.end());
//	double maxVal = *std::max_element(channel.begin(), channel.end());

//	// Определяем оптимальные значения для меток по оси y
//	const int numLabelsY = 6;
//	double rangeY = maxVal - minVal;
//	double labelStepY = rangeY / (numLabelsY - 1);
//	QVector<double> labelValuesY(numLabelsY);

//	for (int i = 0; i < numLabelsY; ++i) {
//		labelValuesY[i] = minVal + i * labelStepY;
//	}

//	// Определяем оптимальные значения для меток по оси x
//	const int numLabelsX = 6;
//	double labelStepX = reader->all_time / (numLabelsX - 1);
//	QVector<double> labelValuesX(numLabelsX);

//	for (int i = 0; i < numLabelsX; ++i) {
//		labelValuesX[i] = i * labelStepX;
//	}

//	// Рисуем оси координат
//	const int axisXStart = 70;
//	const int axisYStart = 70;
//	const int axisXEnd = width - 50;
//	const int axisYEnd = height - 50;

//	drawLine(axisXStart, axisYEnd, axisXEnd, axisYEnd, pixels, width);
//	drawLine(axisXStart, axisYStart, axisXStart, axisYEnd, pixels, width);

//	// Рисуем подписи на осях координат
//	// Подпись оси x
//	drawText(axisXStart + 200, axisYEnd + 30, "Time (s)", pixels, width);

//	// Подпись оси y
//	drawRotatedText(-axisYEnd + 50, axisXStart - 60, "Values", pixels, width);

//	// Рисуем сетку
//	// Рисуем горизонтальные линии сетки
//	double stepY = (axisYEnd - axisYStart) / (numLabelsY - 1);
//	for (int i = 0; i < numLabelsY; ++i) {
//		double y = axisYEnd - i * stepY;
//		drawLine(axisXStart, y, axisXEnd, y, pixels, width);
//	}

//	// Рисуем вертикальные линии сетки
//	double stepX = (axisXEnd - axisXStart) / (numLabelsX - 1);
//	for (int i = 0; i < numLabelsX; ++i) {
//		double x = axisXStart + i * stepX;
//		drawLine(x, axisYStart, x, axisYEnd, pixels, width);
//	}

//	// Рисуем метки на осях координат
//	// Метки на оси y
//	for (int i = 0; i < numLabelsY; ++i) {
//		double y = axisYEnd - i * stepY;
//		drawText(axisXStart - 50, y - 5, QString::number(labelValuesY[i]), pixels, width);
//	}

//	// Метки на оси x
//	for (int i = 0; i < numLabelsX; ++i) {
//		double x = axisXStart + i * stepX;
//		double label = labelValuesX[i];
//		QString labelText;
//		if (label >= 1000) {
//			labelText = QString::number(label / 1000) + "k";
//		} else {
//			labelText = QString::number(label);
//		}
//		drawText(x - 25, axisYEnd + 20, labelText, pixels, width);
//	}

//	// Рисуем график сигнала
//	int scaleY = (axisYEnd - axisYStart) / rangeY;
//	int scaleX = (axisXEnd - axisXStart) / reader->all_time;
//	int numPoints = channel.size();
//	for (int i = 0; i < numPoints - 1; ++i) {
//		int x1 = i * scaleX + axisXStart;
//		int y1 = (channel[i] - minVal) * scaleY + axisYStart;
//		int x2 = (i + 1) * scaleX + axisXStart;
//		int y2 = (channel[i + 1] - minVal) * scaleY + axisYStart;
//		drawLine(x1, y1, x2, y2, pixels, width);
//	}
//}

//void Signal::drawLine(int x1, int y1, int x2, int y2, uchar *pixels, int width) {
//	int dx = abs(x2 - x1);
//	int dy = abs(y2 - y1);
//	int sx = x1 < x2 ? 1 : -1;
//	int sy = y1 < y2 ? 1 : -1;
//	int err = dx - dy;

//	while (true) {
//		int index = (y1 * width + x1) / 8;
//		int bit = (y1 * width + x1) % 8;
//		pixels[index] |= (1 << bit);
//		qInfo() << "index: " << index << " " << pixels[index];

//		if (x1 == x2 && y1 == y2) {
//			break;
//		}

//		int err2 = 2 * err;

//		if (err2 > -dy) {
//			err -= dy;
//			x1 += sx;
//		}

//		if (err2 < dx) {
//			err += dx;
//			y1 += sy;
//		}
//	}
//}

//void Signal::drawText(int x, int y, const QString &text, uchar *pixels, int width) {
//	QPainter painter;
//	painter.begin(reinterpret_cast<QImage *>(pixels));

//	// Устанавливаем шрифт и цвет
//	QFont font;
//	font.setPointSize(10);
//	painter.setFont(font);
//	painter.setPen(Qt::white);

//	// Рисуем текст
//	painter.drawText(QPointF(x, y), text);

//	painter.end();
//}

//void Signal::drawRotatedText(int x, int y, const QString &text, uchar *pixels, int width) {
//	QPainter painter;
//	painter.begin(reinterpret_cast<QImage *>(pixels));

//	// Устанавливаем шрифт, цвет и угол поворота
//	QFont font;
//	font.setPointSize(10);
//	painter.setFont(font);
//	painter.setPen(Qt::white);
//	painter.rotate(-90);

//	// Рисуем текст
//	painter.drawText(QPointF(x, y), text);

//	painter.end();
//}

void Signal::drawAxes(const QVector<double> &channel, QImage &image) {
	QPainter painter(&image);
	painter.setPen(Qt::black);

	// Определяем минимальное и максимальное значение по оси y
	double minVal = *std::min_element(channel.begin(), channel.end());
	double maxVal = *std::max_element(channel.begin(), channel.end());

	// Определяем оптимальные значения для меток по оси y
	const int numLabelsY = 6;
	double rangeY = maxVal - minVal;
	double labelStepY = rangeY / (numLabelsY - 1);
	QVector<double> labelValuesY(numLabelsY);

	for (int i = 0; i < numLabelsY; ++i) {
		labelValuesY[i] = minVal + i * labelStepY;
	}

	// Определяем оптимальные значения для меток по оси x
	const int numLabelsX = 6;
	double labelStepX = reader->all_time / (numLabelsX - 1);
	QVector<double> labelValuesX(numLabelsX);

	for (int i = 0; i < numLabelsX; ++i) {
		labelValuesX[i] = i * labelStepX;
	}

	// Рисуем оси координат
	const int axisXStart = 70;
	const int axisYStart = 70;
	const int axisXEnd = image.width() - 50;
	const int axisYEnd = image.height() - 50;

	painter.drawLine(QPointF(axisXStart, axisYEnd), QPointF(axisXEnd, axisYEnd));
	painter.drawLine(QPointF(axisXStart, axisYStart), QPointF(axisXStart, axisYEnd));

	// Рисуем подписи на осях координат
	QFont font("Times New Roman", 10);
	painter.setFont(font);

	// Подпись оси x
	QString xLabel = "Time (s)";
	QRectF xLabelRect = QRectF(axisXStart + 200, axisYEnd + 30, 50, 20);
	painter.drawText(xLabelRect, Qt::AlignCenter, xLabel);

	// Подпись оси y
	QString yLabel = "Values";
	QRectF yLabelRect = QRectF(-axisYEnd + 50, axisXStart - 60, 50, 20);
	painter.save();
	painter.rotate(-90);
	painter.drawText(yLabelRect, Qt::AlignCenter, yLabel);
	painter.restore();

	// Рисуем сетку
	QPen gridPen(Qt::black, 1, Qt::DotLine);
	painter.setPen(gridPen);

	// Рисуем горизонтальные линии сетки
	double stepY = (axisYEnd - axisYStart) / (numLabelsY - 1);
	for (int i = 0; i < numLabelsY; ++i) {
		double y = axisYEnd - i * stepY;
		painter.drawLine(QPointF(axisXStart, y), QPointF(axisXEnd, y));
	}

	// Рисуем вертикальные линии сетки
	double stepX = (axisXEnd - axisXStart) / (numLabelsX - 1);
	for (int i = 0; i < numLabelsX; ++i) {
		double x = axisXStart + i * stepX;
		painter.drawLine(QPointF(x, axisYStart), QPointF(x, axisYEnd));
	}

	// Рисуем метки на осях координат
	painter.setPen(Qt::black);
	for (int i = 0; i < numLabelsY; ++i) {
		double y = axisYEnd - i * stepY;
		QString label;
		if (labelValuesY[i] < 10) {
			label = QString::number(labelValuesY[i], 'f', 2);
		} else {
			label = QString::number(labelValuesY[i], 'f', 0);
		}
		QRectF labelRect = QRectF(axisXStart - 60, y - 10, 50, 20);
		painter.drawText(labelRect, Qt::AlignRight | Qt::AlignVCenter, label);
	}

	for (int i = 0; i < numLabelsX; ++i) {
		double x = axisXStart + i * stepX;
		QString label;
		if (labelValuesY[i] < 10) {
			label = QString::number(labelValuesX[i], 'f', 2);
		} else {
			label = QString::number(labelValuesX[i], 'f', 0);
		}
		QRectF labelRect = QRectF(x - 25, axisYEnd + 10, 50, 20);
		painter.drawText(labelRect, Qt::AlignHCenter | Qt::AlignTop, label);
	}

	// Рисуем график
	double scaleX = (axisXEnd - axisXStart) / reader->all_time;
	double scaleY = (axisYEnd - axisYStart) / rangeY;

	QPen graphPen(QColor(0, 127, 255), 1.5);
	painter.setPen(graphPen);

	QPointF lastPoint;
	for (int i = 0; i < channel.size(); ++i) {
		double x = axisXStart + i / reader->rate * scaleX;
		double y = axisYEnd - (channel[i] - minVal) * scaleY;

		QPointF point(x, y);
		if (i == 0) {
			lastPoint = point;
			continue;
		}

		painter.drawLine(lastPoint, point);
		lastPoint = point;
	}
}
