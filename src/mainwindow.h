#pragma once

#include <QMainWindow>

namespace fssp {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
};

}  // namespace fssp
