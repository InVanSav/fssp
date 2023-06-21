#pragma once

#include <QVBoxLayout>
#include <QWidget>

#include "navigationwaveform.h"
#include "signaldata.h"

namespace fssp {

class NavigationDialog : public QWidget {
  Q_OBJECT
 public:
  explicit NavigationDialog(std::shared_ptr<SignalData> data,
                            QWidget *parent = nullptr);

  void drawWaveforms();

 private:
  std::shared_ptr<SignalData> m_data;
  std::vector<NavigationWaveform *> m_waveforms;

  int m_leftSelection;
  int m_rightSelection;

  QVBoxLayout *vBox;
};

}  // namespace fssp
