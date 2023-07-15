#pragma once

#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QLabel>
#include <QWidget>

#include "signaldata.h"

namespace fssp {

class BaseSpectrum : public QLabel {
  Q_OBJECT
 public:
  explicit BaseSpectrum(std::shared_ptr<SignalData> signalData, int number,
                        int min_width = 200, int min_height = 50,
                        QWidget *parent = nullptr);

  int number() const;

  enum class AxisType {
    AxisXLeft,
    AxisYTop,
    AxisYBottom,
  };

  enum class NameType {
    VerticalLeft,
    HorizontalBottom,
    HorizontalTop,
  };

  class ImageIsNull : public std::exception {
   public:
    virtual const char *what() const throw() { return "Image is Null"; }
  };

  void setWidth(int width);
  void setheight(int height);

  void setPadding(int left, int right, int top, int bottom);

  void setOffset(int left, int right, int top, int bottom);

  void setTextMargin(int left, int right, int top, int bottom);

  void updateRelative();

  bool isImageNull() const;

  void initImage();

  void fill(QColor color);

  void drawGrid();

  void drawAxes(BaseSpectrum::AxisType axisType);
  void drawName(BaseSpectrum::NameType nameType);

  void drawBresenham();

  void showWaveform();

  virtual void drawWaveform() = 0;

 protected:
  QImage p_image;

  QImage::Format p_imageFormat = QImage::Format_ARGB32;
  QFont p_font{"Monospace", 10};

  QColor p_mainColor{0, 0, 0};
  QColor p_graphColor{0, 127, 255};
  QColor p_gridColor{153, 153, 153, 100};

  std::shared_ptr<SignalData> p_signalData;
  int p_number;

  double p_leftFreq;
  double p_rightFreq;

  double p_pixelPerFreq;
  double p_pixelPerData;

  double p_freqPerPixel;
  double p_dataPerPixel;

  double p_curMaxValue;
  double p_curMinValue;

  double p_maxValue;
  double p_minValue;

  double p_dataRange;
  int p_arrayRange;
  double p_freqRange;

  int p_xLabelsNumber;
  int p_yLabelsNumber;

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

  int p_curDataDelimitersNumber = 1;
  int p_curFreqDelimitersNumber = 1;

  const std::vector<double> p_multiples = {
      0.000001,  0.000002,  0.000005,  0.00001,   0.00002,  0.00005,  0.0001,
      0.0002,    0.0005,    0.001,     0.002,     0.005,    0.01,     0.02,
      0.05,      0.1,       0.2,       0.5,       1,        2,        5,
      10,        20,        50,        100,       200,      500,      1000,
      2000,      5000,      10000,     20000,     50000,    100000,   200000,
      500000,    1000000,   2000000,   5000000,   10000000, 20000000, 50000000,
      100000000, 200000000, 500000000, 1000000000};

  double p_dataDelimiter = p_multiples[0];
  double p_freqDelimiter = p_multiples[0];

 private:
  void calculateDataDelimiter();
  void calculateFreqDelimiter();
};

}  // namespace fssp
