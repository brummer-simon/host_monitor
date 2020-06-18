#!/bin/sh

# Cleanup
[[ -e build-release ]] && rm -rf build-release

# Build Release
cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -B "build-release" -S . && make -C build-release all && sudo make -C build-release install
