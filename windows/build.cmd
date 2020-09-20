:CheckCl
@where cl 1> nul 2>&1
@if %errorlevel% neq 0 goto CheckClang
@goto UseCl

:CheckClang
@where clang++ 1> nul 2>&1
@if %errorlevel% neq 0 goto CheckGcc
@goto UseClang

:CheckGcc
@where g++ 1> nul 2>&1
@if %errorlevel% neq 0 goto Fatal
@goto UseGcc

:Fatal
@echo No compiler detected
@exit /b 1

:UseCl
cl /nologo /Fe:bin\wdmgr.exe /std:c++17 /O2 /EHsc ..\wdmgr.cpp
@exit /b 0

:UseClang
clang++ -o bin\wdmgr.exe -std=c++17 -O3 ..\wdmgr.cpp
@exit /b 0

:UseGcc
g++ -o bin\wdmgr.exe -std=c++17 -O3 ..\wdmgr.cpp
@exit /b 0
