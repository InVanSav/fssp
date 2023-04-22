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
		drawAxes(image, reader->data[i]);
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

void Signal::drawAxes(QImage &image, const QVector<double> &channel) {
	QPainter painter(&image);
	painter.setPen(Qt::black);

	// Определяем минимальное и максимальное значение по оси y
	double minVal = *std::min_element(channel.begin(), channel.end());
	double maxVal = *std::max_element(channel.begin(), channel.end());

	// Определяем оптимальные значения для меток по оси y
	int numLabelsY = 6;
	double rangeY = maxVal - minVal;
	double labelStepY = rangeY / numLabelsY;
	QVector<double> labelValuesY(numLabelsY);

	for (int i = 0; i < numLabelsY; i++) {
		labelValuesY[i] = minVal + i * labelStepY;
	}

	// Определяем оптимальные значения для меток по оси x
	int numLabelsX = 6;
	double duration = channel.size() / reader->rate;
	double labelStepX = duration / numLabelsX;
	QVector<double> labelValuesX(numLabelsX);

	for (int i = 0; i < numLabelsX; i++) {
		labelValuesX[i] = i * labelStepX;
	}

	// Рисуем оси координат
	int axisXStart = 70;
	int axisYStart = 70;
	int axisXEnd = image.width() - 50;
	int axisYEnd = image.height() - 50;

	painter.drawLine(QPointF(axisXStart, axisYEnd), QPointF(axisXEnd, axisYEnd));
	painter.drawLine(QPointF(axisXStart, axisYStart), QPointF(axisXStart, axisYEnd));

	// Рисуем сетку
	QPen gridPen(Qt::black, 1, Qt::DotLine);
	painter.setPen(gridPen);

	double stepY = (axisYEnd - axisYStart) / numLabelsY;
	for (int i = 1; i < numLabelsY; i++) {
		int y = axisYEnd - i * stepY;
		painter.drawLine(QPointF(axisXStart, y), QPointF(axisXEnd, y));
	}

	double stepX = (axisXEnd - axisXStart) / numLabelsX;
	for (int i = 1; i < numLabelsX; i++) {
		int x = axisXStart + i * stepX;
		painter.drawLine(QPointF(x, axisYStart), QPointF(x, axisYEnd));
	}

	painter.setPen(Qt::black);
	QFont font;
	font.setPointSize(6);
	painter.setFont(font);

	// Рисуем метки и значения по оси y
	for (int i = 0; i < numLabelsY; i++) {
		int y = axisYEnd - i * stepY;
		QString label = QString::number(labelValuesY[i], 'f', 0);
		painter.save(); // сохраняем текущую систему координат
		painter.translate(axisXStart - 10, y); // переводим начало координат в точку рисования текста
		painter.rotate(-90); // поворачиваем систему координат на 90 градусов против часовой стрелки
		painter.drawText(QPointF(0, 0), label); // рисуем текст
		painter.restore(); // восстанавливаем предыдущую систему координат
	}

	// Рисуем метки и значения по оси x
	for (int i = 0; i < numLabelsX; i++) {
		int x = axisXStart + i * stepX;
		QString label = QString::number(labelValuesX[i], 'f', 0);
		painter.drawText(QPointF(x, axisYEnd + 20), label);
	}

	// Добавляем подписи к осям
	QFont labelFont;
	labelFont.setPointSize(10);
	painter.setFont(labelFont);
	painter.drawText(QPointF((image.width() / 2) - 40, axisYEnd + 40), "Time (seconds)");
	painter.rotate(-90);
	painter.drawText(QPointF((-image.height() / 2) - 20, axisXStart - 40), "Amplitude");
	painter.rotate(90);

	painter.end();

	// Отрисовываем график осциллограммы
	double scale = (axisYEnd - axisYStart) / (maxVal - minVal);
	int numSamples = channel.size();

	for (int i = 0; i < numSamples - 1; i++) {
		int x1 = round(i * (axisXEnd - axisXStart) / numSamples) + axisXStart;
		int x2 = round((i + 1) * (axisXEnd - axisXStart) / numSamples) + axisXStart;
		int y1 = round((channel[i] - minVal) * scale) + axisYStart;
		int y2 = round((channel[i + 1] - minVal) * scale) + axisYStart;

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
