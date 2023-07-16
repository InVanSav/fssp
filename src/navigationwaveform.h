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
  void onChangedGraphTimeRange();

 protected:
  void mousePressEvent(QMouseEvent *event) override;
  void showContextMenu(const QPoint &pos);

  void paintEvent(QPaintEvent *event) override;

 private:
  void changeVisibilityAction(bool visible);

 private:
  void drawName();

  bool m_isVisible;

  QRect m_selectionRect;
};

}  // namespace fssp
