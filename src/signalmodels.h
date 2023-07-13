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
  explicit DelayedSingleImpulseModel(std::shared_ptr<SignalData> signalData,
                                     QWidget *parent = nullptr);

  void calc() override;

 private:
  QSpinBox *delaySpinBox;
};

//

class DelayedSingleJumpModel : public BaseModel {
  Q_OBJECT
 public:
  explicit DelayedSingleJumpModel(std::shared_ptr<SignalData> signalData,
                                  QWidget *parent = nullptr);

  void calc() override;

 private:
  QSpinBox *delaySpinBox;
};

//

class DiscretDecreasingExpModel : public BaseModel {
  Q_OBJECT
 public:
  explicit DiscretDecreasingExpModel(std::shared_ptr<SignalData> signalData,
                                     QWidget *parent = nullptr);

  void calc() override;

 private:
  QDoubleSpinBox *expBaseSpinBox;
};

//

class DiscretSinModel : public BaseModel {
  Q_OBJECT
 public:
  explicit DiscretSinModel(std::shared_ptr<SignalData> signalData,
                           QWidget *parent = nullptr);

  void calc() override;

 private:
  QDoubleSpinBox *initPhaseSpinBox;
  QDoubleSpinBox *circFreqSpinBox;
  QDoubleSpinBox *amplitudeSpinBox;
};

//

class RectGridModel : public BaseModel {
  Q_OBJECT
 public:
  explicit RectGridModel(std::shared_ptr<SignalData> signalData,
                         QWidget *parent = nullptr);

  void calc() override;

 private:
  QSpinBox *periodSpinBox;
};

//

class SawModel : public BaseModel {
  Q_OBJECT
 public:
  explicit SawModel(std::shared_ptr<SignalData> signalData,
                    QWidget *parent = nullptr);

  void calc() override;

 private:
  QSpinBox *periodSpinBox;
};

//

class ExpEnvelopeModel : public BaseModel {
  Q_OBJECT
 public:
  explicit ExpEnvelopeModel(std::shared_ptr<SignalData> signalData,
                            QWidget *parent = nullptr);

  void calc() override;

 private:
  QDoubleSpinBox *envelopeWidthSpinBox;
  QDoubleSpinBox *carrierFreqSpinBox;
  QDoubleSpinBox *amplitudeSpinBox;
  QDoubleSpinBox *initPhaseSpinBox;
};

//

class BalanceEnvelopeModel : public BaseModel {
  Q_OBJECT
 public:
  explicit BalanceEnvelopeModel(std::shared_ptr<SignalData> signalData,
                                QWidget *parent = nullptr);

  void calc() override;

 private:
  QDoubleSpinBox *freqEnvelopeSpinBox;
  QDoubleSpinBox *carrierFreqSpinBox;
  QDoubleSpinBox *amplitudeSpinBox;
  QDoubleSpinBox *initPhaseSpinBox;
};

//

class TonalEnvelopeModel : public BaseModel {
  Q_OBJECT
 public:
  explicit TonalEnvelopeModel(std::shared_ptr<SignalData> signalData,
                              QWidget *parent = nullptr);

  void calc() override;

 private:
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
  explicit LinearFreqModulationModel(std::shared_ptr<SignalData> signalData,
                                     QWidget *parent = nullptr);

  void calc() override;

 private:
  QDoubleSpinBox *amplitudeSpinBox;
  QDoubleSpinBox *initPhaseSpinBox;
  QDoubleSpinBox *startFreqSpinBox;
  QDoubleSpinBox *finishFreqSpinBox;
};

//

class WhiteNoiseModel : public BaseModel {
  Q_OBJECT
 public:
  explicit WhiteNoiseModel(std::shared_ptr<SignalData> signalData,
                           QWidget *parent = nullptr);

  void calc() override;

 private:
  QDoubleSpinBox *minSpinBox;
  QDoubleSpinBox *maxSpinBox;

  double randomValue;
};

//

class NormalWhiteNoiseModel : public BaseModel {
  Q_OBJECT
 public:
  explicit NormalWhiteNoiseModel(std::shared_ptr<SignalData> signalData,
                                 QWidget *parent = nullptr);

  void calc() override;

 private:
  QDoubleSpinBox *averageSpinBox;
  QDoubleSpinBox *dispersionSpinBox;

  double randomValue;
};

//

class MovingAverageAutoregressModel : public BaseModel {
  Q_OBJECT
 public:
  explicit MovingAverageAutoregressModel(std::shared_ptr<SignalData> signalData,
                                         QWidget *parent = nullptr);

  void calc() override;

 private:
  QDoubleSpinBox *autoregressionSpinBox;
  QDoubleSpinBox *averageSpinBox;
  QDoubleSpinBox *dispersionSpinBox;

  double randomValue;
};

}  // namespace fssp
