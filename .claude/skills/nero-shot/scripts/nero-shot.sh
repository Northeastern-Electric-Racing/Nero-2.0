#!/usr/bin/env bash
# nero-shot.sh PAGE [OUT] — take one screenshot from an already-running,
# watch-mode NEROApp and block until it lands, then print the saved PNG path.
#
# Turns the fire-and-forget trigger file into a synchronous call: it clears the
# sentinel, writes the request, and waits for NEROApp to drop "<trigger>.done".
#
# The app must already be running with:
#   QT_QPA_PLATFORM=offscreen NERO_SCREENSHOT_WATCH=1 \
#     NERO_SCREENSHOT_WATCH_PATH="$NERO_SHOT_TRIGGER" ./NEROApp
# (see the /nero-shot skill, which launches it for you). PAGE is a top-level
# label — HOME, OFF, PERFORMANCE, ENDURANCE, "PIT - DRIVE", "PIT - KEVIN"
# (case-insensitive). OUT is optional; default is <trigger dir>/<page>.png.
#
# Env:
#   NERO_SHOT_TRIGGER  trigger file the app watches (default /tmp/nero-shot)
#   NERO_SHOT_TIMEOUT  whole seconds to wait for a capture (default 15)
set -euo pipefail

if [ "$#" -lt 1 ]; then
  echo "usage: $(basename "$0") PAGE [OUT]" >&2
  exit 2
fi

page=$1
out=${2:-}
# Absolutize a relative OUT against the caller's cwd — NEROApp resolves it from
# its own working directory otherwise, so the saved (and printed) path would not
# match where the caller expects it.
if [ -n "$out" ] && [ "${out#/}" = "$out" ]; then
  out="$PWD/$out"
fi
trigger=${NERO_SHOT_TRIGGER:-/tmp/nero-shot}
timeout=${NERO_SHOT_TIMEOUT:-15}
done_file="$trigger.done"

if [ ! -e "$trigger" ]; then
  echo "nero-shot: trigger '$trigger' not found — is NEROApp running in watch mode?" >&2
  exit 3
fi

# Fresh handshake: drop any stale sentinel, then fire the request.
rm -f "$done_file"
printf '%s %s\n' "$page" "$out" >"$trigger"

# Poll for the sentinel NEROApp writes once the grab (or failure) completes.
# Wait for it to be non-empty (-s): the app creates the file on open and then
# writes the payload, so -f alone could catch it in the empty instant between.
tries=$((timeout * 10))
i=0
while [ ! -s "$done_file" ]; do
  i=$((i + 1))
  if [ "$i" -ge "$tries" ]; then
    echo "nero-shot: timed out after ${timeout}s waiting for $done_file" >&2
    exit 1
  fi
  sleep 0.1
done

# Sentinel format: <status> "<path>"  (status is ok|fail)
result=$(cat "$done_file")
status=${result%% *}
path=${result#* }
path=${path#\"}
path=${path%\"}

if [ "$status" != "ok" ]; then
  echo "nero-shot: capture failed for '$page' ($result)" >&2
  exit 1
fi

echo "$path"
