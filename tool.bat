@echo off
@REM chcp 65001

setlocal enabledelayedexpansion

REM 获取当前目录的完整路径,提取目录名称
for %%I in ("%cd%\.") do set "ProjectName=%%~nI"

set TargrtFile=%cd%\build\%ProjectName%.elf
set   MakeFile=%cd%\build\Makefile
set  CMakeFile=%cd%\CMakeLists.txt

echo %Date% %Time%
echo cd          : %cd%
echo ProjectName : %ProjectName%
echo TargrtFile  : %TargrtFile%
echo MakeFile    : %MakeFile%
echo CMakeFile   : %CMakeFile%

IF not EXIST "%CMakeFile%" (
    echo need CMakeLists.txt file
    exit /b 0
)

IF not EXIST "%MakeFile%" (
    cmake -S . -B build -G "MinGW Makefiles"
)

if /i "%~1"=="download" (
    echo build
    cmake --build build -j12
    echo download
    C:/ENV/EmbeddedToolChain/OpenOCD/bin/openocd.exe -f "./env/stm32f4discovery.cfg" -c "program ./build/%ProjectName%.elf" -c reset -c shutdown
) else if /i "%~1"=="debug" (
    echo debug
) else if /i "%~1"=="build" (
    echo build
    cmake --build build -j12
) else (
    echo unknown arg: %~1
)

endlocal
exit /b 0