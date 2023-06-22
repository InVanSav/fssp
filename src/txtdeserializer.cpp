#include "txtdeserializer.h"

namespace fssp {

SignalData TxtDeserializer::operator()(const QString &absoluteFilePath) {
  std::vector<QString> v = {"test1", "test2"};
  return SignalData(QDateTime(), QDateTime(), 10, 1, 100, std::move(v),
                    {{10.0, 20.0, 30.0, 40.0}});
}

}  // namespace fssp
