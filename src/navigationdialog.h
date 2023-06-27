#pragma once

#include <QGroupBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "navigationwaveform.h"
#include "signaldata.h"

namespace fssp {

class NavigationDialog : public QGroupBox {
  Q_OBJECT
 public:
  explicit NavigationDialog(std::shared_ptr<SignalData> data,
                            QWidget *parent = nullptr);

  void drawWaveforms();

 private slots:
  void onWaveformVisibilityChange(int number, bool isVisible);

 private:
  std::shared_ptr<SignalData> m_data;
  std::vector<NavigationWaveform *> m_waveforms;

  QWidget *scrollContent;

  int m_waveformWidth;
  int m_waveformHeight;
};

}  // namespace fssp
