echo off
set NESTL_SRC=%cd%
set NESTL_BUILD_ROOT=%NESTL_SRC%\..


if [%1] == [] goto usage

if %1 == 2008 set MSVC_GENERATOR="Visual Studio 9 2008"&& set OUT_DIR=msvc-2008
if %1 == 2010 set MSVC_GENERATOR="Visual Studio 10 2010"&& set OUT_DIR=msvc-2010
if %1 == 2012 set MSVC_GENERATOR="Visual Studio 11 2012"&& set OUT_DIR=msvc-2012


set GMOCK_DIR=D:/prograholic/projects/gmock-1.7.0
if NOT [%2] == [] set GMOCK_DIR=%2


if [%MSVC_GENERATOR%] == [] goto usage

call :configure build\C++-03\%OUT_DIR%        %MSVC_GENERATOR% No  No
call :configure build\C++-03-no-std\%OUT_DIR% %MSVC_GENERATOR% No  Yes
call :configure build\C++-11\%OUT_DIR%        %MSVC_GENERATOR% Yes No
call :configure build\C++-11-no-std\%OUT_DIR% %MSVC_GENERATOR% Yes Yes

goto end


:configure
cd %NESTL_BUILD_ROOT%
mkdir %1
cd %1
cmake %NESTL_SRC% -DNESTL_GOOGLEMOCK_DIR=%GMOCK_DIR% -G %2 -DNESTL_CONFIG_HAS_CXX11=%3 -DNESTL_DISABLE_STD=%4
goto end

:usage
echo %0 ^<msvc_ver^> {gmock_dir}



:end
cd %NESTL_SRC%