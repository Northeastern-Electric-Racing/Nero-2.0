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

REM Zenoh needs no toolchain of its own: CMake downloads the zenoh-c release
REM for this machine the first time it configures.

REM Add Qt tools to PATH permanently
set ADD=%QTDIR%\bin;%MINGW%\bin;%CMAKE_PATH%;%NINJA_PATH%
powershell -NoProfile -Command "$p=[Environment]::GetEnvironmentVariable('Path','User'); foreach($d in '%ADD%'.Split(';')){ if($d -and (($p -split ';') -notcontains $d)){ $p=$p.TrimEnd(';') + ';' + $d } }; [Environment]::SetEnvironmentVariable('Path',$p,'User')"

REM Build NERO
call "%~dp0compile-qt-windows.bat"
if %errorlevel% neq 0 exit /b %errorlevel%

REM Dev-only WASM build for Playwright; emsdk needs a real Python on Windows, not the Store alias
set WASM_READY=0
where npm >nul 2>nul && python --version >nul 2>nul && if exist C:\Qt\6.8.3\wasm_singlethread\ set WASM_READY=1
if %WASM_READY%==1 (
    call npm ci --prefix "%~dp0..\playwright" || exit /b 1
    call "%~dp0compile-qt-wasm.bat" || exit /b 1
) else (
    echo "Skipping the Playwright WASM build: needs Qt 6.8.3 WebAssembly (single-threaded), Node 20+, and Python 3"
)

echo "Setup complete. Restart Qt Creator to pick up the new PATH."

REM Return to the caller's directory.
POPD
