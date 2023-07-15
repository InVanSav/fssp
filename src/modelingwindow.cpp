#include "modelingwindow.h"

#include <QGroupBox>
#include <QPushButton>

#include "modelingwaveform.h"
#include "signalmodels.h"

namespace fssp {

ModelingWindow::ModelingWindow(std::shared_ptr<SignalData> signalData,
                               bool lockHeader, QWidget *parent)
    : QDialog{parent} {
  p_signalData = signalData;
  m_isHeaderLocked = lockHeader;

  setWindowTitle(tr("Modeling"));

  QGroupBox *formGroupBox = new QGroupBox(tr("Model"));

  m_comboBox = new QComboBox();
  m_comboBox->addItem(tr("Delayed single impulse"));
  m_comboBox->addItem(tr("Delayed single jump"));
  m_comboBox->addItem(tr("Discrete decreasing expnential"));
  m_comboBox->addItem(tr("Discrete decreasing sin"));
  m_comboBox->addItem(tr("Rectangle grid"));
  m_comboBox->addItem(tr("'Saw'"));
  m_comboBox->addItem(tr("Exponential envelope model"));
  m_comboBox->addItem(tr("Balance envelope model"));
  m_comboBox->addItem(tr("Tonal envelope model"));
  m_comboBox->addItem(tr("Linear envelope model"));
  m_comboBox->addItem(tr("White noise model"));
  m_comboBox->addItem(tr("Normal white noise model"));
  m_comboBox->addItem(tr("Moving average autoregress model"));

  connect(m_comboBox, &QComboBox::currentIndexChanged, this,
          &ModelingWindow::onComboBoxChange);

  m_model = new DelayedSingleImpulseModel(p_signalData);
  if (m_isHeaderLocked) {
    m_model->lockHeader();
  }

  m_formScrollArea = new QScrollArea();
  m_formScrollArea->setFrameShape(QFrame::NoFrame);
  m_formScrollArea->setWidgetResizable(true);
  m_formScrollArea->setWidget(m_model);

  QPushButton *calculateButton = new QPushButton(tr("Calculate"));
  connect(calculateButton, &QPushButton::clicked, this,
          &ModelingWindow::onCalcButtonPress);

  QVBoxLayout *formLayout = new QVBoxLayout();
  formLayout->addWidget(m_comboBox);
  formLayout->addSpacing(10);
  formLayout->addWidget(m_formScrollArea);
  formLayout->addSpacing(10);
  formLayout->addWidget(calculateButton);
  formLayout->setAlignment(calculateButton, Qt::AlignCenter);

  formGroupBox->setLayout(formLayout);
  formGroupBox->setMaximumHeight(350);

  m_previewScrollArea = new QScrollArea();
  m_previewScrollArea->setFrameShape(QFrame::NoFrame);
  m_previewScrollArea->setWidgetResizable(true);

  m_model->calc();
  std::shared_ptr<SignalData> modelSignalData =
      std::make_shared<SignalData>(std::move(m_model->getData()));

  ModelingWaveform *waveform = new ModelingWaveform(modelSignalData);
  waveform->drawWaveform();

  QVBoxLayout *waveformLayout = new QVBoxLayout();
  waveformLayout->addWidget(waveform);
  waveformLayout->setAlignment(waveform, Qt::AlignCenter);

  QWidget *scrollWigdet = new QWidget();
  scrollWigdet->setLayout(waveformLayout);

  m_previewScrollArea->setWidget(scrollWigdet);

  QGroupBox *previewGroupBox = new QGroupBox(tr("Preview"));
  QVBoxLayout *previewLayout = new QVBoxLayout();
  previewLayout->addWidget(m_previewScrollArea);
  previewGroupBox->setLayout(previewLayout);

  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  connect(cancelButton, &QPushButton::clicked, this,
          &ModelingWindow::onCancelButtonPress);

  QPushButton *addButton = new QPushButton(tr("Add"));
  connect(addButton, &QPushButton::clicked, this,
          &ModelingWindow::onAddButtonPress);

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(addButton);
  buttonLayout->setAlignment(addButton, Qt::AlignVCenter | Qt::AlignLeft);
  buttonLayout->addWidget(cancelButton);
  buttonLayout->setAlignment(cancelButton, Qt::AlignVCenter | Qt::AlignRight);

  QWidget *buttonWidget = new QWidget();
  buttonWidget->setLayout(buttonLayout);
  buttonWidget->setMinimumHeight(buttonWidget->sizeHint().height());
  buttonWidget->setMaximumHeight(buttonWidget->sizeHint().height());

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(previewGroupBox);
  mainLayout->addSpacing(10);
  mainLayout->addWidget(formGroupBox);
  mainLayout->addSpacing(10);
  mainLayout->addWidget(buttonWidget);

  setLayout(mainLayout);

  setMinimumWidth(1000);
  setMinimumHeight(800);
}

SignalData ModelingWindow::getData() const { return m_model->getData(); }

void ModelingWindow::onComboBoxChange(int index) {
  switch (index) {
    case 0: {
      m_model = new DelayedSingleImpulseModel(p_signalData);
      break;
    }
    case 1: {
      m_model = new DelayedSingleJumpModel(p_signalData);
      break;
    }
    case 2: {
      m_model = new DiscretDecreasingExpModel(p_signalData);
      break;
    }
    case 3: {
      m_model = new DiscretSinModel(p_signalData);
      break;
    }
    case 4: {
      m_model = new RectGridModel(p_signalData);
      break;
    }
    case 5: {
      m_model = new SawModel(p_signalData);
      break;
    }
    case 6: {
      m_model = new ExpEnvelopeModel(p_signalData);
      break;
    }
    case 7: {
      m_model = new BalanceEnvelopeModel(p_signalData);
      break;
    }
    case 8: {
      m_model = new TonalEnvelopeModel(p_signalData);
      break;
    }
    case 9: {
      m_model = new LinearFreqModulationModel(p_signalData);
      break;
    }
    case 10: {
      m_model = new WhiteNoiseModel(p_signalData);
      break;
    }
    case 11: {
      m_model = new NormalWhiteNoiseModel(p_signalData);
      break;
    }
    case 12: {
      m_model = new MovingAverageAutoregressModel(p_signalData);
      break;
    }
  }

  if (m_isHeaderLocked) {
    m_model->lockHeader();
  }

  m_formScrollArea->setWidget(m_model);
}

void ModelingWindow::onCalcButtonPress() {
  m_model->updateChannelName();
  m_model->calc();
  std::shared_ptr<SignalData> modelSignalData =
      std::make_shared<SignalData>(std::move(m_model->getData()));

  ModelingWaveform *waveform = new ModelingWaveform(modelSignalData);
  waveform->drawWaveform();

  QVBoxLayout *waveformLayout = new QVBoxLayout();
  waveformLayout->addWidget(waveform);
  waveformLayout->setAlignment(waveform, Qt::AlignCenter);

  QWidget *scrollWigdet = new QWidget();
  scrollWigdet->setLayout(waveformLayout);

  m_previewScrollArea->setWidget(scrollWigdet);
}

void ModelingWindow::onAddButtonPress() {
  m_model->calc();
  accept();
}

void ModelingWindow::onCancelButtonPress() { reject(); }

}  // namespace fssp
