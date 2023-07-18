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
  explicit BaseModel(std::shared_ptr<SignalData> signalData,
                     QWidget *parent = nullptr);

  SignalData getData();

  virtual void calc() = 0;

  void updateChannelName();

  void lockHeader();

 protected:
  QFormLayout *p_formLayout;

  QDoubleSpinBox *p_freqSpinBox;
  QSpinBox *p_sampleNumberSpinBox;
  QLineEdit *p_channelNameLineEdit;
  QDateTimeEdit *p_dateTimeEdit;

  QDoubleSpinBox *addDoubleSpinBox(const QString name, const double value,
                                   const double min = INT_MIN,
                                   const double max = INT_MAX);

  QSpinBox *addSpinBox(const QString name, const int value,
                       const int min = INT_MIN, const int max = INT_MAX);

  QLineEdit *addLineEdit(const QString name, const int value);

  std::vector<double> p_data;

  int number;
  int count;

 private:
  void createFields();
  void createForm();
};

}  // namespace fssp
