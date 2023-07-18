#include "graphwaveform.h"

namespace fssp {

GraphWaveform::GraphWaveform(std::shared_ptr<SignalData> signalData, int number,
                             QWidget *parent)
    : BaseWaveform{signalData, number, 300, 100, parent} {
  m_isTop = false;
  m_isBottom = false;

  m_isSelected = p_signalData->isSelected();
  m_isCtrlPressed = false;

  setWidth(800);
  setHeight(300);
  setTextMargin(5, 5, 3, 3);
  setOffset(p_maxTextHeight + p_paddingLeft * 2 + p_maxAxisTextWidth + 5, 15,
            10, 10);
  setPadding(3, 3, 3, 3);

  p_data = p_signalData->data()[p_number];

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
  fill();
  drawName();

  drawAxisY();
  if (m_isTop) {
    drawAxisTopX();
  } else if (m_isBottom) {
    drawAxisBottomX();
  }

  drawGrid();
  drawBresenham();

  showWaveform();
}

void GraphWaveform::updateRelative() {
  p_leftArray = p_signalData->leftArray();
  p_rightArray = p_signalData->rightArray();

  p_arrayRange = p_rightArray - p_leftArray + 1;

  if (p_signalData->isGlobalScale()) {
    p_maxValue = *std::max_element(p_data.begin(), p_data.end());
    p_minValue = *std::min_element(p_data.begin(), p_data.end());
  } else {
    p_maxValue = *std::max_element(p_data.begin() + p_leftArray,
                                   p_data.begin() + p_rightArray);
    p_minValue = *std::min_element(p_data.begin() + p_leftArray,
                                   p_data.begin() + p_rightArray);
  }

  p_dataRange = std::abs(p_maxValue - p_minValue);

  p_xLabelsNumber = ((p_height - (p_offsetBottom + p_paddingBottom)) -
                     (p_offsetTop + p_paddingTop)) /
                    p_maxTextHeight;

  p_yLabelsNumber = ((p_width - (p_offsetRight + p_paddingRight)) -
                     (p_offsetLeft + p_paddingLeft)) /
                    p_maxAxisTextWidth;

  m_pixelPerData = ((p_height - (p_offsetBottom + p_paddingBottom)) -
                    (p_offsetTop + p_paddingTop)) /
                   p_dataRange;

  m_pixelPerTime = ((p_width - (p_offsetRight + p_paddingRight)) -
                    (p_offsetLeft + p_paddingLeft)) /
                   static_cast<double>(p_signalData->timeRange());

  m_dataPerPixel = 1 / m_pixelPerData;

  m_timePerPixel = 1 / m_pixelPerTime;
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
                                   m_timePerPixel);

    p_signalData->setLeftTime(p_signalData->leftTime() +
                              std::abs(m_selectionRect.topLeft().x() -
                                       (p_offsetLeft + p_paddingLeft)) *
                                  m_timePerPixel);

    p_signalData->calculateArrayRange();
    p_signalData->setSelected(true);

    updateRelative();

    emit p_signalData->changedGraphTimeRange();
  }

  if (m_isToolTipShow) {
    m_isToolTipShow = false;
    drawWaveform();
  }
}

void GraphWaveform::paintEvent(QPaintEvent *event) {
  QLabel::paintEvent(event);

  QPainter painter(this);

  if (m_isSelected) {
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::red);
    painter.setBrush(QColor(255, 0, 0, 100));
    painter.drawRect(m_selectionRect);
  } else if (m_isToolTipShow) {
    painter.setPen(Qt::black);

    QLine xLine{
        QPoint{p_offsetLeft + p_paddingLeft, m_toolTipPoint.y()},
        QPoint{p_width - (p_offsetRight + p_paddingRight), m_toolTipPoint.y()}};
    painter.drawLine(xLine);

    QLine yLine{QPoint{m_toolTipPoint.x(), p_offsetTop + p_paddingTop},
                QPoint{m_toolTipPoint.x(),
                       p_height - (p_offsetBottom + p_paddingBottom)}};
    painter.drawLine(yLine);
  }
}

void GraphWaveform::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Control) {
    m_isCtrlPressed = true;
    m_isSelected = false;
  }
}

void GraphWaveform::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Control) {
    m_isCtrlPressed = false;
    if (m_isToolTipShow) {
      m_isToolTipShow = false;
      drawWaveform();
    }
  }
}

void GraphWaveform::showToolTip(QMouseEvent *event) {
  if (!validateToolTipPoint(event)) return;

  size_t timeStart =
      (event->pos().x() - (p_offsetLeft + p_paddingLeft)) * m_timePerPixel +
      p_signalData->leftTime();

  int arrayStart = (p_signalData->samplesNumber() - 1) * timeStart /
                   (p_signalData->allTime() - 1);

  size_t timeEnd =
      (event->pos().x() + 1 - (p_offsetLeft + p_paddingLeft)) * m_timePerPixel +
      p_signalData->leftTime();

  int arrayEnd = (p_signalData->samplesNumber() - 1) * timeEnd /
                 (p_signalData->allTime() - 1);

  double max =
      *std::max_element(p_data.begin() + arrayStart, p_data.begin() + arrayEnd);

  double min =
      *std::min_element(p_data.begin() + arrayStart, p_data.begin() + arrayEnd);

  double avg = (max + min) / 2;

  int avgPixel =
      (p_maxValue - avg) * m_pixelPerData + p_offsetTop + p_paddingTop;

  double data;
  if (event->pos().y() < avgPixel) {
    data = max;
  } else {
    data = min;
  }

  QDateTime fullTime = p_signalData->startTime().addMSecs(timeStart);

  QString tooltipText =
      QString(tr("Value: %1 \n Time: %2"))
          .arg(data)
          .arg(QLocale::system().toString(fullTime, "dd.MM.yyyy hh:mm:ss.zzz"));

  m_toolTipPoint.setX(event->pos().x());
  m_toolTipPoint.setY((p_maxValue - data) * m_pixelPerData + p_offsetTop +
                      p_paddingTop);
  m_isToolTipShow = true;

  QToolTip::showText(mapToGlobal(m_toolTipPoint), tooltipText, this);

  update();
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

void GraphWaveform::drawName() {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  QPainter painter(&p_image);
  painter.setPen(p_mainColor);
  painter.setFont(p_font);

  QRect textRect = {p_paddingLeft, p_paddingTop, p_maxTextHeight,
                    p_height - (p_paddingTop + p_paddingBottom)};

  painter.translate(textRect.center());
  painter.rotate(-90);
  painter.translate(-textRect.height() / 2 - p_paddingLeft,
                    -textRect.width() / 2 - p_paddingTop);

  textRect.setWidth(p_height - (p_paddingTop + p_paddingBottom));
  textRect.setHeight(p_maxTextHeight * 2);

  painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap,
                   p_signalData->channelsName()[p_number]);
}

void GraphWaveform::drawAxisY() {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  QPainter painter(&p_image);
  painter.setPen(QPen(p_mainColor, p_axisLineWidth));
  painter.setFont(p_font);

  QPoint axisStart = {p_offsetLeft + p_paddingLeft, p_offsetTop + p_paddingTop};
  QPoint axisEnd = {p_offsetLeft + p_paddingLeft,
                    p_height - (p_offsetBottom + p_paddingBottom)};

  int p1 = axisStart.x() - 5;
  int p2 = axisStart.x();

  painter.drawLine(axisStart, axisEnd);

  int textX1 = p1 - (p_textMarginRight + p_maxAxisTextWidth);
  int textX2 = p1 - p_textMarginRight;

  size_t delimitersNumber = 0;
  double delimiter = 1;
  for (int i = 0; i < p_dataMultiples.size(); ++i) {
    delimitersNumber = p_dataRange / p_dataMultiples[i];

    if (delimitersNumber <= p_xLabelsNumber) {
      delimiter = p_dataMultiples[i];
      break;
    }
  }

  double curValue = p_maxValue;
  if (p_maxValue > 0) {
    curValue -= std::fmod(p_maxValue, delimiter);
  } else {
    curValue += std::fmod(p_maxValue, delimiter);
  }

  int count = 0;
  for (; delimiter < 1; ++count) delimiter *= 10;
  delimiter /= std::pow(10, count);

  double step = m_pixelPerData * delimiter;
  int startY =
      axisStart.y() + std::abs(m_pixelPerData * (p_maxValue - curValue));
  for (int i = 0; i < delimitersNumber; ++i) {
    int y = startY + step * i;
    painter.drawLine(QPoint{p1, y}, QPoint{p2, y});

    QRect textRect{QPoint{textX1, y - (p_maxTextHeight / 2 +
                                       p_maxTextHeight % 2 + p_axisLineWidth)},
                   QPoint{textX2, y + p_maxTextHeight / 2}};

    painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter,
                     QString::number(curValue, 'f', count));

    if (i != delimitersNumber - 1) {
      curValue -= delimiter;
    }
  }

  if (std::abs(p_minValue - curValue) >= delimiter) {
    int y = startY + step * delimitersNumber;

    if (y > axisEnd.y()) return;

    curValue -= delimiter;

    painter.drawLine(QPoint{p1, y}, QPoint{p2, y});
    QRect textRect{QPoint{textX1, y - (p_maxTextHeight / 2 +
                                       p_maxTextHeight % 2 + p_axisLineWidth)},
                   QPoint{textX2, y + p_maxTextHeight / 2}};

    painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter,
                     QString::number(curValue, 'f', count));
  }
}

void GraphWaveform::drawAxisTopX() {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  size_t timeRange = p_signalData->timeRange();

  QPainter painter(&p_image);
  painter.setPen(QPen(p_mainColor, p_axisLineWidth));
  painter.setFont(p_font);

  QPoint axisStart = {p_offsetLeft + p_paddingLeft, p_offsetTop + p_paddingTop};
  QPoint axisEnd = {p_width - (p_offsetRight + p_paddingRight),
                    p_offsetTop + p_paddingTop};

  int p1 = axisStart.y() - 5;
  int p2 = axisStart.y();

  painter.drawLine(axisStart, axisEnd);

  int textY1 = p1 - (p_textMarginBottom + p_maxTextHeight);
  int textY2 = p1 - p_textMarginBottom;

  size_t delimitersNumber = 0;
  size_t delimiter = 1;
  for (int i = 0; i < p_timeMultiples.size(); ++i) {
    delimitersNumber = timeRange / p_timeMultiples[i];

    if (delimitersNumber <= p_yLabelsNumber) {
      delimiter = p_timeMultiples[i];
      break;
    }
  }

  QString unitOfTime;
  size_t divisionBase;
  if (delimiter < p_timeMultiples[8]) {
    unitOfTime = tr("msec");
    divisionBase = p_timeMultiples[1];
  } else if (delimiter < p_timeMultiples[14]) {
    unitOfTime = tr("sec");
    divisionBase = p_timeMultiples[8];
  } else if (delimiter < p_timeMultiples[20]) {
    unitOfTime = tr("min");
    divisionBase = p_timeMultiples[14];
  } else if (delimiter < p_timeMultiples[25]) {
    unitOfTime = tr("h");
    divisionBase = p_timeMultiples[20];
  } else if (delimiter < p_timeMultiples[27]) {
    unitOfTime = tr("d");
    divisionBase = p_timeMultiples[25];
  } else if (delimiter < p_timeMultiples[29]) {
    unitOfTime = tr("w");
    divisionBase = p_timeMultiples[27];
  } else if (delimiter < p_timeMultiples[33]) {
    unitOfTime = tr("m");
    divisionBase = p_timeMultiples[29];
  } else if (delimiter <= p_timeMultiples[37]) {
    unitOfTime = tr("y");
    divisionBase = p_timeMultiples[33];
  }

  size_t curValue = std::floor(timeRange);
  curValue -= curValue % delimiter;

  size_t step = std::round(m_pixelPerTime * delimiter);
  int startX = axisEnd.x() - m_pixelPerTime * (timeRange - curValue);
  for (int i = 0; i < delimitersNumber; ++i) {
    int x = startX - step * i;
    painter.drawLine(QPoint{x, p1}, QPoint{x, p2});

    QRect textRect{QPoint{x - (p_maxAxisTextWidth / 2 + p_maxAxisTextWidth % 2 +
                               p_axisLineWidth),
                          textY1},
                   QPoint{x + p_maxAxisTextWidth / 2, textY2}};

    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                     QString::number(curValue / divisionBase) + unitOfTime);

    if (i != delimitersNumber - 1) {
      curValue -= delimiter;
    }
  }

  if (std::abs(p_minValue - curValue) >= delimiter) {
    int x = startX - step * delimitersNumber;

    if (x < axisStart.x()) return;

    curValue -= delimiter;

    if (!curValue) return;

    painter.drawLine(QPoint{x, p1}, QPoint{x, p2});
    QRect textRect{QPoint{x - (p_maxAxisTextWidth / 2 + p_maxAxisTextWidth % 2 +
                               p_axisLineWidth),
                          textY1},
                   QPoint{x + p_maxAxisTextWidth / 2, textY2}};

    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                     QString::number(curValue));
  }
}

void GraphWaveform::drawAxisBottomX() {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  size_t timeRange = p_signalData->timeRange();

  QPainter painter(&p_image);
  painter.setPen(QPen(p_mainColor, p_axisLineWidth));
  painter.setFont(p_font);

  QPoint axisStart = {p_offsetLeft + p_paddingLeft,
                      p_height - (p_offsetBottom + p_paddingBottom)};
  QPoint axisEnd = {p_width - (p_offsetRight + p_paddingRight),
                    p_height - (p_offsetBottom + p_paddingBottom)};

  int p1 = axisStart.y() + 5;
  int p2 = axisStart.y();

  painter.drawLine(axisStart, axisEnd);

  int textY1 = p1 + (p_textMarginTop + p_maxTextHeight);
  int textY2 = p1 + p_textMarginTop;

  size_t delimitersNumber = 0;
  size_t delimiter = 1;
  for (int i = 0; i < p_timeMultiples.size(); ++i) {
    delimitersNumber = timeRange / p_timeMultiples[i];

    if (delimitersNumber <= p_yLabelsNumber) {
      delimiter = p_timeMultiples[i];
      break;
    }
  }

  QString unitOfTime;
  size_t divisionBase;
  if (delimiter < p_timeMultiples[8]) {
    unitOfTime = tr("msec");
    divisionBase = p_timeMultiples[1];
  } else if (delimiter < p_timeMultiples[14]) {
    unitOfTime = tr("sec");
    divisionBase = p_timeMultiples[8];
  } else if (delimiter < p_timeMultiples[20]) {
    unitOfTime = tr("min");
    divisionBase = p_timeMultiples[14];
  } else if (delimiter < p_timeMultiples[25]) {
    unitOfTime = tr("h");
    divisionBase = p_timeMultiples[20];
  } else if (delimiter < p_timeMultiples[27]) {
    unitOfTime = tr("d");
    divisionBase = p_timeMultiples[25];
  } else if (delimiter < p_timeMultiples[29]) {
    unitOfTime = tr("w");
    divisionBase = p_timeMultiples[27];
  } else if (delimiter < p_timeMultiples[33]) {
    unitOfTime = tr("m");
    divisionBase = p_timeMultiples[29];
  } else if (delimiter <= p_timeMultiples[37]) {
    unitOfTime = tr("y");
    divisionBase = p_timeMultiples[33];
  }

  size_t curValue = std::floor(timeRange);
  curValue -= curValue % delimiter;

  size_t step = std::round(m_pixelPerTime * delimiter);
  int startX = axisEnd.x() - m_pixelPerTime * (timeRange - curValue);
  for (int i = 0; i < delimitersNumber; ++i) {
    int x = startX - step * i;
    painter.drawLine(QPoint{x, p1}, QPoint{x, p2});

    QRect textRect{QPoint{x - (p_maxAxisTextWidth / 2 + p_maxAxisTextWidth % 2 +
                               p_axisLineWidth),
                          textY1},
                   QPoint{x + p_maxAxisTextWidth / 2, textY2}};

    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                     QString::number(curValue / divisionBase) + unitOfTime);

    if (i != delimitersNumber - 1) {
      curValue -= delimiter;
    }
  }

  if (std::abs(p_minValue - curValue) >= delimiter) {
    int x = startX - step * delimitersNumber;

    if (x < axisStart.x()) return;

    curValue -= delimiter;

    if (!curValue) return;

    painter.drawLine(QPoint{x, p1}, QPoint{x, p2});
    QRect textRect{QPoint{x - (p_maxAxisTextWidth / 2 + p_maxAxisTextWidth % 2 +
                               p_axisLineWidth),
                          textY1},
                   QPoint{x + p_maxAxisTextWidth / 2, textY2}};

    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                     QString::number(curValue));
  }
}

void GraphWaveform::drawGrid() {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  if (!p_signalData->isGridEnabled()) return;

  size_t timeRange = p_signalData->timeRange();

  QPainter painter(&p_image);
  painter.setPen(QPen(p_gridColor, p_gridLineWidth));
  painter.setFont(p_font);

  int p1;
  int p2;

  QPoint axisStart;
  QPoint axisEnd;

  size_t timeDelimitersNumber = 0;
  size_t timeDelimiter = 1;
  for (int i = 0; i < p_timeMultiples.size(); ++i) {
    timeDelimitersNumber = timeRange / p_timeMultiples[i];

    if (timeDelimitersNumber <= p_yLabelsNumber) {
      timeDelimiter = p_timeMultiples[i];
      break;
    }
  }

  size_t dataDelimitersNumber = 0;
  double dataDelimiter = 1;
  for (int i = 0; i < p_dataMultiples.size(); ++i) {
    dataDelimitersNumber = p_dataRange / p_dataMultiples[i];

    if (dataDelimitersNumber <= p_xLabelsNumber) {
      dataDelimiter = p_dataMultiples[i];
      break;
    }
  }

  double curValueY = p_maxValue;
  if (p_maxValue > 0) {
    curValueY -= std::fmod(p_maxValue, dataDelimiter);
  } else {
    curValueY += std::fmod(p_maxValue, dataDelimiter);
  }

  axisStart = {p_offsetLeft + p_paddingLeft, p_offsetTop + p_paddingTop};
  axisEnd = {p_offsetLeft + p_paddingLeft,
             p_height - (p_offsetBottom + p_paddingBottom)};

  double stepY = m_pixelPerData * dataDelimiter;
  int startY =
      axisStart.y() + std::abs(m_pixelPerData * (p_maxValue - curValueY));

  p1 = axisStart.x();
  p2 = axisStart.x() + (p_width - ((p_paddingLeft + p_offsetLeft) +
                                   (p_paddingRight + p_offsetRight)));

  for (int i = 0; i < dataDelimitersNumber; ++i) {
    int y = startY + stepY * i;
    painter.drawLine(QPoint{p1, y}, QPoint{p2, y});
  }

  if (std::abs(p_minValue - curValueY) >= dataDelimiter) {
    int y = startY + stepY * dataDelimitersNumber;

    if (y <= axisEnd.y()) {
      painter.drawLine(QPoint{p1, y}, QPoint{p2, y});
    }
  }

  axisStart = {p_offsetLeft + p_paddingLeft, p_offsetTop + p_paddingTop};
  axisEnd = {p_width - (p_offsetRight + p_paddingRight),
             p_offsetTop + p_paddingTop};

  size_t curValueX = timeRange;
  curValueX -= curValueX % timeDelimiter;

  size_t stepX = std::round(m_pixelPerTime * timeDelimiter);
  int startX = axisEnd.x() - m_pixelPerTime * (timeRange - curValueX);

  p1 = axisStart.y();
  p2 = axisStart.y() + (p_height - ((p_paddingTop + p_offsetTop) +
                                    (p_paddingBottom + p_offsetBottom)));

  for (int i = 0; i < timeDelimitersNumber; ++i) {
    int x = startX - stepX * i;
    painter.drawLine(QPoint{x, p1}, QPoint{x, p2});
  }
}

}  // namespace fssp
