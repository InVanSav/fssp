#include "modelingwindow.h"

#include <QGroupBox>
#include <QPushButton>

#include "modelingwaveform.h"
#include "signalmodels.h"

namespace fssp {

ModelingWindow::ModelingWindow(std::shared_ptr<SignalData> signalData,
                               QWidget *parent)
    : QWidget{parent} {
  p_signalData = signalData;

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

  m_formScrollArea = new QScrollArea();
  m_formScrollArea->setWidget(m_model);
  m_formScrollArea->setAlignment(Qt::AlignHCenter);

  QPushButton *calculateButton = new QPushButton(tr("Calculate"));
  connect(calculateButton, &QPushButton::clicked, this,
          &ModelingWindow::onCalcButtonPress);

  QVBoxLayout *formLayout = new QVBoxLayout();
  formLayout->addWidget(m_comboBox);
  formLayout->addWidget(m_formScrollArea);
  formLayout->addSpacing(10);
  formLayout->addWidget(calculateButton);
  formLayout->setAlignment(calculateButton, Qt::AlignCenter);

  formGroupBox->setLayout(formLayout);
  formGroupBox->setFixedWidth(420);

  m_previewScrollArea = new QScrollArea();

  QGroupBox *previewGroupBox = new QGroupBox(tr("Preview"));
  QVBoxLayout *previewLayout = new QVBoxLayout();
  previewLayout->addWidget(m_previewScrollArea);
  previewLayout->setAlignment(m_previewScrollArea, Qt::AlignCenter);
  previewGroupBox->setLayout(previewLayout);

  QHBoxLayout *contentLayout = new QHBoxLayout();
  contentLayout->addWidget(formGroupBox);
  contentLayout->addSpacing(10);
  contentLayout->addWidget(previewGroupBox);

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

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addLayout(contentLayout);
  mainLayout->addSpacing(10);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);

  setMinimumWidth(1000);
  setMinimumHeight(400);
}

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

  m_formScrollArea->setWidget(m_model);
}

void ModelingWindow::onCalcButtonPress() {
  m_model->calc();
  std::shared_ptr<SignalData> signalData =
      std::make_shared<SignalData>(std::move(m_model->getData()));

  ModelingWaveform *waveform = new ModelingWaveform(signalData);
  waveform->drawWaveform();

  m_previewScrollArea->setWidget(waveform);
}

void ModelingWindow::onAddButtonPress() {}

void ModelingWindow::onCancelButtonPress() { close(); }

}  // namespace fssp
