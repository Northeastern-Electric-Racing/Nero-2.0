
#include "mqtt_client.h"

#include "../utils/server_data.h"
#include <QJsonDocument>
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
  QJsonDocument jsonDocument = QJsonDocument::fromJson(message);

  if (jsonDocument.isNull() || !jsonDocument.isObject()) {
    qDebug() << "Error: Unable to parse JSON data.";
    return;
  }

  ServerData serverData(jsonDocument.object());

  DataInfo dataInfo(topic.name(), serverData.unit, serverData.value);
}

void MqttClient::updateMessage(const QMqttMessage &msg) {
  const QString debug = QDateTime::currentDateTime().toString() +
                        QLatin1String(" Received Topic: ") +
                        msg.topic().name() + QLatin1String(" Message: ") +
                        msg.payload() + QLatin1Char('\n');
  qDebug() << debug;
}