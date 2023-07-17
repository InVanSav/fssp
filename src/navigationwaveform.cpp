#include "navigationwaveform.h"

namespace fssp {

NavigationWaveform::NavigationWaveform(std::shared_ptr<SignalData> signalData,
                                       int number, QWidget *parent)
    : BaseWaveform{signalData, number, 200, 80, parent} {
  m_isVisible = p_signalData->visibleWaveforms()[p_number];

  connect(p_signalData.get(), &SignalData::changedGraphTimeRange, this,
          &NavigationWaveform::onChangedGraphTimeRange);

  setTextMargin(5, 5, 3, 3);
  setOffset(0, 0, 0, p_maxTextHeight);

  p_data = p_signalData->data()[p_number];

  p_leftArray = p_signalData->leftArray();
  p_rightArray = p_signalData->rightArray();

  p_arrayRange = p_rightArray - p_leftArray + 1;

  p_minValue = *std::min_element(p_data.begin(), p_data.end());
  p_maxValue = *std::max_element(p_data.begin(), p_data.end());

  p_dataRange = std::abs(p_maxValue - p_minValue);
}

void NavigationWaveform::drawWaveform() {
  initImage();
  fill();
  drawName();
  drawBresenham();
  showWaveform();

  onChangedGraphTimeRange();
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
  p_signalData->setWaveformVisibility(p_number, visible);

  emit p_signalData->changedWaveformVisibility();
}

void NavigationWaveform::onChangedGraphTimeRange() {
  double timePerPixel =
      p_signalData->allTime() / (p_width - (p_offsetLeft + p_paddingLeft +
                                            p_offsetRight + p_paddingRight));

  double leftX = p_signalData->leftTime() / timePerPixel;
  double rightX = p_signalData->rightTime() / timePerPixel;

  m_selectionRect =
      QRect(QPoint(leftX, p_offsetTop + p_paddingTop),
            QPoint(rightX, height() - (p_offsetBottom + p_paddingBottom)));

  update();
}

void NavigationWaveform::paintEvent(QPaintEvent *event) {
  QLabel::paintEvent(event);
  if (!p_signalData->isSelected()) return;

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::red);
  painter.setBrush(QColor(255, 0, 0, 100));
  painter.drawRect(m_selectionRect);
}

void NavigationWaveform::drawName() {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  QPainter painter(&p_image);
  painter.setPen(p_mainColor);
  painter.setFont(p_font);

  QRect textRect{p_paddingLeft, p_height - (p_maxTextHeight + p_paddingBottom),
                 p_width - (p_paddingRight + p_paddingLeft), p_maxTextHeight};

  painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap,
                   p_signalData->channelsName()[p_number]);
}

}  // namespace fssp
