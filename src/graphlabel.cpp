#include "graphlabel.h"

QList<GraphLabel *> GraphLabel::allGraphLabels;

GraphLabel::GraphLabel(QWidget *parent) :
		QLabel(parent), isSelecting(false) {
	allGraphLabels.append(this);
}

void GraphLabel::setNumber(int number_) {
	number = number_;
}

int GraphLabel::getNumber() {
	return number;
}

bool GraphLabel::containsGraph(int number) {
	for (GraphLabel *graphLabel : GraphLabel::getAllGraphLabels()) {
		if (graphLabel->getNumber() == number) {
			return true;
		}
	}
	return false;
}

void GraphLabel::deleteGraph(int number) {
	QList<GraphLabel *> graphLabels = GraphLabel::getAllGraphLabels();
	for (GraphLabel *graphLabel : graphLabels) {
		if (graphLabel->getNumber() == number) {
			graphLabels.removeAll(graphLabel);
			delete graphLabel;
			break;
		}
	}
	GraphLabel::setAllGraphLabels(graphLabels);
}

QList<GraphLabel *> GraphLabel::getAllGraphLabels() {
	return allGraphLabels;
}

void GraphLabel::setAllGraphLabels(QList<GraphLabel *> graphLabels) {
	allGraphLabels = graphLabels;
}

void GraphLabel::mousePressEvent(QMouseEvent *event) {
	if (event->button() != Qt::LeftButton) {
		return;
	}

	isSelecting = true;
	startPoint = event->pos();
	startPoint.y =
			selectionRect = QRect();
}

void GraphLabel::mouseMoveEvent(QMouseEvent *event) {
	if (isSelecting) {
		QPoint currentPos = event->pos();
		int x = qMin(startPoint.x(), currentPos.x());
		int y = qMin(startPoint.y(), currentPos.y());
		int width = qMax(startPoint.x(), currentPos.x()) - x;
		int height = qMax(startPoint.y(), currentPos.y()) - y;
		selectionRect = QRect(x, y, width, height);
		update();
	}
}

void GraphLabel::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton && isSelecting && !selectionRect.isNull()) {
		isSelecting = false;
		emit selectionFinished(selectionRect, number);
	}
}

void GraphLabel::paintEvent(QPaintEvent *event) {
	QLabel::paintEvent(event);
	if (isSelecting) {
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setPen(Qt::red);
		painter.setBrush(QColor(255, 0, 0, 100));
		painter.drawRect(selectionRect);
	}
}
