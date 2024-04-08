// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "app_environment.h"
#include "controllers/configurationcontroller.h"
#include "controllers/debugtablecontroller.h"
#include "controllers/efficiencycontroller.h"
#include "controllers/flappybirdcontroller.h"
#include "controllers/headercontroller.h"
#include "controllers/homecontroller.h"
#include "controllers/keyboardcontroller.h"
#include "controllers/navigationcontroller.h"
#include "controllers/offviewcontroller.h"
#include "import_qml_components_plugins.h"
#include "import_qml_plugins.h"
#include "src/models/mock_model.h"
#include "src/models/raspberry_model.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>

int main(int argc, char *argv[]) {
  set_qt_environment();

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  QString osName = QSysInfo::machineHostName();

  Model *model;

  if (osName == "raspberrypi-sta") {
    model = new RaspberryModel;
    model->connectToMQTT();
  } else {
    model = new MockModel;
    QThread *dataThread = new QThread;

    model->moveToThread(dataThread);
    dataThread->start();
  }

  HomeController homeController(model);
  HeaderController headerController(model);
  OffViewController offViewController(model);
  DebugTableController tableController(model);
  NavigationController navigationController(model);
  FlappyBirdController flappyBirdController(model);
  ConfigurationController configurationController(model);
  KeyboardController keyboardController(model);
  EfficiencyController efficencyController(model);

  const QUrl url(u"qrc:Main/main.qml"_qs);
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  engine.rootContext()->setContextProperty("homeController", &homeController);
  engine.rootContext()->setContextProperty("headerController",
                                           &headerController);
  engine.rootContext()->setContextProperty("offViewController",
                                           &offViewController);
  engine.rootContext()->setContextProperty("debugTableController",
                                           &tableController);
  engine.rootContext()->setContextProperty("navigationController",
                                           &navigationController);
  engine.rootContext()->setContextProperty("flappyBirdController",
                                           &flappyBirdController);
  engine.rootContext()->setContextProperty("configurationController",
                                           &configurationController);
  engine.rootContext()->setContextProperty("keyboardViewController",
                                           &keyboardController);
  engine.rootContext()->setContextProperty("efficiencyController",
                                           &efficencyController);

  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
  engine.addImportPath(":/");

  engine.load(url);

  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  return app.exec();
}
