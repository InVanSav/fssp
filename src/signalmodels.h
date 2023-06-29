#pragma once

#include <stdlib.h>
#include <time.h>

#include <QWidget>
#include <cmath>

#include "basemodel.h"
#include "qspinbox.h"

namespace fssp {

//

class DelayedSingleImpulseModel : public BaseModel {
  Q_OBJECT
 public:
  explicit DelayedSingleImpulseModel(QWidget *parent = nullptr);

  explicit DelayedSingleImpulseModel(std::shared_ptr<SignalData> signalData,
                                     QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QSpinBox *delaySpinBox;
};

//

class DelayedSingleJumpModel : public BaseModel {
  Q_OBJECT
 public:
  explicit DelayedSingleJumpModel(QWidget *parent = nullptr);

  explicit DelayedSingleJumpModel(std::shared_ptr<SignalData> signalData,
                                  QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QSpinBox *delaySpinBox;
};

//

class DiscretDecreasingExpModel : public BaseModel {
  Q_OBJECT
 public:
  explicit DiscretDecreasingExpModel(QWidget *parent = nullptr);

  explicit DiscretDecreasingExpModel(std::shared_ptr<SignalData> signalData,
                                     QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QDoubleSpinBox *expBaseSpinBox;
};

//

class DiscretSinModel : public BaseModel {
  Q_OBJECT
 public:
  explicit DiscretSinModel(QWidget *parent = nullptr);

  explicit DiscretSinModel(std::shared_ptr<SignalData> signalData,
                           QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QDoubleSpinBox *initPhaseSpinBox;
  QDoubleSpinBox *circFreqSpinBox;
  QDoubleSpinBox *amplitudeSpinBox;
};

//

class RectGridModel : public BaseModel {
  Q_OBJECT
 public:
  explicit RectGridModel(QWidget *parent = nullptr);

  explicit RectGridModel(std::shared_ptr<SignalData> signalData,
                         QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QDoubleSpinBox *periodSpinBox;
};

//

class SawModel : public BaseModel {
  Q_OBJECT
 public:
  explicit SawModel(QWidget *parent = nullptr);

  explicit SawModel(std::shared_ptr<SignalData> signalData,
                    QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QDoubleSpinBox *periodSpinBox;
};

//

class ExpEnvelopeModel : public BaseModel {
  Q_OBJECT
 public:
  explicit ExpEnvelopeModel(QWidget *parent = nullptr);

  explicit ExpEnvelopeModel(std::shared_ptr<SignalData> signalData,
                            QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QDoubleSpinBox *envelopeWidthSpinBox;
  QDoubleSpinBox *carrierFreqSpinBox;
  QDoubleSpinBox *amplitudeSpinBox;
  QDoubleSpinBox *initPhaseSpinBox;
};

//

class BalanceEnvelopeModel : public BaseModel {
  Q_OBJECT
 public:
  explicit BalanceEnvelopeModel(QWidget *parent = nullptr);

  explicit BalanceEnvelopeModel(std::shared_ptr<SignalData> signalData,
                                QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QDoubleSpinBox *freqEnvelopeSpinBox;
  QDoubleSpinBox *carrierFreqSpinBox;
  QDoubleSpinBox *amplitudeSpinBox;
  QDoubleSpinBox *initPhaseSpinBox;
};

//

class TonalEnvelopeModel : public BaseModel {
  Q_OBJECT
 public:
  explicit TonalEnvelopeModel(QWidget *parent = nullptr);

  explicit TonalEnvelopeModel(std::shared_ptr<SignalData> signalData,
                              QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QDoubleSpinBox *freqEnvelopeSpinBox;
  QDoubleSpinBox *carrierFreqSpinBox;
  QDoubleSpinBox *amplitudeSpinBox;
  QDoubleSpinBox *initPhaseSpinBox;
  QDoubleSpinBox *modulationDepthIndexSpinBox;
};

//

class LinearFreqModulationModel : public BaseModel {
  Q_OBJECT
 public:
  explicit LinearFreqModulationModel(QWidget *parent = nullptr);

  explicit LinearFreqModulationModel(std::shared_ptr<SignalData> signalData,
                                     QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QDoubleSpinBox *amplitudeSpinBox;
  QDoubleSpinBox *initPhaseSpinBox;
  QDoubleSpinBox *startFreqSpinBox;
  QDoubleSpinBox *finishFreqSpinBox;
};

//

class WhiteNoiseModel : public BaseModel {
  Q_OBJECT
 public:
  explicit WhiteNoiseModel(QWidget *parent = nullptr);

  explicit WhiteNoiseModel(std::shared_ptr<SignalData> signalData,
                           QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QSpinBox *minSpinBox;
  QSpinBox *maxSpinBox;

  double randomValue;
};

//

class NormalWhiteNoiseModel : public BaseModel {
  Q_OBJECT
 public:
  explicit NormalWhiteNoiseModel(QWidget *parent = nullptr);

  explicit NormalWhiteNoiseModel(std::shared_ptr<SignalData> signalData,
                                 QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QSpinBox *averageSpinBox;
  QSpinBox *dispersionSpinBox;

  double randomValue;
};

//

class MovingAverageAutoregressModel : public BaseModel {
  Q_OBJECT
 public:
  explicit MovingAverageAutoregressModel(QWidget *parent = nullptr);

  explicit MovingAverageAutoregressModel(std::shared_ptr<SignalData> signalData,
                                         QWidget *parent = nullptr);

  void calc() override;

 private:
  void initialize();

  QSpinBox *autoregressionSpinBox;
  QSpinBox *averageSpinBox;
  QSpinBox *dispersionSpinBox;

  double randomValue;
};

}  // namespace fssp
