#include "socket_reciever.h"
#include <QDebug>
#include <cstring>

#ifndef _WIN32
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#endif

ButtonSocketReceiver::ButtonSocketReceiver(MqttClient *mqttClient,
                                           const QString &socketPath,
                                           QObject *parent)
    : QObject(parent),
    m_mqttClient(mqttClient),
    m_socketPath(socketPath),
    m_serverFd(-1),
    m_clientFd(-1),
    m_serverNotifier(nullptr),
    m_clientNotifier(nullptr),
    m_running(false) {
}

ButtonSocketReceiver::~ButtonSocketReceiver() {
    stop();
}

bool ButtonSocketReceiver::start() {
#ifdef _WIN32
    qWarning() << "ButtonSocketReceiver not supported on Windows";
    return false;
#else
    if (m_running) {
        qWarning() << "ButtonSocketReceiver already running";
        return true;
    }

    qInfo() << "Starting ButtonSocketReceiver on path:" << m_socketPath;

    unlink(m_socketPath.toStdString().c_str());

    m_serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_serverFd == -1) {
        QString error = QString("Failed to create socket: %1").arg(strerror(errno));
        qCritical() << error;
        emit errorOccurred(error);
        return false;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, m_socketPath.toStdString().c_str(), sizeof(addr.sun_path) - 1);

    if (bind(m_serverFd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        QString error = QString("Failed to bind socket: %1").arg(strerror(errno));
        qCritical() << error;
        close(m_serverFd);
        m_serverFd = -1;
        emit errorOccurred(error);
        return false;
    }

    if (listen(m_serverFd, 1) == -1) {
        QString error = QString("Failed to listen on socket: %1").arg(strerror(errno));
        qCritical() << error;
        close(m_serverFd);
        m_serverFd = -1;
        emit errorOccurred(error);
        return false;
    }

    m_serverNotifier = new QSocketNotifier(m_serverFd, QSocketNotifier::Read, this);
    connect(m_serverNotifier, &QSocketNotifier::activated, this, &ButtonSocketReceiver::handleNewConnection);
    m_serverNotifier->setEnabled(true);

    m_running = true;
    qInfo() << "ButtonSocketReceiver listening on:" << m_socketPath;

    return true;
#endif
}

void ButtonSocketReceiver::stop() {
#ifndef _WIN32
    if (!m_running) {
        return;
    }

    qInfo() << "Stopping ButtonSocketReceiver";

    if (m_clientNotifier) {
        m_clientNotifier->setEnabled(false);
        m_clientNotifier->deleteLater();
        m_clientNotifier = nullptr;
    }

    if (m_clientFd != -1) {
        close(m_clientFd);
        m_clientFd = -1;
    }

    if (m_serverNotifier) {
        m_serverNotifier->setEnabled(false);
        m_serverNotifier->deleteLater();
        m_serverNotifier = nullptr;
    }

    if (m_serverFd != -1) {
        close(m_serverFd);
        m_serverFd = -1;
    }

    unlink(m_socketPath.toStdString().c_str());

    m_running = false;
    qInfo() << "ButtonSocketReceiver stopped";
#endif
}

void ButtonSocketReceiver::handleNewConnection() {
#ifndef _WIN32
    qInfo() << "New client connection attempt";

    if (m_clientFd != -1) {
        qWarning() << "Closing existing client connection for new client";
        if (m_clientNotifier) {
            m_clientNotifier->setEnabled(false);
            m_clientNotifier->deleteLater();
            m_clientNotifier = nullptr;
        }
        close(m_clientFd);
        m_clientFd = -1;
    }

    m_clientFd = accept(m_serverFd, nullptr, nullptr);
    if (m_clientFd == -1) {
        QString error = QString("Failed to accept connection: %1").arg(strerror(errno));
        qWarning() << error;
        emit errorOccurred(error);
        return;
    }

    qInfo() << "Client connected, fd:" << m_clientFd;

    m_clientNotifier = new QSocketNotifier(m_clientFd, QSocketNotifier::Read, this);
    connect(m_clientNotifier, &QSocketNotifier::activated, this, &ButtonSocketReceiver::handleClientData);
    m_clientNotifier->setEnabled(true);
#endif
}

void ButtonSocketReceiver::handleClientData() {
#ifndef _WIN32
    char buffer[BUFFER_SIZE];
    ssize_t nBytes = read(m_clientFd, buffer, BUFFER_SIZE - 1);

    if (nBytes > 0) {
        buffer[nBytes] = '\0';
        QString message = QString::fromUtf8(buffer).trimmed();

        qDebug() << "Received button message:" << message;
        processButtonMessage(message);

    } else if (nBytes == 0) {
        qInfo() << "Client disconnected gracefully";
        handleClientDisconnected();
    } else {
        QString error = QString("Error reading from client: %1").arg(strerror(errno));
        qWarning() << error;
        emit errorOccurred(error);
        handleClientDisconnected();
    }
#endif
}

void ButtonSocketReceiver::handleClientDisconnected() {
#ifndef _WIN32
    if (m_clientNotifier) {
        m_clientNotifier->setEnabled(false);
        m_clientNotifier->deleteLater();
        m_clientNotifier = nullptr;
    }

    if (m_clientFd != -1) {
        close(m_clientFd);
        m_clientFd = -1;
    }

    qInfo() << "Client cleanup complete, ready for new connection";
#endif
}

void ButtonSocketReceiver::processButtonMessage(const QString &message) {
    QStringList parts = message.split('_');

    if (parts.size() != 2) {
        qWarning() << "Invalid button message format:" << message;
        return;
    }

    QString buttonName = parts[0].toLower();
    QString state = parts[1].toLower();

    if (state != "down" && state != "up") {
        qWarning() << "Invalid button state:" << state;
        return;
    }

    QString topic = getMqttTopicForButton(buttonName, state);
    float value = getButtonValue(buttonName, state);

    if (topic.isEmpty()) {
        qWarning() << "Unknown button name:" << buttonName;
        return;
    }

    qDebug() << "Publishing button event:" << topic << "=" << value;
    m_mqttClient->sendMessage(topic, value);

    emit buttonEventReceived(buttonName, state);
}

QString ButtonSocketReceiver::getMqttTopicForButton(const QString &buttonName, const QString &state) {
    QMap<QString, QString> buttonTopicMap = {
        {"forward", "Button/Forward"},
        {"backward", "Button/Backward"},
        {"right", "Button/Right"},
        {"enter", "Button/Enter"},
        {"up", "Button/Up"},
        {"down", "Button/Down"},
        {"home", "Button/Home"}
    };

    return buttonTopicMap.value(buttonName, "");
}

float ButtonSocketReceiver::getButtonValue(const QString &buttonName, const QString &state) {
    if (state == "down") {
        if (buttonName == "backward") return 0;
        if (buttonName == "right") return 1;
        if (buttonName == "enter") return 5;
        if (buttonName == "up") return 4;
        if (buttonName == "down") return 3;
        if (buttonName == "home") return 1;
        if (buttonName == "forward") return 1;
        return 1;
    } else {
        return 10;
    }
}
