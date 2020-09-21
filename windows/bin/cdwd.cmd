@echo off
setlocal EnableDelayedExpansion

set /a argc=0
for %%p in (%*) do set /a argc+=1

if %argc% gtr 1 (
  >&2 echo.Incorrect Usage
  echo.Usage: %0 [name]
  echo.  Change working directory to the saved path with name ^<name^> ^(default 'unnamed'^).
  exit /b 1
)

if %argc% gtr 0 (
  set "name=%~1"
) else (
  set name=unnamed
)

set "wdmgr_dir=%USERPROFILE%\_wdmgr"

if exist "%wdmgr_dir%\%name%" (
  set /p dest= < "%wdmgr_dir%\%name%"
) else (
  echo.No path saved with name '%name%'
  exit /b 1
)

endlocal & cd /d "%dest%"
