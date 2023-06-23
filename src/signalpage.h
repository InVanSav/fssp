#pragma once

#include <QHBoxLayout>
#include <QWidget>

#include "graphdialog.h"
#include "navigationdialog.h"
#include "signaldata.h"

namespace fssp {

class SignalPage : public QWidget {
  Q_OBJECT
 public:
  explicit SignalPage(SignalData data, QWidget *parent = nullptr);

 private slots:
  void handleSelection();

 private:
  std::shared_ptr<SignalData> m_data;

  NavigationDialog *m_navDialog;
  GraphDialog *m_graphDialog;

  bool m_isSelected;
  int m_leftSelection;
  int m_rightSelection;
};

}  // namespace fssp
