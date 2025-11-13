# WW3D2 Library - 3D rendering engine
cmake_minimum_required(VERSION 3.15)

# Collect all source files recursively
file(GLOB_RECURSE WW3D2_SOURCES "*.cpp")
file(GLOB_RECURSE WW3D2_HEADERS "*.h")

# Create static library
add_library(WW3D2 STATIC
    ${WW3D2_SOURCES}
    ${WW3D2_HEADERS}
)

# Set target properties
set_target_properties(WW3D2 PROPERTIES
    OUTPUT_NAME "ww3d2"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Lib"
)

# Include directories
target_include_directories(WW3D2
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwlib
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwmath
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwdebug
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwcpuid
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwsaveload
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwaudio
        ${CMAKE_CURRENT_SOURCE_DIR}/../miles6/include
)

# Compiler definitions
target_compile_definitions(WW3D2 PRIVATE
    WIN32
    WINVER=0x400
    _WINDOWS
    WIN32_LEAN_AND_MEAN
    $<$<CONFIG:Debug>:_DEBUG;WWDEBUG>
    $<$<CONFIG:Release>:NDEBUG;IG_DEBUG_STACKTRACE>
)

# Compiler flags
if(MSVC)
    target_compile_options(WW3D2 PRIVATE
        /W3
        /WX
        $<$<CONFIG:Release>:/O2 /Ob2>
        $<$<CONFIG:Debug>:/Od /Zi>
    )
else()
    target_compile_options(WW3D2 PRIVATE
        -Wall
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:Debug>:-g -O0>
    )
endif()

# Link dependencies
target_link_libraries(WW3D2
    PUBLIC
        WWLib
        WWMath
    PRIVATE
        WWDebug
)
