#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

#include "utils.h"

namespace fssp {

class GraphWaveform : public QLabel {
  Q_OBJECT
 public:
  explicit GraphWaveform(int number, QWidget *parent = nullptr);

  void drawWaveform(const std::vector<double> &data, double allTime, int width,
                    int height);

  int number() const;

 protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

  void paintEvent(QPaintEvent *event) override;

 private:
  int m_number;

  const int m_OFFSET_START_X = 60;
  const int m_OFFSET_START_Y = 15;
  const int m_OFFSET_END_X = 30;
  const int m_OFFSET_END_Y = 15;

  bool m_isSelecting;
  QPoint m_startPoint;
  QRect m_selectionRect;
};

}  // namespace fssp
