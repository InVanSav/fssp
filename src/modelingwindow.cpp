#include "modelingwindow.h"

#include <QGroupBox>

#include "signalmodels.h"

namespace fssp {

ModelingWindow::ModelingWindow(QWidget *parent) : QWidget{parent} {
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

  m_model = new DelayedSingleImpulseModel();

  m_formScrollArea = new QScrollArea();
  m_formScrollArea->setWidget(m_model);

  QVBoxLayout *formlayout = new QVBoxLayout();
  formlayout->addWidget(m_comboBox);
  formlayout->addWidget(m_formScrollArea);

  setLayout(formlayout);

  setMinimumWidth(900);
  setMinimumHeight(500);
}

void ModelingWindow::onComboBoxChange(int index) {
  switch (index) {
    case 0: {
      m_model = new DelayedSingleImpulseModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
    case 1: {
      m_model = new DelayedSingleJumpModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
    case 2: {
      m_model = new DiscretDecreasingExpModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
    case 3: {
      m_model = new DiscretSinModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
    case 4: {
      m_model = new RectGridModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
    case 5: {
      m_model = new SawModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
    case 6: {
      m_model = new ExpEnvelopeModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
    case 7: {
      m_model = new BalanceEnvelopeModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
    case 8: {
      m_model = new TonalEnvelopeModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
    case 9: {
      m_model = new LinearFreqModulationModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
    case 10: {
      m_model = new WhiteNoiseModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
    case 11: {
      m_model = new NormalWhiteNoiseModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
    case 12: {
      m_model = new MovingAverageAutoregressModel();
      m_formScrollArea->setWidget(m_model);

      break;
    }
  }
}

}  // namespace fssp
