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

  p_dataRange = std::abs(p_maxValue - p_minValue);

  p_signalData->setGridEnabled(true);
  p_signalData->setGlobalScale(false);
}

int BaseWaveform::number() const { return p_number; }

void BaseWaveform::setWidth(int width) {
  if (width > p_minWidth) {
    p_width = width;
  }
}

void BaseWaveform::setHeight(int height) {
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

bool BaseWaveform::isImageNull() const { return p_image.isNull(); }

void BaseWaveform::initImage() {
  p_image = QImage(p_width, p_height, p_imageFormat);
}

void BaseWaveform::fill() { p_image.fill(p_fillColor); }

void BaseWaveform::drawBresenham() {
  if (isImageNull()) throw BaseWaveform::ImageIsNull();

  int arrayRange = p_signalData->arrayRange();

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

  for (int i = 0; i < arrayRange - 1; ++i) {
    int x1 =
        std::round(i * localWidth / arrayRange) + p_offsetLeft + p_paddingLeft;
    int x2 = std::round((i + 1) * localWidth / arrayRange) + p_offsetLeft +
             p_paddingLeft;
    int y1 =
        localHeight -
        std::floor(
            (p_signalData->data()[p_number][i + p_signalData->leftArray()] -
             p_minValue) *
            scale) +
        p_offsetTop + p_paddingTop;
    int y2 =
        localHeight -
        std::floor(
            (p_signalData->data()[p_number][i + p_signalData->leftArray() + 1] -
             p_minValue) *
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

}  // namespace fssp
