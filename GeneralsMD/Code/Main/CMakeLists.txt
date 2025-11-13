# ==============================================================================
# Main RTS Executable - Command & Conquer: Generals Zero Hour
# ==============================================================================
# This is the final executable that links all game components together
# ==============================================================================

cmake_minimum_required(VERSION 3.15)

# ==============================================================================
# Source Files
# ==============================================================================

# Main entry point
set(MAIN_SOURCES
    WinMain.cpp
    WinMain.h
    resource.h
)

# Windows resource file (icon, etc.)
if(WIN32)
    list(APPEND MAIN_SOURCES
        RTS.RC
        Generals.ico
    )
endif()

# ==============================================================================
# Create Executable Target
# ==============================================================================

add_executable(RTS WIN32
    ${MAIN_SOURCES}
)

# ==============================================================================
# Executable Naming by Configuration
# ==============================================================================

# Set output name based on build configuration
# Release: RTS.exe
# Debug: RTSD.exe
# RelWithDebInfo: RTSI.exe
set_target_properties(RTS PROPERTIES
    OUTPUT_NAME_DEBUG "RTSD"
    OUTPUT_NAME_RELEASE "RTS"
    OUTPUT_NAME_RELWITHDEBINFO "RTSI"
    OUTPUT_NAME_MINSIZEREL "RTS"
)

# ==============================================================================
# Output Directory
# ==============================================================================

# Output to ../Run/ directory
set_target_properties(RTS PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../Run"
)

# For multi-config generators (Visual Studio)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set_target_properties(RTS PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${CMAKE_SOURCE_DIR}/../Run"
    )
endforeach()

# ==============================================================================
# Include Directories
# ==============================================================================

target_include_directories(RTS PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Libraries/Include
    ${CMAKE_SOURCE_DIR}/Libraries/Source
    ${CMAKE_SOURCE_DIR}/GameEngine/Include
    ${CMAKE_SOURCE_DIR}/GameEngineDevice/Include
    ${CMAKE_SOURCE_DIR}/Libraries/Source/WWVegas
    ${CMAKE_SOURCE_DIR}/Libraries/Source/WWVegas/WWLib
    ${CMAKE_SOURCE_DIR}/Libraries/Source/WWVegas/WWMath
    ${CMAKE_SOURCE_DIR}/Libraries/Source/WWVegas/WWDebug
    ${CMAKE_SOURCE_DIR}/Libraries/Source/WWVegas/WWSaveLoad
    ${CMAKE_SOURCE_DIR}/Libraries/Source/WWVegas/WW3D2
    ${CMAKE_SOURCE_DIR}/Libraries/Source/GameSpy
    ${CMAKE_SOURCE_DIR}/Libraries/Source/Compression
    ${CMAKE_SOURCE_DIR}/Libraries/Include/Granny
)

# ==============================================================================
# Link Libraries
# ==============================================================================

# Link order matters! Dependencies must be linked in correct order:
# 1. High-level game engine libraries
# 2. Low-level utility libraries
# 3. Third-party libraries
# 4. System libraries

target_link_libraries(RTS PRIVATE
    # Layer 1: Game Engine Libraries
    GameEngine
    GameEngineDevice

    # Layer 2: WWVegas Core Libraries
    WWMath
    WW3D2
    WWDebug
    WWLib
    WWUtil

    # Layer 3: GameSpy Networking Libraries (stubs)
    ghttp
    peer
    gp
    gstats

    # Layer 4: Utility Libraries
    Compression
    Benchmark

    # Layer 5: Platform-Specific System Libraries
    $<$<PLATFORM_ID:Windows>:d3d8>
    $<$<PLATFORM_ID:Windows>:d3dx8>
    $<$<PLATFORM_ID:Windows>:dxguid>
    $<$<PLATFORM_ID:Windows>:dinput8>
    $<$<PLATFORM_ID:Windows>:dsound>
    $<$<PLATFORM_ID:Windows>:winmm>
    $<$<PLATFORM_ID:Windows>:wsock32>
    $<$<PLATFORM_ID:Windows>:wininet>
    $<$<PLATFORM_ID:Windows>:imm32>
    $<$<PLATFORM_ID:Windows>:vfw32>

    # Linux system libraries
    $<$<PLATFORM_ID:Linux>:GL>
    $<$<PLATFORM_ID:Linux>:X11>
    $<$<PLATFORM_ID:Linux>:m>
    $<$<PLATFORM_ID:Linux>:pthread>
)

# ==============================================================================
# Compiler Definitions
# ==============================================================================

target_compile_definitions(RTS PRIVATE
    WIN32
    _WINDOWS
    WINVER=0x0400
    _WIN32_WINNT=0x0400
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    _MBCS
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Debug>:BROWSER_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:Release>:IG_DEBUG_STACKTRACE>
    $<$<CONFIG:RelWithDebInfo>:NDEBUG>
    $<$<CONFIG:RelWithDebInfo>:_RELEASE>
)

# ==============================================================================
# Compiler Flags
# ==============================================================================

if(MSVC)
    target_compile_options(RTS PRIVATE
        /W3                     # Warning level 3
        /WX                     # Treat warnings as errors
        /GX                     # Enable C++ exception handling
        /Zc:wchar_t             # wchar_t is native type
        /Zc:forScope            # Force conformance in for loop scope
        $<$<CONFIG:Debug>:/MDd> # Multi-threaded Debug DLL runtime
        $<$<CONFIG:Debug>:/Gm>  # Enable minimal rebuild
        $<$<CONFIG:Debug>:/ZI>  # Edit and Continue debug info
        $<$<CONFIG:Debug>:/Od>  # Disable optimization
        $<$<CONFIG:Debug>:/GZ>  # Enable runtime debug checks
        $<$<CONFIG:Release>:/MD> # Multi-threaded DLL runtime
        $<$<CONFIG:Release>:/O2> # Maximize speed
        $<$<CONFIG:Release>:/Ob2> # Inline function expansion
        $<$<CONFIG:Release>:/GL> # Whole program optimization
        $<$<CONFIG:Release>:/Zi> # Debug information (for PDB)
        $<$<CONFIG:RelWithDebInfo>:/MD>
        $<$<CONFIG:RelWithDebInfo>:/O2>
        $<$<CONFIG:RelWithDebInfo>:/Ob2>
        $<$<CONFIG:RelWithDebInfo>:/Zi>
    )
else()
    target_compile_options(RTS PRIVATE
        -Wall
        -Werror
        -fexceptions
        $<$<CONFIG:Debug>:-g3 -O0>
        $<$<CONFIG:Release>:-O3 -march=native>
        $<$<CONFIG:RelWithDebInfo>:-O2 -g>
    )
endif()

# ==============================================================================
# Linker Flags
# ==============================================================================

if(MSVC)
    # Windows-specific linker flags
    target_link_options(RTS PRIVATE
        /SUBSYSTEM:WINDOWS                              # GUI application
        $<$<CONFIG:Debug>:/NODEFAULTLIB:libcd.lib>     # Exclude default debug C library
        $<$<CONFIG:Release>:/NODEFAULTLIB:libc.lib>    # Exclude default C library
        $<$<CONFIG:Release>:/NODEFAULTLIB:debug.lib>   # Exclude debug library in release
        $<$<CONFIG:Release>:/OPT:REF>                  # Eliminate unreferenced functions
        $<$<CONFIG:Release>:/OPT:ICF>                  # Enable COMDAT folding
        $<$<CONFIG:Release>:/LTCG>                     # Link-time code generation
        $<$<CONFIG:Release>:/DEBUG>                    # Generate debug info (PDB)
        $<$<CONFIG:RelWithDebInfo>:/OPT:REF>
        $<$<CONFIG:RelWithDebInfo>:/OPT:ICF>
        $<$<CONFIG:RelWithDebInfo>:/DEBUG>
    )

    # Generate map files for debugging
    target_link_options(RTS PRIVATE
        $<$<CONFIG:Debug>:/MAP:${CMAKE_SOURCE_DIR}/../Run/RTSD.map>
        $<$<CONFIG:Release>:/MAP:${CMAKE_SOURCE_DIR}/../Run/RTS.map>
        $<$<CONFIG:RelWithDebInfo>:/MAP:${CMAKE_SOURCE_DIR}/../Run/RTSI.map>
    )

    # PDB files
    set_target_properties(RTS PROPERTIES
        PDB_NAME_DEBUG "RTSD"
        PDB_NAME_RELEASE "RTS"
        PDB_NAME_RELWITHDEBINFO "RTSI"
        PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../Run"
    )
else()
    # Linux linker flags
    target_link_options(RTS PRIVATE
        $<$<CONFIG:Debug>:-rdynamic>  # Export symbols for better stack traces
        $<$<CONFIG:Release>:-s>        # Strip symbols in release
    )
endif()

# ==============================================================================
# Build Messages
# ==============================================================================

message(STATUS "Configuring RTS Executable")
message(STATUS "  Output Directory: ${CMAKE_SOURCE_DIR}/../Run")
message(STATUS "  Debug Name:       RTSD.exe")
message(STATUS "  Release Name:     RTS.exe")
message(STATUS "  RelWithDebInfo:   RTSI.exe")

# ==============================================================================
# Post-Build Actions
# ==============================================================================

# Copy any required DLLs or data files (if needed)
# add_custom_command(TARGET RTS POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         "${CMAKE_SOURCE_DIR}/path/to/required.dll"
#         $<TARGET_FILE_DIR:RTS>
#     COMMENT "Copying required runtime files..."
# )

# ==============================================================================
# Installation (Optional)
# ==============================================================================

install(TARGETS RTS
    RUNTIME DESTINATION bin
    COMPONENT runtime
)

# Install PDB files for debugging (Windows only)
if(MSVC)
    install(FILES
        $<TARGET_PDB_FILE:RTS>
        DESTINATION bin
        COMPONENT debug
        OPTIONAL
    )
endif()

# ==============================================================================
# Development Notes
# ==============================================================================

# This executable links the following major components:
# - GameEngine: Core game logic and systems
# - GameEngineDevice: Platform-specific rendering and input
# - WWVegas Libraries: Math, 3D rendering, debugging utilities
# - GameSpy SDK: Network multiplayer (stub implementations)
# - Compression: ZLib, LZH, EAC compression libraries
# - Benchmark: Performance testing utilities
#
# The executable outputs different names based on build configuration to allow
# multiple builds to coexist in the same Run directory for testing purposes.
