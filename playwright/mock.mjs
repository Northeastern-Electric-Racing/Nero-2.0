// The router's REST plugin streams every sample as server-sent events, which
// is all this bridge needs, so the mock stays free of a Zenoh client of its own
const ROUTER_REST = "http://localhost:8000";

// Calypso doesn't simulate these, so they are always injected
const GAP_FILL = {
  "BMS/Pack/Voltage": [470],
  "BMS/Status/Balancing": [0],
  "VCU/CarState/state_rejection_error": [0],
};

// Tests drive navigation; calypso's random values would jump between pages
const TEST_OWNED = ["VCU/CarState/nero_index", "VCU/CarState/home_mode", "Wheel/Buttons/button_id"];

// Reads SSE frames off the stream and hands each sample to onSample
async function readSamples(body, onSample) {
  const decoder = new TextDecoder();
  let buffer = "";
  for await (const chunk of body) {
    buffer += decoder.decode(chunk, { stream: true });
    let end;
    while ((end = buffer.indexOf("\n\n")) >= 0) {
      const frame = buffer.slice(0, end);
      buffer = buffer.slice(end + 2);
      const data = frame
        .split("\n")
        .filter((line) => line.startsWith("data:"))
        .map((line) => line.slice(5).trim())
        .join("");
      if (!data) continue;
      const sample = JSON.parse(data);
      // The plugin base64s anything that isn't text, which protobuf never is
      if (sample.value) onSample(sample.key, [...Buffer.from(sample.value, "base64")]);
    }
  }
}

// Feeds the page calypso's Docker mock data, except keys that have been injected
export async function connectMock(page) {
  const injected = new Set(TEST_OWNED);
  const latest = new Map();
  let paused = false;
  const abort = new AbortController();
  const stream = await fetch(`${ROUTER_REST}/**`, {
    headers: { Accept: "text/event-stream" },
    signal: abort.signal,
  });
  readSamples(stream.body, (key, payload) => injected.has(key) || latest.set(key, payload)).catch(() => {});

  // Calypso publishes every 5 ms, so forward only the latest value per key in batches
  const timer = setInterval(() => {
    if (paused || latest.size === 0) return;
    const batch = [...latest];
    latest.clear();
    page
      .evaluate((batch) => batch.forEach(([key, payload]) => window.nero.publishPayload(key, payload)), batch)
      .catch(() => {});
  }, 100);

  const inject = (keys) => {
    for (const key of Object.keys(keys)) {
      injected.add(key);
      latest.delete(key);
    }
    return page.evaluate(
      (keys) => Object.entries(keys).forEach(([key, values]) => window.nero.publish(key, values)),
      keys,
    );
  };
  await inject(GAP_FILL);

  return {
    inject,
    // Calypso cycles through every key in ~300 ms; let a full set arrive, then hold still for a screenshot
    freeze: async () => {
      await new Promise((resolve) => setTimeout(resolve, 500));
      paused = true;
    },
    close: () => {
      clearInterval(timer);
      abort.abort();
    },
  };
}
