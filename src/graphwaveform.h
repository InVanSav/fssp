#pragma once

#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>
#include <QWidget>

#include "basewaveform.h"

namespace fssp {

class GraphWaveform : public BaseWaveform {
  Q_OBJECT
 public:
  explicit GraphWaveform(std::shared_ptr<SignalData> signalData, int number,
                         QWidget *parent = nullptr);

  void drawWaveform() override;

  void updateRelative();

  void setTop();
  void setMiddle();
  void setBottom();

 public slots:
  void onChangedEnableGrid();
  void onChangedGraphTimeRange();
  void onChangedGlobalScale();

 protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

  void showContextMenu(const QPoint &pos);

  void paintEvent(QPaintEvent *event) override;

 private:
  void showToolTip(QMouseEvent *event);
  bool validateToolTipPoint(QMouseEvent *event);

  void initSelection(QMouseEvent *event);

  void drawName();
  void drawAxisY();
  void drawAxisTopX();
  void drawAxisBottomX();
  void drawGrid();

  double m_pixelPerData;
  double m_pixelPerTime;

  double m_dataPerPixel;
  double m_timePerPixel;

  bool m_isTop;
  bool m_isBottom;

  bool m_isSelected;
  bool m_isCtrlPressed;
  bool m_isToolTipShow;

  QPoint m_startPoint;
  QRect m_selectionRect;
  QPoint m_toolTipPoint;
};

}  // namespace fssp
