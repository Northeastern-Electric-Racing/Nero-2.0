REM Change to the directory with this script
PUSHD .
cd /D "%~dp0.."

REM Add Qt and MinGW to the environment variables.
set QTDIR=C:\Qt\6.8.3\mingw_64
for /d %%d in ("C:\Qt\Tools\mingw*_64") do set MINGW=%%~fd
set CMAKE_PATH=C:\Qt\Tools\CMake_64\bin
set NINJA_PATH=C:\Qt\Tools\Ninja
set PATH=%QTDIR%\bin;%MINGW%\bin;%CMAKE_PATH%;%NINJA_PATH%;%PATH%

echo "PATH=%PATH%"

cd NERODevelopment

REM Cleanup the previous build
if exist build\ (
    rd /S /Q build
)

REM Perform the build
mkdir build
pushd build
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=%QTDIR%
ninja

if %errorlevel% neq 0 exit /b %errorlevel%

echo "Build completed"

popd

REM Return to the caller's directory.
POPD
