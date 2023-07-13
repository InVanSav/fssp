#include "graphdialog.h"

namespace fssp {

GraphDialog::GraphDialog(std::shared_ptr<SignalData> data, QWidget *parent)
    : QGroupBox{parent} {
  p_signalData = data;

  connect(p_signalData.get(), &SignalData::dataAdded, this,
          &GraphDialog::onDataAdded);

  m_scrollArea = new QScrollArea();
  m_scrollArea->setFrameShape(QFrame::NoFrame);
  m_scrollArea->setWidgetResizable(true);

  addWaveforms();

  drawWaveforms();

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

  QAction *localizationAction = menuBar->addAction(tr("Local/Global"));
  localizationAction->setCheckable(true);
  localizationAction->setChecked(p_signalData->isGlobalScale());

  connect(localizationAction, &QAction::triggered, this,
          &GraphDialog::changeArrayGlobalizationAction);

  mainLayout->setMenuBar(menuBar);

  mainLayout->addWidget(m_scrollArea);

  setLayout(mainLayout);

  connect(p_signalData.get(), &SignalData::changedWaveformVisibility, this,
          &GraphDialog::onChangedWaveformVisibility);

  setTitle(tr("Graphs"));
}

void GraphDialog::onChangedWaveformVisibility() {
  hideWaveforms();
  drawWaveforms();
}

void GraphDialog::addWaveforms() {
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

  m_scrollArea->setWidget(p_scrollContent);
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
  connect(acceptScale, &QPushButton::clicked, this,
          &GraphDialog::pushAcceptButton);

  QPushButton *denyScale = new QPushButton(tr("Deny"));
  connect(denyScale, &QPushButton::clicked, this, &GraphDialog::pushDenyButton);

  QPushButton *doubleScale = new QPushButton(tr("Scale x2"));
  connect(doubleScale, &QPushButton::clicked, this,
          &GraphDialog::pushDoubleScaleButton);

  QPushButton *resetScale = new QPushButton(tr("Reset"));
  connect(resetScale, &QPushButton::clicked, this,
          &GraphDialog::pushResetButton);

  scaleFromValue = new QDateTimeEdit(p_signalData->startTime());
  scaleFromValue->setCalendarPopup(true);
  scaleToValue = new QDateTimeEdit(p_signalData->endTime());
  scaleToValue->setCalendarPopup(true);

  error = new QLabel();

  formLayout = new QFormLayout(this);
  formLayout->setHorizontalSpacing(5);

  formLayout->addRow(tr("&From:"), scaleFromValue);
  formLayout->addRow(tr("&To:"), scaleToValue);
  formLayout->addRow(error);

  QHBoxLayout *scaleButtons = new QHBoxLayout();
  scaleButtons->addWidget(doubleScale);
  scaleButtons->addWidget(resetScale);

  formLayout->addRow(scaleButtons);

  QHBoxLayout *activationButtons = new QHBoxLayout();
  activationButtons->addWidget(acceptScale);
  activationButtons->addWidget(denyScale);

  formLayout->addRow(activationButtons);

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
  QDateTime fromDateTime = scaleFromValue->dateTime();
  leftTime = p_signalData->startTime().msecsTo(fromDateTime);

  QDateTime toDateTime = scaleToValue->dateTime();
  rightTime = p_signalData->startTime().msecsTo(toDateTime);

  p_signalData->setSelected(true);

  buttonHandler();
}

void GraphDialog::pushDoubleScaleButton() {
  size_t timeRange = p_signalData->rightTime() - p_signalData->leftTime();

  leftTime = p_signalData->leftTime() + (timeRange / 4);
  rightTime = p_signalData->rightTime() - (timeRange / 4);

  p_signalData->setSelected(true);

  buttonHandler();
}

void GraphDialog::pushResetButton() {
  leftTime = 0;
  rightTime = p_signalData->allTime() - 1;

  p_signalData->setSelected(false);

  buttonHandler();
}

void GraphDialog::buttonHandler() {
  if (!validateInputData()) return;

  p_signalData->setLeftTime(leftTime);
  p_signalData->setRightTime(rightTime);

  p_signalData->calculateArrayRange();

  pushDenyButton();

  emit p_signalData->changedGraphTimeRange();
}

bool GraphDialog::validateInputData() {
  if (leftTime > p_signalData->allTime() ||
      rightTime > p_signalData->allTime()) {
    error->setText(tr("Values must be less than range"));
    return false;
  }

  else if (leftTime >= rightTime) {
    error->setText(tr("'From' must be less than 'to'"));
    return false;
  }

  return true;
}

void GraphDialog::changeArrayGlobalizationAction() {
  p_signalData->setGlobalScale(!p_signalData->isGlobalScale());
  emit p_signalData->changedGlobalScale();
}

void GraphDialog::onDataAdded() {
  addWaveforms();
  hideWaveforms();
  drawWaveforms();
}

}  // namespace fssp
