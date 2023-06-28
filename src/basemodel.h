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

  explicit BaseModel(const SignalData &signalData, QWidget *parent = nullptr);

  SignalData getData();

  virtual void calc() = 0;

  QFormLayout *formLayout;

  QDoubleSpinBox *freqSpinBox;
  QSpinBox *sampleNumberSpinBox;
  QLineEdit *channelNameLineEdit;
  QDateTimeEdit *dateTimeEdit;

  double timeForOne;
  double allTime;
  QDateTime endTime;

  std::vector<double> data;

 private:
  void createFields();
  void createForm();
};

}  // namespace fssp
