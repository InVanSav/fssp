#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void aboutFssp();
  void open();

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
};
#endif  // MAINWINDOW_H
