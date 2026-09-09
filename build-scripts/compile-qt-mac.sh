#!/bin/bash
set -e
cd "$(dirname "$0")/.."

QTDIR="$HOME/Qt/6.8.3/macos"
export PATH="$QTDIR/bin:$PATH"

cd NERODevelopment
rm -rf build
mkdir build
cd build
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$QTDIR"
ninja

echo "Build completed"
