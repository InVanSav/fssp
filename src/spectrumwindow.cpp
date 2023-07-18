#include "spectrumwindow.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <limits>

namespace fssp {

SpectrumWindow::SpectrumWindow(std::shared_ptr<SignalData> data,
                               QWidget *parent)
    : QGroupBox{parent} {
  m_signalData = data;

  connect(m_signalData.get(), &SignalData::dataAdded, this,
          &SpectrumWindow::onDataAdded);

  m_scrollArea = new QScrollArea();
  m_scrollArea->setFrameShape(QFrame::NoFrame);
  m_scrollArea->setWidgetResizable(true);

  m_spec = 0;
  m_mode = 0;
  m_collision = 0;
  m_smoothing = 0;

  calculate();

  addWaveforms();

  drawWaveforms();

  QVBoxLayout *mainLayout = new QVBoxLayout();
  QMenuBar *menuBar = new QMenuBar(this);

  QAction *settingsAction = menuBar->addAction(tr("Settings"));
  connect(settingsAction, &QAction::triggered, this,
          &SpectrumWindow::openSettingsAction);

  QAction *gridAction = menuBar->addAction(tr("Grid"));
  gridAction->setCheckable(true);
  gridAction->setChecked(m_signalData->isGridEnabled());

  connect(gridAction, &QAction::triggered, this,
          &SpectrumWindow::enableGridAction);

  QAction *scaleAction = menuBar->addAction(tr("Scale"));
  connect(scaleAction, &QAction::triggered, this,
          &SpectrumWindow::scaleGraphWaveformAction);

  QAction *localizationAction = menuBar->addAction(tr("Range"));
  localizationAction->setCheckable(true);
  localizationAction->setChecked(m_signalData->isGlobalScale());

  connect(localizationAction, &QAction::triggered, this,
          &SpectrumWindow::changeArrayGlobalizationAction);

  mainLayout->setMenuBar(menuBar);

  mainLayout->addWidget(m_scrollArea);

  setLayout(mainLayout);

  connect(m_signalData.get(), &SignalData::changedWaveformVisibility, this,
          &SpectrumWindow::onChangedWaveformVisibility);

  setTitle(tr("Spectrum"));
}

void SpectrumWindow::openSettingsAction() {
  m_settingsForm = new QWidget();

  QFormLayout *settingsForm = new QFormLayout();

  m_specComboBox = new QComboBox();
  m_specComboBox->addItem(tr("Amplitude spectrum"));
  m_specComboBox->addItem(tr("Power Spectral Density"));
  m_specComboBox->setCurrentIndex(m_spec);

  m_modeComboBox = new QComboBox();
  m_modeComboBox->addItem(tr("Normal"));
  m_modeComboBox->addItem(tr("Logarithmic"));
  m_modeComboBox->setCurrentIndex(m_mode);

  m_collisionComboBox = new QComboBox();
  m_collisionComboBox->addItem(tr("Reset"));
  m_collisionComboBox->addItem(tr("Do nothing"));
  m_collisionComboBox->addItem(tr("Equate with adjacent reading"));
  m_collisionComboBox->setCurrentIndex(m_collision);

  m_smoothingValue = new QSpinBox();
  m_smoothingValue->setMinimum(INT_MIN);
  m_smoothingValue->setMaximum(INT_MAX);
  m_smoothingValue->setValue(m_smoothing);

  settingsForm->addRow(tr("Spectral characteristic"), m_specComboBox);
  settingsForm->addRow(tr("Display mode"), m_modeComboBox);
  settingsForm->addRow(tr("Collision resolution"), m_collisionComboBox);
  settingsForm->addRow(tr("Smoothing window width"), m_smoothingValue);

  QHBoxLayout *buttonBox = new QHBoxLayout();

  QPushButton *accept = new QPushButton(tr("Accept"));
  connect(accept, &QPushButton::clicked, this,
          &SpectrumWindow::pushSettingsAcceptButton);

  QPushButton *cancel = new QPushButton(tr("Cancel"));
  connect(cancel, &QPushButton::clicked, this,
          &SpectrumWindow::pushSettingsCancelButton);

  buttonBox->addWidget(accept);
  buttonBox->addWidget(cancel);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addLayout(settingsForm);
  mainLayout->addLayout(buttonBox);

  m_settingsForm->setLayout(mainLayout);

  m_settingsForm->setMinimumSize(m_settingsForm->sizeHint());

  m_settingsForm->setWindowTitle(tr("Spectrum settings"));

  m_settingsForm->show();
}

void SpectrumWindow::pushSettingsAcceptButton() {
  m_spec = m_specComboBox->currentIndex();
  m_mode = m_modeComboBox->currentIndex();
  m_collision = m_collisionComboBox->currentIndex();
  m_smoothing = m_smoothingValue->value();

  calculate();

  for (int i = 0; i < m_spectrumData.size(); ++i) {
    m_waveforms[i]->setData(m_spectrumData[i]);
  }

  drawWaveforms();

  pushSettingsCancelButton();
}

void SpectrumWindow::pushSettingsCancelButton() { m_settingsForm->close(); }

void SpectrumWindow::onChangedWaveformVisibility() {
  hideWaveforms();
  drawWaveforms();
}

void SpectrumWindow::addWaveforms() {
  m_waveforms = std::vector<SpectrumWaveform *>(m_signalData->channelsNumber());

  m_scrollContent = new QWidget();

  QVBoxLayout *vBox = new QVBoxLayout();
  vBox->addSpacing(10);

  for (int i = 0; i < m_signalData->channelsNumber(); ++i) {
    SpectrumWaveform *waveform =
        new SpectrumWaveform(m_signalData, i, m_spectrumData[i]);

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

void SpectrumWindow::drawWaveforms() {
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

void SpectrumWindow::hideWaveforms() {
  QList<SpectrumWaveform *> scrollChildren =
      m_scrollContent->findChildren<SpectrumWaveform *>();

  if (scrollChildren.isEmpty()) return;

  for (SpectrumWaveform *child : scrollChildren) child->hide();
}

void SpectrumWindow::enableGridAction(bool enable) {
  m_signalData->setSpectrumGridEnabled(enable);
  emit m_signalData->changedSpectrumEnableGrid();
}

void SpectrumWindow::scaleGraphWaveformAction() {
  QPushButton *acceptScale = new QPushButton(tr("Ok"));
  connect(acceptScale, &QPushButton::clicked, this,
          &SpectrumWindow::pushScaleAcceptButton);

  QPushButton *denyScale = new QPushButton(tr("Cancel"));
  connect(denyScale, &QPushButton::clicked, this,
          &SpectrumWindow::pushScaleCancelButton);

  QPushButton *doubleScale = new QPushButton(tr("Scale x2"));
  connect(doubleScale, &QPushButton::clicked, this,
          &SpectrumWindow::pushDoubleScaleButton);

  QPushButton *resetScale = new QPushButton(tr("Reset"));
  connect(resetScale, &QPushButton::clicked, this,
          &SpectrumWindow::pushResetButton);

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
  m_scaleForm->setWindowTitle(tr("Scale"));

  m_scaleForm->show();
}

void SpectrumWindow::pushScaleCancelButton() {
  if (!m_scaleForm) return;
  m_scaleForm->close();
}

void SpectrumWindow::pushScaleAcceptButton() {
  QDateTime fromDateTime = m_scaleFromValue->dateTime();
  m_leftFreq = m_signalData->startTime().msecsTo(fromDateTime);

  QDateTime toDateTime = m_scaleToValue->dateTime();
  m_rightFreq = m_signalData->startTime().msecsTo(toDateTime);

  m_signalData->setSelected(true);

  buttonHandler();
}

void SpectrumWindow::pushDoubleScaleButton() {
  size_t timeRange = m_signalData->rightTime() - m_signalData->leftTime();

  m_leftFreq = m_signalData->leftTime() + (timeRange / 4);
  m_rightFreq = m_signalData->rightTime() - (timeRange / 4);

  m_signalData->setSelected(true);

  buttonHandler();
}

void SpectrumWindow::pushResetButton() {
  m_leftFreq = 0;
  m_rightFreq = m_signalData->rate() / 2;

  m_signalData->setSpectrumSelected(false);

  buttonHandler();
}

void SpectrumWindow::buttonHandler() {
  if (!validateInputData()) return;

  m_signalData->setLeftFreq(m_leftFreq);
  m_signalData->setRightFreq(m_rightFreq);

  m_signalData->spectrumCalculateArrayRange();

  pushScaleCancelButton();

  emit m_signalData->changedSpectrumFreqRange();
}

bool SpectrumWindow::validateInputData() {
  if (m_leftFreq > m_signalData->allTime() ||
      m_rightFreq > m_signalData->allTime()) {
    m_error->setText(tr("Values must be less than range"));
    return false;
  }

  else if (m_leftFreq >= m_rightFreq) {
    m_error->setText(tr("'From' must be less than 'to'"));
    return false;
  }

  return true;
}

void SpectrumWindow::changeArrayGlobalizationAction() {
  m_signalData->setSpectrumGlobalScale(!m_signalData->spectrumIsGlobalScale());
  emit m_signalData->changedSpectrumGlobalScale();
}

void SpectrumWindow::onDataAdded() {
  addWaveforms();
  hideWaveforms();
  drawWaveforms();
}

void SpectrumWindow::fft(std::vector<base> &a, bool invert) {
  int n = (int)a.size();

  for (int i = 1, j = 0; i < n; ++i) {
    int bit = n >> 1;
    for (; j >= bit; bit >>= 1) j -= bit;
    j += bit;
    if (i < j) std::swap(a[i], a[j]);
  }

  for (int len = 2; len <= n; len <<= 1) {
    double ang = 2 * M_PI / len * (invert ? -1 : 1);
    base wlen(cos(ang), sin(ang));
    for (int i = 0; i < n; i += len) {
      base w(1);
      for (int j = 0; j < len / 2; ++j) {
        base u = a[i + j], v = a[i + j + len / 2] * w;
        a[i + j] = u + v;
        a[i + j + len / 2] = u - v;
        w *= wlen;
      }
    }
  }
  if (invert)
    for (int i = 0; i < n; ++i) a[i] /= n;
}

void SpectrumWindow::calculate() {
  quint64 tmp = 2;
  while (tmp < m_signalData->samplesNumber()) {
    tmp *= 2;
  }

  std::vector<std::vector<base>> data = std::vector<std::vector<base>>(
      m_signalData->channelsNumber(), std::vector<base>(tmp));

  for (size_t i = 0; i < m_signalData->channelsNumber(); ++i) {
    for (size_t j = 0; j < m_signalData->samplesNumber(); ++j) {
      data[i][j] = base(m_signalData->data()[i][j], 0);
    }
    for (size_t j = m_signalData->samplesNumber(); j < tmp; ++j) {
      data[i][j] = base(0);
    }
  }

  for (size_t i = 0; i < data.size(); ++i) {
    fft(data[i], false);
  }

  m_spectrumData = std::vector<std::vector<double>>(
      m_signalData->channelsNumber(),
      std::vector<double>(tmp / 2));

  for (size_t i = 0; i < data.size(); ++i) {
    for (size_t j = 0; j < tmp / 2; ++j) {
      m_spectrumData[i][j] = m_signalData->timeForOne() * abs(data[i][j]);
    }
  }

  if (m_spec == 1) {
    for (size_t i = 0; i < m_spectrumData.size(); ++i) {
      for (size_t j = 0; j < m_spectrumData[i].size(); ++j) {
        m_spectrumData[i][j] = pow(m_spectrumData[i][j], 2);
      }
    }
  }

  double L = m_smoothing;

  // Применение логарифмического мода.
  if (m_mode == 1) {
    if (m_spec == 0) {
      for (size_t i = 0; i < m_spectrumData.size(); ++i) {
        for (size_t j = 0; j < m_spectrumData[i].size(); ++j) {
          m_spectrumData[i][j] = 20 * log10(m_spectrumData[i][j]);
        }
      }
    } else {
      for (size_t i = 0; i < m_spectrumData.size(); ++i) {
        for (size_t j = 0; j < m_spectrumData[i].size(); ++j) {
          m_spectrumData[i][j] = 10 * log10(m_spectrumData[i][j]);
        }
      }
    }
  }

  // Разрешение коллизий.
  if (m_collision == 0) {
    for (size_t i = 0; i < m_spectrumData.size(); ++i) {
      m_spectrumData[i][0] = 0;
    }
  } else if (m_collision == 2) {
    for (size_t i = 0; i < m_spectrumData.size(); ++i) {
      m_spectrumData[i][0] = m_spectrumData[i][1];
    }
  }

  // Сглаживание.
  for (size_t i = 0; i < m_spectrumData.size(); ++i) {
    QVector<double> tmpV(m_spectrumData[i].size());
    for (size_t j = 0; j < m_spectrumData[i].size(); ++j) {
      double tmp = m_spectrumData[i][j];

      for (size_t k = 1; k < L + 1; ++k) {
        if (j < k) {
          tmp += m_spectrumData[i][k - j];
        } else {
          tmp += m_spectrumData[i][j - k];
        }
      }

      for (size_t k = 1; k < L + 1; ++k) {
        if (j + k >= m_spectrumData[i].size()) {
          tmp += m_spectrumData[i][m_spectrumData[i].size() - 1 -
                                   (j + k - m_spectrumData[i].size())];
        } else {
          tmp += m_spectrumData[i][j + k];
        }
      }

      tmp /= 2. * L + 1.;

      tmpV[j] = tmp;
    }
    for (size_t j = 0; j < m_spectrumData[i].size(); ++j) {
      m_spectrumData[i][j] = tmpV[j];
    }
  }
}

}  // namespace fssp
