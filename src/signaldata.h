#pragma once

#include <QDateTime>

namespace fssp {

class SignalData {
 public:
  SignalData(QDateTime startTime, QDateTime endTime, double rate,
             double timeForOne, double allTime,
             std::vector<std::string> &&channelsName,
             std::vector<std::vector<double>> &&data);

  QDateTime startTime() const;
  QDateTime endTime() const;

  double rate() const;
  double timeForOne() const;
  double allTime() const;

  std::vector<std::string> *channelsName();
  std::vector<std::vector<double>> *data();

 private:
  QDateTime m_startTime;
  QDateTime m_endTime;

  double m_rate;
  double m_timeForOne;
  double m_allTime;

  std::vector<std::string> m_channelsName;
  std::vector<std::vector<double>> m_data;
};

}  // namespace fssp
