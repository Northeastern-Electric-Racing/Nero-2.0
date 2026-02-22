// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "app_environment.h"
#include "controllers/efficiencycontroller.h"
#include "controllers/flappybirdcontroller.h"
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
#include "models/socket_reciever.h"

int main(int argc, char *argv[]) {
    set_qt_environment();

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    RaspberryModel *raspModel = new RaspberryModel();
    raspModel->connectToMQTT();
    Model *model = raspModel;

    // Button input via local socket (no MQTT for buttons)
    ButtonSocketReceiver *buttonReceiver = new ButtonSocketReceiver();

    // Wire socket receiver directly to model
    QObject::connect(buttonReceiver, &ButtonSocketReceiver::buttonDataReceived,
                     raspModel, &RaspberryModel::receiveButtonInput);

    QObject::connect(buttonReceiver, &ButtonSocketReceiver::buttonEventReceived,
                     [](const QString &button, const QString &state) {
                         qInfo() << "Button event:" << button << state;
                     });

    QObject::connect(buttonReceiver, &ButtonSocketReceiver::errorOccurred,
                     [](const QString &error) {
                         qCritical() << "Button receiver error:" << error;
                     });

    if (!buttonReceiver->start()) {
        qCritical() << "Failed to start button socket receiver";
    } else {
        qInfo() << "Button socket receiver started successfully";
    }

    HomeController homeController(model);
    HeaderController headerController(model);
    OffViewController offViewController(model);
    NavigationController navigationController(model);
    FlappyBirdController flappyBirdController(model);
    SnakeController snakeController(model);
    EfficiencyController efficencyController(model);
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
    engine.rootContext()->setContextProperty("efficiencyController",
                                             &efficencyController);
    engine.rootContext()->setContextProperty("speedController", &speedController);

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.loadFromModule("content", "App");

    return app.exec();
}
