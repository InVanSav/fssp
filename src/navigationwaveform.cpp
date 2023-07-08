#include "navigationwaveform.h"

namespace fssp {

NavigationWaveform::NavigationWaveform(std::shared_ptr<SignalData> signalData,
                                       int number, QWidget *parent)
    : BaseWaveform{signalData, number, 200, 80, parent} {
  m_isVisible = false;

  setTextMargin(5, 5, 3, 3);
  setOffset(0, 0, 0, p_maxTextHeight);
  updateRelative();
}

void NavigationWaveform::drawWaveform() {
  initImage();
  fill(QColor{255, 255, 255});
  drawName(BaseWaveform::NameType::HorizontalBottom);
  drawBresenham();
  showWaveform();
}

void NavigationWaveform::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    showContextMenu(event->pos());
  }

  QLabel::mousePressEvent(event);
}

void NavigationWaveform::showContextMenu(const QPoint &pos) {
  QMenu menu(this);
  QAction *waveformAction = menu.addAction("Осциллограмма");
  waveformAction->setCheckable(true);
  waveformAction->setChecked(m_isVisible);

  connect(waveformAction, &QAction::triggered, this,
          &NavigationWaveform::changeVisibilityAction);

  menu.exec(mapToGlobal(pos));
}

void NavigationWaveform::changeVisibilityAction(bool visible) {
  m_isVisible = visible;
  p_signalData->setWaveformVisibility(number(), visible);

  emit p_signalData->changedWaveformVisibility();
}

}  // namespace fssp
