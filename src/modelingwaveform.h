#pragma once

#include <QWidget>
#include <QLabel>

#include "basewaveform.h"

namespace fssp {

class ModelingWaveform : public BaseWaveform {
  Q_OBJECT
 public:
  explicit ModelingWaveform(std::shared_ptr<SignalData> signalData,
                         QWidget *parent = nullptr);

  void drawWaveform() override;
};

}  // namespace fssp
