---
name: nero-shot
description: Screenshot a NERO screen headlessly to verify UI changes. Use after editing QML or a controller that affects a screen to visually confirm the page renders as intended. Two modes — a single shot (spin up a headless instance, grab one page, exit) or a live capture against an already-running watch-mode app. Triggers: "screenshot", "capture", "show me", "verify/check the UI" of a NERO page.
allowed-tools: Bash, Read
---

Use this to **verify UI changes**: after changing QML or a controller that
affects a screen, capture the affected page(s) and `Read` the PNG to confirm it
looks right. Pick the mode that fits — a one-off **single shot**, or a **live
capture** of an app that's already running.

Page labels are case-insensitive: `HOME`, `OFF`, `PERFORMANCE`, `ENDURANCE`,
`PIT - DRIVE`, `PIT - KEVIN`. `$ARGUMENTS` = the page(s) to capture (default `HOME`).

## Setup (both modes)

```bash
NERO=/Users/wyattbracy/Desktop/ner/app_software/NERO/Nero-2.0
BIN="$NERO/NERODevelopment/build/macos_development-Debug/NEROApp"
```

If you changed code, rebuild first: `/build-run build`. No broker is needed —
telemetry reads zero, which is fine for checking layout/rendering.

## Mode A — single shot (one-off, self-contained)

Best for verifying one page after a change, or in CI. Spins up a headless
instance, grabs the page, and exits on its own — run it in the foreground:

```bash
QT_QPA_PLATFORM=offscreen NERO_SCREENSHOT="PERFORMANCE" \
  NERO_SCREENSHOT_OUT=/tmp/performance.png "$BIN"
```

Then `Read /tmp/performance.png`. Multi-word labels work here as-is
(`NERO_SCREENSHOT="PIT - DRIVE"`). For several pages, prefer Mode B — it avoids
the ~2 s startup on every shot.

## Mode B — live capture of a running app (on-demand, repeatable)

Best when you want several shots, or to snapshot the current state without
restarting. Requires the app running in **watch mode** (an app started normally,
e.g. plain `/build-run`, cannot be captured this way — it must have been launched
with `NERO_SCREENSHOT_WATCH=1`).

```bash
export NERO_SHOT_TRIGGER=/tmp/nero-shot
LOG=/tmp/nero-shot.log
```

1. **Ensure a watch-mode app is up.** If `pgrep -x NEROApp` finds nothing, launch
   one (offscreen) with `run_in_background: true`:
   ```bash
   QT_QPA_PLATFORM=offscreen NERO_SCREENSHOT_WATCH=1 \
     NERO_SCREENSHOT_WATCH_PATH="$NERO_SHOT_TRIGGER" "$BIN" >"$LOG" 2>&1
   ```
   Wait for it to arm (run this until-loop with `run_in_background: true`, not a
   bare foreground `sleep` — it exits on its own):
   ```bash
   until grep -q 'NERO_SCREENSHOT_WATCH: watching' "$LOG"; do sleep 0.2; done
   ```

2. **Capture each page** with the wrapper (also `run_in_background: true`, since
   it blocks on the completion sentinel). It prints the saved path on stdout:
   ```bash
   "$NERO/scripts/nero-shot.sh" "PERFORMANCE" /tmp/performance.png
   ```
   Give `PAGE` alone to save to `<trigger dir>/<page>.png`, or `PAGE OUT` for an
   explicit path. Quote multi-word labels: `nero-shot.sh "PIT - DRIVE"`. Exits
   non-zero (no hang) on an unknown page or timeout.

3. Leave the app running for reuse; stop it with `pkill -x NEROApp`.

## Finish

`Read` each PNG to confirm the change landed. Report one line per shot:
`PERFORMANCE -> /tmp/performance.png`.

## Notes

- Live captures are ~0.7–1.5 s each (macOS `QFileSystemWatcher` lag + render
  settle). The wrapper waits on the `<trigger>.done` sentinel — never add a fixed
  sleep of your own.
- Tune the settle delay per launch with `NERO_SCREENSHOT_DELAY_MS` (default 500).
- Fire one live capture at a time; the wrapper enforces the write→wait handshake.
