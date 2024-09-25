@echo off

set CC_STB_INCLUDE=C:\SDK\stb
set CC_DGE_INCLUDE=C:\Programming\cpp\InterestingStuff\defGameEngine
set EMSCRIPTEN_PATH=C:\SDK\emsdk

set CC_FLAGS=-O2 -Wall -pedantic -std=c++20
set EMSCRIPTEN_FLAGS=-sALLOW_MEMORY_GROWTH=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2

call %EMSCRIPTEN_PATH%\emsdk activate latest

if "%1" == "build" goto :build
if "%1" == "run" goto :run

:build
    call em++ %CC_FLAGS% %EMSCRIPTEN_FLAGS% -I"%CC_STB_INCLUDE%" -I"%CC_DGE_INCLUDE%" %2.cpp -o Build\index.html
    goto :leave

:run
    call emrun Build/index.html
    goto :leave

:leave
