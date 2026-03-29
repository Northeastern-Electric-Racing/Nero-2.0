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
    QLocalServer::removeServer(m_serverName);
    m_server->setSocketOptions(QLocalServer::WorldAccessOption);

    if (!m_server->listen(m_serverName)) {
        QString error = QString("Failed to start button socket: %1")
                            .arg(m_server->errorString());
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
    if (!m_running) return;

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
}

void ButtonSocketReceiver::handleNewConnection() {
    QLocalSocket *newClient = m_server->nextPendingConnection();
    if (!newClient) return;

    if (m_client) {
        qWarning() << "Replacing existing button client";
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

    while (m_buffer.contains('\n')) {
        int idx = m_buffer.indexOf('\n');
        QString message = QString::fromUtf8(m_buffer.left(idx)).trimmed();
        m_buffer.remove(0, idx + 1);

        if (!message.isEmpty()) {
            processMessage(message);
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

void ButtonSocketReceiver::processMessage(const QString &message) {
    // Format: "button_{N}_{state}"
    // Examples: "button_0_down", "button_3_up"
    QStringList parts = message.split('_');

    if (parts.size() != 3 || parts[0] != "button") {
        qWarning() << "Invalid button message:" << message;
        return;
    }

    bool ok;
    int buttonNumber = parts[1].toInt(&ok);
    if (!ok) {
        qWarning() << "Invalid button number:" << parts[1];
        return;
    }

    QString state = parts[2].toLower();
    if (state != "down" && state != "up") {
        qWarning() << "Invalid button state:" << state;
        return;
    }

    // On down: value = button number. On up: value = 10 (released).
    float value = (state == "down") ? static_cast<float>(buttonNumber) : 10.0f;
    QString topic = "Wheel/Buttons/button_id";

    qDebug() << "Button:" << buttonNumber << state << "-> value:" << value;

    emit buttonDataReceived(topic, value);
    emit buttonEventReceived(QString::number(buttonNumber), state);
}
