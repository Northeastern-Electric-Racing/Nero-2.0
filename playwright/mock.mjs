import mqtt from "mqtt";
import { vcu } from "../dev-config/dev-vcu.mjs";

// Calypso doesn't simulate these, so they are always injected
const GAP_FILL = {
  "BMS/Pack/Voltage": [470],
  "BMS/Status/Balancing": [0],
  "VCU/CarState/state_rejection_error": [0],
};

// Each page runs its own stand-in VCU, so the Docker mock's dev-vcu output is ignored
const VCU_TOPICS = [
  "VCU/CarState/nero_index",
  "VCU/CarState/home_mode",
  "VCU/CarState/functional_state",
  "Wheel/Buttons/button_id",
];

// Feeds the page calypso's Docker mock data and a stand-in VCU that answers NERO's presses
export async function connectMock(page) {
  const injected = new Set();
  const latest = new Map();
  let paused = false;
  const broker = await mqtt.connectAsync("mqtt://localhost:1883");
  broker.on("message", (topic, payload) => {
    if (!VCU_TOPICS.includes(topic) && !injected.has(topic)) latest.set(topic, [...payload]);
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

  const publish = (topic, value) =>
    page.evaluate(([topic, value]) => window.nero.publish(topic, [value]), [topic, value]).catch(() => {});
  // An injected topic overrides the stand-in VCU, e.g. a nero_index firmware doesn't have yet
  const pageVcu = vcu((topic, value) => injected.has(topic) || publish(topic, value));
  // NERO's own presses (the keyboard) arrive through wasm_bridge.cpp
  await page.exposeFunction("neroSend", (topic, value) => topic === "Wheel/Buttons/button_id" && pageVcu.press(value));

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
    press: async (...buttons) => {
      for (const button of buttons) await pageVcu.press(button);
    },
    // Calypso cycles through every topic in ~300 ms; let a full set arrive, then hold still for a screenshot
    freeze: async () => {
      await new Promise((resolve) => setTimeout(resolve, 500));
      paused = true;
    },
    vcuLost: pageVcu.stop,
    close: () => {
      clearInterval(timer);
      pageVcu.stop();
      return broker.endAsync();
    },
  };
}
