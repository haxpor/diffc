@echo off
setlocal

set OUT=diffp4

REM Check if 1st argument to clean the stuff
if "%1" == "clean" (
	call :clean
	EXIT /B %ERRORLEVEL%
)

REM Otherwise call build
call :build
EXIT /B %ERRORLEVEL%

:clean
del /f %OUT%.obj %OUT%.exe
EXIT /B 0

:build
cl.exe /EHsc /std:c++17 /W4 /O2 /Fe:%OUT%.exe /Fo:%OUT%.obj src/main.cpp
EXIT /B 0
