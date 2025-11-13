# Compression Library (ZLib + EAC + LZH)
cmake_minimum_required(VERSION 3.15)

# Collect ZLib source files
file(GLOB ZLIB_SOURCES
    "ZLib/*.c"
)

# Collect LZH compression sources
file(GLOB LZH_SOURCES
    "LZHCompress/CompLibSource/*.cpp"
    "LZHCompress/*.cpp"
)

# Collect EAC compression sources
file(GLOB EAC_SOURCES
    "EAC/*.cpp"
)

# Main compression manager
set(COMPRESSION_SOURCES
    CompressionManager.cpp
)

# Create static library
add_library(Compression STATIC
    ${ZLIB_SOURCES}
    ${LZH_SOURCES}
    ${EAC_SOURCES}
    ${COMPRESSION_SOURCES}
)

# Set target properties
set_target_properties(Compression PROPERTIES
    OUTPUT_NAME "Compression"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Lib"
)

# Include directories
target_include_directories(Compression
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ZLib
        ${CMAKE_CURRENT_SOURCE_DIR}/LZHCompress
        ${CMAKE_CURRENT_SOURCE_DIR}/EAC
    PRIVATE
        ${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Include
)

# Compiler definitions
target_compile_definitions(Compression PRIVATE
    WIN32
    Z_PREFIX
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG;_RELEASE>
    $<$<CONFIG:Release>:IG_DEBUG_STACKTRACE>
)

# Compiler flags
if(MSVC)
    target_compile_options(Compression PRIVATE
        /W3
        /WX
        $<$<CONFIG:Release>:/O2 /Ob2>
        $<$<CONFIG:Debug>:/Od /Zi>
    )
else()
    target_compile_options(Compression PRIVATE
        -Wall
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:Debug>:-g -O0>
    )
endif()

# Note: ZLib sources are gitignored and may need to be obtained separately
# The standard zlib 1.x library should work with this configuration
