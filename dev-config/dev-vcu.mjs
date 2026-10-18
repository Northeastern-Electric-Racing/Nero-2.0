// Stand-in VCU navigation (Cerberus-2.0 u_statemachine.c, without gating). Used by the Docker
// mock for the native app, and by playwright/mock.mjs once per page for the WASM build.
const [ESC, LEFT, ENTER, RIGHT] = [0, 1, 5, 6];

// publish(topic, value) sends one value; returns press() and stop()
export function vcu(publish) {
  let index = 0;
  let home = true;
  const sync = () =>
    Promise.all([
      publish("VCU/CarState/nero_index", index),
      publish("VCU/CarState/home_mode", home ? 1 : 0),
      publish("VCU/CarState/functional_state", 0), // READY
    ]);
  const heartbeat = setInterval(sync, 200);
  sync();

  return {
    press(button) {
      if (button === ESC) home = true;
      if (button === ENTER) home = false;
      if (home && button === RIGHT) index = (index + 1) % 8; // firmware MAX_NERO_STATES
      if (home && button === LEFT) index = Math.max(index - 1, 0);
      // The wheel sends each press to NERO as well
      return Promise.all([publish("Wheel/Buttons/button_id", button), sync()]);
    },
    stop: () => clearInterval(heartbeat),
  };
}

// Run directly (the dev-vcu service in compose.nero-dev.yml): answer NERO's presses over MQTT.
// Plain function, not top-level await, so Playwright can also load this file for mock.mjs.
async function serve() {
  const { default: mqtt } = await import("mqtt");
  // ServerData with values = [v]
  const serverData = (v) => {
    const payload = Buffer.from([0x22, 4, 0, 0, 0, 0]);
    payload.writeFloatLE(v, 2);
    return payload;
  };
  const broker = await mqtt.connectAsync(process.env.MQTT_URL ?? "mqtt://localhost:1883");
  const { press } = vcu((topic, value) => broker.publish(topic, serverData(value)));
  // NERO publishes presses under NERO/; values is serialized last
  broker.on("message", (_, payload) => press(payload.readFloatLE(payload.length - 4)));
  await broker.subscribeAsync("NERO/Wheel/Buttons/button_id");
}
if (process.argv[1]?.endsWith("dev-vcu.mjs")) serve();
