#include "spectrumwaveform.h"

namespace fssp {

SpectrumWaveform::SpectrumWaveform(std::shared_ptr<SignalData> signalData,
                                   int number,
                                   std::vector<double> &spectrumData,
                                   QWidget *parent)
    : BaseWaveform{signalData, number, 300, 100, parent} {
  m_isTop = false;
  m_isBottom = false;

  m_isCtrlPressed = false;

  setWidth(800);
  setHeight(300);
  setTextMargin(5, 5, 3, 3);
  setOffset(p_maxTextHeight + p_paddingLeft * 2 + p_maxAxisTextWidth + 5, 15,
            10, 10);
  setPadding(3, 3, 3, 3);

  p_data = spectrumData;

  updateRelative();

  connect(p_signalData.get(), &SignalData::changedSpectrumEnableGrid, this,
          &SpectrumWaveform::onChangedEnableGrid);

  connect(p_signalData.get(), &SignalData::changedSpectrumFreqRange, this,
          &SpectrumWaveform::onChangedGraphTimeRange);

  connect(p_signalData.get(), &SignalData::changedSpectrumGlobalScale, this,
          &SpectrumWaveform::onChangedGlobalScale);

  setFocusPolicy(Qt::StrongFocus);
}

void SpectrumWaveform::setData(std::vector<double> &spectrumData) {
  p_data = spectrumData;
  updateRelative();
}

void SpectrumWaveform::drawWaveform() {
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

void SpectrumWaveform::updateRelative() {
  p_leftArray = p_signalData->spectrumLeftArray();
  p_rightArray = p_signalData->spectrumRightArray();

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

  m_pixelPerFreq = ((p_width - (p_offsetRight + p_paddingRight)) -
                    (p_offsetLeft + p_paddingLeft)) /
                   p_signalData->freqRange();

  m_dataPerPixel = 1 / m_pixelPerData;

  m_freqPerPixel = 1 / m_pixelPerFreq;
}

void SpectrumWaveform::setTop() {
  m_isTop = true;
  m_isBottom = false;

  setOffset(p_offsetLeft, p_offsetRight, (p_maxTextHeight + 5) * 2, 10);
  updateRelative();
}

void SpectrumWaveform::setMiddle() {
  m_isTop = false;
  m_isBottom = false;

  setOffset(p_offsetLeft, p_offsetRight, 10, 10);
  updateRelative();
}

void SpectrumWaveform::setBottom() {
  m_isTop = false;
  m_isBottom = true;

  setOffset(p_offsetLeft, p_offsetRight, 10, (p_maxTextHeight + 5) * 2);
  updateRelative();
}

void SpectrumWaveform::mousePressEvent(QMouseEvent *event) {
  if (event->button() != Qt::LeftButton) return;

  if (m_isCtrlPressed) {
    showToolTip(event);
    return;
  }

  initSelection(event);
}

void SpectrumWaveform::mouseMoveEvent(QMouseEvent *event) {
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

void SpectrumWaveform::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton && m_isSelected &&
      !m_selectionRect.isNull()) {
    m_isSelected = false;

    p_signalData->setRightFreq(p_signalData->leftFreq() +
                               std::abs(m_selectionRect.bottomRight().x() -
                                        (p_offsetLeft + p_paddingLeft)) *
                                   m_freqPerPixel);

    p_signalData->setLeftFreq(p_signalData->leftFreq() +
                              std::abs(m_selectionRect.topLeft().x() -
                                       (p_offsetLeft + p_paddingLeft)) *
                                  m_freqPerPixel);

    p_signalData->spectrumCalculateArrayRange();
    p_signalData->setSpectrumSelected(true);

    updateRelative();

    emit p_signalData->changedSpectrumFreqRange();
  }

  if (m_isToolTipShow) {
    m_isToolTipShow = false;
    drawWaveform();
  }
}

void SpectrumWaveform::paintEvent(QPaintEvent *event) {
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

void SpectrumWaveform::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Control) {
    m_isCtrlPressed = true;
    m_isSelected = false;
  }
}

void SpectrumWaveform::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Control) {
    m_isCtrlPressed = false;
    if (m_isToolTipShow) {
      m_isToolTipShow = false;
      drawWaveform();
    }
  }
}

void SpectrumWaveform::showToolTip(QMouseEvent *event) {
  if (!validateToolTipPoint(event)) return;

  double freqStart =
      (event->pos().x() - (p_offsetLeft + p_paddingLeft)) * m_freqPerPixel +
      p_signalData->leftFreq();

  int arrayStart = (p_signalData->samplesNumber() / 2 - 1) * freqStart /
                   (p_signalData->rate() / 2);

  double freqEnd =
      (event->pos().x() + 1 - (p_offsetLeft + p_paddingLeft)) * m_freqPerPixel +
      p_signalData->leftFreq();

  int arrayEnd = (p_signalData->samplesNumber() / 2 - 1) * freqEnd /
                 (p_signalData->rate() / 2);

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

  //  QDateTime fullTime = p_signalData->startTime().addMSecs(freqStart);

  QString tooltipText =
      QString(tr("Value: %1 \n Freq: %2")).arg(data).arg(freqStart);

  m_toolTipPoint.setX(event->pos().x());
  m_toolTipPoint.setY((p_maxValue - data) * m_pixelPerData + p_offsetTop +
                      p_paddingTop);
  m_isToolTipShow = true;

  QToolTip::showText(mapToGlobal(m_toolTipPoint), tooltipText, this);

  update();
}

bool SpectrumWaveform::validateToolTipPoint(QMouseEvent *event) {
  if (event->pos().x() > (p_width - (p_offsetRight + p_paddingRight)))
    return false;

  if (event->pos().x() < (p_offsetLeft + p_paddingLeft)) return false;

  if (event->pos().y() > (p_height - (p_offsetBottom + p_paddingBottom)))
    return false;

  if (event->pos().y() < (p_offsetTop + p_paddingTop)) return false;

  return true;
}

void SpectrumWaveform::initSelection(QMouseEvent *event) {
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

void SpectrumWaveform::onChangedEnableGrid() { drawWaveform(); }

void SpectrumWaveform::onChangedGraphTimeRange() {
  updateRelative();
  drawWaveform();
}

void SpectrumWaveform::onChangedGlobalScale() {
  updateRelative();
  drawWaveform();
}

void SpectrumWaveform::drawName() {
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

  if (m_isTop) {
    textRect = {p_paddingLeft, p_maxTextHeight + p_paddingTop,
                p_width - (p_paddingRight + p_paddingLeft), p_maxTextHeight};
    painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap,
                     tr("Freq (HZ)"));
  } else if (m_isBottom) {
    textRect = {p_paddingLeft, p_height - (p_maxTextHeight + p_paddingBottom),
                p_width - (p_paddingRight + p_paddingLeft), p_maxTextHeight};
    painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap,
                     tr("Freq (HZ)"));
  }
}

void SpectrumWaveform::drawAxisY() {
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

    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
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

    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                     QString::number(curValue, 'f', count));
  }
}

void SpectrumWaveform::drawAxisTopX() {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  double freqRange = p_signalData->freqRange();

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
  double delimiter = 1;
  for (int i = 0; i < p_dataMultiples.size(); ++i) {
    delimitersNumber = freqRange / p_dataMultiples[i];

    if (delimitersNumber <= p_xLabelsNumber) {
      delimiter = p_dataMultiples[i];
      break;
    }
  }

  double curValue = freqRange;
  curValue -= std::fmod(curValue, delimiter);

  int count = 0;
  for (; delimiter < 1; ++count) delimiter *= 10;
  delimiter /= std::pow(10, count);

  double step = m_pixelPerFreq * delimiter;
  int startX =
      axisEnd.x() - std::abs(m_pixelPerFreq * (freqRange - curValue)) + 1;
  for (int i = 0; i < delimitersNumber; ++i) {
    int x = startX - step * i;
    painter.drawLine(QPoint{x, p1}, QPoint{x, p2});

    QRect textRect{QPoint{x - (p_maxAxisTextWidth / 2 + p_maxAxisTextWidth % 2 +
                               p_axisLineWidth),
                          textY1},
                   QPoint{x + p_maxAxisTextWidth / 2, textY2}};

    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                     QString::number(curValue, 'f', count));

    if (i != delimitersNumber - 1) {
      curValue -= delimiter;
    }
  }

  if (std::abs(p_minValue - curValue) >= delimiter) {
    int x = startX - step * delimitersNumber;

    if (x < axisStart.x()) return;

    curValue -= delimiter;

    painter.drawLine(QPoint{x, p1}, QPoint{x, p2});
    QRect textRect{QPoint{x - (p_maxAxisTextWidth / 2 + p_maxAxisTextWidth % 2 +
                               p_axisLineWidth),
                          textY1},
                   QPoint{x + p_maxAxisTextWidth / 2, textY2}};

    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                     QString::number(curValue, 'f', count));
  }
}

void SpectrumWaveform::drawAxisBottomX() {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  double freqRange = p_signalData->freqRange();

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
  double delimiter = 1;
  for (int i = 0; i < p_dataMultiples.size(); ++i) {
    delimitersNumber = freqRange / p_dataMultiples[i];

    if (delimitersNumber <= p_xLabelsNumber) {
      delimiter = p_dataMultiples[i];
      break;
    }
  }

  double curValue = freqRange;
  curValue -= std::fmod(curValue, delimiter);

  int count = 0;
  for (; delimiter < 1; ++count) delimiter *= 10;
  delimiter /= std::pow(10, count);

  double step = m_pixelPerFreq * delimiter;
  int startX =
      axisEnd.x() - std::abs(m_pixelPerFreq * (freqRange - curValue)) + 1;
  for (int i = 0; i < delimitersNumber; ++i) {
    int x = startX - step * i;
    painter.drawLine(QPoint{x, p1}, QPoint{x, p2});

    QRect textRect{QPoint{x - (p_maxAxisTextWidth / 2 + p_maxAxisTextWidth % 2 +
                               p_axisLineWidth),
                          textY1},
                   QPoint{x + p_maxAxisTextWidth / 2, textY2}};

    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                     QString::number(curValue, 'f', count));

    if (i != delimitersNumber - 1) {
      curValue -= delimiter;
    }
  }

  if (std::abs(p_minValue - curValue) >= delimiter) {
    int x = startX - step * delimitersNumber;

    if (x < axisStart.x()) return;

    curValue -= delimiter;

    painter.drawLine(QPoint{x, p1}, QPoint{x, p2});
    QRect textRect{QPoint{x - (p_maxAxisTextWidth / 2 + p_maxAxisTextWidth % 2 +
                               p_axisLineWidth),
                          textY1},
                   QPoint{x + p_maxAxisTextWidth / 2, textY2}};

    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                     QString::number(curValue, 'f', count));
  }
}

void SpectrumWaveform::drawGrid() {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  if (!p_signalData->spectrumIsGridEnabled()) return;

  double freqRange = p_signalData->freqRange();

  QPainter painter(&p_image);
  painter.setPen(QPen(p_gridColor, p_gridLineWidth));
  painter.setFont(p_font);

  int p1;
  int p2;

  QPoint axisStart;
  QPoint axisEnd;

  size_t freqDelimitersNumber = 0;
  double freqDelimiter = 1;
  for (int i = 0; i < p_dataMultiples.size(); ++i) {
    freqDelimitersNumber = freqRange / p_dataMultiples[i];

    if (freqDelimitersNumber <= p_yLabelsNumber) {
      freqDelimiter = p_dataMultiples[i];
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

  double curValueX = freqRange;
  curValueX -= std::fmod(curValueX, freqDelimiter);

  double stepX = std::round(m_pixelPerFreq * freqDelimiter);
  int startX = axisEnd.x() - std::abs(m_pixelPerFreq * (freqRange - curValueX));

  p1 = axisStart.y();
  p2 = axisStart.y() + (p_height - ((p_paddingTop + p_offsetTop) +
                                    (p_paddingBottom + p_offsetBottom)));

  for (int i = 0; i < freqDelimitersNumber; ++i) {
    int x = startX - stepX * i;
    painter.drawLine(QPoint{x, p1}, QPoint{x, p2});
  }
}

}  // namespace fssp
