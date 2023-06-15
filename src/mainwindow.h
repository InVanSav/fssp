#pragma once

#include <QMainWindow>

namespace fssp {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void aboutFssp();
  void open();
  void aboutSignal();

 private:
  void createActions();
  void createMenus();

  QMenu *fileMenu;
  QMenu *modelingMenu;
  QMenu *analizeMenu;
  QMenu *filterMenu;
  QMenu *settingsMenu;
  QMenu *helpMenu;

  QAction *aboutFsspAct;
  QAction *openAct;
  QAction *aboutSignalAct;
};

}  // namespace fssp
