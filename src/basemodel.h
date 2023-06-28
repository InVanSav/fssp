#pragma once

#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QWidget>

#include "signaldata.h"

namespace fssp {

class BaseModel : public QWidget {
  Q_OBJECT
 public:
  explicit BaseModel(QWidget *parent = nullptr);

  explicit BaseModel(std::shared_ptr<SignalData> signalData,
                     QWidget *parent = nullptr);

  SignalData getData();

  virtual void calc() = 0;

 protected:
  QFormLayout *p_formLayout;

  QDoubleSpinBox *p_freqSpinBox;
  QSpinBox *p_sampleNumberSpinBox;
  QLineEdit *p_channelNameLineEdit;
  QDateTimeEdit *p_dateTimeEdit;

  std::vector<double> p_data;

 private:
  void createFields();
  void createForm();
};

}  // namespace fssp
