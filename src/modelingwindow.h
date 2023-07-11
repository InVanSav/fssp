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
  explicit ModelingWindow(std::shared_ptr<SignalData> signalData,
                          QWidget *parent = nullptr);

 protected slots:
  void onComboBoxChange(int index);
  void onCalcButtonPress();
  void onAddButtonPress();
  void onCancelButtonPress();

 private:
  std::shared_ptr<SignalData> p_signalData;

  BaseModel *m_model;

  QComboBox *m_comboBox;
  QScrollArea *m_formScrollArea;

  QScrollArea *m_previewScrollArea;
};

}  // namespace fssp
