PUSHD .
cd /D "%~dp0.."

for /d %%d in ("C:\Qt\Tools\mingw*_64") do set MINGW=%%~fd
set PATH=%MINGW%\bin;C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\Ninja;%PATH%

REM Emscripten must match the version the Qt 6.8.3 WASM kit was built with
set EMSDK_DIR=%USERPROFILE%\emsdk
if not exist "%EMSDK_DIR%\" git clone --depth 1 --branch 3.1.56 https://github.com/emscripten-core/emsdk.git "%EMSDK_DIR%"
call "%EMSDK_DIR%\emsdk.bat" install 3.1.56 || (git -C "%EMSDK_DIR%" pull && call "%EMSDK_DIR%\emsdk.bat" install 3.1.56) || exit /b 1
call "%EMSDK_DIR%\emsdk.bat" activate 3.1.56 >nul || exit /b 1
set EMSDK_QUIET=1
call "%EMSDK_DIR%\emsdk_env.bat"

call C:\Qt\6.8.3\wasm_singlethread\bin\qt-cmake.bat -S NERODevelopment -B NERODevelopment\build-wasm -GNinja -DCMAKE_BUILD_TYPE=Release "-DEMSDK_DIR=%EMSDK_DIR:\=/%" || exit /b 1
cmake --build NERODevelopment\build-wasm || exit /b 1

POPD
