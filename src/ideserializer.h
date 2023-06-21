#pragma once

#include "signaldata.h"

namespace fssp {

struct IDeserializer {
  virtual SignalData operator()(const QString &absoluteFilePath) = 0;
};

}  // namespace fssp
