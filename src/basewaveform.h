#pragma once

#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QLabel>
#include <QWidget>

#include "signaldata.h"

namespace fssp {

class BaseWaveform : public QLabel {
  Q_OBJECT
 public:
  explicit BaseWaveform(std::shared_ptr<SignalData> signalData, int number,
                        int min_width = 200, int min_height = 50,
                        QWidget *parent = nullptr);

  class ImageIsNull : public std::exception {
   public:
    virtual const char *what() const throw() { return "Image is Null"; }
  };

  int number() const;

  void setWidth(int width);

  void setHeight(int height);

  void setPadding(int left, int right, int top, int bottom);

  void setOffset(int left, int right, int top, int bottom);

  void setTextMargin(int left, int right, int top, int bottom);

  virtual void drawWaveform() = 0;

 protected:
  bool isImageNull() const;

  void initImage();

  void fill();

  void drawBresenham();

  void showWaveform();

  std::shared_ptr<SignalData> p_signalData;
  int p_number;

  std::vector<double> p_data;

  int p_leftArray;
  int p_rightArray;

  int p_arrayRange;

  double p_maxValue;
  double p_minValue;

  double p_dataRange;

  QImage p_image;

  QImage::Format p_imageFormat = QImage::Format_ARGB32;
  QFont p_font{"Monospace", 10};

  QColor p_fillColor{255, 255, 255};
  QColor p_mainColor{0, 0, 0};
  QColor p_graphColor{0, 127, 255};
  QColor p_gridColor{153, 153, 153, 100};

  int p_axisLineWidth = 2;
  int p_gridLineWidth = 1;

  int p_minWidth;
  int p_minHeight;

  int p_width;
  int p_height;

  int p_paddingLeft = 0;
  int p_paddingRight = 0;
  int p_paddingTop = 0;
  int p_paddingBottom = 0;

  int p_offsetLeft = 0;
  int p_offsetRight = 0;
  int p_offsetTop = 0;
  int p_offsetBottom = 0;

  int p_maxAxisTextSymbols = 11;

  int p_textMarginLeft = 0;
  int p_textMarginRight = 0;
  int p_textMarginTop = 0;
  int p_textMarginBottom = 0;

  int p_maxTextHeight =
      QFontMetrics(p_font).height() + p_textMarginTop + p_textMarginBottom;
  int p_maxAxisTextWidth = QFontMetrics(p_font).averageCharWidth() * 11 +
                           p_textMarginLeft + p_textMarginRight;

  int p_xLabelsNumber;
  int p_yLabelsNumber;

  const std::vector<double> p_dataMultiples = {
      0.00000001,  0.00000002,  0.00000005,   0.0000001,    0.0000002,
      0.0000005,   0.000001,    0.000002,     0.000005,     0.00001,
      0.00002,     0.00005,     0.0001,       0.0002,       0.0005,
      0.001,       0.002,       0.005,        0.01,         0.02,
      0.05,        0.1,         0.2,          0.5,          1,
      2,           5,           10,           20,           50,
      100,         200,         500,          1000,         2000,
      5000,        10000,       20000,        50000,        100000,
      200000,      500000,      1000000,      2000000,      5000000,
      10000000,    20000000,    50000000,     100000000,    200000000,
      500000000,   1000000000,  2000000000,   5000000000,   10000000000,
      20000000000, 50000000000, 100000000000, 200000000000, 500000000000};

  const std::vector<size_t> p_timeMultiples = {
      1,
      2,
      5,
      10,
      50,
      100,
      200,
      500,
      1000,  // msec 8
      2000,
      5000,
      10000,
      15000,
      30000,
      60000,  // sec 14
      120'000,
      300'000,
      600'000,
      900'000,
      1'800'000,
      3'600'000,  // min 20
      7'200'000,
      14'400'000,
      28'800'000,
      57'600'000,
      86'400'000,  // hours 25
      172'800'000,
      604'800'000,  // days 27
      1'209'600'000,
      2'592'000'000,  // weeks 29
      5'184'000'000,
      7'776'000'000,
      15'552'000'000,
      31'104'000'000,  // months 33
      62'208'000'000,
      155'520'000'000,
      311'040'000'000,
      622'080'000'000  // years 37
  };
};

}  // namespace fssp
