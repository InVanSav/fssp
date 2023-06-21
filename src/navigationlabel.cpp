#include "navigationlabel.h"

namespace fssp {

NavigationLabel::NavigationLabel(const int number, QWidget *parent)
    : QLabel{parent} {
  m_number = number;
}

}  // namespace fssp
