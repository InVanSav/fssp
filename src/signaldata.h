#pragma once

#include <QDateTime>

namespace fssp {

class SignalData {
 public:
  explicit SignalData(const QDateTime &startTime, const QDateTime &endTime,
                      const double &rate, const double &timeForOne,
                      const double &allTime,
                      std::vector<QString> &&channelsName,
                      std::vector<std::vector<double>> &&data);

  SignalData(const SignalData &that);

  SignalData(SignalData &&that);

  SignalData &operator=(SignalData that);

  friend void swap(SignalData &first, SignalData &second);

  QDateTime startTime() const;
  QDateTime endTime() const;

  double rate() const;
  double timeForOne() const;
  double allTime() const;

  std::vector<QString> *channelsName();
  std::vector<std::vector<double>> *data();

 private:
  QDateTime m_startTime;
  QDateTime m_endTime;

  double m_rate;
  double m_timeForOne;
  double m_allTime;

  std::vector<QString> m_channelsName;
  std::vector<std::vector<double>> m_data;
};

}  // namespace fssp
