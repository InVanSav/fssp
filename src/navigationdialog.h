#pragma once

#include <QWidget>

#include "signaldata.h"
#include "navigationlabel.h"

namespace fssp {

class NavigationDialog : public QWidget {
  Q_OBJECT
 public:
  explicit NavigationDialog(std::shared_ptr<SignalData> data, QWidget *parent = nullptr);

  void Draw();

 private:
  std::shared_ptr<SignalData> m_data;
  std::vector<NavigationLabel *> m_labels;

  int m_leftSelection;
  int m_rightSelection;
};

}  // namespace fssp
