#include "graphwaveform.h"

namespace fssp {

// GraphWaveform::GraphWaveform(int number, QWidget *parent) : QLabel{parent} {
//   m_number = number;
//   m_isSelecting = false;
// }

// void GraphWaveform::drawWaveform(const std::vector<double> &data,
//                                  double allTime, int width, int height,
//                                  bool isSelected, int leftSelection,
//                                  int rightSelection) {
//   QImage image(width, height, QImage::Format_ARGB32);
//   image.fill(qRgb(255, 255, 255));

//  int from = 0;
//  int to = data.size() - 1;

//  if (isSelected) {
//    from = leftSelection;
//    to = rightSelection;
//  }

//  bresenhamDraw(data, from, to, image, width, height, m_OFFSET_START_X,
//                m_OFFSET_START_Y, m_OFFSET_END_X, m_OFFSET_END_Y,
//                qRgb(0, 127, 255));

//  QPixmap pixmap = QPixmap::fromImage(image);

//  setPixmap(pixmap);
//}

// void GraphWaveform::mousePressEvent(QMouseEvent *event) {
//   if (event->button() != Qt::LeftButton) {
//     return;
//   }

//  m_isSelecting = true;
//  m_startPoint = event->pos();

//  m_startPoint.setY(m_OFFSET_START_Y);

//  if (m_startPoint.x() < m_OFFSET_START_X) {
//    m_startPoint.setX(m_OFFSET_START_X);
//  }

//  if (m_startPoint.x() > (width() - m_OFFSET_END_X)) {
//    m_startPoint.setX(width() - m_OFFSET_END_X);
//  }

//  m_selectionRect = QRect();
//}

// void GraphWaveform::mouseMoveEvent(QMouseEvent *event) {
//   if (!m_isSelecting) return;

//  QPoint currentPos = event->pos();

//  currentPos.setY(height() - m_OFFSET_END_Y);

//  if (currentPos.x() < m_OFFSET_START_X) {
//    currentPos.setX(m_OFFSET_START_X);
//  }

//  if (currentPos.x() > (width() - m_OFFSET_END_X)) {
//    currentPos.setX(width() - m_OFFSET_END_X);
//  }

//  m_selectionRect = m_startPoint.x() > currentPos.x()
//                        ? QRect(currentPos, m_startPoint)
//                        : QRect(m_startPoint, currentPos);
//  update();
//}

// void GraphWaveform::mouseReleaseEvent(QMouseEvent *event) {
//   if (event->button() == Qt::LeftButton && m_isSelecting &&
//       !m_selectionRect.isNull()) {
//     m_isSelecting = false;
//     emit selectionFinished(m_selectionRect.topLeft().x() - m_OFFSET_START_X,
//                            m_selectionRect.bottomRight().x() -
//                            m_OFFSET_START_X, width() - (m_OFFSET_START_X +
//                            m_OFFSET_END_X));
//   }
// }

// void GraphWaveform::paintEvent(QPaintEvent *event) {
//   QLabel::paintEvent(event);
//   if (!m_isSelecting) return;

//  QPainter painter(this);
//  painter.setRenderHint(QPainter::Antialiasing);
//  painter.setPen(Qt::red);
//  painter.setBrush(QColor(255, 0, 0, 100));
//  painter.drawRect(m_selectionRect);
//}

GraphWaveform::GraphWaveform(std::shared_ptr<SignalData> signalData, int number,
                             QWidget *parent)
    : BaseWaveform{signalData, number, 300, 100, parent} {
  m_isTop = false;
  m_isBottom = false;

  setWidth(800);
  setheight(300);
  setTextMargin(5, 5, 3, 3);
  setOffset(p_maxTextHeight + p_paddingLeft * 2 + p_maxAxisTextWidth + 5, 15,
            10, 10);
  setPadding(3, 3, 3, 3);
  updateRelative();
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

}  // namespace fssp
