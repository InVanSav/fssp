#pragma once

#include <QLabel>

namespace fssp {

class NavigationWaveform : public QLabel {
  Q_OBJECT
 public:
  explicit NavigationWaveform(const int number, QWidget *parent = nullptr);

  void drawWaveform(const std::vector<double> &data, bool isSelected,
                    int leftSelection, int rightSelection);

 private:
  int m_number;

  const int m_WIDTH = 200;
  const int m_HEIGHT = 50;
};

}  // namespace fssp
