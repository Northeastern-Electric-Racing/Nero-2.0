#!/bin/bash
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR/.."

QTDIR="$HOME/Qt/6.8.3/gcc_64"

# Install system dependencies
sudo apt update
sudo apt install -y build-essential cmake ninja-build protobuf-compiler

# Fetch the Qt MQTT submodule
git submodule update --init deps/qtmqtt

# Build and install Qt MQTT into the Qt kit
cd deps/qtmqtt
rm -rf build
for CONFIG in Release Debug; do
    mkdir -p "build/$CONFIG"
    pushd "build/$CONFIG"
    "$QTDIR/bin/qt-cmake" ../.. -GNinja -DCMAKE_BUILD_TYPE="$CONFIG" -DCMAKE_INSTALL_PREFIX="$QTDIR" || exit 1
    ninja || exit 1
    ninja install || exit 1
    popd
done
cd ../..

# Add Qt to PATH permanently
SHELL_RC="$HOME/.bashrc"
[ -f "$HOME/.zshrc" ] && SHELL_RC="$HOME/.zshrc"
QT_PATH_LINE="export PATH=\"$QTDIR/bin:\$PATH\""
grep -qF "$QT_PATH_LINE" "$SHELL_RC" 2>/dev/null || echo "$QT_PATH_LINE" >> "$SHELL_RC"

# Build NERO
"$SCRIPT_DIR/compile-qt-linux.sh"

echo "Setup complete. Restart Qt Creator to pick up the new PATH."
