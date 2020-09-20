@echo off
for /f %%p in ('wdmgr sh %*') do set WDMGR_DIR=%%p
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
cd /d %WDMGR_DIR%
