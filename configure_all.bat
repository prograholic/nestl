rem echo off
set NESTL_SRC=%cd%
set NESTL_BUILD_ROOT=%NESTL_SRC%\..


if [%1] == [] goto usage

if %1 == 2008 set MSVC_GENERATOR="Visual Studio 9 2008"&& set OUT_DIR=msvc-2008
if %1 == 2010 set MSVC_GENERATOR="Visual Studio 10 2010"&& set OUT_DIR=msvc-2010
if %1 == 2012 set MSVC_GENERATOR="Visual Studio 11 2012"&& set OUT_DIR=msvc-2012
if %1 == 2015 set MSVC_GENERATOR="Visual Studio 14 2015"&& set OUT_DIR=msvc-2015


set GTEST_DIR=d:/pixel/src/gtest/1.8.0/googletest
if NOT [%2] == [] set GTEST_DIR=%2


if [%MSVC_GENERATOR%] == [] goto usage

call :configure build\C++-11\%OUT_DIR%        %MSVC_GENERATOR% No
call :configure build\C++-11-no-std\%OUT_DIR% %MSVC_GENERATOR% Yes

goto end


:configure
cd %NESTL_BUILD_ROOT%
mkdir %1
cd %1
cmake %NESTL_SRC% -DNESTL_GOOGLETEST_DIR=%GTEST_DIR% -G %2 -DNESTL_DISABLE_STD=%3
goto end

:usage
echo %0 ^<msvc_ver^> {gtest_dir}



:end
cd %NESTL_SRC%