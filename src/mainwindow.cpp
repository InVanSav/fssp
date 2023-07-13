#include "mainwindow.h"

#include "modelingwindow.h"

namespace fssp {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  createActions();
  createMenus();

  m_lastDir = QDir::homePath();
  m_tabWidget = new QTabWidget();

  m_tabWidget->setTabsClosable(true);
  connect(m_tabWidget, &QTabWidget::tabCloseRequested, this,
          &MainWindow::handleCloseTabEvent);
  setCentralWidget(m_tabWidget);

  setMinimumWidth(900);
  setMinimumHeight(600);
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

void MainWindow::modNewSignal() {
  std::shared_ptr<SignalData> signalData = std::make_shared<SignalData>();

  if (m_tabWidget->count()) {
    SignalPage *signalPage =
        dynamic_cast<SignalPage *>(m_tabWidget->currentWidget());
    signalData = signalPage->getSignalData();
  }

  ModelingWindow *modWindow = new ModelingWindow(signalData, false, this);

  int ret = modWindow->exec();
  if (!ret) return;

  SignalPage *signalPage = new SignalPage(modWindow->getData());

  m_tabWidget->addTab(signalPage, tr("New File"));
}

void MainWindow::modInCurSignal() {
  if (!m_tabWidget->count()) {
    return;
  }

  SignalPage *signalPage =
      dynamic_cast<SignalPage *>(m_tabWidget->currentWidget());
  std::shared_ptr<SignalData> signalData = signalPage->getSignalData();

  ModelingWindow *modWindow = new ModelingWindow(signalData, true, this);

  int ret = modWindow->exec();
  if (!ret) return;

  SignalData modelingData = modWindow->getData();

  signalData->addData(modelingData.channelsName()[0], modelingData.data()[0]);
}

void MainWindow::handleCloseTabEvent(int index) {
  QWidget *signalPage = m_tabWidget->widget(index);
  m_tabWidget->removeTab(index);
  delete signalPage;
}

void MainWindow::createActions() {
  m_aboutFsspAct = new QAction(tr("About"), this);
  connect(m_aboutFsspAct, &QAction::triggered, this, &MainWindow::aboutFssp);

  m_openAct = new QAction(tr("&Open"), this);
  connect(m_openAct, &QAction::triggered, this, &MainWindow::open);

  m_aboutSignalAct = new QAction(tr("About signal"), this);
  connect(m_aboutSignalAct, &QAction::triggered, this,
          &MainWindow::aboutSignal);

  m_modNewSignalAct = new QAction(tr("Model new signal..."), this);
  connect(m_modNewSignalAct, &QAction::triggered, this,
          &MainWindow::modNewSignal);

  m_modInCurSignalAct = new QAction(tr("Model in current signal..."), this);
  connect(m_modInCurSignalAct, &QAction::triggered, this,
          &MainWindow::modInCurSignal);

  m_statisticAct = new QAction(tr("Statistic"), this);
  connect(m_statisticAct, &QAction::triggered, this,
          &MainWindow::chooseStatisticSignal);
}

void MainWindow::createMenus() {
  m_fileMenu = menuBar()->addMenu(tr("&File"));
  m_fileMenu->addAction(m_openAct);
  m_fileMenu->addAction(m_aboutSignalAct);

  m_modelingMenu = menuBar()->addMenu(tr("&Modeling"));
  m_modelingMenu->addAction(m_modNewSignalAct);
  m_modelingMenu->addAction(m_modInCurSignalAct);

  m_analizeMenu = menuBar()->addMenu(tr("&Analysis"));
  m_analizeMenu->addAction(m_statisticAct);

  m_filterMenu = menuBar()->addMenu(tr("&Filter"));

  m_settingsMenu = menuBar()->addMenu(tr("&Settings"));

  m_helpMenu = menuBar()->addMenu(tr("Help"));
  m_helpMenu->addAction(m_aboutFsspAct);
}

void MainWindow::chooseStatisticSignal() {
  if (!m_tabWidget->count()) {
    QMessageBox::information(nullptr, tr("Error"), tr("No signal open"),
                             QMessageBox::Ok);
    return;
  }

  SignalPage *signalPage =
      dynamic_cast<SignalPage *>(m_tabWidget->currentWidget());

  QDialog *dialog = new QDialog();
  dialog->setWindowTitle(tr("Choose Signal"));

  QComboBox *comboBox = new QComboBox();
  for (int i = 0; i < signalPage->getSignalData()->channelsNumber(); ++i) {
    comboBox->addItem(signalPage->getSignalData()->channelsName()[i]);
  }

  QSpinBox *spinBox = new QSpinBox();
  spinBox->setMaximum(INT_MAX);
  spinBox->setValue(100);

  QDialogButtonBox *buttonBox =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(buttonBox);

  QLabel *note = new QLabel(tr("Number of intervals"));

  QVBoxLayout *dialogLayout = new QVBoxLayout();
  dialogLayout->addWidget(comboBox);

  dialogLayout->addWidget(note);
  dialogLayout->addWidget(spinBox);
  dialogLayout->addLayout(buttonLayout, Qt::AlignCenter);

  dialog->setLayout(dialogLayout);
  dialog->setFixedSize(dialog->sizeHint());

  dialog->exec();

  if (dialog->result() == QDialog::Rejected) dialog->reject();

  if (dialog->result() == QDialog::Accepted) {
    Statistic *statistic =
        new Statistic(signalPage->getSignalData(), comboBox->currentIndex(),
                      spinBox->value());

    statistic->show();

    dialog->reject();
  }
}

}  // namespace fssp
