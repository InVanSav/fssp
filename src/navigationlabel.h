#pragma once

#include <QLabel>

namespace fssp {

class NavigationLabel : public QLabel {
  Q_OBJECT
 public:
  explicit NavigationLabel(const int number, QWidget *parent = nullptr);

  void Draw(const std::vector<double> &data, int leftSelection,
            int rightSelection);

 private:
  int m_number;

  const int m_WIDTH = 200;
  const int m_HEIGHT = 50;
};

}  // namespace fssp
