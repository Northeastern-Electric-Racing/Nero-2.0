#ifndef SOCKET_RECIEVER_H
#define SOCKET_RECIEVER_H

#include <QObject>
#include <QString>
#include <QSocketNotifier>
#include "mqtt_client.h"

/**
 * @brief ButtonSocketReceiver listens on a Unix domain socket for button
 * press messages from a GPIO client and publishes them to MQTT topics
 */
class ButtonSocketReceiver : public QObject {
    Q_OBJECT

public:
    explicit ButtonSocketReceiver(MqttClient *mqttClient,
                                  const QString &socketPath = "/tmp/nero_buttons_socket",
                                  QObject *parent = nullptr);
    ~ButtonSocketReceiver();

    bool start();
    void stop();
    bool isRunning() const { return m_running; }

signals:
    void buttonEventReceived(const QString &buttonName, const QString &state);
    void errorOccurred(const QString &errorMessage);

private slots:
    void handleNewConnection();
    void handleClientData();
    void handleClientDisconnected();

private:
    void processButtonMessage(const QString &message);
    QString getMqttTopicForButton(const QString &buttonName, const QString &state);
    float getButtonValue(const QString &buttonName, const QString &state);

    MqttClient *m_mqttClient;
    QString m_socketPath;
    int m_serverFd;
    int m_clientFd;
    QSocketNotifier *m_serverNotifier;
    QSocketNotifier *m_clientNotifier;
    bool m_running;

    static const int BUFFER_SIZE = 256;
};

#endif
