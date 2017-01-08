echo off
set NESTL_SRC=%cd%
set NESTL_BUILD_ROOT=%NESTL_SRC%\..

if [%1] == [] goto usage


set CMAKE_GENERATOR="MinGW Makefiles"

set OUT_DIR=mingw32-gcc-%1


set GMOCK_DIR=d:/pixel/projects/gtest/1.7.0
if NOT [%2] == [] set GMOCK_DIR=%2


call :configure build\C++-11\%OUT_DIR%        %1 No
call :configure build\C++-11-no-std\%OUT_DIR% %1 Yes

goto end


:configure
cd %NESTL_BUILD_ROOT%
mkdir %1
cd %1
set CC=mingw32-gcc && set CXX=mingw32-g++ && cmake %NESTL_SRC% -DNESTL_GOOGLEMOCK_DIR=%GMOCK_DIR% -G %CMAKE_GENERATOR% -DNESTL_DISABLE_STD=%3
goto end

:usage
echo %0 ^<mgw_ver^> {gmock_dir}



:end
cd %NESTL_SRC%