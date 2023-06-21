#pragma once

#include "ideserializer.h"

namespace fssp {

struct TxtDeserializer : public IDeserializer {
 public:
  explicit TxtDeserializer() = default;
  SignalData operator()(const QString &absoluteFilePath) override;
};

}  // namespace fssp
