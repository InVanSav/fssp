#pragma once

#include "signaldata.h"

namespace fssp {

struct BaseDeserializer {
  virtual SignalData operator()(const QString &absoluteFilePath) = 0;
};

}  // namespace fssp
