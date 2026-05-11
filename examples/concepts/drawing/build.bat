@echo off
setlocal enabledelayedexpansion

mkdir build
cd build

cmake -G "Visual Studio 17" -A x64 ..

for /f %%i in ('powershell -command "(Get-WmiObject -Class Win32_Processor).NumberOfLogicalProcessors"') do set THREADS=%%i

cmake --build . --config Release -- /m:%THREADS%
xcopy /E /Y /I bin\Release\* bin\
rmdir /S /Q bin\Release

endlocal
