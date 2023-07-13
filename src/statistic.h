#pragma once

#include <QComboBox>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include "signaldata.h"

namespace fssp {

class Statistic : public QDialog {
  Q_OBJECT
 public:
  explicit Statistic(std::shared_ptr<SignalData> data, int curSignal,
                     int intervalsNumber, QWidget *parent = nullptr);

 private:
  void calculateStatistic();
  void showDialog();

 private:
  std::shared_ptr<SignalData> p_signalData;

  QGraphicsView *p_graph;

  double p_minValue;
  double p_maxValue;
  double p_avgValue = 0;

  double p_dispersion = 0;
  double p_standardDeviation;

  double p_variationFactor = 0;
  double p_asymmetryFactor = 0;
  double p_kurtosisFactor = 0;

  double p_median;

  double p_minQuantile;
  double p_maxQuantile;

  int p_intervalsNumber;
  int p_curSignal;
};

}  // namespace fssp
