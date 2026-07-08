# Nero-2.0

The Cars Dashboard Written in C++ With Qt Framework

### Onboarding

> For initial installation, visit here: https://nerdocs.atlassian.net/wiki/x/CIBORQ

#### Opening the Project

Clone This repository to a directory of your choosing

Select the cmakelists.txt file inside the NERO Development to open in qt creator

It should generate the project

Select the kit for your respective desktop and press build and run

## Development with Mock Telemetry

### Quick Start

1. Start mock telemetry (Docker required)

```bash
docker compose -f compose.nero-dev.yml up -d
```

2. Run NERO in Qt Creator

3. Stop when mock data when done

```bash
docker compose -f compose.nero-dev.yml down
```

### Troubleshooting

Make sure you don't have any env vars set if running locally.

```bash
# View logs
docker compose -f compose.nero-dev.yml logs -f

# Check status
docker ps | grep nero
```

### Formatting

We use clang-format and QML format for formatting our files

#### Mac

brew install clang-format

#### Linux

sudo apt-get install clang-format

#### Windows

Visit the LLVM Download page: https://releases.llvm.org/download.html
Choose the appropriate version for Windows. Look for the "Pre-Built Binaries" section.
Download the Windows installer (MSVC) for the desired LLVM version.

Follow the installation instructions. You can usually go with the default settings, but make sure to select "clang-format" as one of the components to install.

Right-click on "This PC" (or "Computer") on your desktop or in the File Explorer.
Choose "Properties" from the context menu.
Click on "Advanced system settings" on the left sidebar.
In the System Properties window, click the "Environment Variables" button.
Under "System variables," find and select the "Path" variable, then click the "Edit" button.
Click the "New" button and add the path to the bin directory of your LLVM installation (e.g., C:\Program Files\LLVM\bin).
Click OK to close all the windows.

Type clang-format --version and press Enter. You should see the version information for clang-format if the installation was successful.
### Running Formats
#### Mac/Linux Format C++ files
```bash
git ls-files "*.cpp" "*.h" ":!deps/*" | xargs clang-format -i
```
#### Mac/Linux Format QML files
```bash
git ls-files "*.qml" ":!deps/*" | xargs ~/Qt/6.8.3/macos/bin/qmlformat -i
```
#### Windows Format C++ files
```bash
for /f %f in ('git ls-files "*.cpp" "*.h" ":!deps/*"') do clang-format -i "%f"
```
#### Windows Format QML files
```bash
for /f %f in ('git ls-files "*.qml" ":!deps/*"') do C:\Qt\6.8.3\mingw_64\bin\qmlformat.exe -i "%f"
```

## Button Layout

MQTT topic `Wheel/Buttons/button_id` carries the 0-indexed button ordinal,
matching the VCU `button_t` enum in `Cerberus-2.0/Core/Inc/u_buttons.h`.

| 0 | ESC                     |
| 1 | LEFT                    |
| 2 | LAUNCH_CONTROL_TOGGLE   |
| 3 | UP_REGEN                |
| 4 | DOWN_REGEN              |
| 5 | ENTER                   |
| 6 | RIGHT                   |
| 7 | TRACTION_CONTROL_TOGGLE |
| 8 | UP_TORQUE               |
| 9 | DOWN_TORQUE             |

Confluence reference (silkscreen is 1-indexed; wire = silkscreen − 1):
https://nerdocs.atlassian.net/wiki/spaces/NER/pages/1526988828/Button+IO+25

### Testing out Enviornment Variables (Locally)

Go into Projects, go into Run, go into Enviornment, and add variables named `ClIENT1_PORT` and `CLIENT2_PORT`.

Then, change the option from Build Enviornment to System Enviornment.

Finally, save and run it.

### Headless Screenshots (Dev)

Capture any page as a PNG without a display. Both modes do nothing unless their env vars are set.

Commands are bash. On Windows use Git Bash or the Qt Creator run config.

Capture a single page and exit

```bash
QT_QPA_PLATFORM=offscreen NERO_SCREENSHOT=PERFORMANCE NERO_SCREENSHOT_OUT=perf.png ./NEROApp
```

Keep the app running and capture on demand

```bash
# Enable watch mode, trigger file defaults to /tmp/nero-shot
NERO_SCREENSHOT_WATCH=1 ./NEROApp &

# Or point it at a specific trigger file
NERO_SCREENSHOT_WATCH=1 NERO_SCREENSHOT_WATCH_PATH=/tmp/nero-shot ./NEROApp &

# Saves performance.png next to the trigger file
echo PERFORMANCE > /tmp/nero-shot

# An optional trailing path (one with a slash/backslash, or a file extension
# like .png) sets the output; multi-word labels work because only that trailing
# path is split off
echo "ENDURANCE /tmp/out.png" > /tmp/nero-shot
echo "PIT - DRIVE /tmp/pit.png" > /tmp/nero-shot

# Each capture reports completion: a `NERO_SHOT_DONE ok "<path>"` line in the
# app's log (stderr), plus a `<trigger>.done` sentinel file for callers not
# reading the log
```

Page labels match the top-level menu and are case-insensitive. Use HOME to capture the menu screen. The render settle delay before each grab defaults to 500 ms; override it with `NERO_SCREENSHOT_DELAY_MS`.

For scripting, `.claude/skills/nero-shot/scripts/nero-shot.sh PAGE [OUT]` drives a running watch-mode app and blocks until the capture lands, printing the saved path — it waits on the sentinel, so there is no guessing at timing:

```bash
QT_QPA_PLATFORM=offscreen NERO_SCREENSHOT_WATCH=1 ./NEROApp &
.claude/skills/nero-shot/scripts/nero-shot.sh PERFORMANCE /tmp/perf.png   # prints /tmp/perf.png
```

Agents can invoke the `/nero-shot` skill, which launches the app if needed and wraps this.

### Testing Out Enviornment Variables (On the Car)

First, ssh into godzilla: `ssh godzilla2@192.168.100.57 password <LINUX SERVER PASSWORD>`

Next, update the NERO code: `./update_bin.sh nero <ODY_TPU_ROOT_PASSWORD>`

After it's updated, exit out and ssh into the car: `ssh root@192.168.100.12, use <ODY_TPU_ROOT_PASSWORD>`

Then, nano into the ENV variables: `nano /etc/init.d/S99nero2`

Then, go to the ENV variables and change `CLIENT1_PORT`, `CLIENT2_PORT`, and `HOST`.
(Correct: `CLIENT1_PORT=1883, CLIENT2_PORT=1882, HOST="92.168.100.12"`)

After, exit out and save all the changed variables.

Then, exit out of the car. In `~/Projects/Odysseus`, run `./update_bin.sh nero <ODY_TPU_ROOT_PASSWORD>`.

If correct port and host variables, buttons should work. Else, no buttons should work.
