#include "socket_reciever.h"
#include <QDebug>

ButtonSocketReceiver::ButtonSocketReceiver(const QString &serverName,
                                           QObject *parent)
    : QObject(parent),
    m_server(nullptr),
    m_client(nullptr),
    m_serverName(serverName),
    m_running(false) {
}

ButtonSocketReceiver::~ButtonSocketReceiver() {
    stop();
}

bool ButtonSocketReceiver::start() {
    if (m_running) {
        qWarning() << "ButtonSocketReceiver already running";
        return true;
    }

    m_server = new QLocalServer(this);

    // Remove any stale socket from a previous crash
    QLocalServer::removeServer(m_serverName);

    // Full access so Python test scripts can connect
    m_server->setSocketOptions(QLocalServer::WorldAccessOption);

    if (!m_server->listen(m_serverName)) {
        QString error = QString("Failed to start local server: %1").arg(m_server->errorString());
        qCritical() << error;
        emit errorOccurred(error);
        delete m_server;
        m_server = nullptr;
        return false;
    }

    connect(m_server, &QLocalServer::newConnection,
            this, &ButtonSocketReceiver::handleNewConnection);

    m_running = true;
    qInfo() << "ButtonSocketReceiver listening on:" << m_server->fullServerName();
    return true;
}

void ButtonSocketReceiver::stop() {
    if (!m_running) {
        return;
    }

    qInfo() << "Stopping ButtonSocketReceiver";

    if (m_client) {
        m_client->disconnectFromServer();
        m_client->deleteLater();
        m_client = nullptr;
    }

    if (m_server) {
        m_server->close();
        m_server->deleteLater();
        m_server = nullptr;
    }

    m_running = false;
    qInfo() << "ButtonSocketReceiver stopped";
}

void ButtonSocketReceiver::handleNewConnection() {
    QLocalSocket *newClient = m_server->nextPendingConnection();
    if (!newClient) {
        return;
    }

    // If we already have a client, close old one
    if (m_client) {
        qWarning() << "Closing existing button client for new connection";
        m_client->disconnectFromServer();
        m_client->deleteLater();
    }

    m_client = newClient;
    m_buffer.clear();

    connect(m_client, &QLocalSocket::readyRead,
            this, &ButtonSocketReceiver::handleClientData);
    connect(m_client, &QLocalSocket::disconnected,
            this, &ButtonSocketReceiver::handleClientDisconnected);

    qInfo() << "Button client connected";
}

void ButtonSocketReceiver::handleClientData() {
    if (!m_client) return;

    m_buffer.append(m_client->readAll());

    // Process complete newline-delimited messages
    while (m_buffer.contains('\n')) {
        int idx = m_buffer.indexOf('\n');
        QString message = QString::fromUtf8(m_buffer.left(idx)).trimmed();
        m_buffer.remove(0, idx + 1);

        if (!message.isEmpty()) {
            qDebug() << "Received button message:" << message;
            processButtonMessage(message);
        }
    }
}

void ButtonSocketReceiver::handleClientDisconnected() {
    qInfo() << "Button client disconnected";
    if (m_client) {
        m_client->deleteLater();
        m_client = nullptr;
    }
    m_buffer.clear();
}

void ButtonSocketReceiver::processButtonMessage(const QString &message) {
    // Expected format: "buttonname_state" (e.g., "forward_down", "enter_up")
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

    QString topic = getTopicForButton(buttonName);
    float value = getButtonValue(buttonName, state);

    if (topic.isEmpty()) {
        qWarning() << "Unknown button name:" << buttonName;
        return;
    }

    qDebug() << "Button:" << buttonName << state << "-> topic:" << topic << "value:" << value;

    emit buttonDataReceived(topic, value);
    emit buttonEventReceived(buttonName, state);
}

QString ButtonSocketReceiver::getTopicForButton(const QString &buttonName) {
    if (buttonName == "home") {
        return "MPU/State/Mode";
    }
    if (buttonName == "forward" || buttonName == "backward" ||
        buttonName == "right" || buttonName == "enter" ||
        buttonName == "up" || buttonName == "down") {
        return "Wheel/Buttons/button_id";
    }
    return QString();
}

float ButtonSocketReceiver::getButtonValue(const QString &buttonName, const QString &state) {
    if (state == "down") {
        if (buttonName == "backward") return 0;
        if (buttonName == "forward") return 1;
        if (buttonName == "right")   return 1;
        if (buttonName == "down")    return 3;
        if (buttonName == "up")      return 4;
        if (buttonName == "enter")   return 5;
        if (buttonName == "home")    return 1;
        return 1;
    } else {
        return 10;
    }
}
