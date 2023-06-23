#include "navigationwaveform.h"

namespace fssp {

NavigationWaveform::NavigationWaveform(const int number, QWidget *parent)
    : QLabel{parent} {
  m_number = number;
}

void NavigationWaveform::drawWaveform(const std::vector<double> &data,
                                      bool isSelected, int leftSelection,
                                      int rightSelection) {
  QImage image(m_WIDTH, m_HEIGHT, QImage::Format_ARGB32);
  image.fill(qRgb(255, 255, 255));
  bresenhamDraw(data, image, m_WIDTH, m_HEIGHT, 0, 0, 0, 0, qRgb(0, 127, 255));

  QPixmap pixmap = QPixmap::fromImage(image);

  if (!isSelected) {
    setPixmap(pixmap);
    return;
  }

  double dataPerPixel = data.size() / m_WIDTH;
  int leftBorder = (leftSelection * m_WIDTH) / data.size();
  int rightBorder = (rightSelection * m_WIDTH) / data.size();

  QPainter *painter = new QPainter(&pixmap);
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(Qt::red);
  painter->setBrush(QColor{255, 0, 0, 100});
  painter->drawRect(
      QRect{QPoint{leftBorder, 0}, QPoint{rightBorder, m_HEIGHT}});

  setPixmap(pixmap);
}

}  // namespace fssp
