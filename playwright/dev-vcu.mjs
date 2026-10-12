import mqtt from "mqtt";

// Stand-in wheel and VCU nav (Cerberus-2.0 u_statemachine.c, without gating) for keyboard testing
const [ESC, LEFT, ENTER, RIGHT] = [0, 1, 5, 6];
let index = 0;
let home = true;

// ServerData with values = [v]
const serverData = (v) => {
  const payload = Buffer.from([0x22, 4, 0, 0, 0, 0]);
  payload.writeFloatLE(v, 2);
  return payload;
};
const publish = () => {
  broker.publish("VCU/CarState/nero_index", serverData(index));
  broker.publish("VCU/CarState/home_mode", serverData(home ? 1 : 0));
};

const broker = await mqtt.connectAsync("mqtt://localhost:1883");
// NERO publishes keyboard presses under NERO/, so re-send them as the wheel would
broker.on("message", (_, payload) => {
  broker.publish("Wheel/Buttons/button_id", payload);
  const button = payload.readFloatLE(payload.length - 4);
  if (button === ESC) home = true;
  if (button === ENTER) home = false;
  if (home && button === RIGHT) index = (index + 1) % 8; // firmware MAX_NERO_STATES
  if (home && button === LEFT) index = Math.max(index - 1, 0);
  publish();
});
await broker.subscribeAsync("NERO/Wheel/Buttons/button_id");
setInterval(publish, 200);
