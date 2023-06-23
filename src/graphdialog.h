#pragma once

#include <QGroupBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "graphwaveform.h"
#include "signaldata.h"

namespace fssp {

class GraphDialog : public QGroupBox {
  Q_OBJECT
 public:
  explicit GraphDialog(std::shared_ptr<SignalData> data,
                       QWidget *parent = nullptr);

  void drawWaveforms();

 private slots:
  void onSelectionFinished(int leftX, int rightX, int realWidth);

 private:
  std::shared_ptr<SignalData> m_data;
  std::vector<GraphWaveform *> m_waveforms;

  QWidget *scrollContent;

  int m_waveformWidth;
  int m_waveformHeight;
};

}  // namespace fssp
