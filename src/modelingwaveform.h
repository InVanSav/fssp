#pragma once

#include <QLabel>
#include <QWidget>

#include "basewaveform.h"

namespace fssp {

class ModelingWaveform : public BaseWaveform {
  Q_OBJECT
 public:
  explicit ModelingWaveform(std::shared_ptr<SignalData> signalData,
                            QWidget *parent = nullptr);

  void drawWaveform() override;

  void updateRelative();

 private:
  void drawAxisX();
  void drawAxisY();

  void drawGrid();

  double m_pixelPerData;
  double m_pixelPerTime;
};

}  // namespace fssp
