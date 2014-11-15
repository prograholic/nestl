echo off


if [%1] == [] goto usage

if %1 == 2008 set OUT_DIR=msvc-2008
if %1 == 2012 set OUT_DIR=msvc-2012

if [%OUT_DIR%] == [] goto usage

set NESTL_SRC=%cd%
set NESTL_BUILD_ROOT=%NESTL_SRC%\..



call :build %NESTL_BUILD_ROOT%\build\C++-03\%OUT_DIR%
call :build %NESTL_BUILD_ROOT%\build\C++-03-no-std\%OUT_DIR%
call :build %NESTL_BUILD_ROOT%\build\C++-11\%OUT_DIR%
call :build %NESTL_BUILD_ROOT%\build\C++-11-no-std\%OUT_DIR%

goto end


:build
cmake --build %1


:usage
echo %0 ^<mcvc_ver^>

:end