#ifndef SOCKET_RECIEVER_H
#define SOCKET_RECIEVER_H

#include <QObject>
#include <QString>
#include <QLocalServer>
#include <QLocalSocket>

/**
 * @brief ButtonSocketReceiver listens for button press messages
 * and emits signals with the button topic and value.
 *
 * Uses QLocalServer for cross-platform support:
 *   - Linux: Unix domain socket at /tmp/nero_buttons_socket
 *   - Windows: Named pipe \\.\pipe\nero_buttons_socket
 *
 * Protocol: newline-delimited text: "buttonname_state\n"
 *   e.g. "forward_down\n", "enter_up\n"
 */
class ButtonSocketReceiver : public QObject {
    Q_OBJECT

public:
    explicit ButtonSocketReceiver(const QString &serverName = "nero_buttons_socket",
                                  QObject *parent = nullptr);
    ~ButtonSocketReceiver();

    bool start();
    void stop();
    bool isRunning() const { return m_running; }

signals:
    void buttonDataReceived(const QString &topic, float value);
    void buttonEventReceived(const QString &buttonName, const QString &state);
    void errorOccurred(const QString &errorMessage);

private slots:
    void handleNewConnection();
    void handleClientData();
    void handleClientDisconnected();

private:
    void processButtonMessage(const QString &message);
    QString getTopicForButton(const QString &buttonName);
    float getButtonValue(const QString &buttonName, const QString &state);

    QLocalServer *m_server;
    QLocalSocket *m_client;
    QString m_serverName;
    bool m_running;
    QByteArray m_buffer;
};

#endif // SOCKET_RECIEVER_H
