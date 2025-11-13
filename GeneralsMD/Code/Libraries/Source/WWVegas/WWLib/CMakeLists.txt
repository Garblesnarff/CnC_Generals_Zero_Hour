# WWLib Library - Core utility library
cmake_minimum_required(VERSION 3.15)

# Collect all source files
file(GLOB WWLIB_SOURCES "*.cpp")
file(GLOB WWLIB_HEADERS "*.h" "*.H")

# Create static library
add_library(WWLib STATIC
    ${WWLIB_SOURCES}
    ${WWLIB_HEADERS}
)

# Set target properties
set_target_properties(WWLib PROPERTIES
    OUTPUT_NAME "WWLib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Lib"
)

# Include directories
target_include_directories(WWLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwdebug
)

# Compiler definitions
target_compile_definitions(WWLib PRIVATE
    WIN32
    WINVER=0x400
    _WINDOWS
    WIN32_LEAN_AND_MEAN
    REGEX_MALLOC
    STDC_HEADERS
    $<$<CONFIG:Debug>:_DEBUG;WWDEBUG>
    $<$<CONFIG:Release>:NDEBUG;IG_DEBUG_STACKTRACE>
)

# Compiler flags
if(MSVC)
    target_compile_options(WWLib PRIVATE
        /W3
        /WX
        $<$<CONFIG:Release>:/O2 /Ob2>
        $<$<CONFIG:Debug>:/Od /Zi>
    )
else()
    target_compile_options(WWLib PRIVATE
        -Wall
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:Debug>:-g -O0>
    )
endif()

# Link dependencies
target_link_libraries(WWLib
    PRIVATE
        WWDebug
)
