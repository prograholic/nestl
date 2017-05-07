echo off
set NESTL_SRC=%cd%
set NESTL_BUILD_ROOT=%NESTL_SRC%\..


if [%1] == [] goto usage

if %1 == 2013 set MSVC_GENERATOR="Visual Studio 12 2013"&& set OUT_DIR=msvc-2013
if %1 == 2013_64 set MSVC_GENERATOR="Visual Studio 12 2013 Win64"&& set OUT_DIR=msvc-2013_64
if %1 == 2015 set MSVC_GENERATOR="Visual Studio 14"&& set OUT_DIR=msvc-2015
if %1 == 2015_64 set MSVC_GENERATOR="Visual Studio 14 Win64"&& set OUT_DIR=msvc-2015_64
if %1 == 2017 set MSVC_GENERATOR="Visual Studio 15"&& set OUT_DIR=msvc-2017
if %1 == 2017_64 set MSVC_GENERATOR="Visual Studio 15 Win64"&& set OUT_DIR=msvc-2017_64


set GMOCK_DIR=D:\pixel\src\gtest\googletest\googletest
if NOT [%2] == [] set GMOCK_DIR=%2


if [%MSVC_GENERATOR%] == [] goto usage

call :configure build\%OUT_DIR% %MSVC_GENERATOR% Yes Yes

goto end


:configure
cd %NESTL_BUILD_ROOT%
mkdir %1
cd %1
cmake %NESTL_SRC% -G %2
goto end

:usage
echo %0 ^<msvc_ver^>



:end
cd %NESTL_SRC%