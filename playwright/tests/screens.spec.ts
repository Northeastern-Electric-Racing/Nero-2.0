import { test as base, expect, type Page } from "@playwright/test";
import { connectMock } from "../mock.mjs";

// Top-level menu slots, and wheel button ids from readme "Button Layout"
const PAGE = { OFF: 0, PIT_DRIVE: 1, PERFORMANCE: 3, ENDURANCE: 4, GAMES: 5, THEMES: 6, EXIT: 7 };
const [ESC, LEFT, UP, DOWN, ENTER, RIGHT] = [0, 1, 3, 4, 5, 6];

// A step presses wheel buttons through the page's stand-in VCU, or injects topics no VCU sends
type Step = number[] | Record<string, number[]>;

const test = base.extend<{ mock: Awaited<ReturnType<typeof connectMock>> }>({
  mock: async ({ page }, use) => {
    await page.goto("/index.html");
    await page.waitForFunction(() => (window as any).nero);
    const mock = await connectMock(page);
    await use(mock);
    await mock.close();
  },
});

// Every page starts on the menu at OFF, like the VCU after power-up
const select = (page: number) => Array(page).fill(RIGHT);
const open = (page: number) => [...select(page), ENTER];

// Live data moves menus by under 0.5% and data pages by up to ~9%; distinct screens differ by 6%+
const MENU = 0.01;
const LIVE_DATA = 0.12;

const cases: Record<string, { steps: Step[]; tolerance?: number }> = {
  home: { steps: [select(PAGE.PERFORMANCE)] },
  // Left at OFF wraps to EXIT, like the firmware
  "home-wrap": { steps: [[LEFT]] },
  off: { steps: [open(PAGE.OFF)], tolerance: LIVE_DATA },
  "pit-drive": { steps: [open(PAGE.PIT_DRIVE)], tolerance: LIVE_DATA },
  performance: { steps: [open(PAGE.PERFORMANCE)], tolerance: LIVE_DATA },
  endurance: { steps: [open(PAGE.ENDURANCE)], tolerance: LIVE_DATA },
  "back-home": { steps: [open(PAGE.PERFORMANCE), [ESC]] },
  // Firmware sends a bitmask; 12 is shutdown open and brakes not engaged
  "vcu-rejected": { steps: [select(PAGE.PERFORMANCE), { "VCU/CarState/state_rejection_error": [12] }] },
  "critical-fault": {
    steps: [open(PAGE.PERFORMANCE), { "BMS/Faults/Critical/Cell_Voltage_High": [1] }],
    tolerance: LIVE_DATA,
  },
  games: { steps: [open(PAGE.GAMES)] },
  "themes-next": { steps: [open(PAGE.THEMES), [RIGHT]] },
  // One Down moves exactly one entry, to SNAKE
  "games-down": { steps: [open(PAGE.GAMES), [DOWN]] },
  // From the first entry, Up and Left wrap to the last, DOOM
  "games-up": { steps: [open(PAGE.GAMES), [UP]] },
  "games-left": { steps: [open(PAGE.GAMES), [LEFT]] },
  // Leaving a game and reopening GAMES shows the list, not the game
  "game-cleared": { steps: [open(PAGE.GAMES), [ENTER, ESC, ENTER]] },
  "themes-light": { steps: [open(PAGE.THEMES), [ENTER, ESC]] },
  // A nero_index firmware has but NERO doesn't: the menu stays up with a notice
  "unknown-page": { steps: [{ "VCU/CarState/nero_index": [9], "VCU/CarState/home_mode": [0] }] },
  // After missed frames, the next frame opens the slot it names (OFF), not the one heard last (EXIT)
  "exit-after-missed-frames": {
    steps: [
      { "VCU/CarState/home_mode": [1], "VCU/CarState/nero_index": [PAGE.EXIT] },
      { "VCU/CarState/home_mode": [0], "VCU/CarState/nero_index": [PAGE.OFF] },
    ],
    tolerance: LIVE_DATA,
  },
};

const run = (mock: Awaited<ReturnType<typeof connectMock>>, steps: Step[]) =>
  steps.reduce((done, step) => done.then(() => (Array.isArray(step) ? mock.press(...step) : mock.inject(step))), Promise.resolve());

for (const [name, { steps, tolerance = MENU }] of Object.entries(cases)) {
  test(name, async ({ page, mock }) => {
    await run(mock, steps);
    await mock.freeze();
    await expect(page).toHaveScreenshot(`${name}.png`, { maxDiffPixelRatio: tolerance });
  });
}

// NERO has quit once its window is gone and the page renders like an empty one
const closed = async (page: Page) => {
  const empty = await page.context().newPage();
  const expected = await empty.screenshot();
  await empty.close();
  return (await page.screenshot()).equals(expected);
};

test("exit", async ({ page, mock }) => {
  await mock.press(...open(PAGE.EXIT));
  await expect.poll(() => closed(page)).toBe(true);
});

// A NERO started while the VCU already sits on EXIT stays up until the VCU opens EXIT again
test("exit-at-startup", async ({ page, mock }) => {
  await mock.press(...open(PAGE.EXIT));
  await page.reload();
  await page.waitForFunction(() => (window as any).nero);
  await mock.freeze();
  await expect(page).toHaveScreenshot("exit-at-startup.png", { maxDiffPixelRatio: MENU });
  expect(await closed(page)).toBe(false);
  await mock.press(ESC, ENTER);
  await expect.poll(() => closed(page)).toBe(true);
});

// The state label falls back to NO VCU once car state stops
test("vcu-lost", async ({ page, mock }) => {
  await mock.press(...select(PAGE.PERFORMANCE));
  await mock.freeze();
  await expect(page).toHaveScreenshot("home.png", { maxDiffPixelRatio: MENU });
  mock.vcuLost();
  await page.waitForTimeout(1500);
  await mock.freeze();
  await expect(page).toHaveScreenshot("vcu-lost.png", { maxDiffPixelRatio: MENU });
});

// Games animate on their own, so check Flappy Bird opens and its game loop runs
test("flappy-bird", async ({ page, mock }) => {
  await mock.press(...open(PAGE.GAMES));
  const list = await page.screenshot();
  await mock.press(ENTER);
  await page.waitForTimeout(1000);
  expect((await page.screenshot()).equals(list)).toBe(false);
  // It waits for Enter, then runs its game loop
  await mock.press(ENTER);
  const start = await page.screenshot();
  await page.waitForTimeout(200);
  expect((await page.screenshot()).equals(start)).toBe(false);
});
