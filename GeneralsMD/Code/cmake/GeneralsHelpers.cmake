# ==============================================================================
# Command & Conquer: Generals Zero Hour - CMake Helper Functions
# ==============================================================================
# This module provides common functions and macros for building Generals ZH
# components in a consistent manner across all subdirectories.
# ==============================================================================

include_guard(GLOBAL)

# ==============================================================================
# Function: generals_add_library
# ==============================================================================
# Creates a static library with standard Generals configuration
#
# Usage:
#   generals_add_library(
#       NAME <library_name>
#       SOURCES <source_files...>
#       [HEADERS <header_files...>]
#       [INCLUDES <include_dirs...>]
#       [DEPENDS <dependencies...>]
#       [DEFINES <preprocessor_defines...>]
#       [PCH <precompiled_header>]
#   )
#
# Example:
#   generals_add_library(
#       NAME WWMath
#       SOURCES vector.cpp matrix.cpp quaternion.cpp
#       HEADERS vector.h matrix.h quaternion.h
#       INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include
#       DEFINES WWMATH_EXPORTS
#   )
# ==============================================================================

function(generals_add_library)
    set(options "")
    set(oneValueArgs NAME PCH)
    set(multiValueArgs SOURCES HEADERS INCLUDES DEPENDS DEFINES)
    cmake_parse_arguments(LIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT LIB_NAME)
        message(FATAL_ERROR "generals_add_library: NAME is required")
    endif()

    if(NOT LIB_SOURCES)
        message(FATAL_ERROR "generals_add_library: SOURCES is required")
    endif()

    # Create the library
    add_library(${LIB_NAME} STATIC ${LIB_SOURCES} ${LIB_HEADERS})

    # Set library output name based on configuration
    set_target_properties(${LIB_NAME} PROPERTIES
        OUTPUT_NAME "${LIB_NAME}$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Internal>:Internal>$<$<CONFIG:Profile>:Profile>"
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Libraries/Lib
        POSITION_INDEPENDENT_CODE ON
    )

    # Add common include directories
    target_include_directories(${LIB_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${LIB_INCLUDES}
        PRIVATE
            ${GENERALS_COMMON_INCLUDES}
    )

    # Add preprocessor definitions
    if(LIB_DEFINES)
        target_compile_definitions(${LIB_NAME} PRIVATE ${LIB_DEFINES})
    endif()

    # Add library dependencies
    if(LIB_DEPENDS)
        target_link_libraries(${LIB_NAME} PUBLIC ${LIB_DEPENDS})
    endif()

    # Precompiled headers
    if(USE_PRECOMPILED_HEADERS AND LIB_PCH)
        target_precompile_headers(${LIB_NAME} PRIVATE ${LIB_PCH})
    endif()

    # Organize in IDE
    get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
    get_filename_component(PARENT_NAME ${PARENT_DIR} NAME)
    set_target_properties(${LIB_NAME} PROPERTIES FOLDER "Libraries/${PARENT_NAME}")

    message(STATUS "  [+] Library: ${LIB_NAME}")
endfunction()

# ==============================================================================
# Function: generals_add_executable
# ==============================================================================
# Creates an executable with standard Generals configuration
#
# Usage:
#   generals_add_executable(
#       NAME <executable_name>
#       SOURCES <source_files...>
#       [HEADERS <header_files...>]
#       [INCLUDES <include_dirs...>]
#       [DEPENDS <dependencies...>]
#       [DEFINES <preprocessor_defines...>]
#       [LIBS <external_libraries...>]
#       [PCH <precompiled_header>]
#       [WIN32]
#   )
# ==============================================================================

function(generals_add_executable)
    set(options WIN32)
    set(oneValueArgs NAME PCH)
    set(multiValueArgs SOURCES HEADERS INCLUDES DEPENDS DEFINES LIBS)
    cmake_parse_arguments(EXE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT EXE_NAME)
        message(FATAL_ERROR "generals_add_executable: NAME is required")
    endif()

    if(NOT EXE_SOURCES)
        message(FATAL_ERROR "generals_add_executable: SOURCES is required")
    endif()

    # Create the executable
    if(EXE_WIN32 AND WIN32)
        add_executable(${EXE_NAME} WIN32 ${EXE_SOURCES} ${EXE_HEADERS})
    else()
        add_executable(${EXE_NAME} ${EXE_SOURCES} ${EXE_HEADERS})
    endif()

    # Set executable output name based on configuration
    set_target_properties(${EXE_NAME} PROPERTIES
        OUTPUT_NAME "${EXE_NAME}$<$<CONFIG:Debug>:D>$<$<CONFIG:Internal>:I>$<$<CONFIG:Profile>:P>"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Run
    )

    # Add common include directories
    target_include_directories(${EXE_NAME}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${LIB_INCLUDES}
            ${GENERALS_COMMON_INCLUDES}
    )

    # Add preprocessor definitions
    if(EXE_DEFINES)
        target_compile_definitions(${EXE_NAME} PRIVATE ${EXE_DEFINES})
    endif()

    # Link dependencies
    if(EXE_DEPENDS)
        target_link_libraries(${EXE_NAME} PRIVATE ${EXE_DEPENDS})
    endif()

    # Link external libraries
    if(EXE_LIBS)
        target_link_libraries(${EXE_NAME} PRIVATE ${EXE_LIBS})
    endif()

    # Precompiled headers
    if(USE_PRECOMPILED_HEADERS AND EXE_PCH)
        target_precompile_headers(${EXE_NAME} PRIVATE ${EXE_PCH})
    endif()

    # Organize in IDE
    set_target_properties(${EXE_NAME} PROPERTIES FOLDER "Executables")

    message(STATUS "  [+] Executable: ${EXE_NAME}")
endfunction()

# ==============================================================================
# Function: generals_add_tool
# ==============================================================================
# Creates a development tool executable
# Similar to generals_add_executable but with different defaults
# ==============================================================================

function(generals_add_tool)
    set(options "")
    set(oneValueArgs NAME PCH)
    set(multiValueArgs SOURCES HEADERS INCLUDES DEPENDS DEFINES LIBS)
    cmake_parse_arguments(TOOL "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT TOOL_NAME)
        message(FATAL_ERROR "generals_add_tool: NAME is required")
    endif()

    if(NOT TOOL_SOURCES)
        message(FATAL_ERROR "generals_add_tool: SOURCES is required")
    endif()

    # Create the executable (tools are always WIN32 GUI apps on Windows)
    if(WIN32)
        add_executable(${TOOL_NAME} WIN32 ${TOOL_SOURCES} ${TOOL_HEADERS})
    else()
        add_executable(${TOOL_NAME} ${TOOL_SOURCES} ${TOOL_HEADERS})
    endif()

    # Tools go to Run directory with other executables
    set_target_properties(${TOOL_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Run
    )

    # Add include directories
    target_include_directories(${TOOL_NAME}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${TOOL_INCLUDES}
            ${GENERALS_COMMON_INCLUDES}
    )

    # Add definitions
    if(TOOL_DEFINES)
        target_compile_definitions(${TOOL_NAME} PRIVATE ${TOOL_DEFINES})
    endif()

    # Link dependencies
    if(TOOL_DEPENDS)
        target_link_libraries(${TOOL_NAME} PRIVATE ${TOOL_DEPENDS})
    endif()

    # Link external libraries
    if(TOOL_LIBS)
        target_link_libraries(${TOOL_NAME} PRIVATE ${TOOL_LIBS})
    endif()

    # Precompiled headers
    if(USE_PRECOMPILED_HEADERS AND TOOL_PCH)
        target_precompile_headers(${TOOL_NAME} PRIVATE ${TOOL_PCH})
    endif()

    # Organize in IDE
    set_target_properties(${TOOL_NAME} PROPERTIES FOLDER "Tools")

    message(STATUS "  [+] Tool: ${TOOL_NAME}")
endfunction()

# ==============================================================================
# Function: generals_glob_sources
# ==============================================================================
# Globs source files from current directory
# Follows Generals convention of looking for .cpp, .c, .h files
#
# Usage:
#   generals_glob_sources(VAR_NAME)
#
# Example:
#   generals_glob_sources(SOURCES)
#   generals_add_library(NAME MyLib SOURCES ${SOURCES})
# ==============================================================================

function(generals_glob_sources OUT_VAR)
    file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    )
    file(GLOB_RECURSE HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    )
    set(${OUT_VAR} ${SOURCES} ${HEADERS} PARENT_SCOPE)
endfunction()

# ==============================================================================
# Function: generals_source_group_by_dir
# ==============================================================================
# Organizes source files into IDE folders based on directory structure
#
# Usage:
#   generals_source_group_by_dir(source_files...)
# ==============================================================================

function(generals_source_group_by_dir)
    foreach(FILE ${ARGN})
        get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

        # Get relative path from current source directory
        file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${PARENT_DIR}")

        if(NOT REL_PATH OR REL_PATH STREQUAL "")
            set(REL_PATH "Source Files")
        endif()

        # Convert to Windows-style path separators for Visual Studio
        string(REPLACE "/" "\\" GROUP_PATH "${REL_PATH}")

        source_group("${GROUP_PATH}" FILES "${FILE}")
    endforeach()
endfunction()

# ==============================================================================
# Function: generals_disable_warnings
# ==============================================================================
# Disables specific warnings for legacy code
#
# Usage:
#   generals_disable_warnings(target warning_numbers...)
#
# Example:
#   generals_disable_warnings(MyLib 4244 4267 4996)
# ==============================================================================

function(generals_disable_warnings TARGET)
    if(MSVC)
        foreach(WARNING ${ARGN})
            target_compile_options(${TARGET} PRIVATE /wd${WARNING})
        endforeach()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # GCC/Clang warning names are different
        target_compile_options(${TARGET} PRIVATE -w)
    endif()
endfunction()

# ==============================================================================
# Function: generals_copy_runtime_files
# ==============================================================================
# Copies DLLs and runtime files to output directory
#
# Usage:
#   generals_copy_runtime_files(target file1 file2 ...)
# ==============================================================================

function(generals_copy_runtime_files TARGET)
    foreach(FILE ${ARGN})
        if(EXISTS ${FILE})
            add_custom_command(TARGET ${TARGET} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${FILE}
                    $<TARGET_FILE_DIR:${TARGET}>
                COMMENT "Copying ${FILE} to output directory"
            )
        endif()
    endforeach()
endfunction()

# ==============================================================================
# Function: generals_set_debug_working_dir
# ==============================================================================
# Sets the working directory for debugging in Visual Studio
#
# Usage:
#   generals_set_debug_working_dir(target directory)
# ==============================================================================

function(generals_set_debug_working_dir TARGET DIR)
    if(MSVC)
        set_target_properties(${TARGET} PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${DIR}"
        )
    endif()
endfunction()

# ==============================================================================
# Macro: generals_require_libraries
# ==============================================================================
# Checks that required libraries exist before proceeding
#
# Usage:
#   generals_require_libraries(lib1 lib2 lib3)
# ==============================================================================

macro(generals_require_libraries)
    foreach(LIB ${ARGN})
        if(NOT TARGET ${LIB})
            message(FATAL_ERROR "Required library '${LIB}' not found. Check build order.")
        endif()
    endforeach()
endmacro()

# ==============================================================================
# Function: generals_add_version_info
# ==============================================================================
# Adds version information resource to Windows executables
#
# Usage:
#   generals_add_version_info(target rc_file)
# ==============================================================================

function(generals_add_version_info TARGET RC_FILE)
    if(WIN32 AND EXISTS ${RC_FILE})
        target_sources(${TARGET} PRIVATE ${RC_FILE})
    endif()
endfunction()

# ==============================================================================
# Platform-Specific Helpers
# ==============================================================================

# Check if we're building for 32-bit or 64-bit
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(GENERALS_64BIT TRUE)
    set(GENERALS_PLATFORM_BITS "x64")
else()
    set(GENERALS_64BIT FALSE)
    set(GENERALS_PLATFORM_BITS "x86")
endif()

message(STATUS "Generals CMake Helpers loaded (${GENERALS_PLATFORM_BITS})")

# ==============================================================================
# End of GeneralsHelpers.cmake
# ==============================================================================
