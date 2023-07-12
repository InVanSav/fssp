#pragma once

#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
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
  void hideWaveforms();

 public slots:
  void onChangedWaveformVisibility();

 private:
  void enableGridAction(bool enable);
  void scaleGraphWaveformAction();
  void changeArrayGlobalizationAction();

  void pushAcceptButton();
  void pushDenyButton();

  void pushDoubleScaleButton();
  void pushResetButton();

  bool validateInputData();
  void buttonHandler();

 private:
  std::shared_ptr<SignalData> p_signalData;
  std::vector<GraphWaveform *> p_waveforms;

  size_t leftTime;
  size_t rightTime;

  QLineEdit *scaleFromValue;
  QLineEdit *scaleToValue;

  QWidget *scaleForm;
  QWidget *p_scrollContent;

  QFormLayout *formLayout;
  QLabel *error;
};

}  // namespace fssp
