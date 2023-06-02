#include "signal.h"

#include <QLabel>
#include <QVBoxLayout>

#include "customlabel.h"

QVBoxLayout *Signal::layout = nullptr;

Signal::Signal(QString fileName, QWidget *parent) :
		QWidget{ parent } {
	if (layout) {
		CustomLabel::clearAllLabels(layout);
	}

	layout = new QVBoxLayout(this);

	reader = new FileReader(fileName);
	reader->read();

	// Создаем изображение формата чб, красим в белый,
	// вызываем отрисовку осциллограммы поканально
	for (int i = 0; i < reader->data.size(); ++i) {
		QImage image(WIDTH, HEIGHT, QImage::Format_Mono);
		image.fill(1);
		drawWaveform(reader->data[i], image);
		images.push_back(image);
	}

	for (int i = 0; i < images.size(); ++i) {
		CustomLabel *image = new CustomLabel(reader, i);
		image->setPixmap(QPixmap::fromImage(images[i]));
		layout->addWidget(image);
		QLabel *label = new QLabel();
		label->setText(reader->channels_names[i]);
		layout->addWidget(label);
		layout->setAlignment(image, Qt::AlignCenter);
		layout->setAlignment(label, Qt::AlignCenter);
	}

	setWindowTitle(tr("Signals"));

	setFixedSize(sizeHint());
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
