#include "navigationwaveform.h"

namespace fssp {

NavigationWaveform::NavigationWaveform(std::shared_ptr<SignalData> signalData,
                                       int number, QWidget *parent)
    : BaseWaveform{signalData, number, 200, 80, parent} {
  m_isVisible = false;

  m_leftX = 0;
  m_rightX = p_minWidth;

  connect(p_signalData.get(), &SignalData::changedGraphTimeRange, this,
          &NavigationWaveform::onChangedGraphTimeRange);

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

void NavigationWaveform::onChangedGraphTimeRange() {
  m_isSelected = true;

  long double timePerPixel =
      p_timeRange / (p_width - (p_offsetLeft + p_paddingLeft + p_offsetRight +
                                p_paddingRight));

  m_leftX = p_signalData->leftTime() / timePerPixel;
  m_rightX = p_signalData->rightTime() / timePerPixel;

  m_selectionRect =
      QRect(QPoint(m_leftX, 0), QPoint(m_rightX, height() - p_maxTextHeight));

  update();
}

void NavigationWaveform::paintEvent(QPaintEvent *event) {
  QLabel::paintEvent(event);
  if (!m_isSelected) return;

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::red);
  painter.setBrush(QColor(255, 0, 0, 100));
  painter.drawRect(m_selectionRect);
}

}  // namespace fssp
