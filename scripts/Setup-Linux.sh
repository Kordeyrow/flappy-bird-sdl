#!/bin/bash

pushd ..
vendor/bin/premake/linux/premake5 --cc=clang --file=build.lua gmake2
popd
