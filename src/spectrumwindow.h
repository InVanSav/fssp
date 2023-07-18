#pragma once

#include <QComboBox>
#include <QDateTimeEdit>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <complex>

#include "signaldata.h"
#include "spectrumwaveform.h"

namespace fssp {

typedef std::complex<double> base;

class SpectrumWindow : public QGroupBox {
  Q_OBJECT
 public:
  explicit SpectrumWindow(std::shared_ptr<SignalData> data,
                          QWidget *parent = nullptr);

  void drawWaveforms();
  void hideWaveforms();

 protected slots:
  void onChangedWaveformVisibility();
  void onChangedGraphTimeRange();

  void onDataAdded();

 private:
  void fft(std::vector<base> &a, bool invert);
  void calculate();

  void addWaveforms();

  void openSettingsAction();
  void enableGridAction(bool enable);
  void scaleGraphWaveformAction();
  void changeArrayGlobalizationAction();

  void pushSettingsAcceptButton();
  void pushSettingsCancelButton();

  void pushScaleAcceptButton();
  void pushScaleCancelButton();

  void pushDoubleScaleButton();
  void pushResetButton();

  bool validateInputData();
  void buttonHandler();

  std::shared_ptr<SignalData> m_signalData;
  std::vector<SpectrumWaveform *> m_waveforms;

  std::vector<std::vector<double>> m_spectrumData;

  double m_leftFreq;
  double m_rightFreq;

  QComboBox *m_specComboBox;
  int m_spec;
  QComboBox *m_modeComboBox;
  int m_mode;
  QComboBox *m_collisionComboBox;
  int m_collision;

  QSpinBox *m_smoothingValue;
  double m_smoothing;

  QDoubleSpinBox *m_scaleFromValue;
  QDoubleSpinBox *m_scaleToValue;

  QWidget *m_settingsForm;
  QWidget *m_scaleForm;
  QWidget *m_scrollContent;

  QScrollArea *m_scrollArea;

  QFormLayout *m_formLayout;
  QLabel *m_error;
};

}  // namespace fssp
