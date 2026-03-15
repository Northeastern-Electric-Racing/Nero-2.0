// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "app_environment.h"
#include "controllers/endurancecontroller.h"
#include "controllers/flappybirdcontroller.h"
#include "controllers/headercontroller.h"
#include "controllers/homecontroller.h"
#include "controllers/navigationcontroller.h"
#include "controllers/offviewcontroller.h"
#include "controllers/snakecontroller.h"
#include "controllers/game2048controller.h"
#include "controllers/speedcontroller.h"
#include "import_qml_components_plugins.h"
#include "import_qml_plugins.h"
#include "models/raspberry_model.h"
#include "models/socket_reciever.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

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
  Game2048Controller game2048Controller(model);
  EnduranceController enduranceController(model);
  SpeedController speedController(model);

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
  engine.rootContext()->setContextProperty("game2048Controller",
                                           &game2048Controller);
  engine.rootContext()->setContextProperty("enduranceController",
                                           &enduranceController);
  engine.rootContext()->setContextProperty("speedController", &speedController);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.loadFromModule("content", "App");

  return app.exec();
}
