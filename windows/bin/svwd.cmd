@echo off
setlocal EnableDelayedExpansion

set /a wdmgr_argc=0
for %%p in (%*) do set /a wdmgr_argc+=1

if %wdmgr_argc% gtr 1 (
  >&2 echo.Incorrect Usage
  echo.Usage: %0 [name]
  echo.  Save the current working directory as ^<name^> ^(default 'unnamed'^).
  exit /b 1
)

if %wdmgr_argc% gtr 0 (
  set "wdmgr_name=%~1"
) else (
  set wdmgr_name=unnamed
)

set "wdmgr_dir=%USERPROFILE%\_wdmgr"

if not exist "%wdmgr_dir%" (
  md "%wdmgr_dir%" && cd > "%wdmgr_dir%\%wdmgr_name%"
) else (
  cd > "%wdmgr_dir%\%wdmgr_name%"
)
