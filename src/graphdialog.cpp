#include "graphdialog.h"

namespace fssp {

GraphDialog::GraphDialog(std::shared_ptr<SignalData> data, QWidget *parent)
    : QGroupBox{parent} {
  p_signalData = data;
  p_waveforms = std::vector<GraphWaveform *>(p_signalData->channelsNumber());

  p_scrollContent = new QWidget();

  QVBoxLayout *vBox = new QVBoxLayout();
  vBox->addSpacing(10);

  GraphWaveform *topWaveform = new GraphWaveform(p_signalData, 0);
  topWaveform->setTop();
  p_waveforms[0] = topWaveform;
  vBox->addWidget(topWaveform);

  for (int i = 1; i < p_signalData->channelsNumber() - 1; ++i) {
    GraphWaveform *midWaveform = new GraphWaveform(p_signalData, i);

    p_waveforms[i] = midWaveform;
    vBox->addWidget(midWaveform);
  }

  if (p_signalData->channelsNumber() > 1) {
    GraphWaveform *botWaveform =
        new GraphWaveform(p_signalData, p_signalData->channelsNumber() - 1);
    botWaveform->setBottom();
    p_waveforms[p_signalData->channelsNumber() - 1] = botWaveform;
    vBox->addWidget(botWaveform);
  }

  vBox->addSpacing(10);

  vBox->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  p_scrollContent->setLayout(vBox);

  QScrollArea *scrollArea = new QScrollArea();
  scrollArea->setFrameShape(QFrame::NoFrame);
  scrollArea->setWidget(p_scrollContent);
  scrollArea->setWidgetResizable(true);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  QMenuBar *menuBar = new QMenuBar(this);

  QAction *gridAction = menuBar->addAction(tr("Enable grid"));
  gridAction->setCheckable(true);
  gridAction->setChecked(p_signalData->isGridEnabled());

  connect(gridAction, &QAction::triggered, this,
          &GraphDialog::enableGridAction);

  QAction *scaleAction = menuBar->addAction(tr("Scale graph"));
  connect(scaleAction, &QAction::triggered, this,
          &GraphDialog::scaleGraphWaveformAction);

  //  QAction *rangeAction = menuBar->addAction(tr("Enable grid"));
  //  gridAction->setCheckable(true);
  //  gridAction->setChecked(p_signalData->isGridEnabled());

  //  connect(gridAction, &QAction::triggered, this,
  //          &GraphDialog::enableGridAction);

  mainLayout->setMenuBar(menuBar);

  mainLayout->addWidget(scrollArea);

  setLayout(mainLayout);

  connect(p_signalData.get(), &SignalData::changedWaveformVisibility, this,
          &GraphDialog::onChangedWaveformVisibility);

  setTitle(tr("Graphs"));
}

void GraphDialog::onChangedWaveformVisibility() {
  hideWaveforms();
  drawWaveforms();
}

void GraphDialog::drawWaveforms() {
  bool firstFlag = true;
  int last = 0;
  int count = 0;
  for (int i = 0; i < p_waveforms.size(); ++i) {
    if (!p_signalData->visibleWaveforms()[i]) continue;

    if (firstFlag) {
      p_waveforms[i]->setTop();
      firstFlag = false;
    } else {
      p_waveforms[i]->setMiddle();
    }

    p_waveforms[i]->drawWaveform();
    p_waveforms[i]->show();

    last = i;
    ++count;
  }

  if (count <= 1) return;

  p_waveforms[last]->setBottom();
  p_waveforms[last]->drawWaveform();
}

void GraphDialog::hideWaveforms() {
  QList<GraphWaveform *> scrollChildren =
      p_scrollContent->findChildren<GraphWaveform *>();

  if (scrollChildren.isEmpty()) return;

  for (GraphWaveform *child : scrollChildren) child->hide();
}

void GraphDialog::enableGridAction(bool enable) {
  p_signalData->setGridEnabled(enable);
  emit p_signalData->changedEnableGrid();
}

void GraphDialog::scaleGraphWaveformAction() {
  QPushButton *acceptScale = new QPushButton(tr("Accept"));
  QPushButton *denyScale = new QPushButton(tr("Deny"));

  scaleFromValue = new QLineEdit(QString::number(0));
  scaleToValue = new QLineEdit(QString::number(p_signalData->allTime() - 1));

  connect(acceptScale, &QPushButton::clicked, this,
          &GraphDialog::pushAcceptButton);
  connect(denyScale, &QPushButton::clicked, this, &GraphDialog::pushDenyButton);

  QLabel *warning = new QLabel(tr("Values must be in milliseconds"));

  QFormLayout *formLayout = new QFormLayout(this);

  formLayout->addRow(warning);
  formLayout->addRow(tr("&From:"), scaleFromValue);
  formLayout->addRow(tr("&To:"), scaleToValue);

  QHBoxLayout *buttons = new QHBoxLayout();
  buttons->addWidget(acceptScale);
  buttons->addWidget(denyScale);
  formLayout->addRow(buttons);

  scaleForm = new QWidget();
  scaleForm->setLayout(formLayout);
  scaleForm->setWindowTitle(tr("Scale Graph"));

  scaleForm->show();
}

void GraphDialog::pushDenyButton() {
  if (!scaleForm) return;
  scaleForm->close();
}

void GraphDialog::pushAcceptButton() {
  bool result;

  size_t leftTime = scaleFromValue->text().toLongLong(&result);
  if (!result) return;

  size_t rightTime = scaleToValue->text().toLongLong(&result);
  if (!result) return;

  if (leftTime < 0 || rightTime < 0) return;

  if (leftTime > p_signalData->allTime() || rightTime > p_signalData->allTime())
    return;

  if (leftTime >= rightTime) return;

  pushDenyButton();

  emit p_signalData->changedGraphTimeRange(leftTime, rightTime);
}

}  // namespace fssp
