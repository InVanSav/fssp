#pragma once

#include <QWidget>

#include "basemodel.h"

namespace fssp {

class ModelingWindow : public QWidget {
  Q_OBJECT
 public:
  explicit ModelingWindow(QWidget *parent = nullptr);

 private:
  BaseModel *model;
};

}  // namespace fssp
