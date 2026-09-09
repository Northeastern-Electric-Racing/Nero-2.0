# nero-shot — per-platform paths

The built NEROApp binary lives under `NERODevelopment/build/<kit>/`, where
`<kit>` is your Qt Creator kit's directory name — so the exact path depends on
your OS and kit. The Setup snippet in [SKILL.md](SKILL.md) auto-discovers it;
use the table below to set `BIN` by hand when you have built several kits, or
just to know where your build lands.

| Platform | Binary path (from the repo root) |
| --- | --- |
| macOS | `NERODevelopment/build/macos_development-Debug/NEROApp` |
| Linux | `NERODevelopment/build/<kit>/NEROApp` |
| Windows | `NERODevelopment\build\<kit>\NEROApp.exe` |

`<kit>` is the kit directory Qt Creator created — macOS's is
`macos_development-Debug`; on Linux/Windows it is whatever your kit is named
(e.g. `Desktop-Debug`). If unsure, look inside `NERODevelopment/build/`.

## Building

The standard flow is to open `NERODevelopment/CMakeLists.txt` in Qt Creator and
build the NEROApp target (see Onboarding in the repo readme). To build headless
from a terminal, point CMake at your kit's build directory:

```bash
cmake --build NERODevelopment/build/<kit> --target NEROApp
```

## Headless capture

Capture uses Qt's offscreen platform plugin. On macOS/Linux, set it inline:

```bash
QT_QPA_PLATFORM=offscreen NERO_SCREENSHOT_WATCH=1 "$BIN"
```

**Blank PNGs on headless Linux?** The offscreen plugin still needs a way to
render the Qt Quick scene; on a box with no GPU/EGL the grab can come back empty.
The tool detects an empty grab and fails loudly (a `NERO_SCREENSHOT: grab is
blank` warning + a `fail` sentinel) instead of saving a blank image. If you hit
it, force the software renderer:

```bash
QT_QUICK_BACKEND=software QT_QPA_PLATFORM=offscreen NERO_SCREENSHOT_WATCH=1 "$BIN"
```

The check treats a grab whose pixels are all one color as a failed render. That
holds for every NERO page today — each draws the header plus content — but if a
screen is ever legitimately one flat color, set `NERO_SCREENSHOT_ALLOW_UNIFORM=1`
to skip it. A truly empty grab still fails either way.

**Windows:** WSL is the clean path — the `scripts/nero-shot.sh` wrapper needs a
POSIX shell, and under WSL the shell and a WSL build share one pathspace, so the
paths line up. A native-Windows build has no `/tmp`, so set
`NERO_SCREENSHOT_WATCH_PATH` (and `NERO_SHOT_TRIGGER` for the wrapper) to a real
path; in PowerShell set env vars with `$env:VAR='...'`. Mixing a Git Bash shell
with a native-Windows build is not supported — their path conventions differ.
