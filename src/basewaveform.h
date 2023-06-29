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

  virtual void drawWaveform() = 0;

  bool isImageNull() const;

  void fill(QColor color);

  void drawGrid();

  void drawAxes(BaseWaveform::AxisType axisType);

  void drawName(BaseWaveform::NameType nameType);

 protected:
  QImage p_image;

  QImage::Format p_imageFormat = QImage::Format_ARGB32;
  QFont p_font{"Monospace", 10};

  QColor mainColor{0, 0, 0};
  QColor graphColor{0, 127, 255};
  QColor gridColor{153, 153, 153};

  std::shared_ptr<SignalData> p_signalData;
  int p_number;

  int p_leftArray;
  int p_rightArray;

  int p_timeRange;

  double p_maxValue;
  double p_minValue;

  double p_dataRange;

  double p_pixelPerData;

  int p_xLabelsNumber;
  int p_yLabelsNumber;

  int p_lineWidth = 2;

  int p_stepX;
  int p_stepY;

  int p_minWidth = 900;
  int p_minHeight = 300;

  int p_width;
  int p_height;

  int p_maxNameTextSymbols = 30;
  int p_maxAxisTextSymbols = 11;

  int p_textMarginLeft = 3;
  int p_textMarginRight = 10;
  int p_textMarginTop = 10;
  int p_textMarginBottom = 1;

  int p_maxTextHeight =
      QFontMetrics(p_font).height() + p_textMarginTop + p_textMarginBottom;
  int p_maxNameTextWidth = QFontMetrics(p_font).averageCharWidth() * 30 +
                           p_textMarginLeft + p_textMarginRight;
  int p_maxAxisTextWidth = QFontMetrics(p_font).averageCharWidth() * 11 +
                           p_textMarginLeft + p_textMarginRight;

  int p_paddingLeft = 0;
  int p_paddingRight = 0;
  int p_paddingTop = 0;
  int p_paddingBottom = 0;

  int p_offsetLeft = 200;
  int p_offsetRight = 0;
  int p_offsetTop = 10;
  int p_offsetBottom = 10;
};

}  // namespace fssp
