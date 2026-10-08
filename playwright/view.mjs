import { chromium } from "@playwright/test";
import { execSync } from "node:child_process";
import { createServer } from "http-server";
import { connectMock } from "./mock.mjs";

execSync("docker compose -f ../compose.nero-dev.yml up -d", { stdio: "inherit" });

const server = createServer({ root: "../NERODevelopment/build-wasm", cache: -1 });
await new Promise((resolve) => server.listen(0, "localhost", resolve));

// An app window has no tabs, bookmarks, or warning banners, so the page can match the car's 800×480 display
const context = await chromium.launchPersistentContext("", {
  channel: "chrome",
  headless: false,
  viewport: null,
  chromiumSandbox: true,
  ignoreDefaultArgs: ["--enable-automation"],
  // Port 9222 lets the playwright-cdp MCP drive this same window
  args: [`--app=http://localhost:${server.server.address().port}/index.html`, "--remote-debugging-port=9222"],
});
const page = context.pages()[0] ?? (await context.waitForEvent("page"));
const cdp = await context.newCDPSession(page);
const { windowId } = await cdp.send("Browser.getWindowForTarget");
await cdp.send("Browser.setWindowBounds", { windowId, bounds: { windowState: "normal" } });

// Window managers add their own borders, so adjust until the page itself measures 800×480
for (let attempt = 0; attempt < 3; attempt++) {
  await page.waitForTimeout(300);
  const { bounds } = await cdp.send("Browser.getWindowBounds", { windowId });
  const [width, height] = await page.evaluate(() => [innerWidth, innerHeight]);
  if (width === 800 && height === 480) break;
  await cdp.send("Browser.setWindowBounds", {
    windowId,
    bounds: { width: bounds.width + 800 - width, height: bounds.height + 480 - height },
  });
}
const [width, height] = await page.evaluate(() => [innerWidth, innerHeight]);
if (width !== 800 || height !== 480) console.warn(`The window manager kept the page at ${width}×${height}, not 800×480`);
context.on("close", () => process.exit());

await page.waitForFunction(() => window.nero);
await connectMock(page);
