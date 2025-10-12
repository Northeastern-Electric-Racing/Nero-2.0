
#include "mqtt_client.h"

#include <QJsonDocument>
#include <QTimer>
#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <chrono>
#include <serverdata.qpb.h>

MqttClient::MqttClient(QObject *parent, int port, QList<QString> topics)
    : QObject(parent) {  // Initialize hostname with host parameter
    m_client = new QMqttClient();
    m_port = port;
    m_topics = topics;
    connect(m_client, &QMqttClient::stateChanged, this,
            &MqttClient::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this,
            &MqttClient::brokerDisconnected);
    connect(m_client, &QMqttClient::messageReceived, this,
            &MqttClient::receiveMessage);
    connect(m_client, &QMqttClient::connected, this,
            &MqttClient::brokerConnected);
    setClientPort(m_port);
    m_client->setHostname(hostname);  // This now uses the passed host
    updateLogStateChange();
}

MqttClient::~MqttClient() {
  if (m_sub != nullptr) {
    delete m_sub;
  }
}

void MqttClient::connectToHost() { m_client->connectToHost(); }

void MqttClient::updateLogStateChange() {
  const QString content =
      QDateTime::currentDateTime().toString() + QLatin1String(": State Change");
  qDebug() << content << " " << m_client->state();
}

void MqttClient::brokerDisconnected() {
  qDebug("Disconnected");

  QTimer::singleShot(5000, this,
                     &MqttClient::connectToHost); // 5000 ms = 5 seconds
}

void MqttClient::setClientPort(int p) { m_client->setPort(p); }

void MqttClient::subscribe() {
  for (const auto &topic : this->m_topics) {
    QMqttSubscription *subscription = m_client->subscribe(topic, QoS);
    if (!subscription) {
      qDebug("Could not subscribe. Is there a valid connection?");
    } else {
      m_sub = subscription;
      connect(m_sub, &QMqttSubscription::messageReceived, this,
              &MqttClient::updateMessage);
    }
  }
}

void MqttClient::brokerConnected() {
  qDebug("connected");
  this->subscribe();
}

void MqttClient::receiveMessage(const QByteArray &message,
                                const QMqttTopicName &topic) {
  serverdata::v2::ServerData serverData;

  bool success = m_serializer.deserialize(&serverData, message);

  if (success) {
    emit emitServerData(serverData, topic.name());
  } else {
    qDebug() << "Failed to decode server message";
  }
}

void MqttClient::updateMessage(const QMqttMessage &msg) {
  const QString debug = QDateTime::currentDateTime().toString() +
                        QLatin1String(" Received Topic: ") +
                        msg.topic().name() + QLatin1String(" Message: ") +
                        msg.payload() + QLatin1Char('\n');
  // qDebug() << debug;
}

void MqttClient::sendMessage(const QString topic, const float value) {
  serverdata::v2::ServerData serverData;
  serverData.setUnit("");
  serverData.setValues({value});

  auto now = std::chrono::system_clock::now();
  auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
                          now.time_since_epoch())
                          .count();
  QtProtobuf::uint64 timeUs = static_cast<QtProtobuf::uint64>(microseconds);

  serverData.setTimeUs(timeUs);
  QByteArray data = serverData.serialize(&this->m_serializer);
  m_client->publish("NERO/" + topic, data);
}
