#include "navigationwaveform.h"

#include "utils.h"

namespace fssp {

NavigationWaveform::NavigationWaveform(const int number, QWidget *parent)
    : QLabel{parent} {
  m_number = number;
}

void NavigationWaveform::drawWaveform(const std::vector<double> &data,
                                      int leftSelection, int rightSelection) {
  QImage image(m_WIDTH, m_HEIGHT, QImage::Format_ARGB32);
  image.fill(qRgb(255, 255, 255));
  //  bresenhamDraw(data, image, m_WIDTH, m_HEIGHT, 0, 0, 0, 0, qRgb(0, 0, 0));
  setPixmap(QPixmap::fromImage(image));
}

}  // namespace fssp
