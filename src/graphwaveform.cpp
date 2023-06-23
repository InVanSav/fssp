#include "graphwaveform.h"

namespace fssp {

GraphWaveform::GraphWaveform(int number, QWidget *parent) : QLabel{parent} {
  m_number = number;
}

void GraphWaveform::drawWaveform(const std::vector<double> &data,
                                 double allTime) {
  QImage image(m_WIDTH, m_HEIGHT, QImage::Format_ARGB32);
  image.fill(qRgb(255, 255, 255));
  bresenhamDraw(data, image, m_WIDTH, m_HEIGHT, m_OFFSET_START_X,
                m_OFFSET_START_Y, m_OFFSET_END_X, m_OFFSET_END_Y,
                qRgb(0, 127, 255));

  QPixmap pixmap = QPixmap::fromImage(image);

  setPixmap(pixmap);
}

}  // namespace fssp
