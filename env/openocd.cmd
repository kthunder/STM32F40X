set old_dir=%cd%
cd /d %~dp0%

openocd.exe -f "./stm32f4discovery.cfg" ^
-c "program %1" ^
-c reset ^
-c shutdown

cd /d %old_dir%