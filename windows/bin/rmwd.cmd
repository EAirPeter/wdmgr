@echo off
setlocal EnableDelayedExpansion

set /a wdmgr_argc=0
for %%p in (%*) do set /a wdmgr_argc+=1

if %wdmgr_argc% gtr 1 (
  >&2 echo.Incorrect Usage
  echo.Usage: %0 [name]
  echo.  Remove the saved path ^<name^> ^(default 'unnamed'^).
  exit /b 1
)

set "wdmgr_dir=%USERPROFILE%\.wdmgr"

if %wdmgr_argc% gtr 0 (
  set "name=%~1"
) else (
  set name=unnamed
)

if exist "%wdmgr_dir%\%name%" (
  del "%wdmgr_dir%\%name%"
) else (
  >&2 echo.No path saved with name '%name%'
)
