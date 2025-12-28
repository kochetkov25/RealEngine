#!/usr/bin/env bash
# RealEngine Build Script for Linux/macOS
# Usage:
#   ./build.sh                    # Build with default settings (Release)
#   ./build.sh --config Debug     # Build in Debug mode
#   ./build.sh --clean            # Clean build directory before building
#   ./build.sh --configure-only   # Only run CMake configuration, don't build
#   ./build.sh --verbose          # Show verbose build output
#   ./build.sh --help             # Show this help message

set -euo pipefail

# Default values
BUILD_DIR="build"
CONFIG="Release"
CLEAN=false
CONFIGURE_ONLY=false
VERBOSE=false

# Colors for output (if terminal supports it)
if [[ -t 1 ]]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    CYAN='\033[0;36m'
    GRAY='\033[0;37m'
    NC='\033[0m' # No Color
else
    RED=''
    GREEN=''
    YELLOW=''
    CYAN=''
    GRAY=''
    NC=''
fi

# Parse command-line arguments
show_help() {
    cat << EOF
RealEngine Build Script

Usage: $0 [OPTIONS]

Options:
    --config TYPE          Build configuration (Debug, Release, RelWithDebInfo, MinSizeRel)
                          Default: Release
    --clean                Clean build directory before building
    --configure-only       Only run CMake configuration, don't build
    --verbose              Show verbose build output
    --help                 Show this help message

Examples:
    $0                          # Build in Release mode
    $0 --config Debug           # Build in Debug mode
    $0 --clean                  # Clean and build
    $0 --configure-only         # Only configure, don't build
EOF
}

while [[ $# -gt 0 ]]; do
    case $1 in
        --config)
            CONFIG="$2"
            if [[ ! "$CONFIG" =~ ^(Debug|Release|RelWithDebInfo|MinSizeRel)$ ]]; then
                echo -e "${RED}Error: Invalid build type: $CONFIG${NC}"
                echo "Valid types: Debug, Release, RelWithDebInfo, MinSizeRel"
                exit 1
            fi
            shift 2
            ;;
        --clean)
            CLEAN=true
            shift
            ;;
        --configure-only)
            CONFIGURE_ONLY=true
            shift
            ;;
        --verbose)
            VERBOSE=true
            shift
            ;;
        --help|-h)
            show_help
            exit 0
            ;;
        *)
            echo -e "${RED}Error: Unknown option: $1${NC}"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo -e "${CYAN}=== RealEngine Build Script ===${NC}"
echo -e "${YELLOW}Build Type: $CONFIG${NC}"
echo -e "${YELLOW}Build Directory: $BUILD_DIR${NC}"
echo ""

# Clean build directory if requested
if [[ "$CLEAN" == true ]]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    if [[ -d "$BUILD_DIR" ]]; then
        rm -rf "$BUILD_DIR"
        echo -e "${GREEN}Build directory cleaned.${NC}"
    else
        echo -e "${GRAY}Build directory does not exist, skipping clean.${NC}"
    fi
    echo ""
fi

# Get CPU count for parallel builds
if command -v nproc &> /dev/null; then
    CPU_COUNT=$(nproc)
elif command -v sysctl &> /dev/null; then
    CPU_COUNT=$(sysctl -n hw.ncpu)
else
    CPU_COUNT=4  # Fallback
fi

echo -e "${GRAY}Using $CPU_COUNT parallel jobs${NC}"
echo ""

# Configure CMake
echo -e "${CYAN}Configuring CMake...${NC}"

# Check if Ninja is available (preferred for compile_commands.json)
USE_NINJA=false
if command -v ninja &> /dev/null; then
    USE_NINJA=true
    echo -e "${GRAY}Using Ninja generator (better for compile_commands.json)${NC}"
fi

CMAKE_ARGS=(
    -S .
    -B "$BUILD_DIR"
    -DCMAKE_BUILD_TYPE="$CONFIG"
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
)

# Use Ninja generator if available
if [[ "$USE_NINJA" == true ]]; then
    CMAKE_ARGS+=(-G "Ninja")
fi

if ! cmake "${CMAKE_ARGS[@]}"; then
    echo -e "${RED}CMake configuration failed!${NC}"
    exit 1
fi

echo -e "${GREEN}CMake configuration completed successfully.${NC}"

# Copy compile_commands.json to project root for clangd
COMPILE_COMMANDS_BUILD="$BUILD_DIR/compile_commands.json"
COMPILE_COMMANDS_ROOT="$SCRIPT_DIR/compile_commands.json"

if [[ -f "$COMPILE_COMMANDS_BUILD" ]]; then
    cp -f "$COMPILE_COMMANDS_BUILD" "$COMPILE_COMMANDS_ROOT"
    echo -e "${GRAY}Copied compile_commands.json to project root for clangd.${NC}"
else
    echo -e "${YELLOW}Warning: compile_commands.json not found in build directory.${NC}"
    if [[ "$USE_NINJA" == false ]]; then
        echo -e "${YELLOW}         Install Ninja for better compile_commands.json support:${NC}"
        echo -e "${CYAN}         sudo apt-get install ninja-build  (Debian/Ubuntu)${NC}"
        echo -e "${CYAN}         brew install ninja                 (macOS)${NC}"
    fi
fi

echo ""

# Build if not configure-only
if [[ "$CONFIGURE_ONLY" == false ]]; then
    echo -e "${CYAN}Building project...${NC}"
    
    BUILD_ARGS=(
        --build "$BUILD_DIR"
        --parallel "$CPU_COUNT"
    )
    
    if [[ "$VERBOSE" == true ]]; then
        BUILD_ARGS+=(--verbose)
    fi
    
    if ! cmake "${BUILD_ARGS[@]}"; then
        echo -e "${RED}Build failed!${NC}"
        exit 1
    fi
    
    echo ""
    echo -e "${GREEN}Build completed successfully!${NC}"
    
    # Try to find the executable (location varies by generator)
    EXE_PATHS=(
        "$BUILD_DIR/bin/RealEngine"
        "$BUILD_DIR/RealEngine"
    )
    
    EXE_FOUND=false
    for path in "${EXE_PATHS[@]}"; do
        if [[ -f "$path" ]]; then
            echo -e "${CYAN}Executable location: $path${NC}"
            EXE_FOUND=true
            break
        fi
    done
    
    if [[ "$EXE_FOUND" == false ]]; then
        echo -e "${GRAY}Note: Executable location may vary by build generator${NC}"
    fi
else
    echo -e "${YELLOW}Configuration only mode - skipping build.${NC}"
fi

echo ""
echo -e "${CYAN}=== Build Script Complete ===${NC}"
