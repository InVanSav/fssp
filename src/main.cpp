#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "mainwindow.h"

namespace fssp {

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "fssp_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      a.installTranslator(&translator);
      break;
    }
  }

  MainWindow w;
  w.show();

  return a.exec();
}

}  // namespace fssp
