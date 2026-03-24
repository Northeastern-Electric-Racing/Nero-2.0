#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <QProtobufSerializer>
#include <QTcpSocket>
#include <QtMqtt/QMqttClient>
#include <serverdata.qpb.h>

/**
 * @brief The MqttClient class, wraps the native QMqtt client to functions we
 * only care about
 */
class MqttClient : public QObject {
  Q_OBJECT

public:
  // Updated constructor to accept optional hostname
  MqttClient(QObject *parent = nullptr, int port = 1883,
             QList<QString> topics = {}, QString hostname = QString());
  ~MqttClient();

public slots:
  /**
   * @brief setClientPort, sets the port of the qmqtt client
   * @param p the new port
   */
  void setClientPort(int p);

  /**
   * @brief connectToHost connects to the server
   */
  void connectToHost();

  /**
   * @brief updateMessage, debugs the message received
   * @param msg, the message received from the subscription
   */
  void updateMessage(const QMqttMessage &msg);

  /**
   * @brief sendMessage, Sends a message over the mqtt server with the NERO
   * topic and then the passed following topic path.
   * @param msg, the message to send
   */
  void sendMessage(const QString topic, const float value);

signals:
  /**
   * @brief emitServerData, Emits the parsed server data object from protobuf
   */
  void emitServerData(const serverdata::v2::ServerData, const QString);

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

private:
  QMqttClient *m_client;
  QMqttSubscription *m_sub;
  QString m_hostname;
  int m_port = 1883;
  qint8 QoS = 0;
  QString default_topic = "#";
  QProtobufSerializer m_serializer;
  QList<QString> m_topics = {};
};
#endif // MQTTCLIENT_H
