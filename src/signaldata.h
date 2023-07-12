#pragma once

#include <QDateTime>

namespace fssp {

class SignalData : public QObject {
  Q_OBJECT
 public:
  explicit SignalData();

  explicit SignalData(const QDateTime &startTime, const QDateTime &endTime,
                      const double rate, const double timeForOne,
                      const size_t allTime, std::vector<QString> &&channelsName,
                      std::vector<std::vector<double>> &&data);

  SignalData(const SignalData &that);

  SignalData(SignalData &&that);

  SignalData &operator=(SignalData that);

  friend void swap(SignalData &first, SignalData &second);

  QDateTime startTime() const;
  QDateTime endTime() const;

  double rate() const;
  double timeForOne() const;
  size_t allTime() const;

  int leftArray();
  int rightArray();

  int leftTime();
  int rightTime();

  void setGridEnabled(bool isGridEnabled);

  void setLeftArray(int leftArray);
  void setRightArray(int rightArray);

  void setLeftTime(int leftTime);
  void setRightTime(int rightTime);

  bool isGridEnabled() const;

  void calculateArrayRange();

  const std::vector<QString> &channelsName() const;
  const std::vector<std::vector<double>> &data() const;

  int channelsNumber() const;
  int samplesNumber() const;

  const std::vector<bool> &visibleWaveforms() const;
  void setWaveformVisibility(int number, bool isVisible);

 signals:
  void changedGraphTimeRange();
  void changedWaveformVisibility();
  void changedEnableGrid();

 private:
  QDateTime m_startTime;
  QDateTime m_endTime;

  double m_rate;
  double m_timeForOne;

  size_t m_allTime;

  std::vector<QString> m_channelsName;
  std::vector<std::vector<double>> m_data;

  int m_channelsNumber;
  int m_samplesNumber;

  int m_leftArray;
  int m_rightArray;

  size_t m_leftTime;
  size_t m_rightTime;

  bool m_isGridEnabled;

  std::vector<bool> m_visibleWaveforms;
};

}  // namespace fssp
