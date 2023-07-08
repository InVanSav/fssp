#include "signaldata.h"

namespace fssp {

SignalData::SignalData() {
  m_samplesNumber = 1000;
  m_rate = 1;
  m_timeForOne = 1 / m_rate;
  m_allTime = m_timeForOne * m_timeForOne * 1000;

  m_startTime = QDateTime{QDate{2000, 1, 1}, QTime{0, 0}};
  m_endTime = m_startTime.addMSecs(m_allTime);

  m_channelsNumber = 1;

  m_channelsName = std::vector<QString>(m_channelsNumber);
  m_data = std::vector<std::vector<double>>(
      m_channelsNumber, std::vector<double>(m_samplesNumber));

  m_isSelected = false;
  m_leftSelection = 0;
  m_rightSelection = m_data[0].size() - 1;
}

SignalData::SignalData(const QDateTime &startTime, const QDateTime &endTime,
                       const double rate, const double timeForOne,
                       const size_t allTime,
                       std::vector<QString> &&channelsName,
                       std::vector<std::vector<double>> &&data) {
  m_startTime = startTime;
  m_endTime = endTime;

  m_rate = rate;
  m_timeForOne = timeForOne;
  m_allTime = allTime;

  m_channelsName = std::move(channelsName);
  m_data = std::move(data);

  m_channelsNumber = m_channelsName.size();
  m_samplesNumber = m_data[0].size();

  m_isSelected = false;
  m_leftSelection = 0;
  m_rightSelection = m_data[0].size() - 1;
}

SignalData::SignalData(const SignalData &that) {
  m_startTime = that.m_startTime;
  m_endTime = that.m_endTime;

  m_rate = that.m_rate;
  m_timeForOne = that.m_timeForOne;
  m_allTime = that.m_allTime;

  m_channelsName = that.m_channelsName;
  m_data = that.m_data;

  m_channelsNumber = that.m_channelsNumber;
  m_samplesNumber = that.m_samplesNumber;

  m_isSelected = that.m_isSelected;
  m_leftSelection = that.m_leftSelection;
  m_rightSelection = that.m_rightSelection;

  m_visibleWaveforms = std::vector<bool>(m_channelsNumber, false);
}

SignalData::SignalData(SignalData &&that) {
  m_startTime = that.m_startTime;
  m_endTime = that.m_endTime;

  m_rate = that.m_rate;
  m_timeForOne = that.m_timeForOne;
  m_allTime = that.m_allTime;

  m_channelsName = std::move(that.m_channelsName);
  m_data = std::move(that.m_data);

  m_channelsNumber = that.m_channelsNumber;
  m_samplesNumber = that.m_samplesNumber;

  m_isSelected = that.m_isSelected;
  m_leftSelection = that.m_leftSelection;
  m_rightSelection = that.m_rightSelection;

  m_visibleWaveforms = std::move(that.m_visibleWaveforms);
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

  swap(first.m_channelsNumber, second.m_channelsNumber);
  swap(first.m_samplesNumber, second.m_samplesNumber);

  swap(first.m_isSelected, second.m_isSelected);
  swap(first.m_leftSelection, second.m_leftSelection);
  swap(first.m_rightSelection, second.m_rightSelection);

  swap(first.m_visibleWaveforms, second.m_visibleWaveforms);
}

QDateTime SignalData::startTime() const { return m_startTime; }

QDateTime SignalData::endTime() const { return m_endTime; }

double SignalData::rate() const { return m_rate; }

double SignalData::timeForOne() const { return m_timeForOne; }

size_t SignalData::allTime() const { return m_allTime; }

const std::vector<QString> &SignalData::channelsName() const {
  return m_channelsName;
}

const std::vector<std::vector<double>> &SignalData::data() const {
  return m_data;
}

int SignalData::channelsNumber() const { return m_channelsNumber; }

int SignalData::samplesNumber() const { return m_samplesNumber; }

bool SignalData::isSelected() const { return m_isSelected; }

int SignalData::leftSelection() const { return m_leftSelection; }

int SignalData::rightSelection() const { return m_rightSelection; }

bool SignalData::setIsSelected(bool isSelected) {
  m_isSelected = isSelected;
  return this->isSelected();
}

int SignalData::setLeftSelection(int leftSelection) {
  m_leftSelection = leftSelection;
  return this->leftSelection();
}

int SignalData::setRightSelection(int rightSelection) {
  m_rightSelection = rightSelection;
  return this->rightSelection();
}

const std::vector<bool> &SignalData::visibleWaveforms() const {
  return m_visibleWaveforms;
}

void SignalData::setWaveformVisibility(int number, bool isVisible) {
  m_visibleWaveforms[number] = isVisible;
}

}  // namespace fssp
