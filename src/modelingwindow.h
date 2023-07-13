#pragma once

#include <QComboBox>
#include <QDialog>
#include <QScrollArea>
#include <QWidget>

#include "basemodel.h"

namespace fssp {

class ModelingWindow : public QDialog {
  Q_OBJECT
 public:
  explicit ModelingWindow(std::shared_ptr<SignalData> signalData,
                          bool lockHeader = false, QWidget *parent = nullptr);

  SignalData getData() const;

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

  bool m_isHeaderLocked;
};

}  // namespace fssp
