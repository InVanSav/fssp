#include "signaldata.h"

namespace fssp {

SignalData::SignalData(const QDateTime &startTime, const QDateTime &endTime,
                       const double &rate, const double &timeForOne,
                       const double &allTime,
                       std::vector<std::string> &&channelsName,
                       std::vector<std::vector<double>> &&data) {
  m_startTime = startTime;
  m_endTime = endTime;
  m_rate = rate;
  m_channelsName = std::move(channelsName);
  m_data = std::move(data);
}

SignalData::SignalData(const SignalData &that) {
  m_startTime = that.m_startTime;
  m_endTime = that.m_endTime;
  m_rate = that.m_rate;
  m_timeForOne = that.m_timeForOne;
  m_allTime = that.m_allTime;
  m_channelsName = that.m_channelsName;
  m_data = that.m_data;
}

SignalData::SignalData(SignalData &&that) {
  m_startTime = that.m_startTime;
  m_endTime = that.m_endTime;
  m_rate = that.m_rate;
  m_timeForOne = that.m_timeForOne;
  m_allTime = that.m_allTime;
  m_channelsName = std::move(that.m_channelsName);
  m_data = std::move(that.m_data);
}

SignalData &SignalData::operator=(SignalData that) {
  swap(*this, that);

  return *this;
}

void swap(SignalData &first, SignalData &second) {
  using std::swap;

  swap(first.m_startTime, second.m_startTime);
  swap(first.m_endTime, second.m_endTime);
  swap(first.m_rate, second.m_rate);
  swap(first.m_timeForOne, second.m_timeForOne);
  swap(first.m_allTime, second.m_allTime);
  swap(first.m_channelsName, second.m_channelsName);
  swap(first.m_data, second.m_data);
}

QDateTime SignalData::startTime() const { return m_startTime; }

QDateTime SignalData::endTime() const { return m_endTime; }

double SignalData::rate() const { return m_rate; }

double SignalData::timeForOne() const { return m_timeForOne; }

double SignalData::allTime() const { return m_allTime; }

std::vector<std::string> *SignalData::channelsName() { return &m_channelsName; }

std::vector<std::vector<double>> *SignalData::data() { return &m_data; }

}  // namespace fssp
