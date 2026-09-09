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

REM Fetch the Qt MQTT submodule
git submodule update --init deps/qtmqtt

REM Build and install Qt MQTT into the Qt kit
cd deps\qtmqtt
if exist build\ (
    rd /S /Q build
)
for %%c in (Release Debug) do (
    mkdir build\%%c
    pushd build\%%c
    call "%QTDIR%\bin\qt-cmake.bat" ..\.. -GNinja -DCMAKE_BUILD_TYPE=%%c -DCMAKE_INSTALL_PREFIX=%QTDIR% || exit /b 1
    ninja || exit /b 1
    ninja install || exit /b 1
    popd
)

REM Add Qt tools to PATH permanently
set ADD=%QTDIR%\bin;%MINGW%\bin;%CMAKE_PATH%;%NINJA_PATH%
powershell -NoProfile -Command "$p=[Environment]::GetEnvironmentVariable('Path','User'); foreach($d in '%ADD%'.Split(';')){ if($d -and (($p -split ';') -notcontains $d)){ $p=$p.TrimEnd(';') + ';' + $d } }; [Environment]::SetEnvironmentVariable('Path',$p,'User')"

REM Build NERO
call "%~dp0compile-qt-windows.bat"
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Setup complete. Restart Qt Creator to pick up the new PATH."

REM Return to the caller's directory.
POPD
