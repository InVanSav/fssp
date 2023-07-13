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
  delaySpinBox = addSpinBox(tr("Delay:"), signalData->samplesNumber() / 3);
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
  delaySpinBox = addSpinBox(tr("Delay"), signalData->samplesNumber() / 3);
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
  expBaseSpinBox = addDoubleSpinBox(
      tr("Exponent base:"),
      std::pow(1. / 10., 1. / (double)signalData->samplesNumber() - 1));
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
  circFreqSpinBox =
      addDoubleSpinBox(tr("Circular frequency:"),
                       20 * M_PI / (double)signalData->samplesNumber());
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
  periodSpinBox = addSpinBox(tr("Period:"), signalData->samplesNumber() / 15);
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
  periodSpinBox = addSpinBox(tr("Period:"), signalData->samplesNumber() / 15);
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
  envelopeWidthSpinBox = addDoubleSpinBox(
      tr("Envelope width:"),
      (double)signalData->samplesNumber() / (1.5 * signalData->rate()));
  carrierFreqSpinBox = addDoubleSpinBox(
      tr("Carrier frequency:"),
      15 * (*signalData).rate() / (double)signalData->samplesNumber());
  amplitudeSpinBox = addDoubleSpinBox(tr("Amplitude:"), 1);
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
  freqEnvelopeSpinBox = addDoubleSpinBox(
      tr("Envelope frequency:"),
      3 * signalData->rate() / (double)signalData->samplesNumber());
  carrierFreqSpinBox = addDoubleSpinBox(
      tr("Carrier frequency:"),
      15 * signalData->rate() / (double)signalData->samplesNumber());
  amplitudeSpinBox = addDoubleSpinBox(tr("Amplitude:"), 1);
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
  freqEnvelopeSpinBox = addDoubleSpinBox(
      tr("Envelope frequency:"),
      6 * signalData->rate() / (double)signalData->samplesNumber());
  amplitudeSpinBox = addDoubleSpinBox(tr("Amplitude:"), 1);
  initPhaseSpinBox = addDoubleSpinBox(tr("Init phase:"), 0);
  carrierFreqSpinBox = addDoubleSpinBox(
      tr("Carrier frequency:"),
      15 * signalData->rate() / (double)signalData->samplesNumber());
  modulationDepthIndexSpinBox = addDoubleSpinBox(tr("Modulation depth:"), 0.5);
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
  amplitudeSpinBox = addDoubleSpinBox(tr("Amplitude:"), 1);
  initPhaseSpinBox = addDoubleSpinBox(tr("Init phase:"), 0);
  startFreqSpinBox =
      addDoubleSpinBox(tr("Start frequency:"),
                       5 * signalData->rate() / signalData->samplesNumber());
  finishFreqSpinBox = addDoubleSpinBox(
      tr("Finish frequency:"),
      20 * signalData->rate() / (double)signalData->samplesNumber());
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

  minSpinBox = addDoubleSpinBox(tr("Minimum:"), -5);
  maxSpinBox = addDoubleSpinBox(tr("Maximum:"), 5);
}

void WhiteNoiseModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  p_data = std::vector<double>(n);

  int a = minSpinBox->value();
  int b = maxSpinBox->value();

  for (int i = 0; i < n; ++i) {
    p_data[i] = a + (b - a) * (rand() / (double)RAND_MAX);
  }
}

//

NormalWhiteNoiseModel::NormalWhiteNoiseModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  srand(time(nullptr));

  averageSpinBox = addDoubleSpinBox(tr("Average value:"), 1);
  dispersionSpinBox = addDoubleSpinBox(tr("Dispersion:"), 1);
}

void NormalWhiteNoiseModel::calc() {
  int n = p_sampleNumberSpinBox->value();
  p_data = std::vector<double>(n);

  int a = averageSpinBox->value();
  int q_2 = dispersionSpinBox->value();

  for (int i = 0; i < n; ++i) {
    double nu = -6;
    for (int j = 0; j < 12; ++j) {
      nu += (rand() / (double)RAND_MAX);
    }

    p_data[i] = a + sqrt(q_2) * nu;
  }
}

//

MovingAverageAutoregressModel::MovingAverageAutoregressModel(
    std::shared_ptr<SignalData> signalData, QWidget *parent)
    : BaseModel{signalData, parent} {
  srand(time(nullptr));

  autoregressionSpinBox = addDoubleSpinBox(tr("MA Coefficients:"), 1);
  averageSpinBox = addDoubleSpinBox(tr("AR Coefficients:"), 1);
  dispersionSpinBox = addDoubleSpinBox(tr("Dispersion:"), 1);
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
      nu += (rand() / (double)RAND_MAX);
    }

    normalWhiteNoise[i] = sqrt(q_2) * nu;
  }

  for (int i = 0; i < n; ++i) {
    double sum1 = 0, sum2 = 0;
    int j = 1;
    while (j <= i) {
      if (j - 1 == q) {
        break;
      }
      sum1 += q_coef[j - 1] * normalWhiteNoise[i - j];
      j++;
    }

    j = 1;
    while (j <= i) {
      if (j - 1 == p) {
        break;
      }
      sum2 += p_coef[j - 1] * p_data[i - j];
      j++;
    }
    p_data[i] = normalWhiteNoise[i] + sum1 - sum2;
  }
}

}  // namespace fssp
