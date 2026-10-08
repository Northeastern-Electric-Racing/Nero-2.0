#!/bin/bash
set -e
cd "$(dirname "$0")/.."

# Emscripten must match the version the Qt 6.8.3 WASM kit was built with
EMSDK_DIR="$HOME/emsdk"
[ -d "$EMSDK_DIR" ] || git clone --depth 1 --branch 3.1.56 https://github.com/emscripten-core/emsdk.git "$EMSDK_DIR"
"$EMSDK_DIR/emsdk" install 3.1.56 || (git -C "$EMSDK_DIR" pull && "$EMSDK_DIR/emsdk" install 3.1.56)
"$EMSDK_DIR/emsdk" activate 3.1.56 > /dev/null
EMSDK_QUIET=1 source "$EMSDK_DIR/emsdk_env.sh"

"$HOME/Qt/6.8.3/wasm_singlethread/bin/qt-cmake" -S NERODevelopment -B NERODevelopment/build-wasm -GNinja \
    -DCMAKE_BUILD_TYPE=Release -DEMSDK_DIR="$EMSDK_DIR"
cmake --build NERODevelopment/build-wasm
