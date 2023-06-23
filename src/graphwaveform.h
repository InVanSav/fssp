#pragma once

#include <QLabel>
#include <QWidget>

namespace fssp {

class GraphWaveform : public QLabel {
  Q_OBJECT
 public:
  explicit GraphWaveform(int number, QWidget *parent = nullptr);

  void drawWaveform(const std::vector<double> &data, double allTime);

 private:
  int m_number;

  const int m_WIDTH = 900;
  const int m_HEIGHT = 300;

  const int m_OFFSET_START_X = 60;
  const int m_OFFSET_START_Y = 15;
  const int m_OFFSET_END_X = 30;
  const int m_OFFSET_END_Y = 15;
};

}  // namespace fssp
