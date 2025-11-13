# WWMath Library - Math utilities and collision detection
cmake_minimum_required(VERSION 3.15)

# Collect all source files
set(WWMATH_SOURCES
    aabox.cpp
    aabtreecull.cpp
    cardinalspline.cpp
    catmullromspline.cpp
    colmath.cpp
    colmathaabox.cpp
    colmathaabtri.cpp
    colmathfrustum.cpp
    colmathline.cpp
    colmathobbobb.cpp
    colmathobbox.cpp
    colmathobbtri.cpp
    colmathplane.cpp
    colmathsphere.cpp
    cullsys.cpp
    curve.cpp
    euler.cpp
    frustum.cpp
    gridcull.cpp
    hermitespline.cpp
    lineseg.cpp
    lookuptable.cpp
    matrix3.cpp
    matrix3d.cpp
    matrix4.cpp
    obbox.cpp
    ode.cpp
    pot.cpp
    quat.cpp
    tcbspline.cpp
    tri.cpp
    v3_rnd.cpp
    vehiclecurve.cpp
    vp.cpp
    wwmath.cpp
)

# Collect all header files
set(WWMATH_HEADERS
    aabox.h
    aabtreecull.h
    aaplane.h
    cardinalspline.h
    castres.h
    catmullromspline.h
    colmath.h
    colmathaabox.h
    colmathfrustum.h
    colmathinlines.h
    colmathline.h
    colmathplane.h
    cullsys.h
    culltype.h
    curve.h
    euler.h
    frustum.h
    gridcull.h
    hermitespline.h
    lineseg.h
    lookuptable.h
    matrix3.h
    matrix3d.h
    matrix4.h
    obbox.h
    ode.h
    plane.h
    pot.h
    quat.h
    rect.h
    sphere.h
    tcbspline.h
    tri.h
    v3_rnd.h
    vector2.h
    vector2i.h
    vector3.h
    Vector3i.h
    vector4.h
    vehiclecurve.h
    vp.h
    wwmath.h
    wwmathids.h
)

# Create static library
add_library(WWMath STATIC
    ${WWMATH_SOURCES}
    ${WWMATH_HEADERS}
)

# Set target properties
set_target_properties(WWMath PROPERTIES
    OUTPUT_NAME "WWMath"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Libraries/Lib"
)

# Include directories
target_include_directories(WWMath
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwlib
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwdebug
        ${CMAKE_CURRENT_SOURCE_DIR}/../wwsaveload
)

# Compiler definitions
target_compile_definitions(WWMath PRIVATE
    WIN32
    WINVER=0x400
    _WINDOWS
    WIN32_LEAN_AND_MEAN
    $<$<CONFIG:Debug>:_DEBUG;WWDEBUG;G_CODE_BASE;DIRECTX>
    $<$<CONFIG:Release>:NDEBUG;IG_DEBUG_STACKTRACE>
)

# Compiler flags
if(MSVC)
    target_compile_options(WWMath PRIVATE
        /W3
        /WX
        $<$<CONFIG:Release>:/O2 /Ob2>
        $<$<CONFIG:Debug>:/Od /Zi>
    )
else()
    target_compile_options(WWMath PRIVATE
        -Wall
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:Debug>:-g -O0>
    )
endif()

# Dependencies (if needed)
# WWMath depends on WWLib and WWDebug headers
