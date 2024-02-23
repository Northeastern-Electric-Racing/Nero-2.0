// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "app_environment.h"
#include "controllers/debugtablecontroller.h"
#include "controllers/homecontroller.h"
#include "import_qml_components_plugins.h"
#include "import_qml_plugins.h"
#include "src/models/mock_model.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>

int main(int argc, char *argv[]) {
  set_qt_environment();

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  MockModel *model = new MockModel;
  QThread *dataThread = new QThread;

  model->moveToThread(dataThread);

  HomeController homeController(model);
<<<<<<< HEAD
  HeaderController headerController(model);
=======
  DebugTableController tableController(model);
>>>>>>> d7883ab716d8874701ad2f9790aaef41d77c74a5

  dataThread->start();

  const QUrl url(u"qrc:Main/main.qml"_qs);
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  engine.rootContext()->setContextProperty("model", model);
  engine.rootContext()->setContextProperty("homeController", &homeController);
<<<<<<< HEAD
  engine.rootContext()->setContextProperty("headerController", &headerController);
=======
  engine.rootContext()->setContextProperty("debugTableController",
                                           &tableController);
>>>>>>> d7883ab716d8874701ad2f9790aaef41d77c74a5

  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
  engine.addImportPath(":/");

  engine.load(url);

  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  return app.exec();
}
