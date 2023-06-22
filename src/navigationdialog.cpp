#include "navigationdialog.h"

#include <QScrollArea>

namespace fssp {

NavigationDialog::NavigationDialog(std::shared_ptr<SignalData> data,
                                   QWidget *parent)
    : QWidget{parent} {
  m_data = data;
  m_waveforms = std::vector<NavigationWaveform *>(m_data->data()->size());

  vBox = new QVBoxLayout();
  for (int i = 0; i < m_data->data()->size(); ++i) {
    NavigationWaveform *waveform = new NavigationWaveform(i, this);
    m_waveforms[i] = waveform;
    vBox->addWidget(waveform);
    QLabel *title =
        new QLabel(QString::fromStdString((*m_data->channelsName())[i]));
    vBox->addWidget(title);
  }

  drawWaveforms();

  QScrollArea *scrollArea = new QScrollArea(this);
  scrollArea->setLayout(vBox);
}

void NavigationDialog::drawWaveforms() {
  for (int i = 0; i < m_waveforms.size(); ++i) {
    m_waveforms[i]->drawWaveform((*m_data->data())[i], m_leftSelection,
                                 m_rightSelection);
  }
}

}  // namespace fssp
