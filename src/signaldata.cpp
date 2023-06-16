#include "signaldata.h"

namespace fssp {

SignalData::SignalData(QDateTime startTime, QDateTime endTime, double rate,
                       double timeForOne, double allTime,
                       std::vector<std::string> &&channelsName,
                       std::vector<std::vector<double>> &&data) {
  m_startTime = startTime;
  m_endTime = endTime;
  m_rate = rate;
  m_channelsName = std::move(channelsName);
  m_data = std::move(data);
}

QDateTime SignalData::startTime() const { return m_startTime; }

QDateTime SignalData::endTime() const { return m_endTime; }

double SignalData::rate() const { return m_rate; }

double SignalData::timeForOne() const { return m_timeForOne; }

double SignalData::allTime() const { return m_allTime; }

std::vector<std::string> *SignalData::channelsName() { return &m_channelsName; }

std::vector<std::vector<double>> *SignalData::data() { return &m_data; }

}  // namespace fssp
