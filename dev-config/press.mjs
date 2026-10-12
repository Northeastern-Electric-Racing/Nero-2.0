// Presses wheel buttons through the dev stack's Zenoh router, and stands in
// for the VCU's page selection.
//
//   node dev-config/press.mjs --page themes right enter   open THEMES, then press
//   node dev-config/press.mjs 6 6 5 --gap 500             by ordinal, 500 ms apart
//   node dev-config/press.mjs --home                      back to the home menu
//   node dev-config/press.mjs                             list buttons and pages
//
// NERO only acts on LEFT/RIGHT/ENTER inside an open category (GAMES, THEMES).
// Choosing the page is the VCU's job: it publishes VCU/CarState/nero_index and
// home_mode, and NERO follows. --page and --home publish those in its place.
//
// Needs `docker compose -f compose.nero-dev.yml up -d` running, with calypso's
// Wheel/Buttons and nero_index/home_mode topics disabled so its random values
// don't land between yours.

const ROUTER_REST = process.env.ZENOH_REST ?? "http://localhost:8000";
const BUTTON_KEY = "Wheel/Buttons/button_id";
const PAGE_KEY = "VCU/CarState/nero_index";
const HOME_KEY = "VCU/CarState/home_mode";

// readme.md "Button Layout", matching the VCU's button_t
const BUTTONS = [
  "esc",
  "left",
  "launch_control_toggle",
  "up_regen",
  "down_regen",
  "enter",
  "right",
  "traction_control_toggle",
  "up_torque",
  "down_torque",
];

// The top-level menu in navigationcontroller.cpp, which nero_index indexes
const PAGES = ["off", "pit_drive", "pit_kevin", "performance", "endurance", "games", "themes", "exit"];

function varint(n) {
  const out = [];
  let v = BigInt(n);
  do {
    let b = Number(v & 0x7fn);
    v >>= 7n;
    if (v) b |= 0x80;
    out.push(b);
  } while (v);
  return out;
}

// serverdata.v2.ServerData { time_us = 3; repeated float values = 4 }
function serverData(value) {
  const values = Buffer.alloc(4);
  values.writeFloatLE(value);
  return Buffer.from([0x18, ...varint(BigInt(Date.now()) * 1000n), 0x22, values.length, ...values]);
}

function lookup(arg, names, kind) {
  const lower = arg.toLowerCase();
  const id = /^\d+$/.test(lower) ? Number(lower) : names.indexOf(lower);
  if (!(id >= 0 && id < names.length)) {
    console.error(`Unknown ${kind} "${arg}". Run with no arguments to list them.`);
    process.exit(1);
  }
  return id;
}

function parseArgs(argv) {
  let gap = 300;
  let page;
  let home = false;
  const presses = [];
  for (let i = 0; i < argv.length; i++) {
    if (argv[i] === "--gap") {
      gap = Number(argv[++i]);
      continue;
    }
    if (argv[i] === "--page") {
      page = lookup(argv[++i] ?? "", PAGES, "page");
      continue;
    }
    if (argv[i] === "--home") {
      home = true;
      continue;
    }
    presses.push(lookup(argv[i], BUTTONS, "button"));
  }
  return { gap, page, home, presses };
}

async function put(key, value) {
  const res = await fetch(`${ROUTER_REST}/${key}`, {
    method: "PUT",
    headers: { "Content-Type": "application/protobuf" },
    body: serverData(value),
  }).catch((err) => {
    console.error(`Could not reach the router's REST plugin at ${ROUTER_REST}: ${err.cause?.code ?? err.message}`);
    process.exit(1);
  });
  if (!res.ok) {
    console.error(`Router refused ${key}: HTTP ${res.status}`);
    process.exit(1);
  }
}

const sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

const { gap, page, home, presses } = parseArgs(process.argv.slice(2));
if (page === undefined && !home && presses.length === 0) {
  console.log("Buttons:");
  BUTTONS.forEach((name, id) => console.log(`  ${id}  ${name}`));
  console.log("Pages (--page):");
  PAGES.forEach((name, id) => console.log(`  ${id}  ${name}`));
  process.exit(0);
}

// NERO reads the two together, so send both every time
if (page !== undefined || home) {
  const index = page ?? 0;
  await put(PAGE_KEY, index);
  await put(HOME_KEY, home ? 1 : 0);
  console.log(home ? "home menu" : `page ${PAGES[index]} (${index})`);
  if (presses.length) await sleep(gap);
}

for (const [i, id] of presses.entries()) {
  if (i > 0) await sleep(gap);
  await put(BUTTON_KEY, id);
  console.log(`pressed ${BUTTONS[id]} (${id})`);
}
