@echo off

pushd ..
vendor\bin\premake\windows\premake5.exe --file=build.lua vs2022
popd
pause