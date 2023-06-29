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

  int number() const;

  enum class AxisType {
    AxisXLeft,
    AxisYTop,
    AxisYBottom,
  };

  enum class NameType {
    VerticalLeft,
    HorizontalBottom,
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

  void drawAxes(BaseWaveform::AxisType axisType);

  void drawName(BaseWaveform::NameType nameType);

  void drawBresenham();

  void showWaveform();

  virtual void drawWaveform() = 0;

 protected:
  QImage p_image;

  QImage::Format p_imageFormat = QImage::Format_ARGB32;
  QFont p_font{"Monospace", 10};

  QColor p_mainColor{0, 0, 0};
  QColor p_graphColor{0, 127, 255};
  QColor p_gridColor{153, 153, 153};

  std::shared_ptr<SignalData> p_signalData;
  int p_number;

  int p_leftArray;
  int p_rightArray;

  int p_arrayRange;

  size_t p_timeRange;

  double p_pixelPerTime;

  double p_maxValue;
  double p_minValue;

  double p_dataRange;

  double p_pixelPerData;

  int p_xLabelsNumber;
  int p_yLabelsNumber;

  int p_lineWidth = 2;

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
};

}  // namespace fssp
