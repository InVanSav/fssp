#include "navigationwaveform.h"

namespace fssp {

NavigationWaveform::NavigationWaveform(const int number, QWidget *parent)
    : QLabel{parent} {
  m_number = number;
}

void NavigationWaveform::drawWaveform(const std::vector<double> &data,
                                      int width, int height, bool isSelected,
                                      int leftSelection, int rightSelection) {
  QImage image(width, height, QImage::Format_ARGB32);
  image.fill(qRgb(255, 255, 255));
  bresenhamDraw(data, 0, data.size() - 1, image, width, height, 0, 0, 0, 0,
                qRgb(0, 127, 255));

  QPixmap pixmap = QPixmap::fromImage(image);

  if (!isSelected) {
    setPixmap(pixmap);
    return;
  }

  int leftBorder = (leftSelection * width) / data.size();
  int rightBorder = (rightSelection * width) / data.size();

  QPainter *painter = new QPainter(&pixmap);
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(Qt::red);
  painter->setBrush(QColor{255, 0, 0, 100});
  painter->drawRect(QRect{QPoint{leftBorder, 0}, QPoint{rightBorder, height}});

  setPixmap(pixmap);
}

int NavigationWaveform::number() const { return m_number; }

}  // namespace fssp
