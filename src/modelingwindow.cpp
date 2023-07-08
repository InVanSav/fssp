#include "modelingwindow.h"

#include <QGroupBox>
#include <QPushButton>

#include "signalmodels.h"

namespace fssp {

ModelingWindow::ModelingWindow(std::shared_ptr<SignalData> signalData,
                               QWidget *parent)
    : QWidget{parent} {
  p_signalData = signalData;

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

  QVBoxLayout *formlayout = new QVBoxLayout();
  formlayout->addWidget(m_comboBox);
  formlayout->addWidget(m_formScrollArea);

  formGroupBox->setLayout(formlayout);

  QPushButton *makeModelButton = new QPushButton(tr("Make model"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  QPushButton *okButton = new QPushButton(tr("Add"));

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(makeModelButton);
  buttonLayout->setAlignment(makeModelButton, Qt::AlignVCenter | Qt::AlignLeft);
  buttonLayout->addWidget(cancelButton);
  buttonLayout->setAlignment(cancelButton, Qt::AlignVCenter | Qt::AlignRight);
  buttonLayout->addWidget(okButton);
  buttonLayout->setAlignment(okButton, Qt::AlignVCenter | Qt::AlignRight);

  QWidget *buttons = new QWidget();
  buttons->setLayout(buttonLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(formGroupBox);
  mainLayout->addWidget(buttons);

  setLayout(mainLayout);

  setMinimumWidth(900);
  setMinimumHeight(300);
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

}  // namespace fssp
