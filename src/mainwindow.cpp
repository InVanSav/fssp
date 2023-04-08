#include "mainwindow.h"

#include "signal.h"
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent) {
	createActions();
	createMenus();
}

MainWindow::~MainWindow() {}

void MainWindow::aboutFssp() {
	QMessageBox aboutFssp;
	aboutFssp.setText(tr("Fast Simple Signal Processing 0.2.0"));
	aboutFssp.setInformativeText(
			tr("Made by students of DVFU:\n"
			   "    Kuligin Kirill\n"
			   "    Savickij Ivan\n"
			   "    Arefevef Egor"));
	aboutFssp.exec();
}

void MainWindow::open() {
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open file"),
			QDir::homePath(),
			tr("Text files (*.txt)"));

	signal = new Signal(fileName);
	signal->show();
}

void MainWindow::aboutSignal() {
	if (signal == nullptr) {
		QMessageBox::about(this, tr("About signal"), tr("There is no open signal yet."));
	} else {
		QMessageBox::about(this,
				tr("About signal"),
				tr("Total number of channels - ") + QString::number(signal->reader->channels_num) + tr("\nTotal number of sampels - ") + QString::number(signal->reader->data_num) + tr("\nSampling frequency - ") + QString::number(signal->reader->rate) + tr(" HZ (step between samples ") + QString::number(signal->reader->time_for_r) + tr(" sec)\nDate and time the recording started - ") + signal->reader->start.toString() + tr("\nEnd date and time of recording - ") + signal->reader->end.toString() + tr("\nDuration - ") + signal->reader->durance.toString());
	}
}

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
