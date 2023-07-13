#include "graphwaveform.h"

namespace fssp {

GraphWaveform::GraphWaveform(std::shared_ptr<SignalData> signalData, int number,
                             QWidget *parent)
    : BaseWaveform{signalData, number, 300, 100, parent} {
  m_isTop = false;
  m_isBottom = false;

  m_isCtrlPressed = false;

  setWidth(800);
  setheight(300);
  setTextMargin(5, 5, 3, 3);
  setOffset(p_maxTextHeight + p_paddingLeft * 2 + p_maxAxisTextWidth + 5, 15,
            10, 10);
  setPadding(3, 3, 3, 3);
  updateRelative();

  connect(p_signalData.get(), &SignalData::changedEnableGrid, this,
          &GraphWaveform::onChangedEnableGrid);

  connect(p_signalData.get(), &SignalData::changedGraphTimeRange, this,
          &GraphWaveform::onChangedGraphTimeRange);

  connect(p_signalData.get(), &SignalData::changedGlobalScale, this,
          &GraphWaveform::onChangedGlobalScale);

  setFocusPolicy(Qt::StrongFocus);
}

void GraphWaveform::drawWaveform() {
  initImage();
  fill(QColor{255, 255, 255});
  drawName(BaseWaveform::NameType::VerticalLeft);

  drawAxes(BaseWaveform::AxisType::AxisXLeft);
  if (m_isTop) {
    drawAxes(BaseWaveform::AxisType::AxisYTop);
  } else if (m_isBottom) {
    drawAxes(BaseWaveform::AxisType::AxisYBottom);
  }

  drawGrid();
  drawBresenham();

  showWaveform();
}

void GraphWaveform::setTop() {
  m_isTop = true;
  m_isBottom = false;

  setOffset(p_offsetLeft, p_offsetRight, p_maxTextHeight + 5, 10);
  updateRelative();
}

void GraphWaveform::setMiddle() {
  m_isTop = false;
  m_isBottom = false;

  setOffset(p_offsetLeft, p_offsetRight, 10, 10);
  updateRelative();
}

void GraphWaveform::setBottom() {
  m_isTop = false;
  m_isBottom = true;

  setOffset(p_offsetLeft, p_offsetRight, 10, p_maxTextHeight + 5);
  updateRelative();
}

void GraphWaveform::mousePressEvent(QMouseEvent *event) {
  if (event->button() != Qt::LeftButton) return;

  if (m_isCtrlPressed) {
    showToolTip(event);
    return;
  }

  initSelection(event);
}

void GraphWaveform::mouseMoveEvent(QMouseEvent *event) {
  if (m_isCtrlPressed) {
    showToolTip(event);
    return;
  }

  if (!m_isSelected) return;

  QPoint currentPos = event->pos();

  currentPos.setY(p_height - (p_offsetBottom + p_paddingBottom));

  if (currentPos.x() < (p_offsetLeft + p_paddingLeft)) {
    currentPos.setX(p_offsetLeft + p_paddingLeft);
  }

  if (currentPos.x() > (p_width - (p_offsetRight + p_paddingRight))) {
    currentPos.setX(p_width - (p_offsetRight + p_paddingRight));
  }

  m_selectionRect = m_startPoint.x() > currentPos.x()
                        ? QRect(currentPos, m_startPoint)
                        : QRect(m_startPoint, currentPos);

  update();
}

void GraphWaveform::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton && m_isSelected &&
      !m_selectionRect.isNull()) {
    m_isSelected = false;

    p_signalData->setRightTime(p_signalData->leftTime() +
                               std::abs(m_selectionRect.bottomRight().x() -
                                        (p_offsetLeft + p_paddingLeft)) *
                                   p_timePerPixel);

    p_signalData->setLeftTime(p_signalData->leftTime() +
                              std::abs(m_selectionRect.topLeft().x() -
                                       (p_offsetLeft + p_paddingLeft)) *
                                  p_timePerPixel);

    p_signalData->calculateArrayRange();
    p_signalData->setSelected(true);

    updateRelative();

    emit p_signalData->changedGraphTimeRange();
  }
}

void GraphWaveform::paintEvent(QPaintEvent *event) {
  QLabel::paintEvent(event);
  if (!m_isSelected) return;

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::red);
  painter.setBrush(QColor(255, 0, 0, 100));
  painter.drawRect(m_selectionRect);
}

void GraphWaveform::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Control) m_isCtrlPressed = true;
}

void GraphWaveform::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Control) m_isCtrlPressed = false;
}

void GraphWaveform::showToolTip(QMouseEvent *event) {
  if (!validateToolTipPoint(event)) return;

  size_t time =
      (event->pos().x() - (p_offsetLeft + p_paddingLeft)) * p_timePerPixel;
  double data =
      p_curMaxValue -
      (event->pos().y() - (p_offsetTop + p_paddingTop)) * p_dataPerPixel;

  QDateTime fullTime = p_signalData->startTime().addMSecs(time);

  QString tooltipText =
      QString(tr("Value: %1 \n Time: %2"))
          .arg(data)
          .arg(QLocale::system().toString(fullTime, "dd.MM.yyyy hh:mm:ss.zzz"));

  QToolTip::showText(mapToGlobal(event->pos()), tooltipText, this);
}

bool GraphWaveform::validateToolTipPoint(QMouseEvent *event) {
  if (event->pos().x() > (p_width - (p_offsetRight + p_paddingRight)))
    return false;

  if (event->pos().x() < (p_offsetLeft + p_paddingLeft)) return false;

  if (event->pos().y() > (p_height - (p_offsetBottom + p_paddingBottom)))
    return false;

  if (event->pos().y() < (p_offsetTop + p_paddingTop)) return false;

  return true;
}

void GraphWaveform::initSelection(QMouseEvent *event) {
  m_isSelected = true;
  m_startPoint = event->pos();

  m_startPoint.setY(p_offsetTop + p_paddingTop);

  if (m_startPoint.x() < (p_offsetLeft + p_paddingLeft)) {
    m_startPoint.setX(p_offsetLeft + p_paddingLeft);
  }

  if (m_startPoint.x() > (p_width - (p_offsetRight + p_paddingRight))) {
    m_startPoint.setX(p_width - (p_offsetRight + p_paddingRight));
  }

  m_selectionRect = QRect();
}

void GraphWaveform::onChangedEnableGrid() { drawWaveform(); }

void GraphWaveform::onChangedGraphTimeRange() {
  updateRelative();
  drawWaveform();
}

void GraphWaveform::onChangedGlobalScale() {
  updateRelative();
  drawWaveform();
}

}  // namespace fssp
