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

 private:
  std::shared_ptr<SignalData> m_data;
  std::vector<NavigationWaveform *> m_waveforms;

  bool m_isSelected;
  int m_leftSelection;
  int m_rightSelection;

  QWidget *scrollContent;

  int m_waveformWidth;
  int m_waveformHeight;
};

}  // namespace fssp
