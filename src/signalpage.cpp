#include "signalpage.h"

namespace fssp {

SignalPage::SignalPage(SignalData &&data, QWidget *parent) : QWidget{parent} {
  m_data = std::make_shared<SignalData>(std::move(data));

  m_navDialog = new NavigationDialog(m_data);
}

}  // namespace fssp
