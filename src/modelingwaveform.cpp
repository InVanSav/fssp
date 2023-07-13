#include "modelingwaveform.h"

namespace fssp {

ModelingWaveform::ModelingWaveform(std::shared_ptr<SignalData> signalData,
                                   QWidget *parent)
    : BaseWaveform{signalData, 0, 300, 100, parent} {
  setWidth(900);
  setheight(300);
  setTextMargin(5, 5, 3, 3);
  setOffset(p_maxAxisTextWidth, 15, p_maxTextHeight + 5, 10);
  setPadding(3, 3, 3, 3);
  updateRelative();
}

void ModelingWaveform::drawWaveform() {
  initImage();
  fill(QColor{255, 255, 255});
  //  drawName(BaseWaveform::NameType::VerticalLeft);

  drawAxes(BaseWaveform::AxisType::AxisXLeft);
  drawAxes(BaseWaveform::AxisType::AxisYTop);

  drawGrid();
  drawBresenham();

  showWaveform();
}

}  // namespace fssp
