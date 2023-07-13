#include "navigationdialog.h"

namespace fssp {

NavigationDialog::NavigationDialog(std::shared_ptr<SignalData> data,
                                   QWidget *parent)
    : QGroupBox{parent} {
  m_signalData = data;

  connect(m_signalData.get(), &SignalData::dataAdded, this,
          &NavigationDialog::onDataAdded);

  m_scrollArea = new QScrollArea();
  m_scrollArea->setFrameShape(QFrame::NoFrame);
  m_scrollArea->setWidgetResizable(true);

  addWaveforms();

  drawWaveforms();

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(m_scrollArea);

  setLayout(mainLayout);

  setFixedWidth(sizeHint().rwidth() + 40);

  setTitle(tr("Channels"));
}

void NavigationDialog::addWaveforms() {
  QVBoxLayout *vBox = new QVBoxLayout();
  vBox->addSpacing(10);

  m_waveforms =
      std::vector<NavigationWaveform *>(m_signalData->channelsNumber());
  for (int i = 0; i < m_signalData->channelsNumber(); ++i) {
    NavigationWaveform *waveform = new NavigationWaveform(m_signalData, i);

    waveform->setIsVisible(m_signalData->visibleWaveforms()[i]);

    m_waveforms[i] = waveform;
    vBox->addWidget(waveform);
  }
  vBox->addSpacing(10);

  vBox->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

  QWidget *scrollContent = new QWidget();
  scrollContent->setLayout(vBox);

  m_scrollArea->setWidget(scrollContent);
}

void NavigationDialog::drawWaveforms() {
  for (int i = 0; i < m_waveforms.size(); ++i) {
    m_waveforms[i]->drawWaveform();
  }
}

void NavigationDialog::onDataAdded() {
  addWaveforms();
  drawWaveforms();
}

}  // namespace fssp
