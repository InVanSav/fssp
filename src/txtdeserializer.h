#pragma once

#include <QFile>
#include <QTextStream>

#include "basedeserializer.h"

namespace fssp {

struct TxtDeserializer : public BaseDeserializer {
 public:
  explicit TxtDeserializer() = default;
  SignalData operator()(const QString &absoluteFilePath) override;
};

}  // namespace fssp
