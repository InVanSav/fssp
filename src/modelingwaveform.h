#pragma once

#include <QWidget>
#include <QLabel>

namespace fssp {

class ModelingWaveform : public QWidget {
  Q_OBJECT
 public:
  explicit ModelingWaveform(QWidget *parent = nullptr);

 private:
  QLabel *waveform;
  QLabel *name;
};

}  // namespace fssp
