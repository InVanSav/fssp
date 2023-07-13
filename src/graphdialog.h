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

 protected slots:
  void onChangedWaveformVisibility();

  void onDataAdded();

 private:
  void addWaveforms();

  void enableGridAction(bool enable);
  void scaleGraphWaveformAction();
  void changeArrayGlobalizationAction();

  void pushAcceptButton();
  void pushDenyButton();

  void pushDoubleScaleButton();
  void pushResetButton();

  bool validateInputData();
  void buttonHandler();

  std::shared_ptr<SignalData> p_signalData;
  std::vector<GraphWaveform *> p_waveforms;

  size_t leftTime;
  size_t rightTime;

  QDateTimeEdit *scaleFromValue;
  QDateTimeEdit *scaleToValue;

  QWidget *scaleForm;
  QWidget *p_scrollContent;

  QScrollArea *m_scrollArea;

  QFormLayout *formLayout;
  QLabel *error;
};

}  // namespace fssp
