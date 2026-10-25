// Browser-only: the dev WASM build has no MQTT client, so Playwright's mock.mjs
// feeds the model (window.nero.publish) and receives NERO's presses
// (window.neroSend). Compiles to nothing on desktop and on the car.
#ifdef __EMSCRIPTEN__
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

void RaspberryModel::connectToMQTT() {}

void RaspberryModel::sendMessage(const QString topic, const float value) {
  const emscripten::val send = emscripten::val::global("neroSend");
  if (send.typeOf().as<std::string>() == "function")
    send(topic.toStdString(), value);
}

EMSCRIPTEN_BINDINGS(wasm_bridge) {
  emscripten::function("publish", &publish);
  emscripten::function("publishPayload", &publishPayload);
}
#endif
