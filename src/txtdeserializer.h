#pragma once

#include <QFile>
#include <QTextStream>

#include "ideserializer.h"

namespace fssp {

struct TxtDeserializer : public IDeserializer {
 public:
  explicit TxtDeserializer() = default;
  SignalData operator()(const QString &absoluteFilePath) override;
};

}  // namespace fssp
