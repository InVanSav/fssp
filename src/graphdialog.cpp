#include "graphdialog.h"

namespace fssp {

GraphDialog::GraphDialog(std::shared_ptr<SignalData> data, QWidget *parent)
    : QGroupBox{parent} {
  m_data = data;
  m_waveforms = std::vector<GraphWaveform *>(m_data->channelsNumber());
  m_waveformWidth = 900;
  m_waveformHeight = 300;

  scrollContent = new QWidget();

  QVBoxLayout *vBox = new QVBoxLayout();
  vBox->addSpacing(10);
  for (int i = 0; i < m_data->channelsNumber(); ++i) {
    GraphWaveform *waveform = new GraphWaveform(i, this);
    connect(waveform, &GraphWaveform::selectionFinished, this,
            &GraphDialog::onSelectionFinished);
    m_waveforms[i] = waveform;
    vBox->addWidget(waveform);

    QLabel *title = new QLabel(m_data->channelsName()[i]);
    vBox->addWidget(title);
    vBox->setAlignment(title, Qt::AlignHCenter);

    if (i != m_data->channelsNumber() - 1) {
      vBox->addSpacing(20);
    }
  }
  vBox->addSpacing(10);

  vBox->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  scrollContent->setLayout(vBox);

  drawWaveforms();

  QScrollArea *scrollArea = new QScrollArea();
  scrollArea->setFrameShape(QFrame::NoFrame);
  scrollArea->setWidget(scrollContent);
  scrollArea->setWidgetResizable(true);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(scrollArea);

  setLayout(mainLayout);

  setTitle(tr("Graphs"));
}

void GraphDialog::drawWaveforms() {
  for (int i = 0; i < m_waveforms.size(); ++i) {
    //    if (!m_data->visibleWaveforms()[i]) continue;
    m_waveforms[i]->drawWaveform(m_data->data()[i], m_data->allTime(),
                                 m_waveformWidth, m_waveformHeight,
                                 m_data->isSelected(), m_data->leftSelection(),
                                 m_data->rightSelection());
  }
}

void GraphDialog::onSelectionFinished(int leftX, int rightX, int realWidth) {
  m_data->setIsSelected(true);

  double dataPerPixel =
      (m_data->rightSelection() - m_data->leftSelection()) / realWidth;

  int leftSelection = m_data->leftSelection() + dataPerPixel * leftX;
  int rightSelection = m_data->leftSelection() + dataPerPixel * rightX;

  if (leftSelection > rightSelection) {
    std::swap(leftSelection, rightSelection);
  }

  if (rightSelection - leftSelection < 8 && m_data->samplesNumber() > 16) {
    if (m_data->samplesNumber() - rightSelection > 8) {
      leftSelection = rightSelection;
      rightSelection += 8;
    } else {
      rightSelection = leftSelection;
      leftSelection -= 8;
    }
  }

  m_data->setLeftSelection(leftSelection);
  m_data->setRightSelection(rightSelection);

  emit m_data->selectionEvent();
}

}  // namespace fssp
