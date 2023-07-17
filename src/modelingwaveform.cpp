#include "modelingwaveform.h"

#include <QPainter>
#include <cmath>

namespace fssp {

ModelingWaveform::ModelingWaveform(std::shared_ptr<SignalData> signalData,
                                   QWidget *parent)
    : BaseWaveform{signalData, 0, 300, 100, parent} {
  setWidth(900);
  setHeight(300);
  setTextMargin(5, 5, 3, 3);
  setOffset(p_maxAxisTextWidth, 15, p_maxTextHeight + 5, 10);
  setPadding(3, 3, 3, 3);

  p_data = p_signalData->data()[p_number];

  p_leftArray = p_signalData->leftArray();
  p_rightArray = p_signalData->rightArray();

  p_arrayRange = p_rightArray - p_leftArray + 1;

  p_minValue = *std::min_element(p_data.begin(), p_data.end());
  p_maxValue = *std::max_element(p_data.begin(), p_data.end());

  p_dataRange = std::abs(p_maxValue - p_minValue);

  updateRelative();
}

void ModelingWaveform::drawWaveform() {
  initImage();
  fill();
  drawAxisX();
  drawAxisY();
  drawGrid();
  drawBresenham();
  showWaveform();
}

void ModelingWaveform::updateRelative() {
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
}

void ModelingWaveform::drawAxisX() {
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

void ModelingWaveform::drawAxisY() {
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

void ModelingWaveform::drawGrid() {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

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
