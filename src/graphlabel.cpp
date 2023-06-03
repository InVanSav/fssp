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

QList<int> GraphLabel::getNumbers() {
	QList<int> numbers;
	for (int i = 0; i < allGraphLabels.size(); ++i) {
		numbers.append(allGraphLabels[i]->number);
	}

	return numbers;
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
	allGraphLabels.clear();
	allGraphLabels = graphLabels;
}

void GraphLabel::mousePressEvent(QMouseEvent *event) {
	if (event->button() != Qt::LeftButton) {
		return;
	}

	isSelecting = true;
	startPoint = event->pos();

	startPoint.setY(OFFSET_START_Y);

	if (startPoint.x() < OFFSET_START_X) {
		startPoint.setX(OFFSET_START_X);
	}

	if (startPoint.x() > (OFFSET_START_X + GRAPH_WIDTH)) {
		startPoint.setX(OFFSET_START_X + GRAPH_WIDTH);
	}

	selectionRect = QRect();
}

void GraphLabel::mouseMoveEvent(QMouseEvent *event) {
	if (isSelecting) {
		QPoint currentPos = event->pos();

		currentPos.setY(OFFSET_START_Y + GRAPH_HEIGHT);

		if (currentPos.x() < OFFSET_START_X) {
			currentPos.setX(OFFSET_START_X);
		}

		if (currentPos.x() > (OFFSET_START_X + GRAPH_WIDTH)) {
			currentPos.setX(OFFSET_START_X + GRAPH_WIDTH);
		}

		selectionRect = startPoint.x() > currentPos.x() ? QRect(currentPos, startPoint) : QRect(startPoint, currentPos);
		update();
	}
}

void GraphLabel::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton && isSelecting && !selectionRect.isNull()) {
		isSelecting = false;
		emit selectionFinished(selectionRect);
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
