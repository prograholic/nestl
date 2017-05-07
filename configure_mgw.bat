echo off
set NESTL_SRC=%cd%
set NESTL_BUILD_ROOT=%NESTL_SRC%\..


if [%1] == [] goto usage


set CMAKE_GENERATOR="MinGW Makefiles"

set OUT_DIR=mingw-gcc-%1


if [%CMAKE_GENERATOR%] == [] goto usage

call :configure build\%OUT_DIR% %1

goto end


:configure
cd %NESTL_BUILD_ROOT%
mkdir %1
cd %1
set CC=gcc && set CXX=g++ && cmake %NESTL_SRC% -G %CMAKE_GENERATOR%
goto end

:usage
echo %0 ^<mgw_ver^>



:end
cd %NESTL_SRC%