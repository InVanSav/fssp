#include "navigationdialog.h"

#include <QScrollArea>

namespace fssp {

NavigationDialog::NavigationDialog(std::shared_ptr<SignalData> data,
                                   QWidget *parent)
    : QWidget{parent} {
  m_data = data;
  m_waveforms = std::vector<NavigationWaveform *>(m_data->data()->size());

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
  scrollArea->setWidget(scrollContent);
  scrollArea->setWidgetResizable(true);

  QLabel *widgetTitle = new QLabel(tr("<b>Signals<\b>"));

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addSpacing(5);
  mainLayout->addWidget(widgetTitle);
  mainLayout->setAlignment(widgetTitle, Qt::AlignHCenter);
  mainLayout->addSpacing(5);
  mainLayout->addWidget(scrollArea);

  setLayout(mainLayout);

  setFixedWidth(sizeHint().rwidth() + 40);
}

void NavigationDialog::drawWaveforms() {
  for (int i = 0; i < m_waveforms.size(); ++i) {
    m_waveforms[i]->drawWaveform((*m_data->data())[i], m_leftSelection,
                                 m_rightSelection);
  }
}

}  // namespace fssp
