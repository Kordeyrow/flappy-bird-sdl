@echo off
setlocal EnableDelayedExpansion
REM ==============================================
REM Build Script for Emscripten C++ SDL Project
REM ==============================================

REM Set the path to your Emscripten SDK installation
set EMSDK_PATH=C:\dev\vendor\Emscripten\emsdk

REM Activate Emscripten environment
call "%EMSDK_PATH%\emsdk_env.bat"

REM Navigate to the directory where the script is located
cd /d "%~dp0"

REM Define the output directory
set OUTPUT_DIR=build_web

REM Create the output directory if it doesn't exist
if not exist "%OUTPUT_DIR%" (
    mkdir "%OUTPUT_DIR%"
)

REM Create a temporary file to store the source file list
set SOURCE_FILES_FILE=source_files.txt
if exist "%SOURCE_FILES_FILE%" del "%SOURCE_FILES_FILE%"

REM Find all .cpp files recursively and write to the temporary file
for /R src %%f in (*.cpp) do (
    echo "%%f" >> "%SOURCE_FILES_FILE%"
)

REM Initialize the SOURCE_FILES variable
set "SOURCE_FILES="

REM Read the source files from the temporary file into the variable
for /F "usebackq delims=" %%f in ("%SOURCE_FILES_FILE%") do (
    if "!SOURCE_FILES!"=="" (
        set "SOURCE_FILES=%%f"
    ) else (
        set "SOURCE_FILES=!SOURCE_FILES! %%f"
    )
    echo %%f
)

REM Display the accumulated source files
echo !SOURCE_FILES!

REM Delete the temporary file
del "%SOURCE_FILES_FILE%"

REM Define the assets directory
set ASSETS_DIR=assets

REM Create the res directory if it doesn't exist
if not exist "%ASSETS_DIR%" (
    echo Creating assets directory...
    mkdir "%ASSETS_DIR%"
    echo Assets directory created.
) else (
    echo Assets directory already exists.
)

REM Copy assets from to build_web\assets
if exist "%ASSETS_DIR%" (
    echo Copying assets from %ASSETS_DIR% to %OUTPUT_DIR%\%ASSETS_DIR%...
    xcopy /E /I /Y "%ASSETS_DIR%" "%OUTPUT_DIR%\%ASSETS_DIR%"
    echo Assets copied successfully.
) else (
    echo Source directory %ASSETS_DIR% does not exist, skipping copy.
)

REM Modifiers
REM -s USE_SDL=2 
REM -s USE_SDL_IMAGE=2 
REM -s USE_SDL_TTF=2 
REM -lSDL 
REM --preload-file %ASSETS_DIR%
REM -s ALLOW_MEMORY_GROWTH=1 
REM --use-preload-plugins 
REM -s GL_UNSAFE_OPTS=0
emcc !SOURCE_FILES! -s USE_SDL=2 -s USE_SDL_IMAGE=2 --preload-file %ASSETS_DIR% --use-preload-plugins ^
    -s EXPORTED_FUNCTIONS="['_main', '_start_game']" -s "EXPORTED_RUNTIME_METHODS=['ccall']" ^
    -I src ^
    -I src\sdl2imgui ^
    -I ..\SDL2\include ^
    -I ..\SDL2_image\include ^
    -I . ^
    -o "%OUTPUT_DIR%\flappy-bird.js"
pause

echo.
echo Build complete. Output files are in the "%OUTPUT_DIR%" directory.
pause

REM start "" cmd /k emrun --no_browser --port 8080 index.html