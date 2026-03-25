#include "socket_receiver.h"
#include <QDebug>
#include <QFile>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

ButtonSocketReceiver::ButtonSocketReceiver(const QString &socketPath,
                                           QObject *parent)
    : QObject(parent),
      m_socketPath(socketPath),
      m_fd(-1),
      m_notifier(nullptr),
      m_running(false)
{
}

ButtonSocketReceiver::~ButtonSocketReceiver() {
    stop();
}

bool ButtonSocketReceiver::start() {
    if (m_running) {
        qWarning() << "ButtonSocketReceiver already running";
        return true;
    }

    QFile::remove(m_socketPath);

    m_fd = ::socket(AF_UNIX, SOCK_DGRAM, 0);
    if (m_fd < 0) {
        QString error = QString("Failed to create socket: %1").arg(std::strerror(errno));
        qCritical() << error;
        emit errorOccurred(error);
        return false;
    }

    struct sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, m_socketPath.toUtf8().constData(),
                 sizeof(addr.sun_path) - 1);

    if (::bind(m_fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
        QString error = QString("Failed to bind socket: %1").arg(std::strerror(errno));
        qCritical() << error;
        emit errorOccurred(error);
        ::close(m_fd);
        m_fd = -1;
        return false;
    }

    m_notifier = new QSocketNotifier(m_fd, QSocketNotifier::Read, this);
    connect(m_notifier, &QSocketNotifier::activated,
            this, &ButtonSocketReceiver::handleClientData);

    m_running = true;
    qInfo() << "ButtonSocketReceiver listening on:" << m_socketPath;
    return true;
}

void ButtonSocketReceiver::stop() {
    if (!m_running) return;
    qInfo() << "Stopping ButtonSocketReceiver";

    if (m_notifier) {
        m_notifier->setEnabled(false);
        delete m_notifier;
        m_notifier = nullptr;
    }
    if (m_fd >= 0) {
        ::close(m_fd);
        m_fd = -1;
    }

    QFile::remove(m_socketPath);
    m_running = false;
}

void ButtonSocketReceiver::handleClientData() {
    char buf[1];
    for (;;) {
        ssize_t n = ::recv(m_fd, buf, sizeof(buf), MSG_DONTWAIT);
        if (n <= 0) break;
        processMessage(static_cast<int>(buf));
    }
}

void ButtonSocketReceiver::processMessage(const int buttonNumber) {
    // Format: just the button index, e.g. "0", "5", "9"
    if (buttonNumber < 0 || buttonNumber > 9) {
        qWarning() << "Invalid button message:" << buttonNumber;
        return;
    }

    float value = static_cast<float>(buttonNumber);
    QString topic = "Wheel/Buttons/button_id";

    qDebug() << "Button:" << buttonNumber << "-> value:" << value;
    emit buttonDataReceived(topic, value);
    emit buttonEventReceived(QString::number(buttonNumber), QStringLiteral("down"));
}