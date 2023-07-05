#pragma once

#include <QDateTime>

namespace fssp {

class SignalData : public QObject {
  Q_OBJECT
 public:
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

  const std::vector<QString> &channelsName() const;
  const std::vector<std::vector<double>> &data() const;

  int channelsNumber() const;
  int samplesNumber() const;

  bool isSelected() const;
  int leftSelection() const;
  int rightSelection() const;

  bool setIsSelected(bool isSelected);
  int setLeftSelection(int leftSelection);
  int setRightSelection(int rightSelection);

  const std::vector<bool> &visibleWaveforms() const;
  void setWaveformVisibility(int number, bool isVisible);

 signals:
  void selectionEvent();
  void waveformVisibilityEvent();

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

  bool m_isSelected;
  int m_leftSelection;
  int m_rightSelection;

  std::vector<bool> m_visibleWaveforms;
};

}  // namespace fssp
