#include "home.h"
#include "mainwindow.h"
#include "models/mock_model.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "NERO_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      a.installTranslator(&translator);
      break;
    }
  }

  MainWindow main;
  MockModel m;
  QFrame *mainFrame = new QFrame(&main);
  Home *h = new Home(mainFrame, &m);
  h->setFrameShape(QFrame::Box);
  h->setLineWidth(2);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(h);

  QWidget *centralWidget = new QWidget(&main);
  centralWidget->setLayout(layout);

  main.setCentralWidget(centralWidget);
  main.show();

  return a.exec();
}
