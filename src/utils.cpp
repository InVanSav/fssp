#include "utils.h"

namespace fssp {

void bresenhamDraw(const std::vector<double> &data, int from, int to,
                   QImage &image, const int width, const int height,
                   const int offsetStartX, const int offsetStartY,
                   const int offsetEndX, const int offsetEndY,
                   const QRgb &color) {
  int numSamples = to - from;
  int localWidth = width - (offsetStartX + offsetEndX);
  int localHeight = height - (offsetStartY + offsetEndY);

  double minVal = *std::min_element(data.begin(), data.end());
  double maxVal = *std::max_element(data.begin(), data.end());
  double range = maxVal - minVal + 1;
  double scale = localHeight / range;

  for (int i = 0; i < numSamples; ++i) {
    int x1 = std::round(i * localWidth / numSamples) + offsetStartX;
    int x2 = std::round((i + 1) * localWidth / numSamples) + offsetStartX;
    int y1 = localHeight - std::floor((data[i + from] - minVal) * scale);
    int y2 = localHeight - std::floor((data[i + from + 1] - minVal) * scale);

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
      int x = x1;
      int y = localHeight - y1 + offsetStartY;

      QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));

      QRgb &pixel = line[x];
      pixel = color;

      if (x == x2 && y == (localHeight - y2 + offsetStartY)) {
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
