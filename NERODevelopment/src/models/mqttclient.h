#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <QTcpSocket>
#include <QtMqtt/QMqttClient>

class MqttClient : public QObject {
  Q_OBJECT

public:
  MqttClient(QObject *parent = nullptr);
  ~MqttClient();

public slots:
  void setClientPort(int p);

private slots:

  void updateLogStateChange();

  void brokerDisconnected();

  void on_btn_Subcribe_clicked();

public slots:
  void updateMessage(const QMqttMessage &msg);
  void updateStatus(QMqttSubscription::SubscriptionState state);

private:
  QMqttClient *m_client = nullptr;
  QMqttSubscription *m_sub = nullptr;

  QString hostname = "127.0.0.1";
  //    QString username = "xxxx&xxxx";
  //    QString password = "xxxxx";
  //    QString clientid = "xxxx|securemode=x,signmethod=xxx,timestamp=xxx|";
  int port = 1883;
  qint8 QoS = 0;
  QString default_topic = "/mpu";
};
#endif // MQTTCLIENT_H
