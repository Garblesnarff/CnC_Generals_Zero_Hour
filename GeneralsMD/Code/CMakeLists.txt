# ==============================================================================
# Command & Conquer: Generals Zero Hour - Modern CMake Build System
# ==============================================================================
# Root CMakeLists.txt
#
# This build system modernizes the original Visual Studio 6 project structure
# while maintaining compatibility with the original codebase architecture.
# ==============================================================================

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# ==============================================================================
# Project Definition
# ==============================================================================

project(GeneralsZeroHour
    VERSION 1.0.0
    LANGUAGES C CXX
    DESCRIPTION "Command & Conquer: Generals Zero Hour - Real-Time Strategy Game"
)

# ==============================================================================
# CMake Policies
# ==============================================================================

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW) # MSVC runtime library flags selected by CMAKE_MSVC_RUNTIME_LIBRARY
endif()

# ==============================================================================
# CMake Module Path
# ==============================================================================

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Load helper functions
include(GeneralsHelpers)

# ==============================================================================
# Build Configuration
# ==============================================================================

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Run)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Run)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Libraries/Lib)

# Multi-config generators (Visual Studio, Xcode)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_SOURCE_DIR}/../Run)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_SOURCE_DIR}/../Run)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_SOURCE_DIR}/Libraries/Lib)
endforeach()

# ==============================================================================
# C++ Standard
# ==============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ==============================================================================
# Platform Detection
# ==============================================================================

if(WIN32)
    set(PLATFORM_WINDOWS TRUE)
    add_definitions(-DPLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX TRUE)
    add_definitions(-DPLATFORM_LINUX)
elseif(APPLE)
    set(PLATFORM_MACOS TRUE)
    add_definitions(-DPLATFORM_MACOS)
endif()

# ==============================================================================
# Global Compiler Definitions
# ==============================================================================

# Common definitions for all configurations
add_compile_definitions(
    Z_PREFIX                    # zlib namespace prefix
    $<$<PLATFORM_ID:Windows>:WIN32>
    $<$<PLATFORM_ID:Windows>:_WINDOWS>
    $<$<PLATFORM_ID:Windows>:WINVER=0x0400>
    $<$<PLATFORM_ID:Windows>:_WIN32_WINNT=0x0400>
    $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN>
    $<$<PLATFORM_ID:Windows>:NOMINMAX>
    $<$<PLATFORM_ID:Windows>:_MBCS>
)

# Configuration-specific definitions
add_compile_definitions(
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Debug>:BROWSER_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:Release>:IG_DEBUG_STACKTRACE>
    $<$<CONFIG:RelWithDebInfo>:NDEBUG>
    $<$<CONFIG:RelWithDebInfo>:_RELEASE>
)

# ==============================================================================
# Global Compiler Flags
# ==============================================================================

# --- Windows (MSVC) Compiler Flags ---
if(MSVC)
    # Common flags for all configurations
    add_compile_options(
        /W3                     # Warning level 3
        /WX                     # Treat warnings as errors
        /GX                     # Enable C++ exception handling (deprecated but kept for compatibility)
        /Zc:wchar_t             # wchar_t is native type
        /Zc:forScope            # Force conformance in for loop scope
        $<$<CONFIG:Debug>:/MDd> # Multi-threaded Debug DLL runtime
        $<$<CONFIG:Debug>:/Gm>  # Enable minimal rebuild
        $<$<CONFIG:Debug>:/ZI>  # Edit and Continue debug info
        $<$<CONFIG:Debug>:/Od>  # Disable optimization
        $<$<CONFIG:Debug>:/GZ>  # Enable runtime debug checks
        $<$<CONFIG:Debug>:/RTC1> # Runtime checks
        $<$<CONFIG:Release>:/MD> # Multi-threaded DLL runtime
        $<$<CONFIG:Release>:/O2> # Maximize speed
        $<$<CONFIG:Release>:/Ob2> # Inline function expansion
        $<$<CONFIG:Release>:/Oi> # Enable intrinsic functions
        $<$<CONFIG:Release>:/Ot> # Favor fast code
        $<$<CONFIG:Release>:/Oy> # Omit frame pointers
        $<$<CONFIG:Release>:/GL> # Whole program optimization
        $<$<CONFIG:Release>:/Zi> # Debug information (for PDB)
        $<$<CONFIG:RelWithDebInfo>:/MD>
        $<$<CONFIG:RelWithDebInfo>:/O2>
        $<$<CONFIG:RelWithDebInfo>:/Ob2>
        $<$<CONFIG:RelWithDebInfo>:/Zi>
    )

    # Linker flags for Release builds
    add_link_options(
        $<$<CONFIG:Release>:/LTCG>        # Link-time code generation
        $<$<CONFIG:Release>:/OPT:REF>     # Eliminate unreferenced functions
        $<$<CONFIG:Release>:/OPT:ICF>     # Enable COMDAT folding
        $<$<CONFIG:Release>:/DEBUG>       # Generate debug info
        $<$<CONFIG:RelWithDebInfo>:/DEBUG>
        $<$<CONFIG:RelWithDebInfo>:/OPT:REF>
        $<$<CONFIG:RelWithDebInfo>:/OPT:ICF>
    )

    # Disable specific warnings
    add_compile_options(
        /wd4244 # conversion from 'type1' to 'type2', possible loss of data
        /wd4267 # conversion from 'size_t' to 'type', possible loss of data
        /wd4996 # deprecated function warnings
    )

# --- GCC/Clang Compiler Flags (Linux/macOS) ---
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Common flags
    add_compile_options(
        -Wall                   # Enable most warnings
        -Werror                 # Treat warnings as errors
        -fexceptions            # Enable exception handling
        -fno-strict-aliasing    # Disable strict aliasing optimizations
        $<$<CONFIG:Debug>:-g3>  # Full debug information
        $<$<CONFIG:Debug>:-O0>  # No optimization
        $<$<CONFIG:Debug>:-fno-inline> # Don't inline functions
        $<$<CONFIG:Release>:-O3> # Maximum optimization
        $<$<CONFIG:Release>:-DNDEBUG>
        $<$<CONFIG:Release>:-march=native> # Optimize for current CPU
        $<$<CONFIG:RelWithDebInfo>:-O2>
        $<$<CONFIG:RelWithDebInfo>:-g>
    )

    # Additional GCC-specific flags
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(
            -fno-rtti              # Disable RTTI if not needed
        )
    endif()

    # Suppress specific warnings for legacy code
    add_compile_options(
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -Wno-sign-compare
        -Wno-reorder
    )
endif()

# ==============================================================================
# Global Include Directories
# ==============================================================================

# These will be available to all targets via target_include_directories
set(GENERALS_COMMON_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngineDevice/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/WWVegas
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/WWVegas/WWLib
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/WWVegas/WWMath
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/WWVegas/WWDebug
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/WWVegas/WWSaveLoad
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/WWVegas/WW3D2
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/GameSpy
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/Compression
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Include/Granny
)

# ==============================================================================
# Third-Party Dependencies
# ==============================================================================

# Windows-specific libraries
if(WIN32)
    set(PLATFORM_LIBS
        kernel32
        user32
        gdi32
        winspool
        comdlg32
        advapi32
        shell32
        ole32
        oleaut32
        uuid
        odbc32
        odbccp32
        winmm
        wsock32
        wininet
        imm32
        dsound
        dinput8
        dxguid
        vfw32
    )
endif()

# ==============================================================================
# Build Options
# ==============================================================================

option(BUILD_TOOLS "Build development tools (WorldBuilder, ParticleEditor, etc.)" OFF)
option(BUILD_TESTS "Build test executables" OFF)
option(USE_PRECOMPILED_HEADERS "Use precompiled headers to speed up compilation" ON)
option(ENABLE_PROFILE "Enable profiling support" OFF)

# ==============================================================================
# Subdirectories - Build Order Based on Dependencies
# ==============================================================================

message(STATUS "==============================================================================")
message(STATUS "Building Command & Conquer: Generals Zero Hour")
message(STATUS "==============================================================================")
message(STATUS "Build Type:          ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard:        C++${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler:            ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Platform:            ${CMAKE_SYSTEM_NAME}")
message(STATUS "Build Tools:         ${BUILD_TOOLS}")
message(STATUS "Use PCH:             ${USE_PRECOMPILED_HEADERS}")
message(STATUS "==============================================================================")

# -----------------------------------------------------------------------------
# Layer 1: Base Libraries (No Dependencies)
# -----------------------------------------------------------------------------
message(STATUS "Configuring Layer 1: Base Libraries")

# Core utility libraries
add_subdirectory(Libraries/Source/Benchmark)
add_subdirectory(Libraries/Source/Compression)
add_subdirectory(Libraries/Source/EABrowserDispatch)

# WWVegas libraries
add_subdirectory(Libraries/Source/WWVegas/WWMath)
add_subdirectory(Libraries/Source/WWVegas/WWUtil)
add_subdirectory(Libraries/Source/WWVegas/WWDebug)
add_subdirectory(Libraries/Source/WWVegas/WWLib)
add_subdirectory(Libraries/Source/WWVegas/WWSaveLoad)
add_subdirectory(Libraries/Source/WWVegas/WW3D2)
add_subdirectory(Libraries/Source/WWVegas/wwshade)
add_subdirectory(Libraries/Source/WWVegas/WWDownload)

# Optional: Audio library (if present)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/WPAudio/CMakeLists.txt)
    add_subdirectory(Libraries/Source/WPAudio)
endif()

# GameSpy SDK libraries
add_subdirectory(Libraries/Source/GameSpy/ghttp)
add_subdirectory(Libraries/Source/GameSpy/peer)
add_subdirectory(Libraries/Source/GameSpy/gp)
add_subdirectory(Libraries/Source/GameSpy/gstats)
# Note: Patching library might not have sources in open-source version

# Debugging and profiling libraries
add_subdirectory(Libraries/Source/debug)
add_subdirectory(Libraries/Source/profile)

# -----------------------------------------------------------------------------
# Layer 2: Game Engine Libraries (Depend on Layer 1)
# -----------------------------------------------------------------------------
message(STATUS "Configuring Layer 2: Game Engine")

add_subdirectory(GameEngine)
add_subdirectory(GameEngineDevice)

# -----------------------------------------------------------------------------
# Layer 3: Main Executable (Depends on Layers 1 & 2)
# -----------------------------------------------------------------------------
message(STATUS "Configuring Layer 3: Main Executable")

add_subdirectory(Main)

# -----------------------------------------------------------------------------
# Layer 4: Tools (Optional, Depend on Layers 1, 2 & 3)
# -----------------------------------------------------------------------------
if(BUILD_TOOLS)
    message(STATUS "Configuring Layer 4: Development Tools")

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Tools/WorldBuilder/CMakeLists.txt)
        add_subdirectory(Tools/WorldBuilder)
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Tools/ParticleEditor/CMakeLists.txt)
        add_subdirectory(Tools/ParticleEditor)
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Tools/GUIEdit/CMakeLists.txt)
        add_subdirectory(Tools/GUIEdit)
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Tools/ImagePacker/CMakeLists.txt)
        add_subdirectory(Tools/ImagePacker)
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Tools/MapCacheBuilder/CMakeLists.txt)
        add_subdirectory(Tools/MapCacheBuilder)
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Tools/Launcher/CMakeLists.txt)
        add_subdirectory(Tools/Launcher)
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Tools/versionUpdate/CMakeLists.txt)
        add_subdirectory(Tools/versionUpdate)
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Tools/buildVersionUpdate/CMakeLists.txt)
        add_subdirectory(Tools/buildVersionUpdate)
    endif()
endif()

# ==============================================================================
# Installation Configuration
# ==============================================================================

# Install game executable
install(TARGETS RTS
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install game data (if this is run as a separate command)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/../Run/Data/
    DESTINATION share/generals/Data
    PATTERN "*.ini"
    PATTERN "*.wnd"
    PATTERN "*.str"
)

# ==============================================================================
# Build Summary
# ==============================================================================

message(STATUS "==============================================================================")
message(STATUS "Configuration complete!")
message(STATUS "")
message(STATUS "To build the project:")
message(STATUS "  cmake --build . --config ${CMAKE_BUILD_TYPE}")
message(STATUS "")
message(STATUS "Output directories:")
message(STATUS "  Executable: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "  Libraries:  ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "==============================================================================")

# ==============================================================================
# Development Helpers
# ==============================================================================

# Export compile commands for IDEs and tools (clangd, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Generate a configuration header
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/config.h
)

# ==============================================================================
# Custom Targets
# ==============================================================================

# Clean build artifacts
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    COMMENT "Cleaning all build artifacts..."
)

# Run the game (for development)
if(TARGET RTS)
    add_custom_target(run
        COMMAND $<TARGET_FILE:RTS>
        DEPENDS RTS
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Running Generals Zero Hour..."
    )
endif()

# ==============================================================================
# Testing Support
# ==============================================================================

if(BUILD_TESTS)
    enable_testing()
    message(STATUS "Testing enabled")
endif()

# ==============================================================================
# Documentation
# ==============================================================================

# Generate project documentation with Doxygen (if available)
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# ==============================================================================
# End of Root CMakeLists.txt
# ==============================================================================
