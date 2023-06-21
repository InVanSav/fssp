#include "signalbuilder.h"

#include "ideserializer.h"
#include "txtdeserializer.h"

namespace fssp {

SignalBuilder::SignalBuilder() {}

SignalPage *SignalBuilder::FromFile(const QString &absoluteFilePath,
                                    const QString &fileExtension) {
  IDeserializer *deserializer;
  if (fileExtension == "txt") {
    deserializer = new TxtDeserializer();
  } else {
    throw SignalBuilder::FileTypeError();
  }

  SignalData data = deserializer->operator()(absoluteFilePath);
  return new SignalPage(std::move(data));
}

}  // namespace fssp
