#include "graphdialog.h"

namespace fssp {

GraphDialog::GraphDialog(std::shared_ptr<SignalData> data, QWidget *parent)
    : QGroupBox{parent} {
  m_signalData = data;
  m_waveforms = std::vector<GraphWaveform *>(m_signalData->channelsNumber());

  scrollContent = new QWidget();

  QVBoxLayout *vBox = new QVBoxLayout();
  vBox->addSpacing(10);

  GraphWaveform *topWaveform = new GraphWaveform(m_signalData, 0);
  topWaveform->setTop();
  m_waveforms[0] = topWaveform;
  vBox->addWidget(topWaveform);

  for (int i = 1; i < m_signalData->channelsNumber() - 1; ++i) {
    GraphWaveform *midWaveform = new GraphWaveform(m_signalData, i);

    m_waveforms[i] = midWaveform;
    vBox->addWidget(midWaveform);
  }

  if (m_signalData->channelsNumber() > 1) {
    GraphWaveform *botWaveform =
        new GraphWaveform(m_signalData, m_signalData->channelsNumber() - 1);
    botWaveform->setBottom();
    m_waveforms[m_signalData->channelsNumber() - 1] = botWaveform;
    vBox->addWidget(botWaveform);
  }

  vBox->addSpacing(10);

  vBox->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  scrollContent->setLayout(vBox);

  QScrollArea *scrollArea = new QScrollArea();
  scrollArea->setFrameShape(QFrame::NoFrame);
  scrollArea->setWidget(scrollContent);
  scrollArea->setWidgetResizable(true);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(scrollArea);

  setLayout(mainLayout);

  connect(m_signalData.get(), &SignalData::changedWaveformVisibility, this,
          &GraphDialog::onChangedWaveformVisibility);

  setTitle(tr("Graphs"));
}

void GraphDialog::onChangedWaveformVisibility() {
  deleteWaveforms();
  drawWaveforms();
}

void GraphDialog::drawWaveforms() {
  for (int i = 0; i < m_waveforms.size(); ++i) {
    if (!m_signalData->visibleWaveforms()[i]) continue;
    m_waveforms[i]->drawWaveform();
    m_waveforms[i]->show();
  }
}

void GraphDialog::deleteWaveforms() {
  QList<GraphWaveform *> scrollChildren =
      scrollContent->findChildren<GraphWaveform *>();

  if (scrollChildren.isEmpty()) return;

  for (GraphWaveform *child : scrollChildren) child->hide();
}

// void GraphDialog::onSelectionFinished(int leftX, int rightX, int realWidth) {
//   m_signalData->setIsSelected(true);

//  double dataPerPixel =
//      (m_signalData->rightSelection() - m_signalData->leftSelection()) /
//      realWidth;

//  int leftSelection = m_signalData->leftSelection() + dataPerPixel * leftX;
//  int rightSelection = m_signalData->leftSelection() + dataPerPixel * rightX;

//  if (leftSelection > rightSelection) {
//    std::swap(leftSelection, rightSelection);
//  }

//  if (rightSelection - leftSelection < 8 &&
//      m_signalData->samplesNumber() > 16) {
//    if (m_signalData->samplesNumber() - rightSelection > 8) {
//      leftSelection = rightSelection;
//      rightSelection += 8;
//    } else {
//      rightSelection = leftSelection;
//      leftSelection -= 8;
//    }
//  }

//  m_signalData->setLeftSelection(leftSelection);
//  m_signalData->setRightSelection(rightSelection);

//  emit m_signalData->selectionEvent();
//}

}  // namespace fssp
