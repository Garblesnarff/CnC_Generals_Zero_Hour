# WWUtil Library - Utility functions
cmake_minimum_required(VERSION 3.15)

# Collect all source files
set(WWUTIL_SOURCES
    mathutil.cpp
    miscutil.cpp
)

# Collect all header files
file(GLOB WWUTIL_HEADERS "*.h")

# Create static library
add_library(WWUtil STATIC
    ${WWUTIL_SOURCES}
    ${WWUTIL_HEADERS}
)

# Set target properties
set_target_properties(WWUtil PROPERTIES
    OUTPUT_NAME "wwutil"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Lib"
)

# Include directories
target_include_directories(WWUtil
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwlib
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwmath
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwdebug
)

# Compiler definitions
target_compile_definitions(WWUtil PRIVATE
    WIN32
    WINVER=0x400
    _WINDOWS
    WIN32_LEAN_AND_MEAN
    $<$<CONFIG:Debug>:_DEBUG;WWDEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# Compiler flags
if(MSVC)
    target_compile_options(WWUtil PRIVATE
        /W3
        /WX
        $<$<CONFIG:Release>:/O2 /Ob2>
        $<$<CONFIG:Debug>:/Od /Zi>
    )
else()
    target_compile_options(WWUtil PRIVATE
        -Wall
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:Debug>:-g -O0>
    )
endif()

# Link dependencies
target_link_libraries(WWUtil
    PUBLIC
        WWLib
        WWMath
    PRIVATE
        WWDebug
)
