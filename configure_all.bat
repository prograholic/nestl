echo off
set NESTL_SRC=%cd%
set NESTL_BUILD_ROOT=%NESTL_SRC%\..


set GMOCK_DIR=D:/prograholic/projects/gmock-1.7.0
set MSVC_2008_GENERATOR="Visual Studio 9 2008"


call :configure build\C++-03\msvc-2008        %MSVC_2008_GENERATOR% No  No
call :configure build\C++-03-no-std\msvc-2008 %MSVC_2008_GENERATOR% No  Yes
call :configure build\C++-11\msvc-2008        %MSVC_2008_GENERATOR% Yes No
call :configure build\C++-11-no-std\msvc-2008 %MSVC_2008_GENERATOR% Yes Yes

goto end


:configure
cd %NESTL_BUILD_ROOT%
mkdir %1
cd %1
cmake %NESTL_SRC% -DNESTL_GOOGLEMOCK_DIR=%GMOCK_DIR% -G %2 -DNESTL_ENABLE_CXX11=%3 -DNESTL_DISABLE_STD=%4



:end
cd %NESTL_SRC%