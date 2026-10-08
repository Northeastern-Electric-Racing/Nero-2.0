import { test as base, expect } from "@playwright/test";
import { connectMock } from "../mock.mjs";

const NERO_INDEX = "VCU/CarState/nero_index";
const HOME_MODE = "VCU/CarState/home_mode";
const BUTTON_ID = "Wheel/Buttons/button_id";
// Top-level menu slots, and wheel button ids from readme "Button Layout"
const PAGE = { OFF: 0, PIT_DRIVE: 1, PERFORMANCE: 3, ENDURANCE: 4, GAMES: 5, THEMES: 6 };
const BUTTON = { ENTER: 5, RIGHT: 6 };

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
  performance: { steps: [open(PAGE.PERFORMANCE)], tolerance: LIVE_DATA },
  endurance: { steps: [open(PAGE.ENDURANCE)], tolerance: LIVE_DATA },
  "back-home": { steps: [open(PAGE.PERFORMANCE), { [HOME_MODE]: [1] }] },
  "critical-fault": {
    steps: [{ ...open(PAGE.PERFORMANCE), "BMS/Faults/Critical/Cell_Voltage_High": [1] }],
    tolerance: LIVE_DATA,
  },
  "themes-next": { steps: [open(PAGE.THEMES), { [BUTTON_ID]: [BUTTON.RIGHT] }] },
};

for (const [name, { steps, tolerance = MENU }] of Object.entries(cases)) {
  test(name, async ({ page, mock }) => {
    for (const step of steps) await mock.inject(step);
    await mock.freeze();
    await expect(page).toHaveScreenshot(`${name}.png`, { maxDiffPixelRatio: tolerance });
  });
}

// Games animate on their own, so check the game opens and the app keeps responding
test("flappy-bird", async ({ page, mock }) => {
  await mock.inject(open(PAGE.GAMES));
  await mock.inject({ [BUTTON_ID]: [BUTTON.ENTER] });
  await page.waitForTimeout(1000);
  await mock.inject({ "BMS/Status/Balancing": [0] });
});
