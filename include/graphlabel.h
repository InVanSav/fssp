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
	static void deleteGraph(int number_);

	static QList<GraphLabel *> getAllGraphLabels();
	static void setAllGraphLabels(QList<GraphLabel *> graphLabels);

signals:
	void selectionFinished(const QRect &rect, int number_);

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

	static QList<GraphLabel *> allGraphLabels;
};
