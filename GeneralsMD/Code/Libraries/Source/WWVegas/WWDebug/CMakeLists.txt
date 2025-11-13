# WWDebug Library - Debugging and profiling utilities
cmake_minimum_required(VERSION 3.15)

# Collect all source files
set(WWDEBUG_SOURCES
    wwdebug.cpp
    wwmemlog.cpp
    wwprofile.cpp
)

# Collect all header files
file(GLOB WWDEBUG_HEADERS "*.h")

# Create static library
add_library(WWDebug STATIC
    ${WWDEBUG_SOURCES}
    ${WWDEBUG_HEADERS}
)

# Set target properties
set_target_properties(WWDebug PROPERTIES
    OUTPUT_NAME "WWDebug"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Lib"
)

# Include directories
target_include_directories(WWDebug
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwlib
)

# Compiler definitions
target_compile_definitions(WWDebug PRIVATE
    WIN32
    WINVER=0x400
    _WINDOWS
    WIN32_LEAN_AND_MEAN
    $<$<CONFIG:Debug>:_DEBUG;WWDEBUG>
    $<$<CONFIG:Release>:NDEBUG;IG_DEBUG_STACKTRACE>
)

# Compiler flags
if(MSVC)
    target_compile_options(WWDebug PRIVATE
        /W3
        /WX
        $<$<CONFIG:Release>:/O2 /Ob2>
        $<$<CONFIG:Debug>:/Od /Zi>
    )
else()
    target_compile_options(WWDebug PRIVATE
        -Wall
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:Debug>:-g -O0>
    )
endif()
