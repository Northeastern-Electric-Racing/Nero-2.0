import mqtt from "mqtt";

// Calypso doesn't simulate these, so they are always injected
const GAP_FILL = {
  "BMS/Pack/Voltage": [470],
  "BMS/Status/Balancing": [0],
  "VCU/CarState/state_rejection_error": [0],
};

// The native app's stand-in VCU topics; each page uses its own under its prefix instead
const VCU_TOPICS = [
  "VCU/CarState/nero_index",
  "VCU/CarState/home_mode",
  "VCU/CarState/functional_state",
  "Wheel/Buttons/button_id",
];

// ServerData with values = [v]
const serverData = (v) => {
  const payload = Buffer.from([0x22, 4, 0, 0, 0, 0]);
  payload.writeFloatLE(v, 2);
  return payload;
};

// The WASM build has no MQTT client, so this is it: it feeds the page calypso's Docker mock data
// and connects it to its own stand-in VCU in the Docker mock (dev-config/dev-vcu.mjs)
export async function connectMock(page) {
  const prefix = `sim/${Math.random().toString(36).slice(2, 10)}/`;
  const injected = new Set();
  const latest = new Map();
  let paused = false;
  let delivered = Promise.resolve();
  let onReady = () => {};
  let pressed = false; // after a press, only the car state that follows its echo counts
  const deliver = (topic, payload) =>
    (delivered = delivered.then(() =>
      page.evaluate(([topic, payload]) => window.nero.publishPayload(topic, payload), [topic, [...payload]]).catch(() => {}),
    ));

  // The VCU gets its own connection so its replies never queue behind telemetry. If this process
  // ends without close(), the broker sends the stop itself, so the VCU doesn't run on
  const vcuLink = await mqtt.connectAsync("mqtt://localhost:1883", {
    will: { topic: `${prefix}NERO/stop`, payload: serverData(0), qos: 1 },
  });
  vcuLink.on("message", (topic, payload) => {
    // The VCU's replies, in order and unbatched: each press is an event
    const name = topic.slice(prefix.length);
    if (name.startsWith("NERO/") || injected.has(name)) return;
    deliver(name, payload);
    if (name === "Wheel/Buttons/button_id") pressed = false;
    if (name === "VCU/CarState/functional_state" && !pressed) delivered.then(onReady);
  });
  await vcuLink.subscribeAsync(`${prefix}#`, { qos: 1 });

  const broker = await mqtt.connectAsync("mqtt://localhost:1883");
  broker.on("message", (topic, payload) => {
    if (!topic.startsWith("sim/") && !VCU_TOPICS.includes(topic) && !injected.has(topic)) latest.set(topic, [...payload]);
  });
  await broker.subscribeAsync("#");

  // Calypso publishes every 5 ms, so forward only the latest value per topic in batches
  const timer = setInterval(() => {
    if (paused || latest.size === 0) return;
    const batch = [...latest];
    latest.clear();
    page
      .evaluate((batch) => batch.forEach(([topic, payload]) => window.nero.publishPayload(topic, payload)), batch)
      .catch(() => {});
  }, 100);

  // Resolves once the VCU's next car state reaches the page
  const nextState = () => new Promise((resolve) => (onReady = resolve));
  const send = (message, value = 0) => vcuLink.publishAsync(`${prefix}NERO/${message}`, serverData(value), { qos: 1 });
  // NERO's own presses (the keyboard) arrive through wasm_bridge.cpp
  await page.exposeFunction("neroSend", (topic, value) => send(topic, value));
  // A start sent before dev-vcu subscribes is lost, so repeat it until the VCU answers
  const started = nextState();
  const retry = setInterval(() => send("start"), 500);
  await send("start");
  await started.finally(() => clearInterval(retry));

  const inject = (topics) => {
    for (const topic of Object.keys(topics)) {
      injected.add(topic);
      latest.delete(topic);
    }
    return page.evaluate(
      (topics) => Object.entries(topics).forEach(([topic, values]) => window.nero.publish(topic, values)),
      topics,
    );
  };
  await inject(GAP_FILL);

  return {
    inject,
    // Press wheel buttons like NERO would, each waiting for the VCU's answer
    press: async (...buttons) => {
      for (const button of buttons) {
        const answered = nextState();
        pressed = true;
        await send("Wheel/Buttons/button_id", button);
        await answered;
      }
    },
    // Calypso cycles through every topic in ~300 ms; let a full set arrive, then hold still for a screenshot
    freeze: async () => {
      await new Promise((resolve) => setTimeout(resolve, 500));
      paused = true;
    },
    vcuLost: () => send("stop"),
    close: async () => {
      clearInterval(timer);
      await send("stop");
      await vcuLink.endAsync();
      return broker.endAsync();
    },
  };
}
