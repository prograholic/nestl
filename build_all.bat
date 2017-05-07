echo off

set NESTL_SRC=%cd%
set NESTL_BUILD_ROOT=%NESTL_SRC%\..



call :build %NESTL_BUILD_ROOT%\build\msvc-2013
call :build %NESTL_BUILD_ROOT%\build\msvc-2015
call :build %NESTL_BUILD_ROOT%\build\msvc-2017
call :build %NESTL_BUILD_ROOT%\build\mingw-gcc-5.3

goto end


:build
cmake --build %1
cd %1
ctest -C Debug
cd %NESTL_SRC%

goto end

:usage
echo %0

:end