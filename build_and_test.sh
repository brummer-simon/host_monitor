#!/bin/sh

# Cleanup
[[ -e build-debug ]]   && rm -rf build-debug
[[ -e build-release ]] && rm -rf build-release

# Build Debug
cmake -DCMAKE_BUILD_TYPE=Debug   -G "Unix Makefiles" -B "build-debug"   -S . && make -C build-debug all test

# Build Release
cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -B "build-release" -S . && make -C build-release all test
