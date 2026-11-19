@echo off
if exist "%~dp0..\w64devkit\bin\mingw32-make.exe" set "PATH=%~dp0..\w64devkit\bin;%PATH%"
if exist "C:\raylib\w64devkit\bin\mingw32-make.exe" set "PATH=C:\raylib\w64devkit\bin;%PATH%"
mingw32-make run || pause