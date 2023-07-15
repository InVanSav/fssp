#include "basespectrum.h"

#include <QPainter>

#include "qnamespace.h"

namespace fssp {

BaseSpectrum::BaseSpectrum(std::shared_ptr<SignalData> signalData, int number,
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

  p_leftFreq = 0;
  p_rightFreq = p_signalData->rate() / 2;

  p_signalData->setLeftArray(0);
  p_signalData->setRightArray(p_signalData->samplesNumber() - 1);

  p_signalData->setGridEnabled(true);
  p_signalData->setGlobalScale(false);
}

int BaseSpectrum::number() const { return p_number; }

void BaseSpectrum::setWidth(int width) {
  if (width > p_minWidth) {
    p_width = width;
  }
}

void BaseSpectrum::setheight(int height) {
  if (height > p_minHeight) {
    p_height = height;
  }
}

void BaseSpectrum::setPadding(int left, int right, int top, int bottom) {
  p_paddingLeft = left;
  p_paddingRight = right;
  p_paddingTop = top;
  p_paddingBottom = bottom;
}

void BaseSpectrum::setOffset(int left, int right, int top, int bottom) {
  p_offsetLeft = left;
  p_offsetRight = right;
  p_offsetTop = top;
  p_offsetBottom = bottom;
}

void BaseSpectrum::setTextMargin(int left, int right, int top, int bottom) {
  p_textMarginLeft = left;
  p_textMarginRight = right;
  p_textMarginTop = top;
  p_textMarginBottom = bottom;

  p_maxTextHeight =
      QFontMetrics(p_font).height() + p_textMarginTop + p_textMarginBottom;
  p_maxAxisTextWidth = QFontMetrics(p_font).averageCharWidth() * 11 +
                       p_textMarginLeft + p_textMarginRight;
}

void BaseSpectrum::updateRelative() {
  p_arrayRange = p_signalData->rightArray() - p_signalData->leftArray() + 1;

  p_freqRange = p_rightFreq - p_leftFreq;

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

  p_pixelPerFreq = ((p_width - (p_offsetRight + p_paddingRight)) -
                    (p_offsetLeft + p_paddingLeft)) /
                   static_cast<double>(p_freqRange);

  p_freqPerPixel = 1 / p_pixelPerFreq;

  p_dataPerPixel = 1 / p_pixelPerData;
}

bool BaseSpectrum::isImageNull() const { return p_image.isNull(); }

void BaseSpectrum::initImage() {
  p_image = QImage(p_width, p_height, p_imageFormat);
}

void BaseSpectrum::fill(QColor color) { p_image.fill(color.toRgb()); }

void BaseSpectrum::drawGrid() {
  if (!p_signalData->isGridEnabled()) return;

  QPainter painter(&p_image);
  painter.setPen(QPen(p_gridColor, p_gridLineWidth));
  painter.setFont(p_font);

  int p1;
  int p2;

  QPoint axisStart;
  QPoint axisEnd;

  calculateDataDelimiter();
  calculateFreqDelimiter();

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

  double curValueX = std::floor(p_freqRange);
  curValueX -= std::fmod(curValueX, p_freqDelimiter);

  double stepX = std::round(p_pixelPerFreq * p_freqDelimiter);
  int startX =
      axisEnd.x() - std::abs(p_pixelPerFreq * (p_freqRange - curValueX));

  p1 = axisStart.y();
  p2 = axisStart.y() + (p_height - ((p_paddingTop + p_offsetTop) +
                                    (p_paddingBottom + p_offsetBottom)));

  for (int i = 0; i < p_curFreqDelimitersNumber; ++i) {
    int x = startX - stepX * i;
    painter.drawLine(QPoint{x, p1}, QPoint{x, p2});
  }
}

void BaseSpectrum::drawAxes(BaseSpectrum::AxisType axisType) {
  if (isImageNull()) throw BaseSpectrum::ImageIsNull();

  QPainter painter(&p_image);
  painter.setPen(QPen(p_mainColor, p_axisLineWidth));
  painter.setFont(p_font);

  QPoint axisStart;
  QPoint axisEnd;

  int p1;
  int p2;

  switch (axisType) {
    case BaseSpectrum::AxisType::AxisXLeft: {
      axisStart = {p_offsetLeft + p_paddingLeft, p_offsetTop + p_paddingTop};
      axisEnd = {p_offsetLeft + p_paddingLeft,
                 p_height - (p_offsetBottom + p_paddingBottom)};

      p1 = axisStart.x() - 5;
      p2 = axisStart.x();

      break;
    }

    case BaseSpectrum::AxisType::AxisYTop: {
      axisStart = {p_offsetLeft + p_paddingLeft, p_offsetTop + p_paddingTop};
      axisEnd = {p_width - (p_offsetRight + p_paddingRight),
                 p_offsetTop + p_paddingTop};

      p1 = axisStart.y() - 5;
      p2 = axisStart.y();

      break;
    }

    case BaseSpectrum::AxisType::AxisYBottom: {
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

  if (axisType == BaseSpectrum::AxisType::AxisXLeft) {
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
  } else if (axisType == BaseSpectrum::AxisType::AxisYTop ||
             axisType == BaseSpectrum::AxisType::AxisYBottom) {
    int textY1;
    int textY2;
    if (axisType == BaseSpectrum::AxisType::AxisYTop) {
      textY1 = p1 - (p_textMarginBottom + p_maxTextHeight);
      textY2 = p1 - p_textMarginBottom;
    } else if (axisType == BaseSpectrum::AxisType::AxisYBottom) {
      textY1 = p1 + (p_textMarginTop + p_maxTextHeight);
      textY2 = p1 + p_textMarginTop;
    }

    double curValue = std::floor(p_freqRange);
    curValue -= std::fmod(curValue, p_freqDelimiter);

    int count = 0;
    for (; p_freqDelimiter < 1; ++count) p_freqDelimiter *= 10;
    p_freqDelimiter /= std::pow(10, count);

    double step = std::round(p_pixelPerFreq * p_freqDelimiter);
    int startX =
        axisEnd.x() - std::abs(p_pixelPerFreq * (p_freqRange - curValue));
    for (int i = 0; i < p_curFreqDelimitersNumber; ++i) {
      int x = startX - step * i;
      painter.drawLine(QPoint{x, p1}, QPoint{x, p2});

      QRect textRect;
      if (axisType == BaseSpectrum::AxisType::AxisYTop) {
        textRect = {QPoint{x - (p_maxAxisTextWidth / 2 +
                                p_maxAxisTextWidth % 2 + p_axisLineWidth),
                           textY1},
                    QPoint{x + p_maxAxisTextWidth / 2, textY2}};
      } else if (axisType == BaseSpectrum::AxisType::AxisYBottom) {
        textRect = {QPoint{x - (p_maxAxisTextWidth / 2 +
                                p_maxAxisTextWidth % 2 + p_axisLineWidth),
                           textY1},
                    QPoint{x + p_maxAxisTextWidth / 2, textY2}};
      }

      painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                       QString::number(curValue, 'f', count));

      if (i != p_curFreqDelimitersNumber - 1) {
        curValue -= p_freqDelimiter;
      }
    }

    if (std::abs(p_curMinValue - curValue) >= p_freqDelimiter) {
      int x = startX - step * p_curFreqDelimitersNumber;

      if (x < axisStart.x()) return;

      curValue -= p_freqDelimiter;

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

void BaseSpectrum::drawName(BaseSpectrum::NameType nameType) {
  if (isImageNull()) throw BaseSpectrum::ImageIsNull();

  QPainter painter(&p_image);
  painter.setPen(p_mainColor);
  painter.setFont(p_font);

  QRect textRect;

  switch (nameType) {
    case BaseSpectrum::NameType::VerticalLeft: {
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
    case BaseSpectrum::NameType::HorizontalBottom: {
      textRect = {p_paddingLeft, p_height - (p_maxTextHeight + p_paddingBottom),
                  p_width - (p_paddingRight + p_paddingLeft), p_maxTextHeight};

      break;
    }
    case BaseSpectrum::NameType::HorizontalTop: {
      textRect = {p_paddingLeft, p_maxTextHeight + p_paddingTop,
                  p_width - (p_paddingRight + p_paddingLeft), p_maxTextHeight};

      break;
    }
  }

  painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap,
                   p_signalData->channelsName()[p_number]);
}

void BaseSpectrum::drawBresenham() {
  if (isImageNull()) throw BaseSpectrum::ImageIsNull();

  int localWidth =
      p_width - (p_offsetLeft + p_offsetRight + p_paddingLeft + p_paddingRight);
  int localHeight = p_height - (p_offsetTop + p_offsetBottom + p_paddingTop +
                                p_paddingBottom);

  double scale = 0;

  if (p_dataRange == 0) {
    scale = localHeight / 2;
  } else {
    scale = localHeight / p_dataRange;
  }

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

void BaseSpectrum::showWaveform() {
  QPixmap pixmap = QPixmap::fromImage(p_image);
  setPixmap(pixmap);
}

void BaseSpectrum::calculateDataDelimiter() {
  for (int i = 0; i < p_multiples.size(); ++i) {
    p_curDataDelimitersNumber = p_dataRange / p_multiples[i];

    if (p_curDataDelimitersNumber <= p_xLabelsNumber) {
      p_dataDelimiter = p_multiples[i];
      break;
    }
  }
}

void BaseSpectrum::calculateFreqDelimiter() {
  for (int i = 0; i < p_multiples.size(); ++i) {
    p_curFreqDelimitersNumber = p_freqRange / p_multiples[i];

    if (p_curFreqDelimitersNumber <= p_yLabelsNumber) {
      p_freqDelimiter = p_multiples[i];
      break;
    }
  }
}

}  // namespace fssp
