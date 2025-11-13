#!/bin/bash
# ==============================================================================
# CMake Build System Validation Script
# ==============================================================================
# This script validates the CMake configuration without building
# ==============================================================================

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build_test"

echo "=============================================================================="
echo "CMake Build System Validation for Generals Zero Hour"
echo "=============================================================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print status
print_status() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}✓${NC} $2"
    else
        echo -e "${RED}✗${NC} $2"
    fi
}

# Function to print info
print_info() {
    echo -e "${YELLOW}ℹ${NC} $1"
}

# Check CMake version
echo "Checking CMake installation..."
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}✗${NC} CMake not found. Please install CMake 3.15 or later."
    exit 1
fi

CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
print_status 0 "CMake version: ${CMAKE_VERSION}"

# Compare version
CMAKE_MAJOR=$(echo $CMAKE_VERSION | cut -d'.' -f1)
CMAKE_MINOR=$(echo $CMAKE_VERSION | cut -d'.' -f2)

if [ "$CMAKE_MAJOR" -lt 3 ] || ([ "$CMAKE_MAJOR" -eq 3 ] && [ "$CMAKE_MINOR" -lt 15 ]); then
    echo -e "${RED}✗${NC} CMake version 3.15+ required, found ${CMAKE_VERSION}"
    exit 1
fi

echo ""
echo "Checking file structure..."

# Check for required files
REQUIRED_FILES=(
    "CMakeLists.txt"
    "cmake/config.h.in"
    "cmake/GeneralsHelpers.cmake"
    "CMAKE_BUILD_GUIDE.md"
    "CMAKE_QUICK_REFERENCE.md"
)

for file in "${REQUIRED_FILES[@]}"; do
    if [ -f "${SCRIPT_DIR}/${file}" ]; then
        print_status 0 "${file}"
    else
        print_status 1 "${file} - MISSING"
    fi
done

echo ""
echo "Checking directory structure..."

# Check for required directories
REQUIRED_DIRS=(
    "Libraries/Source"
    "GameEngine"
    "GameEngineDevice"
    "Main"
    "cmake"
)

for dir in "${REQUIRED_DIRS[@]}"; do
    if [ -d "${SCRIPT_DIR}/${dir}" ]; then
        print_status 0 "${dir}/"
    else
        print_status 1 "${dir}/ - MISSING"
    fi
done

echo ""
echo "Creating test build directory..."

# Clean up old test build
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

print_status 0 "Build directory created: ${BUILD_DIR}"

echo ""
echo "Testing CMake configuration..."

# Try to configure (but not build)
if cmake .. -DCMAKE_BUILD_TYPE=Release > cmake_output.log 2>&1; then
    print_status 0 "CMake configuration successful"

    echo ""
    echo "Configuration summary:"
    grep "^-- " cmake_output.log | grep -E "(Building|Build Type|C\+\+ Standard|Compiler|Platform)" || true
else
    print_status 1 "CMake configuration failed"
    echo ""
    echo "Error details:"
    tail -20 cmake_output.log
    echo ""
    echo "Full log: ${BUILD_DIR}/cmake_output.log"
    exit 1
fi

echo ""
echo "Checking generated files..."

# Check if config.h was generated
if [ -f "include/config.h" ]; then
    print_status 0 "config.h generated"
else
    print_status 1 "config.h not generated"
fi

# Check if compile_commands.json was generated
if [ -f "compile_commands.json" ]; then
    print_status 0 "compile_commands.json generated"
else
    print_info "compile_commands.json not generated (expected if CMAKE_EXPORT_COMPILE_COMMANDS not set)"
fi

echo ""
echo "Listing available build targets..."
if cmake --build . --target help 2>/dev/null | grep "^\.\.\." | head -10; then
    echo "..."
    print_info "Run 'cmake --build . --target help' to see all targets"
else
    print_info "Target list not available (generator-dependent)"
fi

echo ""
echo "=============================================================================="
echo "Validation Summary"
echo "=============================================================================="

if [ -f cmake_output.log ] && grep -q "Configuring done" cmake_output.log; then
    echo -e "${GREEN}SUCCESS${NC} - CMake configuration is valid!"
    echo ""
    echo "Next steps:"
    echo "  1. Review the configuration:"
    echo "     cd ${BUILD_DIR}"
    echo "     cat cmake_output.log"
    echo ""
    echo "  2. Build the project:"
    echo "     cmake --build . -j\$(nproc)"
    echo ""
    echo "  3. Clean up test build:"
    echo "     cd ${SCRIPT_DIR}"
    echo "     rm -rf ${BUILD_DIR}"
    echo ""
else
    echo -e "${YELLOW}PARTIAL${NC} - Configuration completed with warnings"
    echo "Review the log file: ${BUILD_DIR}/cmake_output.log"
fi

echo ""
echo "For detailed build instructions, see:"
echo "  - CMAKE_BUILD_GUIDE.md (comprehensive guide)"
echo "  - CMAKE_QUICK_REFERENCE.md (quick commands)"
echo "=============================================================================="

# Don't automatically clean up - let user examine the results
print_info "Test build directory preserved for inspection: ${BUILD_DIR}"
print_info "Remove with: rm -rf ${BUILD_DIR}"
