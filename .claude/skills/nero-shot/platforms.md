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

On Windows PowerShell, set the env var first (`$env:QT_QPA_PLATFORM='offscreen'`)
and run the `.exe`. The `scripts/nero-shot.sh` wrapper assumes a POSIX shell
(macOS/Linux, or Git Bash / WSL on Windows).
