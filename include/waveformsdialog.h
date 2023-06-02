#pragma once

#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QPen>
#include <QScrollArea>
#include <QVBoxLayout>

#include "customlabel.h"
#include "filereader.h"
#include "graphlabel.h"

class CustomLabel;

class WaveformsDialog : public QDialog {
	Q_OBJECT
public:
	WaveformsDialog(FileReader *reader_, QWidget *parent = nullptr);

protected:
	void closeEvent(QCloseEvent *event) override;

public slots:
	void drawGraph(const QVector<double> &channel, QImage &image,
			FileReader *reader, int number, QRect rect = QRect());
	void onSelectionFinished(const QRect &rect, int number_);

private slots:
	void handleButtonClicked();

private:
	void setupUI();

private:
	FileReader *reader;

	const int WIDTH = 600, HEIGHT = 300;
	const int OFFSET_START_X = 70, OFFSET_END_X = 50;
	const int OFFSET_START_Y = 70, OFFSET_END_Y = 50;
	const int GRAPH_WIDTH = WIDTH - (OFFSET_START_X + OFFSET_END_X);

	double leftTime, rightTime;
	int leftArray, rightArray;

	QScrollArea *scrollArea;
	QWidget *scrollContent;
	QVBoxLayout *graphLayout;
};
