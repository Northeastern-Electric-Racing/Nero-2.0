// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "app_environment.h"
#include "controllers/doomcontroller.h"
#include "controllers/endurancecontroller.h"
#include "controllers/flappybirdcontroller.h"
#include "controllers/game2048controller.h"
#include "controllers/headercontroller.h"
#include "controllers/homecontroller.h"
#include "controllers/navigationcontroller.h"
#include "controllers/offviewcontroller.h"
#include "controllers/snakecontroller.h"
#include "controllers/speedcontroller.h"
#include "import_qml_components_plugins.h"
#include "import_qml_plugins.h"
#include "models/raspberry_model.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QTimer>

int main(int argc, char *argv[]) {
  set_qt_environment();

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  Model *model = new RaspberryModel();
  model->connectToMQTT();

  HomeController homeController(model);
  HeaderController headerController(model);
  OffViewController offViewController(model);
  NavigationController navigationController(model);
  FlappyBirdController flappyBirdController(model);
  SnakeController snakeController(model);
  DoomController doomController(model);
  Game2048Controller game2048Controller(model);
  EnduranceController enduranceController(model);
  SpeedController speedController(model);

  // Register DOOM's custom image provider with the QML engine.
  // Unlike other controllers which just expose properties/methods to QML,
  // DOOM renders frames to a raw pixel buffer that needs to be served to
  // QML as an image. QQuickImageProvider lets QML load frames via:
  //   Image { source: "image://doom/frame?" + frameCounter }
  // The "doom" string here is the provider ID that maps to that URI scheme.
  // This must be registered BEFORE engine.loadFromModule() so the QML
  // engine can resolve "image://doom/..." when DoomView.qml is loaded.
  engine.addImageProvider("doom", doomController.createImageProvider());

  engine.rootContext()->setContextProperty("homeController", &homeController);
  engine.rootContext()->setContextProperty("headerController",
                                           &headerController);
  engine.rootContext()->setContextProperty("offViewController",
                                           &offViewController);
  engine.rootContext()->setContextProperty("navigationController",
                                           &navigationController);
  engine.rootContext()->setContextProperty("flappyBirdController",
                                           &flappyBirdController);
  engine.rootContext()->setContextProperty("snakeController", &snakeController);
  engine.rootContext()->setContextProperty("doomController", &doomController);
  engine.rootContext()->setContextProperty("game2048Controller",
                                           &game2048Controller);
  engine.rootContext()->setContextProperty("enduranceController",
                                           &enduranceController);
  engine.rootContext()->setContextProperty("speedController", &speedController);

  QObject::connect(&homeController, &HomeController::toggleFaultAlertsRequested,
                   &headerController, &HeaderController::toggleFaultAlerts);
  QObject::connect(&speedController,
                   &SpeedController::toggleFaultAlertsRequested,
                   &headerController, &HeaderController::toggleFaultAlerts);
  QObject::connect(&enduranceController,
                   &EnduranceController::toggleFaultAlertsRequested,
                   &headerController, &HeaderController::toggleFaultAlerts);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.loadFromModule("content", "App");

  // Screenshot the named page headless, then quit.
  if (qEnvironmentVariableIsSet("NERO_SCREENSHOT")) {
    // Read env in the callback; captured locals would dangle.
    QTimer::singleShot(2000, [&]() { // wait for data + UI
      navigationController.jumpToPage(qEnvironmentVariable("NERO_SCREENSHOT"));
      QTimer::singleShot(500, [&]() { // wait for the page to render
        const QString out =
            qEnvironmentVariable("NERO_SCREENSHOT_OUT", "shot.png");
        if (auto *w = qobject_cast<QQuickWindow *>(
                engine.rootObjects().constFirst())) {
          const bool ok = w->grabWindow().save(out);
          qInfo() << "NERO_SCREENSHOT:" << (ok ? "saved" : "FAILED") << out;
        }
        QCoreApplication::quit();
      });
    });
  }

  return app.exec();
}
