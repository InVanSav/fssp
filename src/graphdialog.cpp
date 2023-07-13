#include "graphdialog.h"

namespace fssp {

GraphDialog::GraphDialog(std::shared_ptr<SignalData> data, QWidget *parent)
    : QGroupBox{parent} {
  m_signalData = data;

  connect(m_signalData.get(), &SignalData::dataAdded, this,
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
  gridAction->setChecked(m_signalData->isGridEnabled());

  connect(gridAction, &QAction::triggered, this,
          &GraphDialog::enableGridAction);

  QAction *scaleAction = menuBar->addAction(tr("Scale graph"));
  connect(scaleAction, &QAction::triggered, this,
          &GraphDialog::scaleGraphWaveformAction);

  QAction *localizationAction = menuBar->addAction(tr("Local/Global"));
  localizationAction->setCheckable(true);
  localizationAction->setChecked(m_signalData->isGlobalScale());

  connect(localizationAction, &QAction::triggered, this,
          &GraphDialog::changeArrayGlobalizationAction);

  mainLayout->setMenuBar(menuBar);

  mainLayout->addWidget(m_scrollArea);

  setLayout(mainLayout);

  connect(m_signalData.get(), &SignalData::changedWaveformVisibility, this,
          &GraphDialog::onChangedWaveformVisibility);

  setTitle(tr("Graphs"));
}

void GraphDialog::onChangedWaveformVisibility() {
  hideWaveforms();
  drawWaveforms();
}

void GraphDialog::addWaveforms() {
  m_waveforms = std::vector<GraphWaveform *>(m_signalData->channelsNumber());

  m_scrollContent = new QWidget();

  QVBoxLayout *vBox = new QVBoxLayout();
  vBox->addSpacing(10);

  for (int i = 0; i < m_signalData->channelsNumber(); ++i) {
    GraphWaveform *waveform = new GraphWaveform(m_signalData, i);

    m_waveforms[i] = waveform;
    vBox->addWidget(waveform);
  }

  m_waveforms[0]->setTop();

  if (m_signalData->channelsNumber() > 1) {
    m_waveforms[m_signalData->channelsNumber() - 1]->setBottom();
  }

  vBox->addSpacing(10);

  vBox->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  m_scrollContent->setLayout(vBox);

  m_scrollArea->setWidget(m_scrollContent);
}

void GraphDialog::drawWaveforms() {
  bool firstFlag = true;
  int last = 0;
  int count = 0;
  for (int i = 0; i < m_waveforms.size(); ++i) {
    if (!m_signalData->visibleWaveforms()[i]) continue;

    if (firstFlag) {
      m_waveforms[i]->setTop();
      firstFlag = false;
    } else {
      m_waveforms[i]->setMiddle();
    }

    m_waveforms[i]->drawWaveform();
    m_waveforms[i]->show();

    last = i;
    ++count;
  }

  if (count <= 1) return;

  m_waveforms[last]->setBottom();
  m_waveforms[last]->drawWaveform();
}

void GraphDialog::hideWaveforms() {
  QList<GraphWaveform *> scrollChildren =
      m_scrollContent->findChildren<GraphWaveform *>();

  if (scrollChildren.isEmpty()) return;

  for (GraphWaveform *child : scrollChildren) child->hide();
}

void GraphDialog::enableGridAction(bool enable) {
  m_signalData->setGridEnabled(enable);
  emit m_signalData->changedEnableGrid();
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

  m_scaleFromValue = new QDateTimeEdit(m_signalData->startTime());
  m_scaleFromValue->setCalendarPopup(true);
  m_scaleToValue = new QDateTimeEdit(m_signalData->endTime());
  m_scaleToValue->setCalendarPopup(true);

  m_error = new QLabel();

  m_formLayout = new QFormLayout(this);
  m_formLayout->setHorizontalSpacing(5);

  m_formLayout->addRow(tr("&From:"), m_scaleFromValue);
  m_formLayout->addRow(tr("&To:"), m_scaleToValue);
  m_formLayout->addRow(m_error);

  QHBoxLayout *scaleButtons = new QHBoxLayout();
  scaleButtons->addWidget(doubleScale);
  scaleButtons->addWidget(resetScale);

  m_formLayout->addRow(scaleButtons);

  QHBoxLayout *activationButtons = new QHBoxLayout();
  activationButtons->addWidget(acceptScale);
  activationButtons->addWidget(denyScale);

  m_formLayout->addRow(activationButtons);

  m_scaleForm = new QWidget();
  m_scaleForm->setLayout(m_formLayout);
  m_scaleForm->setWindowTitle(tr("Scale Graph"));

  m_scaleForm->show();
}

void GraphDialog::pushDenyButton() {
  if (!m_scaleForm) return;
  m_scaleForm->close();
}

void GraphDialog::pushAcceptButton() {
  QDateTime fromDateTime = m_scaleFromValue->dateTime();
  m_leftTime = m_signalData->startTime().msecsTo(fromDateTime);

  QDateTime toDateTime = m_scaleToValue->dateTime();
  m_rightTime = m_signalData->startTime().msecsTo(toDateTime);

  m_signalData->setSelected(true);

  buttonHandler();
}

void GraphDialog::pushDoubleScaleButton() {
  size_t timeRange = m_signalData->rightTime() - m_signalData->leftTime();

  m_leftTime = m_signalData->leftTime() + (timeRange / 4);
  m_rightTime = m_signalData->rightTime() - (timeRange / 4);

  m_signalData->setSelected(true);

  buttonHandler();
}

void GraphDialog::pushResetButton() {
  m_leftTime = 0;
  m_rightTime = m_signalData->allTime() - 1;

  m_signalData->setSelected(false);

  buttonHandler();
}

void GraphDialog::buttonHandler() {
  if (!validateInputData()) return;

  m_signalData->setLeftTime(m_leftTime);
  m_signalData->setRightTime(m_rightTime);

  m_signalData->calculateArrayRange();

  pushDenyButton();

  emit m_signalData->changedGraphTimeRange();
}

bool GraphDialog::validateInputData() {
  if (m_leftTime > m_signalData->allTime() ||
      m_rightTime > m_signalData->allTime()) {
    m_error->setText(tr("Values must be less than range"));
    return false;
  }

  else if (m_leftTime >= m_rightTime) {
    m_error->setText(tr("'From' must be less than 'to'"));
    return false;
  }

  return true;
}

void GraphDialog::changeArrayGlobalizationAction() {
  m_signalData->setGlobalScale(!m_signalData->isGlobalScale());
  emit m_signalData->changedGlobalScale();
}

void GraphDialog::onDataAdded() {
  addWaveforms();
  hideWaveforms();
  drawWaveforms();
}

}  // namespace fssp
