#pragma once

#include <QWidget>

#include "basemodel.h"

namespace fssp {

class DelayedSingleImpulseModel : public BaseModel {
  Q_OBJECT
 public:
  explicit DelayedSingleImpulseModel(QWidget *parent = nullptr);

  explicit DelayedSingleImpulseModel(std::shared_ptr<SignalData> signalData,
                                     QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QSpinBox *delaySpinBox;
};

}  // namespace fssp
