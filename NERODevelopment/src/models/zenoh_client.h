#ifndef ZENOHCLIENT_H
#define ZENOHCLIENT_H

#include <QObject>
#include <QProtobufSerializer>
#include <optional>
#include <serverdata.qpb.h>
#include <vector>
#include <zenoh.hxx>

/**
 * @brief The ZenohClient class, wraps the zenoh-cpp session down to the
 * functions we only care about
 *
 * The car runs a zenoh router on the TPU and everything else — Calypso, the
 * Odysseus daemon, NERO — joins it as a client, so this never listens or
 * scouts; it dials one endpoint and lets zenoh keep the link alive.
 */
class ZenohClient : public QObject {
  Q_OBJECT

public:
  /**
   * @brief ZenohClient
   * @param parent the owning QObject
   * @param keyExprs the key expressions to subscribe to
   * @param endpoint the router to connect to, as "tcp/host:port"
   */
  ZenohClient(QObject *parent = nullptr, QList<QString> keyExprs = {},
              QString endpoint = QString());
  ~ZenohClient();

public slots:
  /**
   * @brief connectToRouter opens the session and subscribes. Retries on its
   * own until the router answers, so NERO can start before zenohd does.
   */
  void connectToRouter();

  /**
   * @brief sendMessage, Puts a message on the zenoh router with the NERO key
   * prefix and then the passed following key path.
   * @param key, the key to put under NERO/
   * @param value, the value to send
   */
  void sendMessage(const QString key, const float value);

signals:
  /**
   * @brief emitServerData, Emits the parsed server data object from protobuf
   */
  void emitServerData(const serverdata::v2::ServerData, const QString);

  /**
   * @brief sampleReceived, carries a sample off the zenoh thread that
   * delivered it and onto the thread this client lives on. Queued, because the
   * two differ — everything below this signal runs where Qt expects it.
   * @param payload, the serialized protobuf payload
   * @param key, the key expression the sample arrived on
   */
  void sampleReceived(const QByteArray payload, const QString key);

private slots:
  /**
   * @brief handleSample, deserializes a sample and republishes it as server
   * data
   * @param payload, the serialized protobuf payload
   * @param key, the key expression the sample arrived on
   */
  void handleSample(const QByteArray &payload, const QString &key);

private:
  /**
   * @brief subscribe, declares a subscriber per key expression determined in
   * the constructor
   */
  void subscribe();

  std::optional<zenoh::Session> m_session;
  std::vector<zenoh::Subscriber<void>> m_subscribers;
  QString m_endpoint;
  QProtobufSerializer m_serializer;
  QList<QString> m_keyExprs = {};
};
#endif // ZENOHCLIENT_H
