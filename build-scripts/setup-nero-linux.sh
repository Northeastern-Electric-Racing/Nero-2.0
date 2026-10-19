#!/bin/bash
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR/.."

QTDIR="$HOME/Qt/6.8.3/gcc_64"

# Install system dependencies. Zenoh needs no toolchain of its own: CMake
# downloads the zenoh-c release for this machine the first time it configures.
sudo apt update
sudo apt install -y build-essential cmake ninja-build protobuf-compiler

# Add Qt to PATH permanently
SHELL_RC="$HOME/.bashrc"
[ -f "$HOME/.zshrc" ] && SHELL_RC="$HOME/.zshrc"
QT_PATH_LINE="export PATH=\"$QTDIR/bin:\$PATH\""
grep -qF "$QT_PATH_LINE" "$SHELL_RC" 2>/dev/null || echo "$QT_PATH_LINE" >> "$SHELL_RC"

# Build NERO
"$SCRIPT_DIR/compile-qt-linux.sh"

# Dev-only WASM build for Playwright
if [ -d "$HOME/Qt/6.8.3/wasm_singlethread" ] && command -v npm &>/dev/null; then
    npm ci --prefix playwright
    "$SCRIPT_DIR/compile-qt-wasm.sh"
else
    echo "Skipping the Playwright WASM build: needs Qt 6.8.3 WebAssembly (single-threaded) and Node 20+"
fi

echo "Setup complete. Restart Qt Creator to pick up the new PATH."
