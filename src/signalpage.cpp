#include "signalpage.h"

#include <QHBoxLayout>

namespace fssp {

SignalPage::SignalPage(SignalData &&data, QWidget *parent) : QWidget{parent} {
  m_data = std::make_shared<SignalData>(std::move(data));

  m_navDialog = new NavigationDialog(m_data);
  m_graphDialog = new GraphDialog(m_data);

  QHBoxLayout *hBox = new QHBoxLayout();
  hBox->addWidget(m_navDialog);
  hBox->setAlignment(m_navDialog, Qt::AlignLeft);
  hBox->addWidget(m_graphDialog);

  setLayout(hBox);
}

}  // namespace fssp
