
#include "mqttclient.h"

#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <QtWidgets/QMessageBox>

MqttClient::MqttClient(QObject *parent) : QObject(parent) {
  m_client = new QMqttClient();

  connect(m_client, &QMqttClient::stateChanged, this,
          &MqttClient::updateLogStateChange);

  connect(m_client, &QMqttClient::disconnected, this,
          &MqttClient::brokerDisconnected);

  connect(m_client, &QMqttClient::messageReceived, this,
          &MqttClient::receiveMessage);

  connect(m_client, &QMqttClient::connected, this,
          &MqttClient::brokerConnected);

  setClientPort(port);
  m_client->setHostname(hostname);
  qDebug() << "Connecting to" << m_client->hostname() << "on port"
           << m_client->port();
  m_client->connectToHost();
  updateLogStateChange();
}

MqttClient::~MqttClient() {
  if (m_sub != nullptr) {
    delete m_sub;
  }
}

void MqttClient::updateLogStateChange() {
  const QString content =
      QDateTime::currentDateTime().toString() + QLatin1String(": State Change");
  qDebug() << content << " " << m_client->state();
}

void MqttClient::brokerDisconnected() { qDebug("Disconnected"); }

void MqttClient::setClientPort(int p) { m_client->setPort(p); }

void MqttClient::subscribe() {
  QMqttSubscription *subscription = m_client->subscribe(default_topic, QoS);
  if (!subscription) {
    qDebug("Could not subscribe. Is there a valid connection?");
  } else {
    m_sub = subscription;
    connect(m_sub, &QMqttSubscription::messageReceived, this,
            &MqttClient::updateMessage);
  }
}

void MqttClient::brokerConnected() {
  qDebug("connected");
  this->subscribe();
}

void MqttClient::receiveMessage(const QByteArray &message,
                                const QMqttTopicName &topic) {
  const QString content = QDateTime::currentDateTime().toString() +
                          QLatin1String(" Received Topic: ") + topic.name() +
                          QLatin1String(" Message: ") + message +
                          QLatin1Char('\n');
  qDebug() << content;
}

void MqttClient::updateMessage(const QMqttMessage &msg) {
  qDebug() << msg.payload();
}
