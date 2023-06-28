#include "basemodel.h"

#include <limits>

namespace fssp {

BaseModel::BaseModel(QWidget *parent) : QWidget{parent} {
  createFields();

  freqSpinBox->setValue(1);
  sampleNumberSpinBox->setValue(1000);
  channelNameLineEdit->setText("Model_1");
  dateTimeEdit->setDateTime(QDateTime::currentDateTime());

  data = std::vector<double>(sampleNumberSpinBox->value());

  timeForOne = 1 / freqSpinBox->value();
  allTime = freqSpinBox->value() * sampleNumberSpinBox->value();
  endTime = dateTimeEdit->dateTime().addSecs(allTime);

  createForm();
}

BaseModel::BaseModel(const SignalData &signalData, QWidget *parent) {
  createFields();

  freqSpinBox->setValue(signalData.rate());
  sampleNumberSpinBox->setValue(signalData.samplesNumber());
  channelNameLineEdit->setText("Model_" +
                               QString::number(signalData.channelsNumber()));
  dateTimeEdit->setDateTime(QDateTime::currentDateTime());

  data = std::vector<double>(sampleNumberSpinBox->value());

  createForm();
}

void BaseModel::createFields() {
  freqSpinBox = new QDoubleSpinBox();
  freqSpinBox->setRange(0, INT_MAX);

  sampleNumberSpinBox = new QSpinBox();
  sampleNumberSpinBox->setRange(1, INT_MAX);

  channelNameLineEdit = new QLineEdit();
  dateTimeEdit = new QDateTimeEdit();
}

void BaseModel::createForm() {
  formLayout = new QFormLayout(this);
  formLayout->setVerticalSpacing(15);
  formLayout->setHorizontalSpacing(15);
  formLayout->addRow(tr("Sampling frequency:"), freqSpinBox);
  formLayout->addRow(tr("Sample's number:"), sampleNumberSpinBox);
  formLayout->addRow(tr("Channel name:"), channelNameLineEdit);
  formLayout->addRow(tr("Date and time:"), dateTimeEdit);
}

SignalData BaseModel::getData() {
  timeForOne = 1 / freqSpinBox->value();
  allTime = freqSpinBox->value() * sampleNumberSpinBox->value();
  endTime = dateTimeEdit->dateTime().addSecs(allTime);

  return SignalData(dateTimeEdit->dateTime(), endTime, freqSpinBox->value(),
                    timeForOne, allTime, {channelNameLineEdit->text()}, {data});
}

}  // namespace fssp
