#pragma once

#include "QDialog"
#include "QLabel"
#include "QMenu"
#include "QMouseEvent"

class WaveformsDialog : public QDialog {
	Q_OBJECT
public:
	WaveformsDialog(QWidget *parent = nullptr) :
			QDialog(parent) {
		setWindowTitle("Осциллограмма");
		setMinimumSize(600, 400);
	}
};

class CustomLabel : public QLabel {
	Q_OBJECT
public:
	CustomLabel(QWidget *parent = nullptr) :
			QLabel(parent), m_waveformChecked_(false) {
		m_waveformAction_ = new QAction("Осциллограмма", this);
		m_waveformAction_->setCheckable(true);
		connect(m_waveformAction_, &QAction::triggered, this, &CustomLabel::showWaveform);

		m_contextMenu_ = new QMenu(this);
		m_contextMenu_->addAction(m_waveformAction_);
	}

protected:
	void mousePressEvent(QMouseEvent *event) override {
		if (event->button() == Qt::RightButton) {
			m_waveformChecked_ = m_waveformAction_->isChecked();
			m_contextMenu_->exec(event->globalPosition().toPoint());
		}
	}

private:
	void showWaveform() {
		QAction *action = qobject_cast<QAction *>(sender());
		if (action) {
			m_waveformChecked_ = action->isChecked();
			if (m_waveformChecked_) {
				// Если стоит галочка
			} else {
				// Галочки нет
			}
		}
	}

private:
	QAction *m_waveformAction_;
	QMenu *m_contextMenu_;
	bool m_waveformChecked_;
};
