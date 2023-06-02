#include "customlabel.h"

QList<CustomLabel *> CustomLabel::allCustomLabels;
QList<CustomLabel *> CustomLabel::checkedLabels;
GraphsDialog *CustomLabel::waveformsDialog = nullptr;

CustomLabel::CustomLabel(FileReader *reader_, int number_, QWidget *parent) :
		reader(reader_), number(number_), QLabel(parent) {
	setContextMenuPolicy(Qt::CustomContextMenu);
	allCustomLabels.append(this);
}

QList<CustomLabel *> &CustomLabel::getAllLabels() {
	return allCustomLabels;
}

void CustomLabel::clearAllLabels(QVBoxLayout *layout) {
	qDeleteAll(allCustomLabels);
	qDeleteAll(checkedLabels);
	allCustomLabels.clear();
	checkedLabels.clear();

	while (QLayoutItem *item = layout->takeAt(0)) {
		delete item->widget();
		delete item;
	}

	layout->parentWidget()->close();
	delete layout;

	if (waveformsDialog) {
		delete waveformsDialog;
		waveformsDialog = nullptr;
	}
}

void CustomLabel::disableCheckedLabelsMenu() {
	for (int i = 0; i < checkedLabels.size(); i++) {
		checkedLabels[i]->waveformEnabled = false;
	}
	checkedLabels.clear();
	waveformsDialog = nullptr;
}

void CustomLabel::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::RightButton) {
		showContextMenu(event->pos());
	}
	QLabel::mousePressEvent(event);
}

void CustomLabel::showContextMenu(const QPoint &pos) {
	QMenu menu(this);
	QAction *waveformAction = menu.addAction("Осциллограмма");
	waveformAction->setCheckable(true);
	waveformAction->setChecked(waveformEnabled);

	connect(waveformAction, &QAction::triggered, this, &CustomLabel::openDialog);

	menu.exec(mapToGlobal(pos));
}

void CustomLabel::openDialog(bool enabled) {
	waveformEnabled = enabled;

	if (waveformEnabled) {
		if (!checkedLabels.contains(this)) {
			checkedLabels.append(this);
		}
		if (!waveformsDialog) {
			waveformsDialog = new GraphsDialog(reader);
			waveformsDialog->setAttribute(Qt::WA_DeleteOnClose);
			waveformsDialog->show();
		}

		QImage image(WIDTH, HEIGHT, QImage::Format_ARGB32);
		image.fill(Qt::white);
		waveformsDialog->drawGraph(reader->data[number], image, reader, number);
	} else {
		GraphLabel::deleteGraph(this->number);
		checkedLabels.removeAll(this);
		if (checkedLabels.isEmpty()) {
			delete waveformsDialog;
			waveformsDialog = nullptr;
		}
		waveformEnabled = false;
	}
}
