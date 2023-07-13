#include "basewaveform.h"

#include <QPainter>

#include "qnamespace.h"

namespace fssp {

BaseWaveform::BaseWaveform(std::shared_ptr<SignalData> signalData, int number,
                           int minWidth, int minHeight, QWidget *parent)
    : QLabel{parent} {
  p_signalData = signalData;
  p_number = number;

  p_minWidth = minWidth;
  p_minHeight = minHeight;

  p_width = p_minWidth;
  p_height = p_minHeight;

  p_image = QImage();

  p_minValue = *std::min_element(p_signalData->data()[p_number].begin(),
                                 p_signalData->data()[p_number].end());
  p_maxValue = *std::max_element(p_signalData->data()[p_number].begin(),
                                 p_signalData->data()[p_number].end());

  p_signalData->setLeftArray(0);
  p_signalData->setRightArray(p_signalData->samplesNumber() - 1);

  p_signalData->setLeftTime(0);
  p_signalData->setRightTime(p_signalData->allTime() - 1);

  p_signalData->setGridEnabled(true);
  p_signalData->setGlobalScale(false);
}

int BaseWaveform::number() const { return p_number; }

void BaseWaveform::setWidth(int width) {
  if (width > p_minWidth) {
    p_width = width;
  }
}

void BaseWaveform::setheight(int height) {
  if (height > p_minHeight) {
    p_height = height;
  }
}

void BaseWaveform::setPadding(int left, int right, int top, int bottom) {
  p_paddingLeft = left;
  p_paddingRight = right;
  p_paddingTop = top;
  p_paddingBottom = bottom;
}

void BaseWaveform::setOffset(int left, int right, int top, int bottom) {
  p_offsetLeft = left;
  p_offsetRight = right;
  p_offsetTop = top;
  p_offsetBottom = bottom;
}

void BaseWaveform::setTextMargin(int left, int right, int top, int bottom) {
  p_textMarginLeft = left;
  p_textMarginRight = right;
  p_textMarginTop = top;
  p_textMarginBottom = bottom;

  p_maxTextHeight =
      QFontMetrics(p_font).height() + p_textMarginTop + p_textMarginBottom;
  p_maxAxisTextWidth = QFontMetrics(p_font).averageCharWidth() * 11 +
                       p_textMarginLeft + p_textMarginRight;
}

void BaseWaveform::updateRelative() {
  p_arrayRange = p_signalData->rightArray() - p_signalData->leftArray() + 1;

  p_timeRange = p_signalData->rightTime() - p_signalData->leftTime();

  p_curMinValue =
      (p_signalData->isGlobalScale())
          ? p_minValue
          : *std::min_element(p_signalData->data()[p_number].begin() +
                                  p_signalData->leftArray(),
                              p_signalData->data()[p_number].begin() +
                                  p_signalData->rightArray());

  p_curMaxValue =
      (p_signalData->isGlobalScale())
          ? p_maxValue
          : *std::max_element(p_signalData->data()[p_number].begin() +
                                  p_signalData->leftArray(),
                              p_signalData->data()[p_number].begin() +
                                  p_signalData->rightArray());

  p_dataRange = std::abs(p_curMaxValue - p_curMinValue);

  p_xLabelsNumber = ((p_height - (p_offsetBottom + p_paddingBottom)) -
                     (p_offsetTop + p_paddingTop)) /
                    p_maxTextHeight;

  p_yLabelsNumber = ((p_width - (p_offsetRight + p_paddingRight)) -
                     (p_offsetLeft + p_paddingLeft)) /
                    p_maxAxisTextWidth;

  p_pixelPerData = ((p_height - (p_offsetBottom + p_paddingBottom)) -
                    (p_offsetTop + p_paddingTop)) /
                   p_dataRange;

  p_pixelPerTime = ((p_width - (p_offsetRight + p_paddingRight)) -
                    (p_offsetLeft + p_paddingLeft)) /
                   static_cast<double>(p_timeRange);

  p_timePerPixel = 1 / p_pixelPerTime;

  p_dataPerPixel = 1 / p_pixelPerData;
}

bool BaseWaveform::isImageNull() const { return p_image.isNull(); }

void BaseWaveform::initImage() {
  p_image = QImage(p_width, p_height, p_imageFormat);
}

void BaseWaveform::fill(QColor color) { p_image.fill(color.toRgb()); }

void BaseWaveform::drawGrid() {
  if (!p_signalData->isGridEnabled()) return;

  QPainter painter(&p_image);
  painter.setPen(QPen(p_gridColor, p_gridLineWidth));
  painter.setFont(p_font);

  int p1;
  int p2;

  QPoint axisStart;
  QPoint axisEnd;

  calculateDataDelimiter();
  calculateTimeDelimiter();

  double curValueY = p_curMaxValue;
  if (p_curMaxValue > 0) {
    curValueY -= std::fmod(p_curMaxValue, p_dataDelimiter);
  } else {
    curValueY += std::fmod(p_curMaxValue, p_dataDelimiter);
  }

  axisStart = {p_offsetLeft + p_paddingLeft, p_offsetTop + p_paddingTop};
  axisEnd = {p_offsetLeft + p_paddingLeft,
             p_height - (p_offsetBottom + p_paddingBottom)};

  double stepY = p_pixelPerData * p_dataDelimiter;
  int startY =
      axisStart.y() + std::abs(p_pixelPerData * (p_curMaxValue - curValueY));

  p1 = axisStart.x();
  p2 = axisStart.x() + (p_width - ((p_paddingLeft + p_offsetLeft) +
                                   (p_paddingRight + p_offsetRight)));

  for (int i = 0; i < p_curDataDelimitersNumber; ++i) {
    int y = startY + stepY * i;
    painter.drawLine(QPoint{p1, y}, QPoint{p2, y});
  }

  if (std::abs(p_curMinValue - curValueY) >= p_dataDelimiter) {
    int y = startY + stepY * p_curDataDelimitersNumber;

    if (y <= axisEnd.y()) {
      curValueY -= p_dataDelimiter;

      painter.drawLine(QPoint{p1, y}, QPoint{p2, y});
    }
  }

  axisStart = {p_offsetLeft + p_paddingLeft, p_offsetTop + p_paddingTop};
  axisEnd = {p_width - (p_offsetRight + p_paddingRight),
             p_offsetTop + p_paddingTop};

  size_t curValueX = std::floor(p_timeRange);
  curValueX -= curValueX % p_timeDelimiter;

  size_t stepX = std::round(p_pixelPerTime * p_timeDelimiter);
  int startX =
      axisEnd.x() - std::abs(p_pixelPerTime * (p_timeRange - curValueX));

  p1 = axisStart.y();
  p2 = axisStart.y() + (p_height - ((p_paddingTop + p_offsetTop) +
                                    (p_paddingBottom + p_offsetBottom)));

  for (int i = 0; i < p_curTimeDelimitersNumber; ++i) {
    int x = startX - stepX * i;
    painter.drawLine(QPoint{x, p1}, QPoint{x, p2});
  }
}

void BaseWaveform::drawAxes(BaseWaveform::AxisType axisType) {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  QPainter painter(&p_image);
  painter.setPen(QPen(p_mainColor, p_axisLineWidth));
  painter.setFont(p_font);

  QPoint axisStart;
  QPoint axisEnd;

  int p1;
  int p2;

  switch (axisType) {
    case BaseWaveform::AxisType::AxisXLeft: {
      axisStart = {p_offsetLeft + p_paddingLeft, p_offsetTop + p_paddingTop};
      axisEnd = {p_offsetLeft + p_paddingLeft,
                 p_height - (p_offsetBottom + p_paddingBottom)};

      p1 = axisStart.x() - 5;
      p2 = axisStart.x();

      break;
    }

    case BaseWaveform::AxisType::AxisYTop: {
      axisStart = {p_offsetLeft + p_paddingLeft, p_offsetTop + p_paddingTop};
      axisEnd = {p_width - (p_offsetRight + p_paddingRight),
                 p_offsetTop + p_paddingTop};

      p1 = axisStart.y() - 5;
      p2 = axisStart.y();

      break;
    }

    case BaseWaveform::AxisType::AxisYBottom: {
      axisStart = {p_offsetLeft + p_paddingLeft,
                   p_height - (p_offsetBottom + p_paddingBottom)};
      axisEnd = {p_width - (p_offsetRight + p_paddingRight),
                 p_height - (p_offsetBottom + p_paddingBottom)};

      p1 = axisStart.y() + 5;
      p2 = axisStart.y();

      break;
    }
  }

  painter.drawLine(axisStart, axisEnd);

  if (axisType == BaseWaveform::AxisType::AxisXLeft) {
    int textX1 = p1 - (p_textMarginRight + p_maxAxisTextWidth);
    int textX2 = p1 - p_textMarginRight;

    calculateDataDelimiter();

    double curValue = p_curMaxValue;
    if (p_curMaxValue > 0) {
      curValue -= std::fmod(p_curMaxValue, p_dataDelimiter);
    } else {
      curValue += std::fmod(p_curMaxValue, p_dataDelimiter);
    }

    int count = 0;
    for (; p_dataDelimiter < 1; ++count) p_dataDelimiter *= 10;
    p_dataDelimiter /= std::pow(10, count);

    double step = p_pixelPerData * p_dataDelimiter;
    int startY =
        axisStart.y() + std::abs(p_pixelPerData * (p_curMaxValue - curValue));
    for (int i = 0; i < p_curDataDelimitersNumber; ++i) {
      int y = startY + step * i;
      painter.drawLine(QPoint{p1, y}, QPoint{p2, y});

      QRect textRect{
          QPoint{textX1, y - (p_maxTextHeight / 2 + p_maxTextHeight % 2 +
                              p_axisLineWidth)},
          QPoint{textX2, y + p_maxTextHeight / 2}};

      painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(curValue, 'f', count));

      if (i != p_curDataDelimitersNumber - 1) {
        curValue -= p_dataDelimiter;
      }
    }

    if (std::abs(p_curMinValue - curValue) >= p_dataDelimiter) {
      int y = startY + step * p_curDataDelimitersNumber;

      if (y > axisEnd.y()) return;

      curValue -= p_dataDelimiter;

      painter.drawLine(QPoint{p1, y}, QPoint{p2, y});
      QRect textRect{
          QPoint{textX1, y - (p_maxTextHeight / 2 + p_maxTextHeight % 2 +
                              p_axisLineWidth)},
          QPoint{textX2, y + p_maxTextHeight / 2}};

      painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(curValue, 'f', count));
    }
  } else if (axisType == BaseWaveform::AxisType::AxisYTop ||
             axisType == BaseWaveform::AxisType::AxisYBottom) {
    int textY1;
    int textY2;
    if (axisType == BaseWaveform::AxisType::AxisYTop) {
      textY1 = p1 - (p_textMarginBottom + p_maxTextHeight);
      textY2 = p1 - p_textMarginBottom;
    } else if (axisType == BaseWaveform::AxisType::AxisYBottom) {
      textY1 = p1 + (p_textMarginTop + p_maxTextHeight);
      textY2 = p1 + p_textMarginTop;
    }

    calculateTimeDelimiter();

    if (p_timeDelimiter < p_timeMultiples[8]) {
      p_signalData->setUnitOfTime(tr("msec"));
      p_signalData->setDivisionBase(1);
    } else if (p_timeDelimiter < p_timeMultiples[14]) {
      p_signalData->setUnitOfTime(tr("sec"));
      p_signalData->setDivisionBase(p_timeMultiples[8]);
    } else if (p_timeDelimiter < p_timeMultiples[20]) {
      p_signalData->setUnitOfTime(tr("min"));
      p_signalData->setDivisionBase(p_timeMultiples[14]);
    } else if (p_timeDelimiter < p_timeMultiples[25]) {
      p_signalData->setUnitOfTime(tr("h"));
      p_signalData->setDivisionBase(p_timeMultiples[20]);
    } else if (p_timeDelimiter < p_timeMultiples[27]) {
      p_signalData->setUnitOfTime(tr("d"));
      p_signalData->setDivisionBase(p_timeMultiples[25]);
    } else if (p_timeDelimiter < p_timeMultiples[29]) {
      p_signalData->setUnitOfTime(tr("w"));
      p_signalData->setDivisionBase(p_timeMultiples[27]);
    } else if (p_timeDelimiter < p_timeMultiples[33]) {
      p_signalData->setUnitOfTime(tr("m"));
      p_signalData->setDivisionBase(p_timeMultiples[29]);
    } else if (p_timeDelimiter <= p_timeMultiples[37]) {
      p_signalData->setUnitOfTime(tr("y"));
      p_signalData->setDivisionBase(p_timeMultiples[33]);
    }

    size_t curValue = std::floor(p_timeRange);
    curValue -= curValue % p_timeDelimiter;

    size_t step = std::round(p_pixelPerTime * p_timeDelimiter);
    int startX =
        axisEnd.x() - std::abs(p_pixelPerTime * (p_timeRange - curValue));
    for (int i = 0; i < p_curTimeDelimitersNumber; ++i) {
      int x = startX - step * i;
      painter.drawLine(QPoint{x, p1}, QPoint{x, p2});

      QRect textRect;
      if (axisType == BaseWaveform::AxisType::AxisYTop) {
        textRect = {QPoint{x - (p_maxAxisTextWidth / 2 +
                                p_maxAxisTextWidth % 2 + p_axisLineWidth),
                           textY1},
                    QPoint{x + p_maxAxisTextWidth / 2, textY2}};
      } else if (axisType == BaseWaveform::AxisType::AxisYBottom) {
        textRect = {QPoint{x - (p_maxAxisTextWidth / 2 +
                                p_maxAxisTextWidth % 2 + p_axisLineWidth),
                           textY1},
                    QPoint{x + p_maxAxisTextWidth / 2, textY2}};
      }

      painter.drawText(
          textRect, Qt::AlignHCenter | Qt::AlignVCenter,
          QString::number(curValue / p_signalData->divisionBase()) +
              p_signalData->unitOfTime());

      if (i != p_curTimeDelimitersNumber - 1) {
        curValue -= p_timeDelimiter;
      }
    }

    if (std::abs(p_curMinValue - curValue) >= p_timeDelimiter) {
      int x = startX - step * p_curTimeDelimitersNumber;

      if (x < axisStart.x()) return;

      curValue -= p_timeDelimiter;

      if (!curValue) return;

      painter.drawLine(QPoint{x, p1}, QPoint{x, p2});
      QRect textRect{QPoint{x - (p_maxAxisTextWidth / 2 +
                                 p_maxAxisTextWidth % 2 + p_axisLineWidth),
                            textY1},
                     QPoint{x + p_maxAxisTextWidth / 2, textY2}};

      painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                       QString::number(curValue));
    }
  }
}

void BaseWaveform::drawName(BaseWaveform::NameType nameType) {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  QPainter painter(&p_image);
  painter.setPen(p_mainColor);
  painter.setFont(p_font);

  QRect textRect;

  switch (nameType) {
    case BaseWaveform::NameType::VerticalLeft: {
      textRect = {p_paddingLeft, p_paddingTop, p_maxTextHeight,
                  p_height - (p_paddingTop + p_paddingBottom)};

      painter.translate(textRect.center());
      painter.rotate(-90);
      painter.translate(-textRect.height() / 2 - p_paddingLeft,
                        -textRect.width() / 2 - p_paddingTop);

      textRect.setWidth(p_height - (p_paddingTop + p_paddingBottom));
      textRect.setHeight(p_maxTextHeight * 2);

      break;
    }
    case BaseWaveform::NameType::HorizontalBottom: {
      textRect = {p_paddingLeft, p_height - (p_maxTextHeight + p_paddingBottom),
                  p_width - (p_paddingRight + p_paddingLeft), p_maxTextHeight};
    }
  }

  painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap,
                   p_signalData->channelsName()[p_number]);
}

void BaseWaveform::drawBresenham() {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  int localWidth =
      p_width - (p_offsetLeft + p_offsetRight + p_paddingLeft + p_paddingRight);
  int localHeight = p_height - (p_offsetTop + p_offsetBottom + p_paddingTop +
                                p_paddingBottom);

  double scale = localHeight / (p_dataRange + 1);

  for (int i = 0; i < p_arrayRange - 1; ++i) {
    int x1 = std::round(i * localWidth / p_arrayRange) + p_offsetLeft +
             p_paddingLeft;
    int x2 = std::round((i + 1) * localWidth / p_arrayRange) + p_offsetLeft +
             p_paddingLeft;
    int y1 =
        localHeight -
        std::floor(
            (p_signalData->data()[p_number][i + p_signalData->leftArray()] -
             p_curMinValue) *
            scale) +
        p_offsetTop + p_paddingTop;

    int y2 =
        localHeight -
        std::floor(
            (p_signalData->data()[p_number][i + p_signalData->leftArray() + 1] -
             p_curMinValue) *
            scale) +
        p_offsetTop + p_paddingTop;

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
      int x = x1;
      int y = y1;

      QRgb *line = reinterpret_cast<QRgb *>(p_image.scanLine(y));

      QRgb &pixel = line[x];
      pixel = p_graphColor.rgb();

      if (x == x2 && y == y2) {
        break;
      }

      int err2 = 2 * err;

      if (err2 > -dy) {
        err -= dy;
        x1 += sx;
      }

      if (err2 < dx) {
        err += dx;
        y1 += sy;
      }
    }
  }
}

void BaseWaveform::showWaveform() {
  QPixmap pixmap = QPixmap::fromImage(p_image);
  setPixmap(pixmap);
}

void BaseWaveform::calculateDataDelimiter() {
  for (int i = 0; i < p_dataMultiples.size(); ++i) {
    p_curDataDelimitersNumber = p_dataRange / p_dataMultiples[i];

    if (p_curDataDelimitersNumber <= p_xLabelsNumber) {
      p_dataDelimiter = p_dataMultiples[i];
      break;
    }
  }
}

void BaseWaveform::calculateTimeDelimiter() {
  for (int i = 0; i < p_timeMultiples.size(); ++i) {
    p_curTimeDelimitersNumber = p_timeRange / p_timeMultiples[i];

    if (p_curTimeDelimitersNumber <= p_yLabelsNumber) {
      p_timeDelimiter = p_timeMultiples[i];
      break;
    }
  }
}

}  // namespace fssp
