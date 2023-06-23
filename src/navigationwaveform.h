#pragma once

#include <QLabel>
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

 private:
  int m_number;
};

}  // namespace fssp
