#include "statisticwindow.h"

namespace fssp {

StatisticWindow::StatisticWindow(std::shared_ptr<SignalData> data, int curSignal,
                     int intervalsNumber, QWidget *parent)
    : QDialog{parent} {
  p_intervalsNumber = intervalsNumber;
  p_curSignal = curSignal;

  p_signalData = data;

  calculateStatistic();

  showDialog();
}

void StatisticWindow::showDialog() {
  setWindowTitle(tr("Statistic of ") +
                 p_signalData->channelsName()[p_curSignal]);

  QLabel *firstParameter =
      new QLabel(tr("Minimum value: ") + QString::number(p_minValue));

  QLabel *secondParameter =
      new QLabel(tr("Maximum value: ") + QString::number(p_maxValue));

  QLabel *thirdParameter =
      new QLabel(tr("Average value: ") + QString::number(p_avgValue));

  QVBoxLayout *textOneLayout = new QVBoxLayout();
  textOneLayout->addWidget(firstParameter);
  textOneLayout->addWidget(secondParameter);
  textOneLayout->addWidget(thirdParameter);

  QLabel *fourthParameter =
      new QLabel(tr("Variation factor: ") + QString::number(p_variationFactor));

  QLabel *fifthParameter =
      new QLabel(tr("Asymmetry factor: ") + QString::number(p_asymmetryFactor));

  QLabel *sixthParameter =
      new QLabel(tr("Kurtosis factor: ") + QString::number(p_kurtosisFactor));

  QVBoxLayout *textTwoLayout = new QVBoxLayout();
  textTwoLayout->addWidget(fourthParameter);
  textTwoLayout->addWidget(fifthParameter);
  textTwoLayout->addWidget(sixthParameter);

  QLabel *seventhParameter =
      new QLabel(tr("Dispersion: ") + QString::number(p_dispersion));

  QLabel *eighthParameter = new QLabel(tr("Standart derivation: ") +
                                       QString::number(p_standardDeviation));

  QLabel *ninthParameter =
      new QLabel(tr("Median: ") + QString::number(p_median));

  QVBoxLayout *textThreeLayout = new QVBoxLayout();
  textThreeLayout->addWidget(seventhParameter);
  textThreeLayout->addWidget(eighthParameter);
  textThreeLayout->addWidget(ninthParameter);

  QLabel *tenthParameter =
      new QLabel(tr("Order quantile 0.05: ") + QString::number(p_minQuantile));

  QLabel *eleventhParameter =
      new QLabel(tr("Order quantile 0.95: ") + QString::number(p_maxQuantile));

  QVBoxLayout *textFourLayout = new QVBoxLayout();
  textFourLayout->addWidget(tenthParameter);
  textFourLayout->addWidget(eleventhParameter);

  QHBoxLayout *textLayout = new QHBoxLayout();
  textLayout->addLayout(textOneLayout);
  textLayout->addLayout(textTwoLayout);
  textLayout->addLayout(textThreeLayout);
  textLayout->addLayout(textFourLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout();

  mainLayout->addLayout(textLayout);
  mainLayout->addWidget(p_graph);

  setLayout(mainLayout);
  setFixedSize(sizeHint());
}

void StatisticWindow::calculateStatistic() {
  if (!p_intervalsNumber) return;

  std::vector<double> data = p_signalData->data()[p_curSignal];

  // Минимум и максимум
  p_minValue = *std::min_element(data.begin(), data.end());
  p_maxValue = *std::max_element(data.begin(), data.end());

  // Среднее
  for (int i = 0; i < data.size(); ++i) p_avgValue += data[i];
  p_avgValue /= static_cast<double>(data.size());

  std::vector<int> histogram(p_intervalsNumber);
  std::sort(data.begin(), data.end());

  size_t j = 0;
  double max_hist = 0;
  double h = (p_maxValue - p_minValue) / static_cast<double>(p_intervalsNumber);
  double cur_right_border = p_minValue + h;
  for (int i = 0; i < data.size(); ++i) {
    if (data[i] <= cur_right_border) {
      histogram[j]++;
    } else {
      if (max_hist < static_cast<double>(histogram[j])) {
        max_hist = static_cast<double>(histogram[j]);
      }
      j++;
      cur_right_border += h;
    }
  }

  // Дисперсия
  for (int i = 0; i < data.size(); ++i) {
    p_dispersion += std::pow((data[i] - p_avgValue), 2);
  }
  p_dispersion /= static_cast<double>(data.size());

  // Среднеквадратичное отклонение
  p_standardDeviation = std::sqrt(p_dispersion);

  // Коэффициент вариации
  p_variationFactor = p_standardDeviation / p_avgValue;

  // Коэффициент ассиметрии
  for (int i = 0; i < data.size(); ++i) {
    p_asymmetryFactor += std::pow((data[i] - p_avgValue), 3);
  }
  p_asymmetryFactor /= static_cast<double>(data.size());
  p_asymmetryFactor /= std::pow(p_standardDeviation, 3);

  // Коэффициент эксцесса
  for (int i = 0; i < data.size(); ++i) {
    p_kurtosisFactor += pow((data[i] - p_avgValue), 4);
  }
  p_kurtosisFactor /= static_cast<double>(data.size());
  p_kurtosisFactor /= pow(p_standardDeviation, 4);
  p_kurtosisFactor -= 3;

  // Медиана
  p_median = data[data.size() / 2];

  // Квантили порядка n
  p_minQuantile = data[data.size() * 0.05];
  p_maxQuantile = data[data.size() * 0.95];

  double width = 740;
  double height = 280;
  h = (width - 40) / p_intervalsNumber;

  p_graph = new QGraphicsView();
  QGraphicsScene *scene = new QGraphicsScene();

  p_graph->setScene(scene);

  for (int i = 0; i < histogram.size(); ++i) {
    QGraphicsRectItem *item = new QGraphicsRectItem();

    item->setRect(
        (h * static_cast<double>(i)), 0, h,
        -(height - 20) * (static_cast<double>(histogram[i]) / max_hist));

    item->setBrush(Qt::gray);
    scene->addItem(item);
  }
}

}  // namespace fssp
