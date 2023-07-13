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
  explicit SignalPage(const SignalData data, QWidget *parent = nullptr);

  std::shared_ptr<SignalData> getSignalData();

 private:
  std::shared_ptr<SignalData> m_signalData;

  NavigationDialog *m_navDialog;
  GraphDialog *m_graphDialog;

  bool m_isSelected;
  int m_leftSelection;
  int m_rightSelection;
};

}  // namespace fssp
