@echo off
setlocal enabledelayedexpansion

set BUILD_TYPE=Release
if "%1"=="-d" set BUILD_TYPE=Debug

if not exist build (
    mkdir build
)

cd build

cmake -G "Visual Studio 17" -A x64 ..

for /f %%i in ('powershell -command "(Get-WmiObject -Class Win32_Processor).NumberOfLogicalProcessors"') do set THREADS=%%i

cmake --build . --config %BUILD_TYPE% -- /m:%THREADS%

xcopy /E /Y "%BUILD_TYPE%\*" "..\"

endlocal
