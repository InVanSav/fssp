#include <QApplication>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QApplication::setApplicationName("FSSP");

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "fssp_" + QLocale(locale).name();
    if (translator.load(baseName)) {
      a.installTranslator(&translator);
      break;
    }
  }

  MainWindow w;
  w.showMaximized();
  return a.exec();
}
