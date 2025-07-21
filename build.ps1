$ErrorActionPreference = "Stop"

$BUILD_DIR = "build"

$CPU_COUNT = [Environment]::ProcessorCount

cmake -S . -B $BUILD_DIR -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build $BUILD_DIR --parallel $CPU_COUNT
