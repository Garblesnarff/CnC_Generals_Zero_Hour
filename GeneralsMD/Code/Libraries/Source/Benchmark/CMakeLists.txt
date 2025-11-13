# Benchmark Library - Performance benchmarking utilities
cmake_minimum_required(VERSION 3.15)

# This library appears to be header-only or has missing source files
# The .dsp references C source files that are not present in the repository

# Collect all header files
file(GLOB BENCHMARK_HEADERS "*.h")

# Create interface library (header-only)
add_library(Benchmark INTERFACE)

# Include directories
target_include_directories(Benchmark
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Note: Source files referenced in .dsp (emfloat.c, misc.c, nbench0.c, nbench1.c, sysspec.c)
# are not present in the repository. This may need to be implemented or obtained separately.
