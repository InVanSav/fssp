#include "signalmodels.h"

#include <limits>

namespace fssp {

DelayedSingleImpulseModel::DelayedSingleImpulseModel(QWidget *parent)
    : BaseModel{parent} {
  initialize();
}

DelayedSingleImpulseModel::DelayedSingleImpulseModel(const SignalData &data,
                                                     QWidget *parent)
    : BaseModel{data, parent} {
  initialize();
}

void DelayedSingleImpulseModel::initialize() {
  delaySpinBox = new QSpinBox();
  delaySpinBox->setRange(INT_MIN, INT_MAX);
  delaySpinBox->setValue(333);

  p_formLayout->addRow(tr("Задержка:"), delaySpinBox);
}

void DelayedSingleImpulseModel::calc() {
  int n = delaySpinBox->value();
  p_data = std::vector<double>(n);

  for (int i = 0; i < n; ++i) {
    if (i == n) {
      p_data[i] = 1.;
    } else {
      p_data[i] = 0;
    }
  }
}

}  // namespace fssp
