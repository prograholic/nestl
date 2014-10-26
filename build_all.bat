echo off
set NESTL_SRC=%cd%
set NESTL_BUILD_ROOT=%NESTL_SRC%\..




call :build %NESTL_BUILD_ROOT%\build\C++-03\msvc-2008
call :build %NESTL_BUILD_ROOT%\build\C++-03-no-std\msvc-2008
call :build %NESTL_BUILD_ROOT%\build\C++-11\msvc-2008
call :build %NESTL_BUILD_ROOT%\build\C++-11-no-std\msvc-2008

goto end


:build
cmake --build %1



:end