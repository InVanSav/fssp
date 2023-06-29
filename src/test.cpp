#include "test.h"

namespace fssp {

test::test(std::shared_ptr<SignalData> signalData, int number, QWidget *parent)
    : BaseWaveform{signalData, number, parent} {}

void test::drawWaveform() {
  p_image = QImage(p_width, p_height, p_imageFormat);
  fill(QColor{255, 255, 255});
  drawAxes(BaseWaveform::AxisType::AxisYTop);
  QPixmap pixmap = QPixmap::fromImage(p_image);
  setPixmap(pixmap);
}

}  // namespace fssp
