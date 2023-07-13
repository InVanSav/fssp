#include "signalpage.h"

namespace fssp {

SignalPage::SignalPage(const SignalData data, QWidget *parent)
    : QWidget{parent} {
  m_signalData = std::make_shared<SignalData>(std::move(data));

  m_isSelected = false;
  m_leftSelection = 0;
  m_rightSelection = 0;

  m_navDialog = new NavigationDialog(m_signalData);
  m_graphDialog = new GraphDialog(m_signalData);

  QHBoxLayout *hBox = new QHBoxLayout();
  hBox->addWidget(m_navDialog);
  hBox->setAlignment(m_navDialog, Qt::AlignLeft);
  hBox->addSpacing(10);
  hBox->addWidget(m_graphDialog);

  setLayout(hBox);
}

std::shared_ptr<SignalData> SignalPage::getSignalData() { return m_signalData; }

}  // namespace fssp
