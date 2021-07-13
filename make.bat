@echo off
setlocal

REM Check if 1st argument to clean the stuff
if "%1" == "clean" (
	call :clean
	EXIT /B %ERRORLEVEL%
)

REM Otherwise call build
call :build
EXIT /B %ERRORLEVEL%

:clean
del /f diffc.obj diffc.exe
EXIT /B 0

:build
cl.exe /EHsc /std:c++17 /W4 /O2 /Fe:diffc.exe /Fo:diffc.obj src/main.cpp
EXIT /B 0
