#pragma once

#include <QImage>
#include <cmath>
#include <vector>

namespace fssp {

void bresenhamDraw(const std::vector<double> &data, QImage &image,
                   const int width, const int height, const int offsetStartX,
                   const int offsetStartY, const int offsetEndX,
                   const int offsetEndY, const QRgb &color);

}  // namespace fssp
