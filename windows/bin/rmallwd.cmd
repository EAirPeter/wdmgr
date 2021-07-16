@echo off
setlocal EnableDelayedExpansion

set /a wdmgr_argc=0
for %%p in (%*) do set /a wdmgr_argc+=1

if %wdmgr_argc% neq 0 (
  >&2 echo.Incorrect Usage
  echo.Usage: %0
  echo.  Remove all saved paths.
  exit /b 1
)

set "wdmgr_dir=%USERPROFILE%\.wdmgr"

if exist "%wdmgr_dir%" (
  rd /s /q "%wdmgr_dir%"
)
