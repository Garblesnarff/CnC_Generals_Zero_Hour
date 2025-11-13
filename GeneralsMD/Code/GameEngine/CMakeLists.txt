# ==============================================================================
# GameEngine Library - Core Game Engine
# ==============================================================================
# This is the main game engine library that includes:
# - Common: Core game systems (memory, player, team, thing, etc.)
# - GameLogic: AI, objects, weapons, modules, terrain, etc.
# - GameClient: Rendering, GUI, display systems
# - GameNetwork: Multiplayer networking and GameSpy integration
# ==============================================================================

cmake_minimum_required(VERSION 3.15)

# ==============================================================================
# Collect Source Files
# ==============================================================================

# Common subsystem - Core game functionality
file(GLOB_RECURSE COMMON_SOURCES
    "Source/Common/*.cpp"
)

# GameLogic subsystem - Game logic, AI, and simulation
file(GLOB_RECURSE GAMELOGIC_SOURCES
    "Source/GameLogic/*.cpp"
)

# GameClient subsystem - Rendering, GUI, and client systems
file(GLOB_RECURSE GAMECLIENT_SOURCES
    "Source/GameClient/*.cpp"
)

# GameNetwork subsystem - Multiplayer and networking
file(GLOB_RECURSE GAMENETWORK_SOURCES
    "Source/GameNetwork/*.cpp"
)

# Collect all header files for IDE organization
file(GLOB_RECURSE GAMEENGINE_HEADERS
    "Include/*.h"
    "Source/*.h"
)

# Combine all sources
set(GAMEENGINE_SOURCES
    ${COMMON_SOURCES}
    ${GAMELOGIC_SOURCES}
    ${GAMECLIENT_SOURCES}
    ${GAMENETWORK_SOURCES}
)

# ==============================================================================
# Create Static Library
# ==============================================================================

add_library(GameEngine STATIC
    ${GAMEENGINE_SOURCES}
    ${GAMEENGINE_HEADERS}
)

# ==============================================================================
# Target Properties
# ==============================================================================

set_target_properties(GameEngine PROPERTIES
    OUTPUT_NAME "GameEngine"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Lib"
)

# ==============================================================================
# Include Directories
# ==============================================================================

target_include_directories(GameEngine
    PUBLIC
        # Public headers for consumers of this library
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/Include/Common
        ${CMAKE_CURRENT_SOURCE_DIR}/Include/GameLogic
        ${CMAKE_CURRENT_SOURCE_DIR}/Include/GameClient
        ${CMAKE_CURRENT_SOURCE_DIR}/Include/GameNetwork
        ${CMAKE_CURRENT_SOURCE_DIR}/Include/Precompiled
    PRIVATE
        # Private implementation headers
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Common
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/GameLogic
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/GameClient
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/GameNetwork

        # WWVegas libraries
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/WWVegas
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/WWVegas/WWLib
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/WWVegas/WWMath
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/WWVegas/WWDebug
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/WWVegas/WWUtil
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/WWVegas/WWSaveLoad
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/WWVegas/WW3D2
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/WWVegas/wwshade
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/WWVegas/WWDownload
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/WWVegas/wwaudio
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/WWVegas/wwcpuid

        # GameSpy SDK
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/GameSpy
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/GameSpy/ghttp
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/GameSpy/peer
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/GameSpy/GP
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/GameSpy/gstats

        # Other libraries
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/Compression
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/Benchmark
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Source/EABrowserDispatch
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Include
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Include/Granny
)

# ==============================================================================
# Compiler Definitions
# ==============================================================================

target_compile_definitions(GameEngine PRIVATE
    WIN32
    _WINDOWS
    WINVER=0x0400
    _WIN32_WINNT=0x0400
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    _MBCS
    Z_PREFIX

    # Game-specific defines
    _GAMEENGINE_
    GAME_DLL=0

    # Configuration-specific defines
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Debug>:WWDEBUG>
    $<$<CONFIG:Debug>:BROWSER_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:Release>:IG_DEBUG_STACKTRACE>
)

# ==============================================================================
# Compiler Flags
# ==============================================================================

if(MSVC)
    target_compile_options(GameEngine PRIVATE
        /W3                     # Warning level 3
        /WX                     # Treat warnings as errors
        /GX                     # Enable C++ exception handling
        /Zc:wchar_t             # wchar_t is native type
        /Zc:forScope            # Force conformance in for loop scope

        # Debug configuration
        $<$<CONFIG:Debug>:/MDd>  # Multi-threaded Debug DLL runtime
        $<$<CONFIG:Debug>:/Gm>   # Enable minimal rebuild
        $<$<CONFIG:Debug>:/ZI>   # Edit and Continue debug info
        $<$<CONFIG:Debug>:/Od>   # Disable optimization
        $<$<CONFIG:Debug>:/GZ>   # Enable runtime debug checks
        $<$<CONFIG:Debug>:/RTC1> # Runtime checks

        # Release configuration
        $<$<CONFIG:Release>:/MD>  # Multi-threaded DLL runtime
        $<$<CONFIG:Release>:/O2>  # Maximize speed
        $<$<CONFIG:Release>:/Ob2> # Inline function expansion
        $<$<CONFIG:Release>:/Oi>  # Enable intrinsic functions
        $<$<CONFIG:Release>:/Ot>  # Favor fast code
        $<$<CONFIG:Release>:/Oy>  # Omit frame pointers
        $<$<CONFIG:Release>:/GL>  # Whole program optimization
        $<$<CONFIG:Release>:/Zi>  # Debug information (for PDB)
    )

    # Disable specific warnings for legacy code
    target_compile_options(GameEngine PRIVATE
        /wd4244 # conversion from 'type1' to 'type2', possible loss of data
        /wd4267 # conversion from 'size_t' to 'type', possible loss of data
        /wd4996 # deprecated function warnings
        /wd4018 # signed/unsigned mismatch
        /wd4800 # forcing value to bool 'true' or 'false'
    )

    # Link-time optimizations for Release
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set_target_properties(GameEngine PROPERTIES
            STATIC_LIBRARY_FLAGS "/LTCG"
        )
    endif()

else()
    # GCC/Clang flags
    target_compile_options(GameEngine PRIVATE
        -Wall
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-sign-compare
        -Wno-reorder
        -fexceptions
        -fno-strict-aliasing

        $<$<CONFIG:Debug>:-g3>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-fno-inline>

        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-march=native>
    )
endif()

# ==============================================================================
# Precompiled Header
# ==============================================================================

if(USE_PRECOMPILED_HEADERS AND COMMAND target_precompile_headers)
    target_precompile_headers(GameEngine PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Include/Precompiled/PreRTS.h"
    )
endif()

# ==============================================================================
# Link Dependencies
# ==============================================================================

target_link_libraries(GameEngine
    PUBLIC
        # WWVegas core libraries
        WWLib
        WWMath
        WW3D2

    PRIVATE
        # WWVegas additional libraries
        WWDebug
        wwutil
        WWSaveLoad
        wwshade
        WWDownload

        # Utility libraries
        Compression
        Benchmark

        # GameSpy SDK (interface libraries)
        # Note: GameSpy libraries are stubs/interface libraries
        # The actual SDK may need to be obtained separately
)

# Note: EABrowserDispatch is an IDL file and doesn't produce a linkable library
# The generated files would be used directly if COM browser dispatch is needed

# ==============================================================================
# Source Organization for IDEs
# ==============================================================================

# Organize sources in IDE project tree
source_group("Source Files\\Common" FILES ${COMMON_SOURCES})
source_group("Source Files\\GameLogic" FILES ${GAMELOGIC_SOURCES})
source_group("Source Files\\GameClient" FILES ${GAMECLIENT_SOURCES})
source_group("Source Files\\GameNetwork" FILES ${GAMENETWORK_SOURCES})
source_group("Header Files" FILES ${GAMEENGINE_HEADERS})

# ==============================================================================
# Build Information
# ==============================================================================

message(STATUS "Configuring GameEngine library:")
message(STATUS "  Total source files: ${CMAKE_MATCH_COUNT}")
message(STATUS "  Common sources: ${COMMON_SOURCES}")
message(STATUS "  GameLogic sources: ${GAMELOGIC_SOURCES}")
message(STATUS "  GameClient sources: ${GAMECLIENT_SOURCES}")
message(STATUS "  GameNetwork sources: ${GAMENETWORK_SOURCES}")
message(STATUS "  Output: ${CMAKE_CURRENT_SOURCE_DIR}/Lib/GameEngine.lib")

# ==============================================================================
# Platform-Specific Settings
# ==============================================================================

if(WIN32)
    # Windows-specific settings
    target_compile_definitions(GameEngine PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _CRT_NONSTDC_NO_WARNINGS
    )
endif()

if(UNIX)
    # Unix/Linux-specific settings
    target_compile_definitions(GameEngine PRIVATE
        PLATFORM_LINUX
    )

    # Link pthread for threading support
    target_link_libraries(GameEngine PRIVATE pthread)
endif()

# ==============================================================================
# Installation (Optional)
# ==============================================================================

# Install library (if needed for distribution)
install(TARGETS GameEngine
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

# Install public headers
install(DIRECTORY Include/
    DESTINATION include/GameEngine
    FILES_MATCHING PATTERN "*.h"
)

# ==============================================================================
# End of GameEngine CMakeLists.txt
# ==============================================================================
