@echo off
REM ==============================================================================
REM CMake Build System Validation Script (Windows)
REM ==============================================================================
REM This script validates the CMake configuration without building
REM ==============================================================================

setlocal enabledelayedexpansion

set SCRIPT_DIR=%~dp0
set BUILD_DIR=%SCRIPT_DIR%build_test

echo ==============================================================================
echo CMake Build System Validation for Generals Zero Hour
echo ==============================================================================
echo.

REM Check CMake installation
echo Checking CMake installation...
where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [X] CMake not found. Please install CMake 3.15 or later.
    echo     Download from: https://cmake.org/download/
    pause
    exit /b 1
)

for /f "tokens=3" %%i in ('cmake --version ^| findstr /R "^cmake"') do set CMAKE_VERSION=%%i
echo [+] CMake version: %CMAKE_VERSION%
echo.

echo Checking file structure...

REM Check for required files
set FILES=CMakeLists.txt cmake\config.h.in cmake\GeneralsHelpers.cmake CMAKE_BUILD_GUIDE.md CMAKE_QUICK_REFERENCE.md

for %%f in (%FILES%) do (
    if exist "%SCRIPT_DIR%%%f" (
        echo [+] %%f
    ) else (
        echo [X] %%f - MISSING
    )
)

echo.
echo Checking directory structure...

REM Check for required directories
set DIRS=Libraries\Source GameEngine GameEngineDevice Main cmake

for %%d in (%DIRS%) do (
    if exist "%SCRIPT_DIR%%%d\" (
        echo [+] %%d\
    ) else (
        echo [X] %%d\ - MISSING
    )
)

echo.
echo Creating test build directory...

REM Clean up old test build
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
)

mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%"

echo [+] Build directory created: %BUILD_DIR%
echo.

echo Testing CMake configuration...
echo.

REM Try to configure (but not build)
cmake .. -G "Visual Studio 17 2022" -A Win32 > cmake_output.log 2>&1
if %ERRORLEVEL% equ 0 (
    echo [+] CMake configuration successful
    echo.
    echo Configuration summary:
    findstr /C:"-- " cmake_output.log | findstr /R "Building Build.Type C++ Compiler Platform" 2>nul
) else (
    echo [X] CMake configuration failed
    echo.
    echo Error details:
    powershell -Command "Get-Content cmake_output.log -Tail 20"
    echo.
    echo Full log: %BUILD_DIR%\cmake_output.log
    pause
    exit /b 1
)

echo.
echo Checking generated files...

if exist "include\config.h" (
    echo [+] config.h generated
) else (
    echo [X] config.h not generated
)

echo.
echo ==============================================================================
echo Validation Summary
echo ==============================================================================

findstr /C:"Configuring done" cmake_output.log >nul 2>nul
if %ERRORLEVEL% equ 0 (
    echo [SUCCESS] - CMake configuration is valid!
    echo.
    echo Next steps:
    echo   1. Review the configuration:
    echo      cd %BUILD_DIR%
    echo      type cmake_output.log
    echo.
    echo   2. Build the project:
    echo      cmake --build . --config Release -j 8
    echo.
    echo   3. Clean up test build:
    echo      cd %SCRIPT_DIR%
    echo      rmdir /s /q %BUILD_DIR%
    echo.
) else (
    echo [PARTIAL] - Configuration completed with warnings
    echo Review the log file: %BUILD_DIR%\cmake_output.log
)

echo.
echo For detailed build instructions, see:
echo   - CMAKE_BUILD_GUIDE.md (comprehensive guide)
echo   - CMAKE_QUICK_REFERENCE.md (quick commands)
echo ==============================================================================
echo.
echo Test build directory preserved for inspection: %BUILD_DIR%
echo Remove with: rmdir /s /q "%BUILD_DIR%"
echo.

pause
