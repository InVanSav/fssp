#pragma once

#include <QWidget>

#include "navigationdialog.h"
#include "signaldata.h"

namespace fssp {

class SignalPage : public QWidget {
  Q_OBJECT
 public:
  explicit SignalPage(SignalData &&data, QWidget *parent = nullptr);

 private:
  std::shared_ptr<SignalData> m_data;

  NavigationDialog *m_navDialog;
};

}  // namespace fssp
