#include "navigationdialog.h"

namespace fssp {

NavigationDialog::NavigationDialog(std::shared_ptr<SignalData> data,
                                   QWidget *parent)
    : QGroupBox{parent} {
  m_signalData = data;
  m_waveforms =
      std::vector<NavigationWaveform *>(m_signalData->channelsNumber());

  scrollContent = new QWidget();

  QVBoxLayout *vBox = new QVBoxLayout();
  vBox->addSpacing(10);
  for (int i = 0; i < m_signalData->channelsNumber(); ++i) {
    NavigationWaveform *waveform = new NavigationWaveform(m_signalData, i);

    m_waveforms[i] = waveform;
    vBox->addWidget(waveform);
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

  setFixedWidth(sizeHint().rwidth() + 40);

  setTitle(tr("Signals"));
}

void NavigationDialog::drawWaveforms() {
  for (int i = 0; i < m_waveforms.size(); ++i) {
    m_waveforms[i]->drawWaveform();
  }
}

}  // namespace fssp
