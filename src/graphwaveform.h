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

 private:
  bool m_isTop;
  bool m_isBottom;

  bool m_isSelected;
  bool m_isCtrlPressed;

  QPoint m_startPoint;
  QRect m_selectionRect;
};

}  // namespace fssp
