@echo off
setlocal
if not exist build (
    mkdir build
)

cd build

cmake -G "MinGW Makefiles" ..

for /f %%i in ('powershell -command "(Get-WmiObject -Class Win32_Processor).NumberOfLogicalProcessors"') do set THREADS=%%i

mingw32-make -j%THREADS%

endlocal