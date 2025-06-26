@echo off
REM This script compiles the C++ application using MSYS2 MinGW 64-bit.

REM --- Configuration ---
REM Set the path to your MSYS2 installation directory.
REM IMPORTANT: Adjust this path if your MSYS2 is installed elsewhere.
set MSYS2_DIR=C:\msys64

REM Set the name of the source file
set SOURCE_FILE=/main.cpp

REM Set the desired name for your executable (will be placed in the output folder).
set EXECUTABLE_NAME=/output/curl_weather_app.exe

REM --- Convert Windows paths to MSYS2 format ---
REM %CD% expands to the current directory (where this .bat file is).
REM We convert backslashes to forward slashes and add the /c/ prefix for MSYS2.
set MSYS2_CURRENT_DIR_PATH=/%CD:\=/%\

REM Construct the full MSYS2 paths for source and executable.
set MSYS2_SOURCE_PATH=%MSYS2_CURRENT_DIR_PATH%%SOURCE_FILE%
set MSYS2_EXECUTABLE_PATH=%MSYS2_CURRENT_DIR_PATH%%EXECUTABLE_NAME%

REM Construct the full MSYS2 path for the library directory.
set MSYS2_LIB_DIR_PATH=%MSYS2_CURRENT_DIR_PATH%%LIB_DIR%

REM --- Compilation Command ---
REM We use the 'mingw64' environment within MSYS2.
REM -L%MSYS2_LIB_DIR_PATH%: Tells the linker to look for libraries in the 'output' folder.
REM -l%LIB_NAME%: Links against 'libparser.dll.a'.
REM -lcurl: Links against libcurl.
REM -static-libgcc -static-libstdc++: Statically links GCC runtime libraries for portability.
"%MSYS2_DIR%\msys2_shell.cmd" -defterm -no-start -mingw64 -c "g++ %MSYS2_SOURCE_PATH% -o %MSYS2_EXECUTABLE_PATH% -lcurl -static-libgcc -static-libstdc++"

echo.
echo Main Application Compilation attempt finished. Check for errors above.
pause
