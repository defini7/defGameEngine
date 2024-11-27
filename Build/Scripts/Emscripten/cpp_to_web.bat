@echo off

set STB_PATH=C:\SDK\stb
set ENGINE_PATH=..\..\..
set SOURCES_PATH=..\..\..\Examples
set EMSCRIPTEN_PATH=C:\SDK\emsdk

set COMPILER_FLAGS=-O2 -Wall -pedantic -std=c++20
set EMSCRIPTEN_FLAGS=-sALLOW_MEMORY_GROWTH=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2

call %EMSCRIPTEN_PATH%\emsdk activate latest

if "%1" == "build" goto :build
if "%1" == "run" goto :run

:build
    if exist "%SOURCES_PATH%\assets" (
        call em++ %COMPILER_FLAGS% %EMSCRIPTEN_FLAGS% -I"%STB_PATH%" -I"%ENGINE_PATH%" -I"%SOURCES_PATH%" %SOURCES_PATH%\%2.cpp -o ..\..\Target\%2\index.js --preload-file %SOURCES_PATH%\assets
    ) else (
        call em++ %COMPILER_FLAGS% %EMSCRIPTEN_FLAGS% -I"%STB_PATH%" -I"%ENGINE_PATH%" %SOURCES_PATH%\%2.cpp -o ..\..\Target\%2\index.js
    )

    goto :leave

:run
    call emrun ..\..\Target\%2\index.html
    goto :leave

:leave
