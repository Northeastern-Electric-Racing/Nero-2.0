
#include "zenoh_client.h"

#include <QDebug>
#include <QSet>
#include <QTimer>
#include <chrono>
#include <serverdata.qpb.h>

namespace {

/**
 * @brief buildConfig, assembles the session config
 *
 * ZENOH_CONFIG wins outright: a json5 conf file is how Calypso, Scylla and the
 * Odysseus daemon are configured, so the car can hand NERO the same file it
 * hands everything else. Without one, dial the endpoint as a plain client.
 */
zenoh::Config buildConfig(const QString &endpoint) {
  const QString confPath = qEnvironmentVariable("ZENOH_CONFIG");
  if (!confPath.isEmpty()) {
    zenoh::ZResult err = Z_OK;
    zenoh::Config fromFile =
        zenoh::Config::from_file(confPath.toStdString(), &err);
    if (err == Z_OK) {
      qInfo() << "Zenoh config read from" << confPath;
      return fromFile;
    }
    qWarning() << "Could not read Zenoh config" << confPath
               << "- falling back to" << endpoint;
  }

  zenoh::Config config = zenoh::Config::create_default();
  config.insert_json5("mode", "\"client\"");
  config.insert_json5("connect/endpoints",
                      QStringLiteral("[\"%1\"]").arg(endpoint).toStdString());
  // 0 is "try once, then tell us". The alternatives either block the GUI
  // thread until a router appears (-1) or sit on it for the timeout, and
  // ZenohClient::connectToRouter already owns the retry.
  config.insert_json5("connect/timeout_ms", "0");
  // Every link on the car is an explicit endpoint. Scouting would only turn up
  // whatever else happens to be on the paddock network.
  config.insert_json5("scouting/multicast/enabled", "false");
  return config;
}

/**
 * @brief defaultEndpoint, the router to dial when no conf file is given
 *
 * HOST carries over from the MQTT setup — it is what the car's init script
 * sets — and the router listens on 7447 unless ZENOH_PORT says otherwise.
 */
QString defaultEndpoint() {
  const QString host =
      qEnvironmentVariable("HOST", QStringLiteral("localhost"));
  const QString port =
      qEnvironmentVariable("ZENOH_PORT", QStringLiteral("7447"));
  return QStringLiteral("tcp/%1:%2").arg(host, port);
}

} // namespace

ZenohClient::ZenohClient(QObject *parent, QList<QString> keyExprs,
                         QString endpoint)
    : QObject(parent) {
  m_keyExprs = keyExprs;
  m_endpoint = endpoint.isEmpty() ? defaultEndpoint() : endpoint;

  zenoh::init_log_from_env_or("error");

  // Samples arrive on a zenoh thread; this hop is what puts them back on the
  // thread that owns the model before anything touches it.
  connect(this, &ZenohClient::sampleReceived, this, &ZenohClient::handleSample,
          Qt::QueuedConnection);
}

// m_subscribers is declared after m_session, so it is torn down first and no
// callback can outlive the session it reads from.
ZenohClient::~ZenohClient() = default;

void ZenohClient::connectToRouter() {
  if (m_session) {
    return;
  }

  zenoh::ZResult err = Z_OK;
  zenoh::Session session = zenoh::Session::open(
      buildConfig(m_endpoint), zenoh::Session::SessionOptions::create_default(),
      &err);

  if (err != Z_OK) {
    qWarning() << "Could not reach the Zenoh router at" << m_endpoint
               << "- retrying in 5s";
    QTimer::singleShot(5000, this, &ZenohClient::connectToRouter);
    return;
  }

  qInfo() << "Connected to the Zenoh router at" << m_endpoint;
  m_session.emplace(std::move(session));
  this->subscribe();
}

void ZenohClient::subscribe() {
  // The key expression list names a few keys twice — the critical BMS faults
  // are both a fault source and a top-level fault list. Subscribing twice
  // would deliver every one of those samples twice.
  QSet<QString> declared;

  for (const auto &keyExpr : this->m_keyExprs) {
    if (declared.contains(keyExpr)) {
      continue;
    }
    declared.insert(keyExpr);

    // Not every topic name is a legal key expression — '?' and '#' are out,
    // and '**' is the only wildcard — so say which one is wrong rather than
    // throwing out of here
    zenoh::ZResult err = Z_OK;
    zenoh::KeyExpr key(keyExpr.toStdString(), true, &err);
    if (err != Z_OK) {
      qWarning() << keyExpr << "is not a valid Zenoh key expression";
      continue;
    }

    zenoh::Subscriber<void> subscriber = m_session->declare_subscriber(
        std::move(key),
        [this](const zenoh::Sample &sample) {
          const std::vector<uint8_t> payload = sample.get_payload().as_vector();
          const std::string_view key = sample.get_keyexpr().as_string_view();
          emit this->sampleReceived(
              QByteArray(reinterpret_cast<const char *>(payload.data()),
                         static_cast<qsizetype>(payload.size())),
              QString::fromUtf8(key.data(),
                                static_cast<qsizetype>(key.size())));
        },
        zenoh::closures::none,
        zenoh::Session::SubscriberOptions::create_default(), &err);

    if (err != Z_OK) {
      qWarning() << "Could not subscribe to" << keyExpr;
      continue;
    }
    m_subscribers.push_back(std::move(subscriber));
  }
}

void ZenohClient::handleSample(const QByteArray &payload, const QString &key) {
  serverdata::v2::ServerData serverData;

  if (m_serializer.deserialize(&serverData, payload)) {
    emit emitServerData(serverData, key);
  } else {
    qDebug() << "Failed to decode server message";
  }
}

void ZenohClient::sendMessage(const QString key, const float value) {
  if (!m_session) {
    return;
  }

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

  auto options = zenoh::Session::PutOptions::create_default();
  options.encoding = zenoh::Encoding::Predefined::application_protobuf();

  zenoh::ZResult err = Z_OK;
  zenoh::KeyExpr keyExpr(("NERO/" + key).toStdString(), true, &err);
  if (err != Z_OK) {
    qWarning() << key << "is not a valid Zenoh key expression";
    return;
  }

  m_session->put(std::move(keyExpr),
                 zenoh::Bytes(std::vector<uint8_t>(data.begin(), data.end())),
                 std::move(options), &err);
  if (err != Z_OK) {
    qWarning() << "Failed to put" << key << "on the Zenoh router";
  }
}
