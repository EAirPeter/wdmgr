@echo off

set /a argc=0
for %%p in (%*) do set /a argc+=1

if %argc% neq 0 (
  >&2 echo.Incorrect Usage
  echo.Usage: %0
  echo.  Set environment variables according to all saved directories.
  exit /b 1
)

set "wdmgr_dir=%USERPROFILE%\_wdmgr"

for %%p in (%wdmgr_dir%\*) do (
  set /p "%%~np=" < %%p
)
