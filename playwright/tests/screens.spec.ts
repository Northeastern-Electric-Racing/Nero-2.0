import { test as base, expect } from "@playwright/test";
import { connectMock } from "../mock.mjs";

const NERO_INDEX = "VCU/CarState/nero_index";
const HOME_MODE = "VCU/CarState/home_mode";
const BUTTON_ID = "Wheel/Buttons/button_id";
// Top-level menu slots, and wheel button ids from readme "Button Layout"
const PAGE = { OFF: 0, PIT_DRIVE: 1, REVERSE: 2, PERFORMANCE: 3, ENDURANCE: 4, GAMES: 5, THEMES: 6, EXIT: 7, UNKNOWN: 9 };
const BUTTON = { LEFT: 1, UP_REGEN: 3, DOWN_REGEN: 4, ENTER: 5, RIGHT: 6 };

type Topics = Record<string, number[]>;

const test = base.extend<{ mock: Awaited<ReturnType<typeof connectMock>> }>({
  mock: async ({ page }, use) => {
    await page.goto("/index.html");
    await page.waitForFunction(() => (window as any).nero);
    const mock = await connectMock(page);
    await use(mock);
    await mock.close();
  },
});

const open = (page: number): Topics => ({ [NERO_INDEX]: [page], [HOME_MODE]: [0] });

// Live data moves menus by under 0.5% and data pages by up to ~9%; distinct screens differ by 6%+
const MENU = 0.01;
const LIVE_DATA = 0.12;

// Each case injects its steps over calypso's live data, then compares the screen
const cases: Record<string, { steps: Topics[]; tolerance?: number }> = {
  home: { steps: [{ [NERO_INDEX]: [PAGE.PERFORMANCE], [HOME_MODE]: [1] }] },
  off: { steps: [open(PAGE.OFF)], tolerance: LIVE_DATA },
  "pit-drive": { steps: [open(PAGE.PIT_DRIVE)], tolerance: LIVE_DATA },
  reverse: { steps: [open(PAGE.REVERSE)], tolerance: LIVE_DATA },
  performance: { steps: [open(PAGE.PERFORMANCE)], tolerance: LIVE_DATA },
  endurance: { steps: [open(PAGE.ENDURANCE)], tolerance: LIVE_DATA },
  "back-home": { steps: [open(PAGE.PERFORMANCE), { [HOME_MODE]: [1] }] },
  // Firmware sends a bitmask; 12 is shutdown open and brakes not engaged
  "vcu-rejected": {
    steps: [{ [NERO_INDEX]: [PAGE.PERFORMANCE], [HOME_MODE]: [1], "VCU/CarState/state_rejection_error": [12] }],
  },
  "critical-fault": {
    steps: [{ ...open(PAGE.PERFORMANCE), "BMS/Faults/Critical/Cell_Voltage_High": [1] }],
    tolerance: LIVE_DATA,
  },
  games: { steps: [open(PAGE.GAMES)] },
  "themes-next": { steps: [open(PAGE.THEMES), { [BUTTON_ID]: [BUTTON.RIGHT] }] },
  // One Down moves exactly one entry, to SNAKE
  "games-down": { steps: [open(PAGE.GAMES), { [BUTTON_ID]: [BUTTON.DOWN_REGEN] }] },
  // From the first entry, Up and Left wrap to the last, DOOM
  "games-up": { steps: [open(PAGE.GAMES), { [BUTTON_ID]: [BUTTON.UP_REGEN] }] },
  "games-left": { steps: [open(PAGE.GAMES), { [BUTTON_ID]: [BUTTON.LEFT] }] },
  snake: { steps: [open(PAGE.GAMES), { [BUTTON_ID]: [BUTTON.DOWN_REGEN] }, { [BUTTON_ID]: [BUTTON.ENTER] }] },
  // Leaving a game and reopening GAMES shows the list, not the game
  "game-cleared": { steps: [open(PAGE.GAMES), { [BUTTON_ID]: [BUTTON.ENTER] }, { [HOME_MODE]: [1] }, open(PAGE.GAMES)] },
  "themes-light": { steps: [open(PAGE.THEMES), { [BUTTON_ID]: [BUTTON.ENTER] }, { [HOME_MODE]: [1] }] },
  // EXIT has no page, so the menu stays up
  exit: { steps: [open(PAGE.EXIT)] },
  "unknown-page": { steps: [open(PAGE.UNKNOWN)] },
};

for (const [name, { steps, tolerance = MENU }] of Object.entries(cases)) {
  test(name, async ({ page, mock }) => {
    for (const step of steps) await mock.inject(step);
    await mock.freeze();
    await expect(page).toHaveScreenshot(`${name}.png`, { maxDiffPixelRatio: tolerance });
  });
}

// The state label falls back to NO VCU once car state stops
test("vcu-lost", async ({ page, mock }) => {
  await mock.inject({ [NERO_INDEX]: [PAGE.PERFORMANCE], [HOME_MODE]: [1] });
  await mock.freeze();
  await expect(page).toHaveScreenshot("home.png", { maxDiffPixelRatio: MENU });
  mock.vcuLost();
  await page.waitForTimeout(1000);
  await mock.freeze();
  await expect(page).toHaveScreenshot("vcu-lost.png", { maxDiffPixelRatio: MENU });
});

// These games animate, so check the game replaces the list and keeps moving
for (const [name, downs] of Object.entries({ "flappy-bird": 0, "2048": 2 })) {
  test(name, async ({ page, mock }) => {
    await mock.inject(open(PAGE.GAMES));
    for (let i = 0; i < downs; i++) await mock.inject({ [BUTTON_ID]: [BUTTON.DOWN_REGEN] });
    const list = await page.screenshot();
    await mock.inject({ [BUTTON_ID]: [BUTTON.ENTER] });
    await page.waitForTimeout(1000);
    const game = await page.screenshot();
    expect(game.equals(list)).toBe(false);
    // Flappy Bird waits for Enter, then runs its game loop
    if (name === "flappy-bird") {
      await mock.inject({ [BUTTON_ID]: [BUTTON.ENTER] });
      const start = await page.screenshot();
      await page.waitForTimeout(200);
      expect((await page.screenshot()).equals(start)).toBe(false);
    }
  });
}
