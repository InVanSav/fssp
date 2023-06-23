#pragma once

#include <QDateTime>

namespace fssp {

class SignalData : public QObject {
  Q_OBJECT
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

  bool isSelected() const;
  int leftSelection() const;
  int rightSelection() const;

  bool setIsSelected(bool isSelected);
  int setLeftSelection(int leftSelection);
  int setRightSelection(int rightSelection);

 signals:
  void selectionEvent();

 private:
  QDateTime m_startTime;
  QDateTime m_endTime;

  double m_rate;
  double m_timeForOne;
  double m_allTime;

  std::vector<QString> m_channelsName;
  std::vector<std::vector<double>> m_data;

  bool m_isSelected;
  int m_leftSelection;
  int m_rightSelection;
};

}  // namespace fssp
