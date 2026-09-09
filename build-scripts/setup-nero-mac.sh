#!/bin/bash
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR/.."

QTDIR="$HOME/Qt/6.8.3/macos"

# Install protobuf via Homebrew if not present
if ! command -v protoc &>/dev/null; then
    brew install protobuf
fi

# Apply AGL fix required on macOS 26 and above
MACOS_MAJOR=$(sw_vers -productVersion | cut -d. -f1)
AGL_FIX_FILE="$QTDIR/lib/cmake/Qt6/FindWrapOpenGL.cmake"
if [ "$MACOS_MAJOR" -ge 26 ] && [ -f "$AGL_FIX_FILE" ]; then
    if grep -q 'target_link_libraries(WrapOpenGL::WrapOpenGL INTERFACE ${__opengl_agl_fw_path})' "$AGL_FIX_FILE"; then
        sed -i '' 's/target_link_libraries(WrapOpenGL::WrapOpenGL INTERFACE \${__opengl_agl_fw_path})/# target_link_libraries(WrapOpenGL::WrapOpenGL INTERFACE ${__opengl_agl_fw_path})/' "$AGL_FIX_FILE"
    fi
fi

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
SHELL_RC="$HOME/.zshrc"
[ -n "$BASH_VERSION" ] && SHELL_RC="$HOME/.bash_profile"
QT_PATH_LINE="export PATH=\"$QTDIR/bin:\$PATH\""
grep -qF "$QT_PATH_LINE" "$SHELL_RC" 2>/dev/null || echo "$QT_PATH_LINE" >> "$SHELL_RC"

# Build NERO
"$SCRIPT_DIR/compile-qt-mac.sh"

echo "Setup complete. Restart Qt Creator to pick up the new PATH."
