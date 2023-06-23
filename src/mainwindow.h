#pragma once

#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>

#include "signalbuilder.h"

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
  void handleCloseTabEvent(int index);

 private:
  void createActions();
  void createMenus();

  QString m_lastDir;
  QTabWidget *m_tabWidget;

  QMenu *m_fileMenu;
  QMenu *m_modelingMenu;
  QMenu *m_analizeMenu;
  QMenu *m_filterMenu;
  QMenu *m_settingsMenu;
  QMenu *m_helpMenu;

  QAction *m_aboutFsspAct;
  QAction *m_openAct;
  QAction *m_aboutSignalAct;
};

}  // namespace fssp
