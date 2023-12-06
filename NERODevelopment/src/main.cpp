// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "app_environment.h"
#include "import_qml_components_plugins.h"
#include "import_qml_plugins.h"
#include "raspberry_model.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "controllers/homecontroller.h"

int main(int argc, char *argv[]) {
  set_qt_environment();

  qmlRegisterType<HomeController>("HomeController", 1, 0, "HomeController");

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  const QUrl url(u"qrc:Main/main.qml"_qs);
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
  engine.addImportPath(":/");

  engine.load(url);

  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  RaspberryModel model;
  model.connectToMQTT();

  return app.exec();
}
