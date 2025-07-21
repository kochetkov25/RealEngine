#!/usr/bin/env bash
set -e
BUILD_DIR="build"
cmake -S . -B "$BUILD_DIR" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build "$BUILD_DIR" --parallel $(nproc)