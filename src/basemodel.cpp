#include "basemodel.h"

#include <limits>

namespace fssp {
BaseModel::BaseModel(std::shared_ptr<SignalData> signalData, QWidget *parent) {
  createFields();

  p_freqSpinBox->setValue(signalData->rate());
  p_sampleNumberSpinBox->setValue(signalData->samplesNumber());
  p_channelNameLineEdit->setText("Model_" +
                                 QString::number(signalData->channelsNumber()));
  p_dateTimeEdit->setDateTime(QDateTime::currentDateTime());

  createForm();
}

void BaseModel::createFields() {
  p_freqSpinBox = new QDoubleSpinBox();
  p_freqSpinBox->setRange(0, INT_MAX);

  p_sampleNumberSpinBox = new QSpinBox();
  p_sampleNumberSpinBox->setRange(1, INT_MAX);

  p_channelNameLineEdit = new QLineEdit();
  p_dateTimeEdit = new QDateTimeEdit();
}

void BaseModel::createForm() {
  p_formLayout = new QFormLayout(this);
  p_formLayout->setVerticalSpacing(15);
  p_formLayout->setHorizontalSpacing(15);
  p_formLayout->addRow(tr("Sampling frequency:"), p_freqSpinBox);
  p_formLayout->addRow(tr("Sample's number:"), p_sampleNumberSpinBox);
  p_formLayout->addRow(tr("Channel name:"), p_channelNameLineEdit);
  p_formLayout->addRow(tr("Date and time:"), p_dateTimeEdit);
}

SignalData BaseModel::getData() {
  int allTime = p_freqSpinBox->value() * p_sampleNumberSpinBox->value();

  return SignalData(p_dateTimeEdit->dateTime(),
                    p_dateTimeEdit->dateTime().addSecs(allTime),
                    p_freqSpinBox->value(), 1 / p_freqSpinBox->value(), allTime,
                    {p_channelNameLineEdit->text()}, {std::move(p_data)});
}

QDoubleSpinBox *BaseModel::addDoubleSpinBox(const QString name,
                                            const double value,
                                            const double min,
                                            const double max) {
  QDoubleSpinBox *spinBox = new QDoubleSpinBox();

  spinBox->setRange(min, max);
  spinBox->setValue(value);

  p_formLayout->addRow(name, spinBox);

  return spinBox;
}

QSpinBox *BaseModel::addSpinBox(const QString name, const int value,
                                const int min, const int max) {
  QSpinBox *spinBox = new QSpinBox();

  spinBox->setRange(min, max);
  spinBox->setValue(value);

  p_formLayout->addRow(name, spinBox);

  return spinBox;
}

}  // namespace fssp
