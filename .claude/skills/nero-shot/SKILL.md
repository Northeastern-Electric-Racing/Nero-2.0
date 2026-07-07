---
name: nero-shot
description: Capture a NERO screen headlessly and return the PNG path. Launches NEROApp in watch mode if needed, then drives scripts/nero-shot.sh for one synchronous capture per page. Use when asked to screenshot, capture, or visually check a NERO page.
allowed-tools: Bash, Read
---

## Arguments

`$ARGUMENTS` — one or more top-level page labels to capture. Labels are
case-insensitive: `HOME`, `OFF`, `PERFORMANCE`, `ENDURANCE`, `PIT - DRIVE`,
`PIT - KEVIN`. Empty → default to `HOME`.

## Your task

Set these once (a scratch trigger keeps captures out of the repo):

```bash
NERO=/Users/wyattbracy/Desktop/ner/app_software/NERO/Nero-2.0
BIN="$NERO/NERODevelopment/build/macos_development-Debug/NEROApp"
export NERO_SHOT_TRIGGER=/tmp/nero-shot
LOG=/tmp/nero-shot.log
```

1. **Ensure a watch-mode app is running.** If `pgrep -x NEROApp` finds nothing,
   launch one (offscreen so no window is needed) with `run_in_background: true`:
   ```bash
   QT_QPA_PLATFORM=offscreen NERO_SCREENSHOT_WATCH=1 \
     NERO_SCREENSHOT_WATCH_PATH="$NERO_SHOT_TRIGGER" "$BIN" >"$LOG" 2>&1
   ```
   Then wait for the watcher to arm (do not use a bare foreground `sleep`; run
   this until-loop with `run_in_background: true` — it exits on its own):
   ```bash
   until grep -q 'NERO_SCREENSHOT_WATCH: watching' "$LOG"; do sleep 0.2; done
   ```
   If `$BIN` is missing, run `/build-run build` first. No broker is needed —
   telemetry just reads zero, which is fine for a screenshot.

2. **Capture each page** with the wrapper (also via `run_in_background: true`,
   since it blocks on the sentinel). It prints the saved path on stdout:
   ```bash
   "$NERO/scripts/nero-shot.sh" "PERFORMANCE" /tmp/performance.png
   ```
   Give `PAGE` alone to save to `<trigger dir>/<page>.png`, or `PAGE OUT` for an
   explicit path. A multi-word label needs quoting: `nero-shot.sh "PIT - DRIVE"`.
   The script exits non-zero and prints the reason on an unknown page or timeout.

3. **View the result** — `Read` each returned PNG to confirm the expected screen.

4. **Report** one line per shot: `PERFORMANCE -> /tmp/performance.png`. Leave the
   watch-mode app running for reuse; note it can be stopped with `pkill -x NEROApp`.

## Notes

- Captures are ~0.7–1.5 s each (macOS `QFileSystemWatcher` lag + the render
  settle timer). The wrapper waits on the `<trigger>.done` sentinel, so never
  add a fixed sleep of your own.
- Tune the settle delay per launch with `NERO_SCREENSHOT_DELAY_MS` (default 500).
- Fire one capture at a time; the wrapper enforces the write→wait handshake.
