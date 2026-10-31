import mqtt from "mqtt";

// Stand-in VCU navigation (Cerberus-2.0 u_statemachine.c, without gating), run by the dev-vcu
// service in compose.nero-dev.yml. The native app uses plain topics; each Playwright page gets
// its own VCU under a "sim/<id>/" topic prefix, so pages never share state.
const [ESC, LEFT, ENTER, RIGHT] = [0, 1, 5, 6];

// ServerData with values = [v]
const serverData = (v) => {
  const payload = Buffer.from([0x22, 4, 0, 0, 0, 0]);
  payload.writeFloatLE(v, 2);
  return payload;
};

const broker = await mqtt.connectAsync(process.env.MQTT_URL ?? "mqtt://localhost:1883");

function vcu(prefix) {
  let index = 0;
  let home = true;
  // Pages' VCUs use QoS 1 so replies survive a test run's telemetry load
  const send = (topic, value) => broker.publish(prefix + topic, serverData(value), { qos: prefix ? 1 : 0 });
  const sync = () => {
    // Calypso publishes the car state's home_mode before its nero_index
    send("VCU/CarState/home_mode", home ? 1 : 0);
    send("VCU/CarState/nero_index", index);
    send("VCU/CarState/functional_state", 0); // READY
  };
  const heartbeat = setInterval(sync, 200);
  sync();

  return {
    press(button) {
      if (button === ESC) home = true;
      if (button === ENTER) home = false;
      // Both wrap, over firmware MAX_NERO_STATES: Left at OFF underflows the firmware's unsigned
      // index, which it then clamps to the last slot, EXIT
      if (home && button === RIGHT) index = (index + 1) % 8;
      if (home && button === LEFT) index = (index + 7) % 8;
      // The wheel sends each press to NERO as well
      send("Wheel/Buttons/button_id", button);
      sync();
    },
    stop: () => clearInterval(heartbeat),
  };
}

const vcus = new Map([["", vcu("")]]);

// NERO publishes presses under NERO/ (values is serialized last); a page's start and stop
// create and end its VCU
broker.on("message", (topic, payload) => {
  // sim/+/NERO/# also matches sim/<id>/NERO itself
  const match = topic.match(/^((?:sim\/[^/]+\/)?)NERO\/(.+)$/);
  if (!match) return;
  const [, prefix, message] = match;
  if (message === "stop") {
    vcus.get(prefix)?.stop();
    vcus.delete(prefix);
    return;
  }
  if (!vcus.has(prefix)) vcus.set(prefix, vcu(prefix));
  if (message === "Wheel/Buttons/button_id" && payload.length >= 4) {
    vcus.get(prefix).press(payload.readFloatLE(payload.length - 4));
  }
});
await broker.subscribeAsync(["NERO/Wheel/Buttons/button_id", "sim/+/NERO/#"], { qos: 1 });
