#include "mainwindow.h"

#include "modelingwindow.h"
#include "spectrumwindow.h"

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

void MainWindow::aboutFssp() {
  QMessageBox::about(this, tr("Fast Simple Signal Processing 0.2.0"),
                     tr("The software package "
                        "allows you to work and explore "
                        "discrete signals using built-in "
                        "analysis and statistics components, "
                        "as well as to simulate new signals.\n\n"
                        "Made by students of DVFU:\n"
                        "    Kuligin Kirill Nikolaevich\n"
                        "    Savickij Ivan Viktorovich\n"
                        "    Arefevef Egor Pavlovich"));
}

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

void MainWindow::save() {
  if (!m_tabWidget->count()) {
    QMessageBox::information(this, tr("Save file"),
                             tr("There is no open signal yet"));
    return;
  }

  SignalPage *signalPage =
      dynamic_cast<SignalPage *>(m_tabWidget->currentWidget());
  std::shared_ptr<SignalData> signalData = signalPage->getSignalData();

  QDialog *dialog = new QDialog();
  dialog->setWindowTitle(tr("Signal saving"));

  std::vector<QCheckBox *> checkBoxes;

  QHBoxLayout *checkBoxesLayout = new QHBoxLayout();

  for (int i = 0; i < signalData->channelsNumber(); ++i) {
    QCheckBox *checkBox = new QCheckBox(signalData->channelsName()[i]);
    checkBoxes.push_back(checkBox);

    checkBoxesLayout->addWidget(checkBox);
  }

  QSpinBox *spinBox = new QSpinBox();
  spinBox->setMaximum(INT_MAX);
  spinBox->setValue(signalData->samplesNumber());

  QDialogButtonBox *buttonBox =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(buttonBox);

  QLabel *noteCheckBoxes = new QLabel(tr("Choose the channel"));
  QLabel *noteSpinBox = new QLabel(tr("Samples number"));

  QVBoxLayout *dialogLayout = new QVBoxLayout();

  dialogLayout->addWidget(noteCheckBoxes);
  dialogLayout->addLayout(checkBoxesLayout);
  dialogLayout->addWidget(noteSpinBox);
  dialogLayout->addWidget(spinBox);
  dialogLayout->addLayout(buttonLayout);

  dialog->setLayout(dialogLayout);
  dialog->setFixedSize(dialog->sizeHint());

  dialog->exec();

  if (dialog->result() == QDialog::Rejected) dialog->reject();

  if (dialog->result() == QDialog::Accepted) {
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save File"), m_lastDir, tr("Text files (*.txt)"));

    QFile out(fileName + ".txt");
    if (out.open(QIODevice::WriteOnly)) {
      QTextStream stream(&out);
      stream << "# channels number\n";
      stream << signalData->channelsNumber() << "\n";
      stream << "# samples number\n";
      stream << spinBox->value() << "\n";
      stream << "# sampling rate\n";
      stream << signalData->rate() << "\n";
      stream << "# start date\n";
      stream << QLocale::system().toString(signalData->startTime().date(),
                                           "dd.MM.yyyy")
             << "\n";
      stream << "# start time\n";
      stream << QLocale::system().toString(signalData->startTime().time(),
                                           "hh:mm:ss.zzz")
             << "\n";
      stream << "# channels names\n";
      for (int i = 0; i < signalData->channelsNumber(); ++i) {
        if (!checkBoxes[i]->isChecked()) continue;
        stream << signalData->channelsName()[i] << ";";
      }
      stream << "\n";

      for (int i = 0; i < spinBox->value(); ++i) {
        for (int j = 0; j < signalData->channelsNumber(); ++j) {
          if (!checkBoxes[j]->isChecked()) continue;
          stream << signalData->data()[j][i] << " ";
        }
        stream << "\n";
      }
      out.close();
    }

    dialog->reject();
  }
}

void MainWindow::aboutSignal() {
  if (!m_tabWidget->count()) {
    QMessageBox::information(this, tr("About signal"),
                             tr("There is no open signal yet"));
    return;
  }

  SignalPage *signalPage =
      dynamic_cast<SignalPage *>(m_tabWidget->currentWidget());

  QMessageBox::about(
      this, tr("About signal"),
      tr("Total number of channels: ") +
          QString::number(signalPage->getSignalData()->channelsNumber()) +
          tr("\nTotal number of sampels: ") +
          QString::number(signalPage->getSignalData()->samplesNumber()) +
          tr("\nSampling frequency: ") +
          QString::number(signalPage->getSignalData()->rate()) +
          tr(" HZ (step between samples ") +
          QString::number(signalPage->getSignalData()->timeForOne()) +
          tr(" sec)\nDate and time the recording started: ") +
          QLocale::system().toString(signalPage->getSignalData()->startTime(),
                                     "dd.MM.yyyy hh:mm:ss.zzz") +
          tr("\nEnd date and time of recording: ") +
          QLocale::system().toString(signalPage->getSignalData()->endTime(),
                                     "dd.MM.yyyy hh:mm:ss.zzz"));
}

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
    QMessageBox::information(
        this, tr("Error"), tr("There is no open signal yet"), QMessageBox::Ok);
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
  signalData->setDefault();
  emit signalData->dataAdded();
}

void MainWindow::spectrumAnalize() {
  SignalPage *signalPage =
      dynamic_cast<SignalPage *>(m_tabWidget->currentWidget());
  std::shared_ptr<SignalData> signalData = signalPage->getSignalData();

  SpectrumWindow *spectrum = new SpectrumWindow(signalData);
  spectrum->show();
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

  m_saveAct = new QAction(tr("&Save"), this);
  connect(m_saveAct, &QAction::triggered, this, &MainWindow::save);

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

  m_spectrumAnalizeAct = new QAction(tr("Spectrum analize"), this);
  connect(m_spectrumAnalizeAct, &QAction::triggered, this,
          &MainWindow::spectrumAnalize);
}

void MainWindow::createMenus() {
  m_fileMenu = menuBar()->addMenu(tr("&File"));
  m_fileMenu->addAction(m_openAct);
  m_fileMenu->addAction(m_saveAct);
  m_fileMenu->addAction(m_aboutSignalAct);

  m_modelingMenu = menuBar()->addMenu(tr("&Modeling"));
  m_modelingMenu->addAction(m_modNewSignalAct);
  m_modelingMenu->addAction(m_modInCurSignalAct);

  m_analizeMenu = menuBar()->addMenu(tr("&Analysis"));
  m_analizeMenu->addAction(m_statisticAct);
  m_analizeMenu->addAction(m_spectrumAnalizeAct);

  m_filterMenu = menuBar()->addMenu(tr("&Filter"));

  m_settingsMenu = menuBar()->addMenu(tr("&Settings"));

  m_helpMenu = menuBar()->addMenu(tr("Help"));
  m_helpMenu->addAction(m_aboutFsspAct);
}

void MainWindow::chooseStatisticSignal() {
  if (!m_tabWidget->count()) {
    QMessageBox::information(
        this, tr("Error"), tr("There is no open signal yet"), QMessageBox::Ok);
    return;
  }

  SignalPage *signalPage =
      dynamic_cast<SignalPage *>(m_tabWidget->currentWidget());

  QDialog *dialog = new QDialog();
  dialog->setWindowTitle(tr("Statistic"));

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

  QLabel *noteComboBox = new QLabel(tr("Choose the channel"));
  QLabel *noteSpinBox = new QLabel(tr("Number of intervals"));

  QVBoxLayout *dialogLayout = new QVBoxLayout();

  dialogLayout->addWidget(noteComboBox);
  dialogLayout->addWidget(comboBox);
  dialogLayout->addWidget(noteSpinBox);
  dialogLayout->addWidget(spinBox);
  dialogLayout->addLayout(buttonLayout, Qt::AlignCenter);

  dialog->setLayout(dialogLayout);
  dialog->setFixedSize(dialog->sizeHint());

  dialog->exec();

  if (dialog->result() == QDialog::Rejected) dialog->reject();

  if (dialog->result() == QDialog::Accepted) {
    StatisticWindow *statistic =
        new StatisticWindow(signalPage->getSignalData(),
                            comboBox->currentIndex(), spinBox->value(), this);

    statistic->show();

    dialog->reject();
  }
}

}  // namespace fssp
