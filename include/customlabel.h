#pragma once

#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QPoint>
#include <QVBoxLayout>
#include <QVector>

#include "filereader.h"
#include "graphlabel.h"
#include "waveformsdialog.h"

class WaveformsDialog;

class CustomLabel : public QLabel {
	Q_OBJECT
public:
	CustomLabel(FileReader *reader_, int number_, QWidget *parent = nullptr);

	static QList<CustomLabel *> &getAllLabels();
	static void clearAllLabels(QVBoxLayout *layout);
	static void disableCheckedLabelsMenu();

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void showContextMenu(const QPoint &pos);

private slots:
	void openDialog(bool enabled);

private:
	bool waveformEnabled = false;
	int number;
	const int WIDTH = 600, HEIGHT = 300;

	FileReader *reader;

	static WaveformsDialog *waveformsDialog;
	static QList<CustomLabel *> allCustomLabels;
	static QList<CustomLabel *> checkedLabels;
};
