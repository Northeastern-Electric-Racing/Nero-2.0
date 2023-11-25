
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
          [](const QByteArray &message, const QMqttTopicName &topic) {
            const QString content = QDateTime::currentDateTime().toString() +
                                    QLatin1String(" Received Topic: ") +
                                    topic.name() + QLatin1String(" Message: ") +
                                    message + QLatin1Char('\n');
          });

  connect(m_client, &QMqttClient::pingResponseReceived, this, []() {
    const QString content = QDateTime::currentDateTime().toString() +
                            QLatin1String(" PingResponse") + QLatin1Char('\n');
  });

  connect(m_client, &QMqttClient::connected, this,
          [this]() { qDebug("Connected"); });

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

void MqttClient::on_btn_Subcribe_clicked() {
  QMqttSubscription *subscription = m_client->subscribe(default_topic, QoS);
  if (!subscription) {
    qDebug("Could not subscribe. Is there a valid connection?");
  } else {
    m_sub = subscription;
    updateStatus(m_sub->state());
    connect(m_sub, &QMqttSubscription::messageReceived, this,
            &MqttClient::updateMessage);
    connect(m_sub, &QMqttSubscription::stateChanged, this,
            &MqttClient::updateStatus);
    connect(m_sub, &QMqttSubscription::qosChanged,
            [this](quint8 qos) { qDebug() << qos; });
  }
}

void MqttClient::updateMessage(const QMqttMessage &msg) {
  qDebug() << msg.payload();
}

void MqttClient::updateStatus(QMqttSubscription::SubscriptionState state) {
  switch (state) {
  case QMqttSubscription::Unsubscribed:
    qDebug("Unsubscribed");
    break;
  case QMqttSubscription::SubscriptionPending:
    qDebug("Pending");
    break;
  case QMqttSubscription::Subscribed:
    qDebug("subscribed");
    break;
  case QMqttSubscription::Error:
    qDebug("Error");
    break;
  default:
    qDebug("--Unknown--");
    break;
  }
}
