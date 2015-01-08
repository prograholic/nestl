echo off


set OUT_DIR=%1

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
goto end

:usage
echo %0 ^<toolchain_dir^>

:end