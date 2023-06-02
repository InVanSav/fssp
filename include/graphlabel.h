#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

class GraphLabel : public QLabel {
	Q_OBJECT

public:
	GraphLabel(QWidget *parent = nullptr);

	void setNumber(int number_);
	int getNumber();

	static bool containsGraph(int number_);
	static void deleteGraphs();

	static void deleteGraph(int number);
	static QList<int> getNumbers();

	static QList<GraphLabel *> getAllGraphLabels();
	static void setAllGraphLabels(QList<GraphLabel *> graphLabels);

signals:
	void selectionFinished(const QRect &rect);

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

	void paintEvent(QPaintEvent *event) override;

private:
	int number;

	bool isSelecting;
	QPoint startPoint;
	QRect selectionRect;

	const int WIDTH = 600, HEIGHT = 300;
	const int OFFSET_START_X = 70, OFFSET_END_X = 50;
	const int OFFSET_START_Y = 70, OFFSET_END_Y = 50;
	const int GRAPH_WIDTH = WIDTH - (OFFSET_START_X + OFFSET_END_X);
	const int GRAPH_HEIGHT = HEIGHT - (OFFSET_START_Y + OFFSET_END_Y);

	static QList<GraphLabel *> allGraphLabels;
};
