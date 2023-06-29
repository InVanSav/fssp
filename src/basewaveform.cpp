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

  p_leftArray = 0;
  p_rightArray = p_signalData->samplesNumber() - 1;
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
}

void BaseWaveform::updateRelative() {
  p_arrayRange = p_rightArray - p_leftArray + 1;

  p_timeRange = p_arrayRange * p_signalData->timeForOne() * 1000;

  p_minValue =
      *std::min_element(p_signalData->data()[p_number].begin() + p_leftArray,
                        p_signalData->data()[p_number].begin() + p_rightArray);
  p_maxValue =
      *std::max_element(p_signalData->data()[p_number].begin() + p_leftArray,
                        p_signalData->data()[p_number].begin() + p_rightArray);

  p_dataRange = std::abs(p_maxValue - p_minValue);

  p_xLabelsNumber = ((p_height - (p_offsetBottom + p_paddingBottom)) -
                     (p_offsetTop + p_paddingTop)) /
                    p_maxTextHeight;

  if (p_xLabelsNumber > p_dataRange) {
    p_xLabelsNumber =
        std::floor(p_dataRange) >= 2 ? std::floor(p_dataRange) : 2;
  }

  p_yLabelsNumber = ((p_width - (p_offsetRight + p_paddingRight)) -
                     (p_offsetLeft + p_paddingLeft)) /
                    p_maxAxisTextWidth;

  p_pixelPerData = ((p_height - (p_offsetBottom + p_paddingBottom)) -
                    (p_offsetTop + p_paddingTop)) /
                   p_dataRange;

  p_pixelPerTime = ((p_width - (p_offsetRight + p_paddingRight)) -
                    (p_offsetLeft + p_paddingLeft)) /
                   static_cast<double>(p_timeRange);
}

bool BaseWaveform::isImageNull() const { return p_image.isNull(); }

void BaseWaveform::initImage() {
  p_image = QImage(p_width, p_height, p_imageFormat);
}

void BaseWaveform::fill(QColor color) { p_image.fill(color.toRgb()); }

void BaseWaveform::drawGrid() {}

void BaseWaveform::drawAxes(BaseWaveform::AxisType axisType) {
  if (isImageNull()) {
    throw BaseWaveform::ImageIsNull();
  }

  std::vector<int> dataMultiples = {
      1,         2,         5,         10,        20,       50,
      100,       200,       500,       1000,      2000,     5000,
      10000,     20000,     50000,     100000,    200000,   500000,
      1000000,   2000000,   5000000,   10000000,  20000000, 50000000,
      100000000, 200000000, 500000000, 1000000000};

  std::vector<int> timeMultiples = {500,     1000,    2000,    5000,   10000,
                                    30000,   60000,   120000,  300000, 600000,
                                    1800000, 3600000, 86400000};

  QPainter painter(&p_image);
  painter.setPen(QPen(p_mainColor, p_lineWidth));
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
    if (p_xLabelsNumber == 2) {
      painter.drawLine(QPoint{p1, axisStart.y()}, QPoint{p2, axisStart.y()});
      painter.drawLine(QPoint{p1, axisEnd.y()}, QPoint{p2, axisEnd.y()});

      QRect textRect{
          QPoint{textX1, axisStart.y() - (p_maxTextHeight / 2 +
                                          p_maxTextHeight % 2 + p_lineWidth)},
          QPoint{textX2, axisStart.y() + p_maxTextHeight / 2}};

      painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(p_maxValue));

      textRect.setTopLeft(
          QPoint{textX1, axisEnd.y() - (p_maxTextHeight / 2 +
                                        p_maxTextHeight % 2 + p_lineWidth)});

      textRect.setBottomRight(
          QPoint{textX2, axisEnd.y() + p_maxTextHeight / 2});

      painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(p_minValue));

      if (p_minValue < 0 && p_maxValue > 0) {
        int y0 =
            p_offsetTop + p_paddingTop + p_pixelPerData * std::abs(p_maxValue);

        if (y0 - axisStart.y() < p_maxTextHeight * 1.5 ||
            axisEnd.y() - y0 < p_maxTextHeight * 1.5) {
          return;
        }

        textRect.setTopLeft(QPoint{
            textX1,
            y0 - (p_maxTextHeight / 2 + p_maxTextHeight % 2 + p_lineWidth)});

        textRect.setBottomRight(QPoint{textX2, y0 + p_maxTextHeight / 2});

        painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, "0");

        painter.drawLine(QPoint{p1, y0}, QPoint{p2, y0});
      }

      return;
    }

    int curDelimitersNumber = 1;
    int delimiter = dataMultiples[0];
    for (int i = 0; i < dataMultiples.size(); ++i) {
      curDelimitersNumber = p_dataRange / dataMultiples[i];

      if (curDelimitersNumber <= p_xLabelsNumber) {
        delimiter = dataMultiples[i];
        break;
      }
    }

    int curValue = std::floor(p_maxValue);
    if (p_maxValue > 0) {
      while (curValue % delimiter) --curValue;
    } else {
      while (std::abs(curValue) % delimiter) ++curValue;
    }

    int step = std::round(p_pixelPerData * delimiter);
    int startY =
        axisStart.y() + std::abs(p_pixelPerData * (p_maxValue - curValue));
    for (int i = 0; i < curDelimitersNumber; ++i) {
      int y = startY + step * i;
      painter.drawLine(QPoint{p1, y}, QPoint{p2, y});

      QRect textRect{QPoint{textX1, y - (p_maxTextHeight / 2 +
                                         p_maxTextHeight % 2 + p_lineWidth)},
                     QPoint{textX2, y + p_maxTextHeight / 2}};

      painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(curValue));

      if (i != curDelimitersNumber - 1) {
        curValue -= delimiter;
      }
    }

    if (std::abs(p_minValue - curValue) >= delimiter) {
      int y = startY + step * curDelimitersNumber;

      if (y > axisEnd.y()) return;

      curValue -= delimiter;

      painter.drawLine(QPoint{p1, y}, QPoint{p2, y});
      QRect textRect{QPoint{textX1, y - (p_maxTextHeight / 2 +
                                         p_maxTextHeight % 2 + p_lineWidth)},
                     QPoint{textX2, y + p_maxTextHeight / 2}};

      painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(curValue));
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

    int curDelimitersNumber = 1;
    int delimiter = timeMultiples[0];
    for (int i = 0; i < timeMultiples.size(); ++i) {
      curDelimitersNumber = p_timeRange / timeMultiples[i];

      if (curDelimitersNumber <= p_yLabelsNumber) {
        delimiter = timeMultiples[i];
        break;
      }
    }

    QString unitOfTime;
    int divisionBase;
    if (delimiter < 10000) {
      unitOfTime = "ms";
      divisionBase = 1;
    } else if (delimiter < 60000) {
      unitOfTime = "s";
      divisionBase = 1000;
    } else if (delimiter < 3600000) {
      unitOfTime = "m";
      divisionBase = 60000;
    } else if (delimiter < 86400000) {
      unitOfTime = "h";
      divisionBase = 3600000;
    }

    int curValue = std::floor(p_signalData->allTime() * 1000);
    while (curValue % delimiter) --curValue;

    int step = std::round(p_pixelPerTime * delimiter);
    int startX =
        axisEnd.x() - std::abs(p_pixelPerTime *
                               ((p_signalData->allTime() * 1000) - curValue));
    for (int i = 0; i < curDelimitersNumber; ++i) {
      int x = startX - step * i;
      painter.drawLine(QPoint{x, p1}, QPoint{x, p2});

      QRect textRect;
      if (axisType == BaseWaveform::AxisType::AxisYTop) {
        textRect = {QPoint{x - (p_maxAxisTextWidth / 2 +
                                p_maxAxisTextWidth % 2 + p_lineWidth),
                           textY1},
                    QPoint{x + p_maxAxisTextWidth / 2, textY2}};
      } else if (axisType == BaseWaveform::AxisType::AxisYBottom) {
        textRect = {QPoint{x - (p_maxAxisTextWidth / 2 +
                                p_maxAxisTextWidth % 2 + p_lineWidth),
                           textY1},
                    QPoint{x + p_maxAxisTextWidth / 2, textY2}};
      }

      painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                       QString::number(curValue / divisionBase) + unitOfTime);

      if (i != curDelimitersNumber - 1) {
        curValue -= delimiter;
      }
    }

    if (std::abs(p_minValue - curValue) >= delimiter) {
      int x = startX - step * curDelimitersNumber;

      if (x < axisStart.x()) return;

      curValue -= delimiter;

      painter.drawLine(QPoint{x, p1}, QPoint{x, p2});
      QRect textRect{QPoint{x - (p_maxAxisTextWidth / 2 +
                                 p_maxAxisTextWidth % 2 + p_lineWidth),
                            textY1},
                     QPoint{x + p_maxAxisTextWidth / 2, textY2}};

      painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
                       QString::number(curValue));
    }
  }
}

void BaseWaveform::drawName(BaseWaveform::NameType nameType) {
  if (isImageNull()) {
    throw BaseWaveform::ImageIsNull();
  }

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
  if (isImageNull()) {
    throw BaseWaveform::ImageIsNull();
  }

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
    int y1 = localHeight -
             std::floor((p_signalData->data()[p_number][i + p_leftArray] -
                         p_minValue) *
                        scale);
    int y2 = localHeight -
             std::floor((p_signalData->data()[p_number][i + p_leftArray + 1] -
                         p_minValue) *
                        scale);

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
      int x = x1;
      int y = localHeight - y1 + p_offsetTop + p_paddingTop;

      QRgb *line = reinterpret_cast<QRgb *>(p_image.scanLine(y));

      QRgb &pixel = line[x];
      pixel = p_graphColor.rgb();

      if (x == x2 && y == (localHeight - y2 + p_offsetTop + p_paddingTop)) {
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

}  // namespace fssp
