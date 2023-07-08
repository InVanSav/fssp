#pragma once

#include <QComboBox>
#include <QScrollArea>
#include <QWidget>

#include "basemodel.h"
#include "graphdialog.h"

namespace fssp {

class ModelingWindow : public QWidget {
  Q_OBJECT
 public:
  explicit ModelingWindow(QWidget *parent = nullptr);

 protected slots:
  void onComboBoxChange(int index);

 private:
  BaseModel *m_model;

  QComboBox *m_comboBox;
  QScrollArea *m_formScrollArea;

  GraphDialog *m_graphDialog;
};

}  // namespace fssp
