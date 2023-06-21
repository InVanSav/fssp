#include "utils.h"

namespace fssp {

void bresenhamDraw(const std::vector<double> &data, QImage &image,
                   const int width, const int height, const int offsetStartX,
                   const int offsetStartY, const int offsetEndX,
                   const int offsetEndY, const QRgb &color) {
  int numSamples = data.size();
  int localWidth = width - (offsetStartX + offsetEndX);
  int localHeight = height - (offsetStartY + offsetEndY);

  double minVal = *std::min_element(data.begin(), data.end());
  double maxVal = *std::max_element(data.begin(), data.end());
  double range = maxVal - minVal;
  double scale = localHeight / range;

  for (int i = 0; i < numSamples - 1; ++i) {
    int x1 = std::round(i * localWidth / numSamples) + offsetStartX;
    int x2 = std::round((i + 1) * localWidth / numSamples) + offsetStartX;
    int y1 = localHeight - std::round((data[i] - minVal) * scale);
    int y2 = localHeight - std::round((data[i + 1] - minVal) * scale);

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
      int x = x1;
      int y = localHeight - y1 + offsetStartY;

      int byteOffset = y * image.bytesPerLine() + offsetStartX * 4;

      uchar *pixel = image.bits() + byteOffset;
      *static_cast<int *>(static_cast<void *>(pixel)) = color;

      if (x == x2 && y == (localHeight - y2)) {
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

}  // namespace fssp
