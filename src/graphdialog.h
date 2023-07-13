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

  std::shared_ptr<SignalData> m_signalData;
  std::vector<GraphWaveform *> m_waveforms;

  size_t m_leftTime;
  size_t m_rightTime;

  QDateTimeEdit *m_scaleFromValue;
  QDateTimeEdit *m_scaleToValue;

  QWidget *m_scaleForm;
  QWidget *m_scrollContent;

  QScrollArea *m_scrollArea;

  QFormLayout *m_formLayout;
  QLabel *m_error;
};

}  // namespace fssp
