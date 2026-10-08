#include "../models/raspberry_model.h"
#include <QCoreApplication>
#include <QProtobufSerializer>
#include <emscripten/bind.h>

static RaspberryModel *model() {
  return QCoreApplication::instance()->findChild<RaspberryModel *>();
}

static void publish(const std::string &topic, const emscripten::val &values) {
  const std::vector<float> list = emscripten::vecFromJSArray<float>(values);
  serverdata::v2::ServerData data;
  data.setValues(QList<float>(list.begin(), list.end()));
  model()->receiveServerData(data, QString::fromStdString(topic));
}

static void publishPayload(const std::string &topic,
                           const emscripten::val &bytes) {
  const std::vector<uint8_t> payload =
      emscripten::vecFromJSArray<uint8_t>(bytes);
  serverdata::v2::ServerData data;
  QProtobufSerializer serializer;
  if (serializer.deserialize(
          &data, QByteArray(reinterpret_cast<const char *>(payload.data()),
                            static_cast<qsizetype>(payload.size())))) {
    model()->receiveServerData(data, QString::fromStdString(topic));
  }
}

EMSCRIPTEN_BINDINGS(vcu_sim) {
  emscripten::function("publish", &publish);
  emscripten::function("publishPayload", &publishPayload);
}
