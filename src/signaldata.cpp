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

  m_leftArray = 0;
  m_rightArray = m_samplesNumber - 1;

  m_leftTime = 0;
  m_rightTime = m_allTime - 1;

  m_isGlobalScale = false;
  m_isGridEnabled = true;
  m_isSelected = false;

  m_spectrumLeftArray = 0;
  m_spectrumRightArray = m_samplesNumber / 2;

  m_leftFreq = 0;
  m_rightFreq = m_rate / 2;

  m_spectrumIsGlobalScale = false;
  m_spectrumIsGridEnabled = true;
  m_isSelected = false;
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

  m_leftArray = 0;
  m_rightArray = m_samplesNumber - 1;

  m_leftTime = 0;
  m_rightTime = m_allTime - 1;

  m_isGlobalScale = false;
  m_isGridEnabled = true;
  m_isSelected = false;

  m_spectrumLeftArray = 0;
  m_spectrumRightArray = m_samplesNumber / 2;

  m_leftFreq = 0;
  m_rightFreq = m_rate / 2;

  m_spectrumIsGlobalScale = false;
  m_spectrumIsGridEnabled = true;
  m_isSelected = false;
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

  m_visibleWaveforms = std::vector<bool>(m_channelsNumber, false);

  m_leftArray = that.m_leftArray;
  m_rightArray = that.m_rightArray;

  m_leftTime = that.m_leftTime;
  m_rightTime = that.m_rightTime;

  m_isGlobalScale = that.m_isGlobalScale;
  m_isGridEnabled = that.m_isGridEnabled;
  m_isSelected = that.m_isSelected;

  m_spectrumLeftArray = that.m_spectrumLeftArray;
  m_spectrumRightArray = that.m_spectrumRightArray;

  m_leftFreq = that.m_leftFreq;
  m_rightFreq = that.m_rightFreq;

  m_spectrumIsGlobalScale = that.m_spectrumIsGlobalScale;
  m_spectrumIsGridEnabled = that.m_spectrumIsGridEnabled;
  m_spectrumIsSelected = that.m_spectrumIsSelected;
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

  m_visibleWaveforms = std::move(that.m_visibleWaveforms);

  m_leftArray = that.m_leftArray;
  m_rightArray = that.m_rightArray;

  m_leftTime = that.m_leftTime;
  m_rightTime = that.m_rightTime;

  m_isGlobalScale = that.m_isGlobalScale;
  m_isGridEnabled = that.m_isGridEnabled;
  m_isSelected = that.m_isSelected;

  m_spectrumLeftArray = that.m_spectrumLeftArray;
  m_spectrumRightArray = that.m_spectrumRightArray;

  m_leftFreq = that.m_leftFreq;
  m_rightFreq = that.m_rightFreq;

  m_spectrumIsGlobalScale = that.m_spectrumIsGlobalScale;
  m_spectrumIsGridEnabled = that.m_spectrumIsGridEnabled;
  m_spectrumIsSelected = that.m_spectrumIsSelected;
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

  swap(first.m_visibleWaveforms, second.m_visibleWaveforms);

  swap(first.m_leftArray, second.m_leftArray);
  swap(first.m_rightArray, second.m_rightArray);

  swap(first.m_leftTime, second.m_leftTime);
  swap(first.m_rightTime, second.m_rightTime);

  swap(first.m_isGlobalScale, second.m_isGlobalScale);
  swap(first.m_isGridEnabled, second.m_isGridEnabled);
  swap(first.m_isSelected, second.m_isSelected);

  swap(first.m_spectrumLeftArray, second.m_spectrumLeftArray);
  swap(first.m_spectrumRightArray, second.m_spectrumRightArray);

  swap(first.m_leftFreq, second.m_leftFreq);
  swap(first.m_rightFreq, second.m_rightFreq);

  swap(first.m_spectrumIsGlobalScale, second.m_spectrumIsGlobalScale);
  swap(first.m_spectrumIsGridEnabled, second.m_spectrumIsGridEnabled);
  swap(first.m_spectrumIsSelected, second.m_spectrumIsSelected);
}

QDateTime SignalData::startTime() const { return m_startTime; }

QDateTime SignalData::endTime() const { return m_endTime; }

double SignalData::rate() const { return m_rate; }

double SignalData::timeForOne() const { return m_timeForOne; }

size_t SignalData::allTime() const { return m_allTime; }

int SignalData::leftArray() const { return m_leftArray; }

int SignalData::rightArray() const { return m_rightArray; }

int SignalData::arrayRange() const { return m_rightArray - m_leftArray; }

size_t SignalData::leftTime() const { return m_leftTime; }

size_t SignalData::rightTime() const { return m_rightTime; }

size_t SignalData::timeRange() const { return m_rightTime - m_leftTime; }

bool SignalData::isGridEnabled() const { return m_isGridEnabled; }

bool SignalData::isGlobalScale() const { return m_isGlobalScale; }

bool SignalData::isSelected() const { return m_isSelected; }

const std::vector<QString> &SignalData::channelsName() const {
  return m_channelsName;
}

const std::vector<std::vector<double>> &SignalData::data() const {
  return m_data;
}

void SignalData::addData(const QString name, std::vector<double> data) {
  ++m_channelsNumber;
  m_channelsName.push_back(name);
  m_data.push_back(data);
  m_visibleWaveforms.push_back(false);
}

int SignalData::channelsNumber() const { return m_channelsNumber; }

int SignalData::samplesNumber() const { return m_samplesNumber; }

const std::vector<bool> &SignalData::visibleWaveforms() const {
  return m_visibleWaveforms;
}

void SignalData::setWaveformVisibility(int number, bool isVisible) {
  m_visibleWaveforms[number] = isVisible;
}

void SignalData::setGridEnabled(bool isGridEnabled) {
  m_isGridEnabled = isGridEnabled;
}

void SignalData::setGlobalScale(bool isGlobalScale) {
  m_isGlobalScale = isGlobalScale;
}

void SignalData::setSelected(bool isSelected) { m_isSelected = isSelected; }

void SignalData::setLeftArray(int leftArray) { m_leftArray = leftArray; }

void SignalData::setRightArray(int rightArray) { m_rightArray = rightArray; }

void SignalData::setLeftTime(size_t leftTime) { m_leftTime = leftTime; }

void SignalData::setRightTime(size_t rightTime) { m_rightTime = rightTime; }

void SignalData::setDefault() {
  m_leftArray = 0;
  m_rightArray = m_samplesNumber - 1;

  m_leftTime = 0;
  m_rightTime = m_allTime - 1;

  m_isGlobalScale = false;
  m_isGridEnabled = true;
  m_isSelected = false;
}

void SignalData::calculateArrayRange() {
  double dataPerTime =
      static_cast<double>(m_samplesNumber) / static_cast<double>(m_allTime);

  m_rightArray = dataPerTime * m_rightTime;
  m_leftArray = dataPerTime * m_leftTime;

  if ((m_rightArray - m_leftArray < 8) && (m_samplesNumber > 16)) {
    if (m_samplesNumber - m_rightArray > 8) {
      m_leftArray = m_rightArray;
      m_rightArray += 8;
    } else {
      m_rightArray = m_leftArray;
      m_leftArray -= 8;
    }
  }
}

int SignalData::spectrumLeftArray() const { return m_spectrumLeftArray; }
int SignalData::spectrumRightArray() const { return m_spectrumRightArray; }

double SignalData::leftFreq() const { return m_leftFreq; }
double SignalData::rightFreq() const { return m_rightFreq; }

bool SignalData::spectrumIsGridEnabled() const {
  return m_spectrumIsGridEnabled;
}
bool SignalData::spectrumIsGlobalScale() const {
  return m_spectrumIsGridEnabled;
}
bool SignalData::spectrumIsSelected() const { return m_spectrumIsSelected; }

void SignalData::setSpectrumLeftArray(int leftArray) {
  m_spectrumLeftArray = leftArray;
}
void SignalData::setSpectrumRightArray(int rightArray) {
  m_spectrumRightArray = rightArray;
}

void SignalData::setLeftFreq(double leftFreq) { m_leftFreq = leftFreq; }
void SignalData::setRightFreq(double rightFreq) { m_rightFreq = rightFreq; }

void SignalData::setSpectrumGridEnabled(bool isGridEnabled) {
  m_spectrumIsGridEnabled = isGridEnabled;
}
void SignalData::setSpectrumGlobalScale(bool isGlobalScale) {
  m_spectrumIsGlobalScale = isGlobalScale;
}
void SignalData::setSpectrumSelected(bool isSelected) {
  m_spectrumIsSelected = isSelected;
}

const std::vector<bool> &SignalData::spectrumVisibleWaveforms() const {
  return m_spectrumVisibleWaveforms;
}

void SignalData::setSpectrumWaveformVisibility(int number, bool isVisible) {
  m_spectrumVisibleWaveforms[number] = isVisible;
}

int SignalData::spectrumArrayRange() const {
  return m_spectrumRightArray - m_spectrumLeftArray;
}

double SignalData::freqRange() const { return m_rightFreq - m_leftFreq; }

void SignalData::spectrumCalculateArrayRange() {
  double dataPerFreq = static_cast<double>(m_samplesNumber / 2) /
                       static_cast<double>(m_rate / 2);

  m_spectrumLeftArray = dataPerFreq * m_leftFreq;
  m_spectrumRightArray = dataPerFreq * m_rightFreq;

  if ((m_spectrumRightArray - m_spectrumLeftArray < 8) &&
      (m_samplesNumber / 2 > 16)) {
    if (m_samplesNumber - m_spectrumRightArray > 8) {
      m_spectrumLeftArray = m_spectrumRightArray;
      m_spectrumRightArray += 8;
    } else {
      m_spectrumRightArray = m_spectrumLeftArray;
      m_spectrumLeftArray -= 8;
    }
  }
}

}  // namespace fssp
