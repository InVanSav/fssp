#pragma once

#include <QMenu>
#include <QMouseEvent>
#include <QPainter>

#include "basewaveform.h"

namespace fssp {

class NavigationWaveform : public BaseWaveform {
  Q_OBJECT
 public:
  explicit NavigationWaveform(std::shared_ptr<SignalData> signalData,
                              int number, QWidget *parent = nullptr);

  void drawWaveform() override;

 public slots:
  void onSelectedGraphRange(int leftX, int rightX, int realWidth);
  void onChangedGraphTimeRange(size_t leftTime, size_t rightTime);

 protected:
  void mousePressEvent(QMouseEvent *event) override;
  void showContextMenu(const QPoint &pos);

  void paintEvent(QPaintEvent *event) override;

 private:
  void changeVisibilityAction(bool visible);

 private:
  bool m_isVisible;
  bool m_isSelected;

  int m_leftX;
  int m_rightX;

  QRect m_selectionRect;
};

}  // namespace fssp
