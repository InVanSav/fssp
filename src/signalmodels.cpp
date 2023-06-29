#include "signalmodels.h"

#include <cmath>
#include <cstdlib>
#include <limits>

#include "qspinbox.h"

namespace fssp {

//

DelayedSingleImpulseModel::DelayedSingleImpulseModel(QWidget *parent)
    : BaseModel{parent} {
  initialize();
}

DelayedSingleImpulseModel::DelayedSingleImpulseModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void DelayedSingleImpulseModel::initialize() {
  delaySpinBox = new QSpinBox();
  delaySpinBox->setRange(INT_MIN, INT_MAX);
  delaySpinBox->setValue(333);

  // Задержка
  p_formLayout->addRow(tr("Delay:"), delaySpinBox);
}

void DelayedSingleImpulseModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  int n_0 = delaySpinBox->value();
  p_data = std::vector<double>(n);

  for (int i = 0; i < n; ++i) {
    if (i == n_0) {
      p_data[i] = 1.;
    } else {
      p_data[i] = 0;
    }
  }
}

//

DelayedSingleJumpModel::DelayedSingleJumpModel(QWidget *parent)
    : BaseModel{parent} {
  initialize();
}

DelayedSingleJumpModel::DelayedSingleJumpModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void DelayedSingleJumpModel::initialize() {
  delaySpinBox = new QSpinBox();
  delaySpinBox->setRange(INT_MIN, INT_MAX);
  delaySpinBox->setValue(333);

  // Задержка
  p_formLayout->addRow(tr("Delay:"), delaySpinBox);
}

void DelayedSingleJumpModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  int n_0 = delaySpinBox->value();
  p_data = std::vector<double>(n);

  for (int i = 0; i < n; ++i) {
    if (i >= n_0) {
      p_data[i] = 1.;
    } else {
      p_data[i] = 0;
    }
  }
}

//

DiscretDecreasingExpModel::DiscretDecreasingExpModel(QWidget *parent)
    : BaseModel{parent} {
  initialize();
}

DiscretDecreasingExpModel::DiscretDecreasingExpModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void DiscretDecreasingExpModel::initialize() {
  expBaseSpinBox = new QDoubleSpinBox();
  expBaseSpinBox->setRange(INT_MIN, INT_MAX);
  expBaseSpinBox->setValue(0.997698);

  // Основание экспоненты
  p_formLayout->addRow(tr("Exponent base:"), expBaseSpinBox);
}

void DiscretDecreasingExpModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  double a = expBaseSpinBox->value();
  p_data = std::vector<double>(n);

  for (int i = 0; i < n; ++i) {
    p_data[i] = std::pow(a, i);
  }
}

//

DiscretSinModel::DiscretSinModel(QWidget *parent) : BaseModel{parent} {
  initialize();
}

DiscretSinModel::DiscretSinModel(std::shared_ptr<SignalData> signalData,
                                 QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void DiscretSinModel::initialize() {
  initPhaseSpinBox = new QDoubleSpinBox();
  initPhaseSpinBox->setRange(INT_MIN, INT_MAX);
  initPhaseSpinBox->setValue(0);

  // Начальная фаза
  p_formLayout->addRow(tr("Init phase:"), initPhaseSpinBox);

  circFreqSpinBox = new QDoubleSpinBox();
  circFreqSpinBox->setRange(INT_MIN, INT_MAX);
  circFreqSpinBox->setValue(0);

  // Круговая частота
  p_formLayout->addRow(tr("Circular frequency:"), circFreqSpinBox);

  amplitudeSpinBox = new QDoubleSpinBox();
  amplitudeSpinBox->setRange(INT_MIN, INT_MAX);
  amplitudeSpinBox->setValue(1);

  // Амплитуда
  p_formLayout->addRow(tr("Amplitude:"), amplitudeSpinBox);
}

void DiscretSinModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  p_data = std::vector<double>(n);

  double a = amplitudeSpinBox->value();
  double w = circFreqSpinBox->value();
  double p = initPhaseSpinBox->value();

  for (int i = 0; i < n; ++i) {
    p_data[i] = a * std::sin(i * w + p);
  }
}

//

RectGridModel::RectGridModel(QWidget *parent) : BaseModel{parent} {
  initialize();
}

RectGridModel::RectGridModel(std::shared_ptr<SignalData> signalData,
                             QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void RectGridModel::initialize() {
  periodSpinBox = new QDoubleSpinBox();
  periodSpinBox->setRange(INT_MIN, INT_MAX);
  periodSpinBox->setValue(66);

  // Период
  p_formLayout->addRow(tr("Period:"), periodSpinBox);
}

void RectGridModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  double L = periodSpinBox->value();
  p_data = std::vector<double>(n);

  for (int i = 0; i < n; ++i) {
    if ((i % static_cast<int>(L)) < (L / 2)) {
      p_data[i] = 1;
    } else {
      p_data[i] = -1;
    }
  }
}

//

SawModel::SawModel(QWidget *parent) : BaseModel{parent} { initialize(); }

SawModel::SawModel(std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void SawModel::initialize() {
  periodSpinBox = new QDoubleSpinBox();
  periodSpinBox->setRange(INT_MIN, INT_MAX);
  periodSpinBox->setValue(66);

  // Период
  p_formLayout->addRow(tr("Period:"), periodSpinBox);
}

void SawModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  double L = periodSpinBox->value();
  p_data = std::vector<double>(n);

  for (int i = 0; i < n; ++i) {
    p_data[i] = (i % static_cast<int>(L)) / L;
  }
}

//

ExpEnvelopeModel::ExpEnvelopeModel(QWidget *parent) : BaseModel{parent} {
  initialize();
}

ExpEnvelopeModel::ExpEnvelopeModel(std::shared_ptr<SignalData> signalData,
                                   QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void ExpEnvelopeModel::initialize() {
  envelopeWidthSpinBox = new QDoubleSpinBox();
  envelopeWidthSpinBox->setRange(INT_MIN, INT_MAX);
  envelopeWidthSpinBox->setValue(0);

  // Ширина огибающей
  p_formLayout->addRow(tr("Envelope width:"), envelopeWidthSpinBox);

  carrierFreqSpinBox = new QDoubleSpinBox();
  carrierFreqSpinBox->setRange(INT_MIN, INT_MAX);
  carrierFreqSpinBox->setValue(0);

  // Частота несущей
  p_formLayout->addRow(tr("Carrier frequency:"), carrierFreqSpinBox);

  amplitudeSpinBox = new QDoubleSpinBox();
  amplitudeSpinBox->setRange(INT_MIN, INT_MAX);
  amplitudeSpinBox->setValue(0);

  // Амплитуда
  p_formLayout->addRow(tr("Amplitude:"), amplitudeSpinBox);

  initPhaseSpinBox = new QDoubleSpinBox();
  initPhaseSpinBox->setRange(INT_MIN, INT_MAX);
  initPhaseSpinBox->setValue(0);

  // Начальная фаза
  p_formLayout->addRow(tr("Init phase:"), initPhaseSpinBox);
}

void ExpEnvelopeModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  p_data = std::vector<double>(n);

  double a = amplitudeSpinBox->value();
  double t = envelopeWidthSpinBox->value();
  double f = carrierFreqSpinBox->value();
  double p = initPhaseSpinBox->value();
  double T = p_freqSpinBox->value();
  double f_d = 1.0 / T;

  for (int i = 0; i < n; ++i) {
    p_data[i] = a * std::exp((-i * T) / t) * std::cos(2 * M_PI * f * i * T + p);
  }
}

//

BalanceEnvelopeModel::BalanceEnvelopeModel(QWidget *parent)
    : BaseModel{parent} {
  initialize();
}

BalanceEnvelopeModel::BalanceEnvelopeModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void BalanceEnvelopeModel::initialize() {
  freqEnvelopeSpinBox = new QDoubleSpinBox();
  freqEnvelopeSpinBox->setRange(INT_MIN, INT_MAX);
  freqEnvelopeSpinBox->setValue(0);

  // Частота огибающей
  p_formLayout->addRow(tr("Envelope frequency:"), freqEnvelopeSpinBox);

  carrierFreqSpinBox = new QDoubleSpinBox();
  carrierFreqSpinBox->setRange(INT_MIN, INT_MAX);
  carrierFreqSpinBox->setValue(0);

  // Частота несущей
  p_formLayout->addRow(tr("Carrier frequency:"), carrierFreqSpinBox);

  amplitudeSpinBox = new QDoubleSpinBox();
  amplitudeSpinBox->setRange(INT_MIN, INT_MAX);
  amplitudeSpinBox->setValue(0);

  // Амплитуда
  p_formLayout->addRow(tr("Amplitude:"), amplitudeSpinBox);

  initPhaseSpinBox = new QDoubleSpinBox();
  initPhaseSpinBox->setRange(INT_MIN, INT_MAX);
  initPhaseSpinBox->setValue(0);

  // Начальная фаза
  p_formLayout->addRow(tr("Init phase:"), initPhaseSpinBox);
}

void BalanceEnvelopeModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  p_data = std::vector<double>(n);

  double a = amplitudeSpinBox->value();
  double f_n = freqEnvelopeSpinBox->value();
  double f_0 = carrierFreqSpinBox->value();
  double p = initPhaseSpinBox->value();
  double T = p_freqSpinBox->value();

  for (int i = 0; i < n; ++i) {
    p_data[i] = a * std::cos(2 * M_PI * f_0 * i * T) *
                std::cos(2 * M_PI * f_n * i * T + p);
  }
}

//

TonalEnvelopeModel::TonalEnvelopeModel(QWidget *parent) : BaseModel{parent} {
  initialize();
}

TonalEnvelopeModel::TonalEnvelopeModel(std::shared_ptr<SignalData> signalData,
                                       QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void TonalEnvelopeModel::initialize() {
  freqEnvelopeSpinBox = new QDoubleSpinBox();
  freqEnvelopeSpinBox->setRange(INT_MIN, INT_MAX);
  freqEnvelopeSpinBox->setValue(0);

  // Частота огибающей
  p_formLayout->addRow(tr("Envelope frequency:"), freqEnvelopeSpinBox);

  carrierFreqSpinBox = new QDoubleSpinBox();
  carrierFreqSpinBox->setRange(INT_MIN, INT_MAX);
  carrierFreqSpinBox->setValue(0);

  // Частота несущей
  p_formLayout->addRow(tr("Carrier frequency:"), carrierFreqSpinBox);

  amplitudeSpinBox = new QDoubleSpinBox();
  amplitudeSpinBox->setRange(INT_MIN, INT_MAX);
  amplitudeSpinBox->setValue(0);

  // Амплитуда
  p_formLayout->addRow(tr("Amplitude:"), amplitudeSpinBox);

  initPhaseSpinBox = new QDoubleSpinBox();
  initPhaseSpinBox->setRange(INT_MIN, INT_MAX);
  initPhaseSpinBox->setValue(0);

  // Начальная фаза
  p_formLayout->addRow(tr("Init phase:"), initPhaseSpinBox);

  modulationDepthIndexSpinBox = new QDoubleSpinBox();
  modulationDepthIndexSpinBox->setRange(INT_MIN, INT_MAX);
  modulationDepthIndexSpinBox->setValue(0);

  // Глубина модуляции
  p_formLayout->addRow(tr("Modulation depth:"), modulationDepthIndexSpinBox);
}

void TonalEnvelopeModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  p_data = std::vector<double>(n);

  double a = amplitudeSpinBox->value();
  double f_n = freqEnvelopeSpinBox->value();
  double f_0 = carrierFreqSpinBox->value();
  double p = initPhaseSpinBox->value();
  double m = modulationDepthIndexSpinBox->value();
  double T = p_freqSpinBox->value();

  for (int i = 0; i < n; ++i) {
    p_data[i] = a * (1 + m * std::cos(2 * M_PI * f_0 * i * T)) *
                std::cos(2 * M_PI * f_n * i * T + p);
  }
}

//

LinearFreqModulationModel::LinearFreqModulationModel(QWidget *parent)
    : BaseModel{parent} {
  initialize();
}

LinearFreqModulationModel::LinearFreqModulationModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void LinearFreqModulationModel::initialize() {
  amplitudeSpinBox = new QDoubleSpinBox();
  amplitudeSpinBox->setRange(INT_MIN, INT_MAX);
  amplitudeSpinBox->setValue(0);

  p_formLayout->addRow(tr("Amplitude:"), amplitudeSpinBox);

  initPhaseSpinBox = new QDoubleSpinBox();
  initPhaseSpinBox->setRange(INT_MIN, INT_MAX);
  initPhaseSpinBox->setValue(0);

  p_formLayout->addRow(tr("Init phase:"), initPhaseSpinBox);

  startFreqSpinBox = new QDoubleSpinBox();
  startFreqSpinBox->setRange(INT_MIN, INT_MAX);
  startFreqSpinBox->setValue(0);

  // Начальная частота
  p_formLayout->addRow(tr("Start frequency:"), startFreqSpinBox);

  finishFreqSpinBox = new QDoubleSpinBox();
  finishFreqSpinBox->setRange(INT_MIN, INT_MAX);
  finishFreqSpinBox->setValue(0);

  // Конечная частота
  p_formLayout->addRow(tr("Finish frequency:"), finishFreqSpinBox);
}

void LinearFreqModulationModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  p_data = std::vector<double>(n);

  double a = amplitudeSpinBox->value();
  double f_k = finishFreqSpinBox->value();
  double f_0 = startFreqSpinBox->value();
  double p = initPhaseSpinBox->value();
  double T = p_freqSpinBox->value();
  double T_max = T * n;

  for (int i = 0; i < n; ++i) {
    p_data[i] =
        a *
        std::cos(2 * M_PI * (f_0 + (i * T) * (f_k - f_0) / T_max) * i * T + p);
  }
}

//

WhiteNoiseModel::WhiteNoiseModel(QWidget *parent) : BaseModel{parent} {
  initialize();
}

WhiteNoiseModel::WhiteNoiseModel(std::shared_ptr<SignalData> signalData,
                                 QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void WhiteNoiseModel::initialize() {
  srand(time(nullptr));

  minSpinBox = new QSpinBox();
  minSpinBox->setRange(INT_MIN, INT_MAX);
  minSpinBox->setValue(-1);

  p_formLayout->addRow(tr("Minimum:"), minSpinBox);

  maxSpinBox = new QSpinBox();
  maxSpinBox->setRange(INT_MIN, INT_MAX);
  maxSpinBox->setValue(1);

  p_formLayout->addRow(tr("Maximum:"), maxSpinBox);

  randomValue = rand() / RAND_MAX;
}

void WhiteNoiseModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  p_data = std::vector<double>(n);

  int a = minSpinBox->value();
  int b = maxSpinBox->value();

  for (int i = 0; i < n; ++i) {
    p_data[i] = a + (b - a) * randomValue;
  }
}

//

NormalWhiteNoiseModel::NormalWhiteNoiseModel(QWidget *parent)
    : BaseModel{parent} {
  initialize();
}

NormalWhiteNoiseModel::NormalWhiteNoiseModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void NormalWhiteNoiseModel::initialize() {
  srand(time(nullptr));

  averageSpinBox = new QSpinBox();
  averageSpinBox->setRange(INT_MIN, INT_MAX);
  averageSpinBox->setValue(-1);

  // Среднее значение
  p_formLayout->addRow(tr("Average value:"), averageSpinBox);

  dispersionSpinBox = new QSpinBox();
  dispersionSpinBox->setRange(INT_MIN, INT_MAX);
  dispersionSpinBox->setValue(1);

  // Дисперсия
  p_formLayout->addRow(tr("Dispersion:"), dispersionSpinBox);

  randomValue = rand() / RAND_MAX;
}

void NormalWhiteNoiseModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  p_data = std::vector<double>(n);

  int a = averageSpinBox->value();
  int q_2 = dispersionSpinBox->value();

  for (int i = 0; i < n; ++i) {
    double nu = -6;
    for (int j = 0; j < 12; ++j) {
      nu += randomValue;
    }

    p_data[i] = a + sqrt(q_2) * nu;
  }
}

//

MovingAverageAutoregressModel::MovingAverageAutoregressModel(QWidget *parent)
    : BaseModel{parent} {
  initialize();
}

MovingAverageAutoregressModel::MovingAverageAutoregressModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  initialize();
}

void MovingAverageAutoregressModel::initialize() {
  srand(time(nullptr));

  autoregressionSpinBox = new QSpinBox();
  autoregressionSpinBox->setRange(INT_MIN, INT_MAX);
  autoregressionSpinBox->setValue(1);

  // Коэффициенты СС (Скользящего Среднего)
  p_formLayout->addRow(tr("MA Coefficients:"), autoregressionSpinBox);

  averageSpinBox = new QSpinBox();
  averageSpinBox->setRange(INT_MIN, INT_MAX);
  averageSpinBox->setValue(1);

  // Коэффициенты АР (АвтоРегрессии)
  p_formLayout->addRow(tr("AR Coefficients:"), averageSpinBox);

  dispersionSpinBox = new QSpinBox();
  dispersionSpinBox->setRange(INT_MIN, INT_MAX);
  dispersionSpinBox->setValue(1);

  // Дисперсия
  p_formLayout->addRow(tr("Dispersion:"), dispersionSpinBox);

  randomValue = rand() / RAND_MAX;
}

void MovingAverageAutoregressModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  int p = autoregressionSpinBox->value();
  int q = averageSpinBox->value();

  double q_2 = dispersionSpinBox->value();

  std::vector<double> p_coef = std::vector<double>(p);
  std::vector<double> q_coef = std::vector<double>(q);
  std::vector<double> normalWhiteNoise = std::vector<double>(n);

  p_data = std::vector<double>(n);

  for (quint64 i = 0; i < n; ++i) {
    double nu = -6;
    for (size_t j = 0; j < 12; ++j) {
      nu += randomValue;
    }

    normalWhiteNoise[i] = 0 + sqrt(q_2) * nu;
  }

  for (int i = 0; i < n; ++i) {
    double sum1 = 0, sum2 = 0;
    int j = 0;
    while (j <= i) {
      if (j == q) {
        break;
      }
      sum1 += q_coef[j] * normalWhiteNoise[i - j];
      j++;
    }

    j = 0;
    while (j <= i) {
      if (j == p) {
        break;
      }
      sum2 += p_coef[j] * p_data[i - j];
      j++;
    }
    p_data[i] = normalWhiteNoise[i] + sum1 - sum2;
  }
}

}  // namespace fssp
