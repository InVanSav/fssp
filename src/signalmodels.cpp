#include "signalmodels.h"

#include <cmath>
#include <cstdlib>
#include <limits>

#include "qspinbox.h"

namespace fssp {

//

DelayedSingleImpulseModel::DelayedSingleImpulseModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  delaySpinBox = addSpinBox(tr("Delay:"), 333);
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

DelayedSingleJumpModel::DelayedSingleJumpModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  delaySpinBox = addSpinBox(tr("Delay"), 333);
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

DiscretDecreasingExpModel::DiscretDecreasingExpModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  expBaseSpinBox = addDoubleSpinBox(tr("Exponent base:"), 0.997698);
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

DiscretSinModel::DiscretSinModel(std::shared_ptr<SignalData> signalData,
                                 QWidget *parent)
    : BaseModel{signalData, parent} {
  initPhaseSpinBox = addDoubleSpinBox(tr("Init phase:"), 0);
  circFreqSpinBox = addDoubleSpinBox(tr("Circular frequency:"), 0);
  amplitudeSpinBox = addDoubleSpinBox(tr("Amplitude:"), 1);
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

RectGridModel::RectGridModel(std::shared_ptr<SignalData> signalData,
                             QWidget *parent)
    : BaseModel{signalData, parent} {
  periodSpinBox = addDoubleSpinBox(tr("Period:"), 66);
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

SawModel::SawModel(std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  periodSpinBox = addDoubleSpinBox(tr("Period:"), 66);
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

ExpEnvelopeModel::ExpEnvelopeModel(std::shared_ptr<SignalData> signalData,
                                   QWidget *parent)
    : BaseModel{signalData, parent} {
  envelopeWidthSpinBox = addDoubleSpinBox(tr("Envelope width:"), 0);
  carrierFreqSpinBox = addDoubleSpinBox(tr("Carrier frequency:"), 0);
  amplitudeSpinBox = addDoubleSpinBox(tr("Amplitude:"), 0);
  initPhaseSpinBox = addDoubleSpinBox(tr("Init phase:"), 0);
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

BalanceEnvelopeModel::BalanceEnvelopeModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  freqEnvelopeSpinBox = addDoubleSpinBox(tr("Envelope frequency:"), 0);
  carrierFreqSpinBox = addDoubleSpinBox(tr("Carrier frequency:"), 0);
  amplitudeSpinBox = addDoubleSpinBox(tr("Amplitude:"), 0);
  initPhaseSpinBox = addDoubleSpinBox(tr("Init phase:"), 0);
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

TonalEnvelopeModel::TonalEnvelopeModel(std::shared_ptr<SignalData> signalData,
                                       QWidget *parent)
    : BaseModel{signalData, parent} {
  freqEnvelopeSpinBox = addDoubleSpinBox(tr("Envelope frequency:"), 0);
  carrierFreqSpinBox = addDoubleSpinBox(tr("Carrier frequency:"), 0);
  amplitudeSpinBox = addDoubleSpinBox(tr("Finish frequency:"), 0);
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

LinearFreqModulationModel::LinearFreqModulationModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  amplitudeSpinBox = addDoubleSpinBox(tr("amplitude:"), 0);
  initPhaseSpinBox = addDoubleSpinBox(tr("init phase:"), 0);
  startFreqSpinBox = addDoubleSpinBox(tr("Start frequency:"), 0);
  finishFreqSpinBox = addDoubleSpinBox(tr("Finish frequency:"), 0);
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

WhiteNoiseModel::WhiteNoiseModel(std::shared_ptr<SignalData> signalData,
                                 QWidget *parent)
    : BaseModel{signalData, parent} {
  srand(time(nullptr));

  minSpinBox = addSpinBox(tr("Minimum:"), -1);
  maxSpinBox = addSpinBox(tr("Maximum:"), 1);

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

NormalWhiteNoiseModel::NormalWhiteNoiseModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  srand(time(nullptr));

  averageSpinBox = addSpinBox(tr("Average value:"), -1);
  dispersionSpinBox = addSpinBox(tr("Dispersion:"), 1);

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

MovingAverageAutoregressModel::MovingAverageAutoregressModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  srand(time(nullptr));

  autoregressionSpinBox = addSpinBox(tr("MA Coefficients:"), 1);
  averageSpinBox = addSpinBox(tr("AR Coefficients:"), 1);
  dispersionSpinBox = addSpinBox(tr("Dispersion:"), 1);

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
