#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <QTcpSocket>
#include <QtMqtt/QMqttClient>

/**
 * @brief The MqttClient class, wraps the native QMqtt client to functions we
 * only care about
 */
class MqttClient : public QObject {
  Q_OBJECT

public:
  MqttClient(QObject *parent = nullptr);
  ~MqttClient();

public slots:
  /**
   * @brief setClientPort, sets the port of the qmqtt client
   * @param p the new port
   */
  void setClientPort(int p);

private slots:

  /**
   * @brief updateLogStateChange, slot that logs the current state of the qmqtt
   * client
   */
  void updateLogStateChange();

  /**
   * @brief brokerDisconnected, emits whenever the broker gets disconnected from
   * the client
   */
  void brokerDisconnected();

  /**
   * @brief subscribe, subscribes to the default topics determined in the header
   * file
   */
  void subscribe();

  /**
   * @brief brokerConnected, emits when the client connects to the broker
   */
  void brokerConnected();

  /**
   * @brief receiveMessage, receives messages for the subscribed to topics
   * @param message, the message of the mqtt payload
   * @param topic, the topic the message is being received on
   */
  void receiveMessage(const QByteArray &message, const QMqttTopicName &topic);

public slots:
  /**
   * @brief updateMessage, debugs the message received
   * @param msg, the message received from the broker
   */
  void updateMessage(const QMqttMessage &msg);

private:
  QMqttClient *m_client = nullptr;
  QMqttSubscription *m_sub = nullptr;

  QString hostname = "192.168.100.176";
  //    QString username = "xxxx&xxxx";
  //    QString password = "xxxxx";
  //    QString clientid = "xxxx|securemode=x,signmethod=xxx,timestamp=xxx|";
  int port = 1883;
  qint8 QoS = 0;
  QString default_topic = "/#";
};
#endif // MQTTCLIENT_H
