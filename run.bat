@echo off
REM This script runs the compiled cURL application with arguments.

REM Pass all arguments received by this batch script (%*) to the executable.
output\curl_weather_app.exe %*