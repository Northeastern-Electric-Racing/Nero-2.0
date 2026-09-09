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
#
# Concurrent calls against one app never cross wires — each reply is tagged with
# its requester, so a call only ever returns its own screenshot, and the app
# captures one at a time rather than navigating out from under a pending grab.
# They are not all guaranteed to be served though: callers share one trigger
# file, so near-simultaneous writes can overwrite each other and the losers time
# out (exit 4). Serialize the calls if every capture has to land.
#
# Exit: 0 saved | 1 capture failed | 2 usage | 3 no live watch-mode app
#       | 4 timed out
set -euo pipefail

if [ "$#" -lt 1 ] || [ -z "$1" ]; then
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
pid_file="$trigger.pid"

if [ ! -e "$trigger" ]; then
  echo "nero-shot: trigger '$trigger' not found — is NEROApp running in watch mode?" >&2
  exit 3
fi

# A trigger file outlives the app that created it, so the check above proves
# nothing about liveness. Watch mode drops its pid alongside the trigger; if
# that process is gone, say so now instead of waiting out the whole timeout. No
# pid file means an app that predates it (or a hand-rolled setup) — let it pass.
if [ -s "$pid_file" ]; then
  app_pid=$(cat "$pid_file")
  if ! kill -0 "$app_pid" 2>/dev/null; then
    echo "nero-shot: watch-mode app (pid $app_pid) is gone — relaunch it" >&2
    exit 3
  fi
fi

# Correlate request and reply: every caller shares one sentinel, so the app
# echoes this id in the payload and we ignore any reply that isn't ours — a
# capture from a call that timed out, was killed, or is simply someone else's.
# Random suffix because pid plus second alone can repeat across quick calls.
req_id="$$-$(date +%s)-$RANDOM"

# Fire the request, one field per line so a page label or output path containing
# spaces survives verbatim. The stale sentinel sitting there is left alone: the
# id check below already rejects it, and deleting it would throw away a reply a
# concurrently waiting caller has not read yet.
printf '%s\n%s\n%s\n' "$page" "$out" "$req_id" >"$trigger"

# Poll for the sentinel NEROApp writes once the grab (or failure) completes.
# Require it non-empty (-s) and carrying our id: the app creates the file on
# open and then writes the payload, so -f alone could catch it in the empty
# instant between, and any other id belongs to another request.
tries=$((timeout * 10))
i=0
result=
while :; do
  if [ -s "$done_file" ]; then
    result=$(cat "$done_file")
    if [ "${result##* }" = "$req_id" ]; then
      break
    fi
  fi
  i=$((i + 1))
  if [ "$i" -ge "$tries" ]; then
    echo "nero-shot: timed out after ${timeout}s waiting for $done_file" >&2
    exit 4
  fi
  sleep 0.1
done

# Sentinel format: <status> "<path>" <id>  (status is ok|fail)
if [[ ! $result =~ ^([a-z]+)\ \"(.*)\"\ (.*)$ ]]; then
  echo "nero-shot: unparseable sentinel for '$page': $result" >&2
  exit 1
fi
status=${BASH_REMATCH[1]}
path=${BASH_REMATCH[2]}

if [ "$status" != "ok" ]; then
  echo "nero-shot: capture failed for '$page' ($result)" >&2
  exit 1
fi

echo "$path"
