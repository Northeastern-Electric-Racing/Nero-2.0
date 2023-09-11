#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QQmlContext>
#include <QTranslator>
#include <homecontroller.h>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  // auto generated translator code
  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "NERO_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      app.installTranslator(&translator);
      break;
    }
  }

  // auto generated gui setup code, loading qml from main.qml
  QQmlApplicationEngine engine;
  const QUrl url(u"qrc:/NERO/main.qml"_qs);
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  // Connecting the controller to the qml context to allow the qml files to use
  // this for signals and slots
  HomeController homeController;

  QQmlContext *rootContext = engine.rootContext();
  rootContext->setContextProperty("homeController", &homeController);

  return app.exec();
}
