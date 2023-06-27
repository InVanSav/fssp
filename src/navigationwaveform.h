#pragma once

#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>

#include "utils.h"

namespace fssp {

class NavigationWaveform : public QLabel {
  Q_OBJECT
 public:
  explicit NavigationWaveform(const int number, QWidget *parent = nullptr);

  void drawWaveform(const std::vector<double> &data, int width, int height,
                    bool isSelected, int leftSelection, int rightSelection);

  int number() const;

 signals:
  void visibilityChange(int number, int isVisible);

 protected:
  void mousePressEvent(QMouseEvent *event) override;
  void showContextMenu(const QPoint &pos);

 private slots:
  void onVisibilityAction(bool isVisible);

 private:
  int m_number;

  bool m_waveformVisible;
};

}  // namespace fssp
