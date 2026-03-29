#ifndef SOCKET_RECIEVER_H
#define SOCKET_RECIEVER_H

#include <QObject>
#include <QString>
#include <QLocalServer>
#include <QLocalSocket>

/**
 * @brief ButtonSocketReceiver listens on a local socket for button events
 * from Odysseus (wheel-buttons service).
 *
 * Uses QLocalServer:
 *   - Linux:   /tmp/wheel_buttons_socket
 *   - Windows: \\.\pipe\wheel_buttons_socket
 *
 * Protocol: newline-delimited text
 * Format:   "button_{N}_{state}\n"
 * Examples: "button_0_down\n", "button_3_up\n"
 *
 * Button mapping (N = value written to Wheel/Buttons/button_id):
 *   0 = backward, 1 = right/forward, 3 = down, 4 = up, 5 = enter
 *   10 = released (sent on _up)
 */
class ButtonSocketReceiver : public QObject {
    Q_OBJECT

public:
    explicit ButtonSocketReceiver(
        const QString &serverName = "wheel_buttons_socket",
        QObject *parent = nullptr);
    ~ButtonSocketReceiver();

    bool start();
    void stop();
    bool isRunning() const { return m_running; }

signals:
    /** Emitted with topic and float value for the model */
    void buttonDataReceived(const QString &topic, float value);

    /** Emitted for logging/monitoring */
    void buttonEventReceived(const QString &buttonName, const QString &state);

    void errorOccurred(const QString &errorMessage);

private slots:
    void handleNewConnection();
    void handleClientData();
    void handleClientDisconnected();

private:
    void processMessage(const QString &message);

    QLocalServer *m_server;
    QLocalSocket *m_client;
    QString m_serverName;
    bool m_running;
    QByteArray m_buffer;
};

#endif
