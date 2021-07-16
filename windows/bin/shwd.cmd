@echo off
setlocal EnableDelayedExpansion

set /a argc=0
for %%p in (%*) do set /a argc+=1

if %argc% gtr 1 (
  >&2 echo.Incorrect Usage
  echo.Usage: %0 [name]
  echo.  Show the path with ^<name^> ^(default 'unnamed'^).
  exit /b 1
)

set "wdmgr_dir=%USERPROFILE%\.wdmgr"

if %argc% gtr 0 (
  set "name=%~1"
) else (
  set name=unnamed
)

if exist "%wdmgr_dir%\%name%" (
  type "%wdmgr_dir%\%name%"
) else (
  >&2 echo.No path saved with name '%name%'
  exit /b 1
)
