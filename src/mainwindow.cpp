#include "mainwindow.h"

#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>

#include "signalbuilder.h"

namespace fssp {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  createActions();
  createMenus();

  m_lastDir = QDir::homePath();
  m_tabWidget = new QTabWidget();
  setCentralWidget(m_tabWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::aboutFssp() {}

void MainWindow::open() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open file"), m_lastDir, tr("Text files (*.txt)"));

  if (fileName == "") {
    return;
  }

  QFileInfo fileInfo(fileName);
  m_lastDir = fileInfo.absolutePath();
  QString ext = fileInfo.suffix();

  SignalPage *signalPage;
  try {
    signalPage = SignalBuilder::FromFile(fileName, ext);
  } catch (SignalBuilder::FileTypeError) {
    QMessageBox msgBox(this);
    msgBox.setText(tr("File type is not supported."));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Ok) {
      emit(MainWindow::open());
    }
  }

  m_tabWidget->addTab(signalPage, fileInfo.fileName());
}

void MainWindow::aboutSignal() {}

void MainWindow::createActions() {
  m_aboutFsspAct = new QAction(tr("About"), this);
  connect(m_aboutFsspAct, &QAction::triggered, this, &MainWindow::aboutFssp);

  m_openAct = new QAction(tr("&Open"), this);
  connect(m_openAct, &QAction::triggered, this, &MainWindow::open);

  m_aboutSignalAct = new QAction(tr("About signal"), this);
  connect(m_aboutSignalAct, &QAction::triggered, this,
          &MainWindow::aboutSignal);
}

void MainWindow::createMenus() {
  m_fileMenu = menuBar()->addMenu(tr("&File"));
  m_fileMenu->addAction(m_openAct);
  m_fileMenu->addAction(m_aboutSignalAct);

  m_modelingMenu = menuBar()->addMenu(tr("&Modeling"));

  m_analizeMenu = menuBar()->addMenu(tr("&Analysis"));

  m_filterMenu = menuBar()->addMenu(tr("&Filter"));

  m_settingsMenu = menuBar()->addMenu(tr("&Settings"));

  m_helpMenu = menuBar()->addMenu(tr("Help"));
  m_helpMenu->addAction(m_aboutFsspAct);
}

}  // namespace fssp
