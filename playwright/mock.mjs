import mqtt from "mqtt";

// Calypso doesn't simulate these, so they are always injected
const GAP_FILL = {
  "BMS/Pack/Voltage": [470],
  "BMS/Status/Balancing": [0],
  "VCU/CarState/state_rejection_error": [0],
};

// Tests own these; calypso's random values would change the page or car state
const TEST_OWNED = [
  "VCU/CarState/nero_index",
  "VCU/CarState/home_mode",
  "VCU/CarState/functional_state",
  "Wheel/Buttons/button_id",
];

// Feeds the page calypso's Docker mock data, except topics that have been injected
export async function connectMock(page) {
  const injected = new Set(TEST_OWNED);
  const latest = new Map();
  let paused = false;
  const broker = await mqtt.connectAsync("mqtt://localhost:1883");
  broker.on("message", (topic, payload) => injected.has(topic) || latest.set(topic, [...payload]));
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

  // Like the VCU, re-send a READY car state every 200 ms, even while frozen
  const sendReady = () =>
    page.evaluate(() => window.nero.publish("VCU/CarState/functional_state", [0])).catch(() => {});
  const heartbeat = setInterval(sendReady, 200);
  await sendReady();

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
    // Calypso cycles through every topic in ~300 ms; let a full set arrive, then hold still for a screenshot
    freeze: async () => {
      await new Promise((resolve) => setTimeout(resolve, 500));
      paused = true;
    },
    vcuLost: () => clearInterval(heartbeat),
    close: () => {
      clearInterval(timer);
      clearInterval(heartbeat);
      return broker.endAsync();
    },
  };
}
