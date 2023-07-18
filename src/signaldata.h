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

  int leftArray() const;
  int rightArray() const;

  int arrayRange() const;

  size_t leftTime() const;
  size_t rightTime() const;

  size_t timeRange() const;

  bool isGridEnabled() const;
  bool isGlobalScale() const;
  bool isSelected() const;

  void setLeftArray(int leftArray);
  void setRightArray(int rightArray);

  void setLeftTime(size_t leftTime);
  void setRightTime(size_t rightTime);

  void setGridEnabled(bool isGridEnabled);
  void setGlobalScale(bool isGlobalScale);
  void setSelected(bool isSelected);

  int spectrumLeftArray() const;
  int spectrumRightArray() const;

  int spectrumArrayRange() const;

  double leftFreq() const;
  double rightFreq() const;

  double freqRange() const;

  double allFreq() const;

  bool spectrumIsGridEnabled() const;
  bool spectrumIsGlobalScale() const;
  bool spectrumIsSelected() const;

  void setSpectrumLeftArray(int leftArray);
  void setSpectrumRightArray(int rightArray);

  void setLeftFreq(double leftFreq);
  void setRightFreq(double rightFreq);

  void setSpectrumGridEnabled(bool isGridEnabled);
  void setSpectrumGlobalScale(bool isGlobalScale);
  void setSpectrumSelected(bool isSelected);

  void setDefault();
  void setSpectrumDefault();

  void calculateArrayRange();
  void spectrumCalculateArrayRange();

  const std::vector<QString> &channelsName() const;
  const std::vector<std::vector<double>> &data() const;

  void addData(const QString name, std::vector<double> data);

  int channelsNumber() const;
  int samplesNumber() const;

  const std::vector<bool> &visibleWaveforms() const;
  void setWaveformVisibility(int number, bool isVisible);

  const std::vector<bool> &spectrumVisibleWaveforms() const;
  void setSpectrumWaveformVisibility(int number, bool isVisible);

 signals:
  void changedGraphTimeRange();

  void changedWaveformVisibility();

  void changedEnableGrid();
  void changedGlobalScale();

  void changedSpectrumFreqRange();

  void changedSpectrumVisibility();

  void changedSpectrumEnableGrid();
  void changedSpectrumGlobalScale();

  void dataAdded();

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
  bool m_isGlobalScale;
  bool m_isSelected;

  bool m_spectrumIsGridEnabled;
  bool m_spectrumIsGlobalScale;
  bool m_spectrumIsSelected;

  int m_spectrumLeftArray;
  int m_spectrumRightArray;

  double m_leftFreq;
  double m_rightFreq;

  std::vector<bool> m_visibleWaveforms;
  std::vector<bool> m_spectrumVisibleWaveforms;
};

}  // namespace fssp
