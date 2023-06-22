#include "signalpage.h"

#include <QHBoxLayout>

namespace fssp {

SignalPage::SignalPage(SignalData &&data, QWidget *parent) : QWidget{parent} {
  m_data = std::make_shared<SignalData>(std::move(data));

  m_navDialog = new NavigationDialog(m_data);

  QHBoxLayout *hBox = new QHBoxLayout();
  hBox->addWidget(m_navDialog);
  hBox->setAlignment(m_navDialog, Qt::AlignLeft);

  setLayout(hBox);
}

}  // namespace fssp
