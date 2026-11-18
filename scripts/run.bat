@echo off
setlocal EnableExtensions

set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=Release"

call "%~dp0build.bat" %CONFIG% || exit /b 1

if /I "%CONFIG%"=="Debug" (
    call "%~dp0_debug_env.bat" || exit /b 1
)

set "GAME_EXE=%~dp0..\build\%CONFIG%\SnowParkour.exe"
if not exist "%GAME_EXE%" (
    echo [run] missing %GAME_EXE%
    exit /b 1
)

echo [run] starting %CONFIG%\SnowParkour.exe
pushd "%~dp0.." >nul || exit /b 1
"%GAME_EXE%"
set "CODE=%ERRORLEVEL%"
popd
exit /b %CODE%
