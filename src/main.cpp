#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QApplication::setApplicationName("FSSP");

  QTranslator translator;
  //  const QStringList uiLanguages = QLocale::system().uiLanguages();
  //  for (const QString &locale : uiLanguages) {
  //    const QString baseName = "fssp_" + QLocale(locale).name();
  //    if (translator.load(":/i18n/" + baseName)) {
  //      a.installTranslator(&translator);
  //      break;
  //    }
  //  }

  const QString baseName = "fssp_ru_RU";
  if (translator.load(baseName)) {
    a.installTranslator(&translator);
  }

  fssp::MainWindow w;
  w.show();
  w.showMaximized();

  return a.exec();
}
