@echo off
setlocal EnableDelayedExpansion

set /a argc=0
for %%p in (%*) do set /a argc+=1

if %argc% neq 0 (
  >&2 echo.Incorrect Usage
  echo.Usage: %0
  echo.  List all saved directories.
  exit /b 1
)

set "wdmgr_dir=%USERPROFILE%\.wdmgr"

set /a num_stored=0
set /a maxlen=0

for %%p in (%wdmgr_dir%\*) do (
  set /a num_stored+=1
  set "name=%%~np"
  set "names[!num_stored!]=!name!"
  set /p contents[!num_stored!]= < %%p
  call :strlen "!name!" len
  if !len! gtr !maxlen! (
    set /a maxlen=!len!
  )
)

set "spaces=                                "

if %num_stored% gtr 0 (
  for /l %%i in (1,1,%num_stored%) do (
    set "padded=%spaces%!names[%%i]!"
    echo.!padded:~-%maxlen%! !contents[%%i]!
  )
) else (
  echo.^(Empty^)
)

exit /b

:strlen
@echo off
setlocal EnableDelayedExpansion
set "s=#%~1"
set /a len=0
for %%n in (4096 2048 1024 512 256 128 64 32 16 8 4 2 1) do (
  if "!s:~%%n,1!" neq "" (
    set /a len+=%%n
    set "s=!s:~%%n!"
  )
)
endlocal & if "%~2" neq "" (set %~2=%len%) else echo %len%
exit /b
