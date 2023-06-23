#include "navigationdialog.h"

namespace fssp {

NavigationDialog::NavigationDialog(std::shared_ptr<SignalData> data,
                                   QWidget *parent)
    : QGroupBox{parent} {
  m_data = data;
  m_waveforms = std::vector<NavigationWaveform *>(m_data->data()->size());
  m_isSelected = false;
  m_leftSelection = 0;
  m_rightSelection = 0;

  scrollContent = new QWidget();

  QVBoxLayout *vBox = new QVBoxLayout();
  vBox->addSpacing(10);
  for (int i = 0; i < m_data->data()->size(); ++i) {
    NavigationWaveform *waveform = new NavigationWaveform(i, this);
    m_waveforms[i] = waveform;
    vBox->addWidget(waveform);
    QLabel *title = new QLabel((*m_data->channelsName())[i]);
    vBox->addWidget(title);
    vBox->setAlignment(title, Qt::AlignHCenter);
    if (i != m_data->data()->size() - 1) {
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

  setFixedWidth(sizeHint().rwidth() + 40);

  setTitle(tr("Signals"));
}

void NavigationDialog::drawWaveforms() {
  for (int i = 0; i < m_waveforms.size(); ++i) {
    m_waveforms[i]->drawWaveform((*m_data->data())[i], m_isSelected,
                                 m_leftSelection, m_rightSelection);
  }
}

}  // namespace fssp
