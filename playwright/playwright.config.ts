import { defineConfig } from "@playwright/test";

export default defineConfig({
  testDir: "tests",
  snapshotPathTemplate: "{testDir}/__screenshots__/{arg}{ext}",
  use: {
    baseURL: "http://127.0.0.1:8137",
    viewport: { width: 800, height: 480 },
    screenshot: "on",
  },
  webServer: [
    {
      command: "http-server ../NERODevelopment/build-wasm -a 127.0.0.1 -p 8137 -s",
      url: "http://127.0.0.1:8137/index.html",
    },
    {
      command: "docker compose -f ../compose.nero-dev.yml up",
      port: 1883,
      reuseExistingServer: true,
      timeout: 180_000,
    },
  ],
});
