#pragma once

#include <QDateTimeEdit>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
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

  size_t p_leftTime;
  size_t p_rightTime;

  QDateTimeEdit *p_scaleFromValue;
  QDateTimeEdit *p_scaleToValue;

  QWidget *p_scaleForm;
  QWidget *p_scrollContent;

  QFormLayout *p_formLayout;
  QLabel *p_error;
};

}  // namespace fssp
