#include "mainwindow.h"

#include "signalmodels.h"

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
  QDialog dialog;
  dialog.setWindowTitle("Моделирование");
  dialog.setGeometry(100, 100, 400, 200);

  QHBoxLayout *mainLayout = new QHBoxLayout(&dialog);

  QGroupBox *groupBox1 = new QGroupBox("Выберите сигнал", &dialog);
  QVBoxLayout *groupBoxLayout1 = new QVBoxLayout(groupBox1);

  QComboBox *comboBox = new QComboBox(groupBox1);
  comboBox->addItem("Задержанный единичный импульс");
  comboBox->addItem("Задержанный единичный скачок");
  comboBox->addItem("Дискретизированная убывающая экспонента");
  comboBox->addItem("Дискретизированная синусоида");
  comboBox->addItem("'Меандр' (прямоугольная решетка)");
  comboBox->addItem("'Пила'");

  QFormLayout *formLayout = new QFormLayout(groupBox1);

  QLineEdit *lineEdit1 = new QLineEdit(groupBox1);
  formLayout->addRow("Поле 1:", lineEdit1);

  QLineEdit *lineEdit2 = new QLineEdit(groupBox1);
  formLayout->addRow("Поле 2:", lineEdit2);

  groupBoxLayout1->addWidget(comboBox);
  groupBoxLayout1->addLayout(formLayout);
  mainLayout->addWidget(groupBox1);

  QGroupBox *groupBox2 = new QGroupBox("График канала", &dialog);
  QHBoxLayout *groupBoxLayout2 = new QHBoxLayout(groupBox2);

  mainLayout->addWidget(groupBox2);

  dialog.exec();
}

void MainWindow::modInCurSignal() {}

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
}

void MainWindow::createMenus() {
  m_fileMenu = menuBar()->addMenu(tr("&File"));
  m_fileMenu->addAction(m_openAct);
  m_fileMenu->addAction(m_aboutSignalAct);

  m_modelingMenu = menuBar()->addMenu(tr("&Modeling"));
  m_modelingMenu->addAction(m_modNewSignalAct);
  m_modelingMenu->addAction(m_modInCurSignalAct);

  m_analizeMenu = menuBar()->addMenu(tr("&Analysis"));

  m_filterMenu = menuBar()->addMenu(tr("&Filter"));

  m_settingsMenu = menuBar()->addMenu(tr("&Settings"));

  m_helpMenu = menuBar()->addMenu(tr("Help"));
  m_helpMenu->addAction(m_aboutFsspAct);
}

}  // namespace fssp
