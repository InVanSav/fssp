#include "mainwindow.h"

#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>

namespace fssp {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  createActions();
  createMenus();
}

MainWindow::~MainWindow() {}

void MainWindow::aboutFssp() {}

void MainWindow::open() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open file"), QDir::homePath(), tr("Text files (*.txt)"));
}

void MainWindow::aboutSignal() {}

void MainWindow::createActions() {
  aboutFsspAct = new QAction(tr("About"), this);
  connect(aboutFsspAct, &QAction::triggered, this, &MainWindow::aboutFssp);

  openAct = new QAction(tr("&Open"), this);
  connect(openAct, &QAction::triggered, this, &MainWindow::open);

  aboutSignalAct = new QAction(tr("About signal"), this);
  connect(aboutSignalAct, &QAction::triggered, this, &MainWindow::aboutSignal);
}

void MainWindow::createMenus() {
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openAct);
  fileMenu->addAction(aboutSignalAct);

  modelingMenu = menuBar()->addMenu(tr("&Modeling"));

  analizeMenu = menuBar()->addMenu(tr("&Analysis"));

  filterMenu = menuBar()->addMenu(tr("&Filter"));

  settingsMenu = menuBar()->addMenu(tr("&Settings"));

  helpMenu = menuBar()->addMenu(tr("Help"));
  helpMenu->addAction(aboutFsspAct);
}

}  // namespace fssp
