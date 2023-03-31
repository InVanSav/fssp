#include "mainwindow.h"

#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  createActions();
  createMenus();
}

MainWindow::~MainWindow() {}

void MainWindow::aboutFssp() {
  QMessageBox aboutFssp;
  aboutFssp.setText(tr("Fast Simple Signal Processing 0.1.1"));
  aboutFssp.setInformativeText(
      tr("Made by students of DVFU:\n"
         "    Kuligin Kirill\n"
         "    Savickij Ivan\n"
         "    Arefevef Egor"));
  aboutFssp.exec();
}

void MainWindow::open() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open file"), QDir::homePath(), tr("Text files (*.txt)"));
  QTextStream out(stdout);
  out << fileName;
}

void MainWindow::createActions() {
  aboutFsspAct = new QAction(tr("About"), this);
  connect(aboutFsspAct, &QAction::triggered, this, &MainWindow::aboutFssp);

  openAct = new QAction(tr("&Open"), this);
  connect(openAct, &QAction::triggered, this, &MainWindow::open);
}

void MainWindow::createMenus() {
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openAct);

  modelingMenu = menuBar()->addMenu(tr("&Modeling"));

  analizeMenu = menuBar()->addMenu(tr("&Analysis"));

  filterMenu = menuBar()->addMenu(tr("&Filter"));

  settingsMenu = menuBar()->addMenu(tr("&Settings"));

  helpMenu = menuBar()->addMenu(tr("Help"));
  helpMenu->addAction(aboutFsspAct);
}
