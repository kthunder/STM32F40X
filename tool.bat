@echo off
setlocal enabledelayedexpansion

REM 获取当前目录的完整路径,提取目录名称
set "fullpath=%cd%"
for %%I in ("%fullpath%\.") do set "current_dir_name=%%~nI" 

if "%~1"=="" (
    echo invoke default command : download
    exit /b 0
)

if /i "%~1"=="download" (
    echo download
    REM 在这里插入命令1的具体内容
    openocd.exe -f "./env/stm32f4discovery.cfg" -c "program ./build/%current_dir_name%.elf" -c reset -c shutdown
) else if /i "%~1"=="debug" (
    echo debug
    REM 在这里插入命令2的具体内容
) else if /i "%~1"=="build" (
    echo build
    REM 在这里插入命令2的具体内容
    cmake -S . -B build -G "MinGW Makefiles"
    cmake --build build -j12
) else (
    echo unknown arg: %~1
)

exit /b 0