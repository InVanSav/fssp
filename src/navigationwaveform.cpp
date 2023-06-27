#include "navigationwaveform.h"

namespace fssp {

NavigationWaveform::NavigationWaveform(const int number, QWidget *parent)
    : QLabel{parent} {
  m_number = number;
  m_waveformVisible = false;
}

void NavigationWaveform::drawWaveform(const std::vector<double> &data,
                                      int width, int height, bool isSelected,
                                      int leftSelection, int rightSelection) {
  QImage image(width, height, QImage::Format_ARGB32);
  image.fill(qRgb(255, 255, 255));
  bresenhamDraw(data, 0, data.size() - 1, image, width, height, 0, 0, 0, 0,
                qRgb(0, 127, 255));

  QPixmap pixmap = QPixmap::fromImage(image);

  if (!isSelected) {
    setPixmap(pixmap);
    return;
  }

  int leftBorder = (leftSelection * width) / data.size();
  int rightBorder = (rightSelection * width) / data.size();

  QPainter *painter = new QPainter(&pixmap);
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(Qt::red);
  painter->setBrush(QColor{255, 0, 0, 100});
  painter->drawRect(QRect{QPoint{leftBorder, 0}, QPoint{rightBorder, height}});

  setPixmap(pixmap);
}

int NavigationWaveform::number() const { return m_number; }

void NavigationWaveform::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    showContextMenu(event->pos());
  }

  QLabel::mousePressEvent(event);
}

void NavigationWaveform::showContextMenu(const QPoint &pos) {
  QMenu menu(this);
  QAction *waveformAction = menu.addAction("Осциллограмма");
  waveformAction->setCheckable(true);
  waveformAction->setChecked(m_waveformVisible);

  connect(waveformAction, &QAction::triggered, this,
          &NavigationWaveform::onVisibilityAction);

  menu.exec(mapToGlobal(pos));
}

void NavigationWaveform::onVisibilityAction(bool visible) {
  m_waveformVisible = visible;

  //  emit visibilityChange(m_number, m_waveformVisible);
}

}  // namespace fssp
