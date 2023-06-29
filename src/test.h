#pragma once

#include <QWidget>

#include "basewaveform.h"

namespace fssp {

class test : public BaseWaveform {
  Q_OBJECT
 public:
  explicit test(std::shared_ptr<SignalData> signalData, int number,
                QWidget *parent = nullptr);

  void drawWaveform() override;
};

}  // namespace fssp
