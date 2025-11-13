# ==============================================================================
# GameEngineDevice Library - Platform-Specific Game Engine Implementation
# ==============================================================================
# Provides device-specific implementations:
#   - W3DDevice: Direct3D rendering layer
#   - Win32Device: Windows platform layer (filesystem, input, display)
#   - VideoDevice: Bink video playback (stubbed)
#   - MilesAudioDevice: Miles Sound System audio (stubbed)
# ==============================================================================

cmake_minimum_required(VERSION 3.15)

# ==============================================================================
# Source Files Collection
# ==============================================================================

# W3DDevice - Direct3D rendering implementation
set(W3D_DEVICE_SOURCES
    # Common
    Source/W3DDevice/Common/W3DConvert.cpp
    Source/W3DDevice/Common/System/W3DFunctionLexicon.cpp
    Source/W3DDevice/Common/System/W3DRadar.cpp
    Source/W3DDevice/Common/Thing/W3DModuleFactory.cpp
    Source/W3DDevice/Common/Thing/W3DThingFactory.cpp

    # GameClient - Core
    Source/W3DDevice/GameClient/BaseHeightMap.cpp
    Source/W3DDevice/GameClient/FlatHeightMap.cpp
    Source/W3DDevice/GameClient/HeightMap.cpp
    Source/W3DDevice/GameClient/TerrainTex.cpp
    Source/W3DDevice/GameClient/TileData.cpp
    Source/W3DDevice/GameClient/WorldHeightMap.cpp
    Source/W3DDevice/GameClient/camerashakesystem.cpp

    # GameClient - Asset Management
    Source/W3DDevice/GameClient/W3DAssetManager.cpp
    Source/W3DDevice/GameClient/W3DAssetManagerExposed.cpp
    Source/W3DDevice/GameClient/W3DFileSystem.cpp

    # GameClient - Buffers
    Source/W3DDevice/GameClient/W3DBibBuffer.cpp
    Source/W3DDevice/GameClient/W3DBridgeBuffer.cpp
    Source/W3DDevice/GameClient/W3DPropBuffer.cpp
    Source/W3DDevice/GameClient/W3DRoadBuffer.cpp
    Source/W3DDevice/GameClient/W3DTreeBuffer.cpp
    Source/W3DDevice/GameClient/W3DVideoBuffer.cpp
    Source/W3DDevice/GameClient/W3dWaypointBuffer.cpp

    # GameClient - Rendering
    Source/W3DDevice/GameClient/W3DCustomEdging.cpp
    Source/W3DDevice/GameClient/W3DDebugDisplay.cpp
    Source/W3DDevice/GameClient/W3DDebugIcons.cpp
    Source/W3DDevice/GameClient/W3DDisplay.cpp
    Source/W3DDevice/GameClient/W3DDisplayString.cpp
    Source/W3DDevice/GameClient/W3DDisplayStringManager.cpp
    Source/W3DDevice/GameClient/W3DDynamicLight.cpp
    Source/W3DDevice/GameClient/W3DGameClient.cpp
    Source/W3DDevice/GameClient/W3DGranny.cpp
    Source/W3DDevice/GameClient/W3DInGameUI.cpp
    Source/W3DDevice/GameClient/W3DMouse.cpp
    Source/W3DDevice/GameClient/W3DParticleSys.cpp
    Source/W3DDevice/GameClient/W3DPoly.cpp
    Source/W3DDevice/GameClient/W3DScene.cpp
    Source/W3DDevice/GameClient/W3DShaderManager.cpp
    Source/W3DDevice/GameClient/W3DShroud.cpp
    Source/W3DDevice/GameClient/W3DSmudge.cpp
    Source/W3DDevice/GameClient/W3DSnow.cpp
    Source/W3DDevice/GameClient/W3DStatusCircle.cpp
    Source/W3DDevice/GameClient/W3DTerrainBackground.cpp
    Source/W3DDevice/GameClient/W3DTerrainTracks.cpp
    Source/W3DDevice/GameClient/W3DTerrainVisual.cpp
    Source/W3DDevice/GameClient/W3DView.cpp
    Source/W3DDevice/GameClient/W3DWebBrowser.cpp

    # GameClient - Shadows
    Source/W3DDevice/GameClient/Shadow/W3DBufferManager.cpp
    Source/W3DDevice/GameClient/Shadow/W3DProjectedShadow.cpp
    Source/W3DDevice/GameClient/Shadow/W3DShadow.cpp
    Source/W3DDevice/GameClient/Shadow/W3DVolumetricShadow.cpp

    # GameClient - Water
    Source/W3DDevice/GameClient/Water/W3DWater.cpp
    Source/W3DDevice/GameClient/Water/W3DWaterTracks.cpp

    # GameClient - GUI
    Source/W3DDevice/GameClient/GUI/W3DGameFont.cpp
    Source/W3DDevice/GameClient/GUI/W3DGameWindow.cpp
    Source/W3DDevice/GameClient/GUI/W3DGameWindowManager.cpp

    # GameClient - GUI Gadgets
    Source/W3DDevice/GameClient/GUI/Gadget/W3DCheckBox.cpp
    Source/W3DDevice/GameClient/GUI/Gadget/W3DComboBox.cpp
    Source/W3DDevice/GameClient/GUI/Gadget/W3DHorizontalSlider.cpp
    Source/W3DDevice/GameClient/GUI/Gadget/W3DListBox.cpp
    Source/W3DDevice/GameClient/GUI/Gadget/W3DProgressBar.cpp
    Source/W3DDevice/GameClient/GUI/Gadget/W3DPushButton.cpp
    Source/W3DDevice/GameClient/GUI/Gadget/W3DRadioButton.cpp
    Source/W3DDevice/GameClient/GUI/Gadget/W3DStaticText.cpp
    Source/W3DDevice/GameClient/GUI/Gadget/W3DTabControl.cpp
    Source/W3DDevice/GameClient/GUI/Gadget/W3DTextEntry.cpp
    Source/W3DDevice/GameClient/GUI/Gadget/W3DVerticalSlider.cpp

    # GameClient - GUI Callbacks
    Source/W3DDevice/GameClient/GUI/GUICallbacks/W3DControlBar.cpp
    Source/W3DDevice/GameClient/GUI/GUICallbacks/W3DMainMenu.cpp
    Source/W3DDevice/GameClient/GUI/GUICallbacks/W3DMOTD.cpp

    # GameClient - Drawable/Draw
    Source/W3DDevice/GameClient/Drawable/Draw/W3DDebrisDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DDefaultDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DDependencyModelDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DLaserDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DModelDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DOverlordAircraftDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DOverlordTankDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DOverlordTruckDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DPoliceCarDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DProjectileStreamDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DPropDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DRopeDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DScienceModelDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DSupplyDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DTankDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DTankTruckDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DTracerDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DTreeDraw.cpp
    Source/W3DDevice/GameClient/Drawable/Draw/W3DTruckDraw.cpp

    # GameLogic
    Source/W3DDevice/GameLogic/W3DGameLogic.cpp
    Source/W3DDevice/GameLogic/W3DGhostObject.cpp
    Source/W3DDevice/GameLogic/W3DTerrainLogic.cpp
)

# Win32Device - Windows platform layer
set(WIN32_DEVICE_SOURCES
    # Common
    Source/Win32Device/Common/Win32BIGFile.cpp
    Source/Win32Device/Common/Win32BIGFileSystem.cpp
    Source/Win32Device/Common/Win32CDManager.cpp
    Source/Win32Device/Common/Win32GameEngine.cpp
    Source/Win32Device/Common/Win32LocalFile.cpp
    Source/Win32Device/Common/Win32LocalFileSystem.cpp
    Source/Win32Device/Common/Win32OSDisplay.cpp

    # GameClient - Input
    Source/Win32Device/GameClient/Win32DIKeyboard.cpp
    Source/Win32Device/GameClient/Win32DIMouse.cpp
    Source/Win32Device/GameClient/Win32Mouse.cpp
)

# VideoDevice - Bink video (stubbed)
set(VIDEO_DEVICE_SOURCES
    Source/VideoDevice/Bink/BinkVideoPlayer.cpp
)

# MilesAudioDevice - Miles Sound System (stubbed)
set(MILES_AUDIO_DEVICE_SOURCES
    Source/MilesAudioDevice/MilesAudioManager.cpp
)

# Combine all source files
set(GAMEENGINE_DEVICE_SOURCES
    ${W3D_DEVICE_SOURCES}
    ${WIN32_DEVICE_SOURCES}
    ${VIDEO_DEVICE_SOURCES}
    ${MILES_AUDIO_DEVICE_SOURCES}
)

# ==============================================================================
# Library Target
# ==============================================================================

add_library(GameEngineDevice STATIC
    ${GAMEENGINE_DEVICE_SOURCES}
)

# ==============================================================================
# Output Configuration
# ==============================================================================

set_target_properties(GameEngineDevice PROPERTIES
    OUTPUT_NAME "GameEngineDevice"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Lib"
    DEBUG_POSTFIX "Debug"
)

# Override archive output directory for all configurations
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set_target_properties(GameEngineDevice PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${CMAKE_CURRENT_SOURCE_DIR}/Lib"
    )
endforeach()

# ==============================================================================
# Include Directories
# ==============================================================================

target_include_directories(GameEngineDevice
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
        ${CMAKE_SOURCE_DIR}/GameEngine/Include
        ${CMAKE_SOURCE_DIR}/Main
        ${CMAKE_SOURCE_DIR}/Libraries/Include
        ${CMAKE_SOURCE_DIR}/Libraries/Include/Granny
        ${CMAKE_SOURCE_DIR}/Libraries/Include/MSS
        ${CMAKE_SOURCE_DIR}/Libraries/Source
        ${CMAKE_SOURCE_DIR}/Libraries/Source/WWVegas
        ${CMAKE_SOURCE_DIR}/Libraries/Source/WWVegas/WW3D2
        ${CMAKE_SOURCE_DIR}/Libraries/Source/WWVegas/WWLib
        ${CMAKE_SOURCE_DIR}/Libraries/Source/WWVegas/WWDebug
        ${CMAKE_SOURCE_DIR}/Libraries/Source/WWVegas/WWMath
        ${CMAKE_SOURCE_DIR}/Libraries/Source/WWVegas/WWSaveLoad
        ${CMAKE_SOURCE_DIR}/Libraries/Source/Benchmark
)

# ==============================================================================
# Compiler Definitions
# ==============================================================================

target_compile_definitions(GameEngineDevice PRIVATE
    WIN32
    _WINDOWS
    _LIB
    WINVER=0x0400
    WIN32_LEAN_AND_MEAN
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG;_RELEASE;IG_DEBUG_STACKTRACE>
    $<$<CONFIG:RelWithDebInfo>:NDEBUG;_RELEASE>
)

# ==============================================================================
# Compiler Options
# ==============================================================================

if(MSVC)
    target_compile_options(GameEngineDevice PRIVATE
        /W3                         # Warning level 3
        /WX                         # Treat warnings as errors
        /GX                         # Enable exception handling
        $<$<CONFIG:Debug>:/MDd>     # Multi-threaded Debug DLL runtime
        $<$<CONFIG:Debug>:/Gm>      # Enable minimal rebuild
        $<$<CONFIG:Debug>:/ZI>      # Edit and Continue debug info
        $<$<CONFIG:Debug>:/Od>      # Disable optimization
        $<$<CONFIG:Debug>:/GZ>      # Enable runtime checks
        $<$<CONFIG:Release>:/MD>    # Multi-threaded DLL runtime
        $<$<CONFIG:Release>:/O2>    # Maximize speed
        $<$<CONFIG:Release>:/Ob2>   # Inline function expansion
        $<$<CONFIG:Release>:/G6>    # Optimize for Pentium Pro
    )
else()
    target_compile_options(GameEngineDevice PRIVATE
        -Wall
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:Debug>:-g -O0>
    )
endif()

# ==============================================================================
# Link Dependencies
# ==============================================================================

# Game Engine dependency
target_link_libraries(GameEngineDevice
    PUBLIC
        GameEngine
    PRIVATE
        # WWVegas libraries
        WW3D2
        WWMath
        WWLib
        WWDebug
)

# Windows-specific libraries
if(WIN32)
    target_link_libraries(GameEngineDevice
        PRIVATE
            # DirectX 8 libraries
            d3d8
            d3dx8
            dinput8
            dsound
            dxguid

            # System libraries
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
            winmm
            wsock32
            wininet
            imm32
            vfw32
    )
endif()

# ==============================================================================
# Source Groups (for IDE organization)
# ==============================================================================

source_group("W3DDevice\\Common" FILES
    Source/W3DDevice/Common/W3DConvert.cpp
)

source_group("W3DDevice\\Common\\System" FILES
    Source/W3DDevice/Common/System/W3DFunctionLexicon.cpp
    Source/W3DDevice/Common/System/W3DRadar.cpp
)

source_group("W3DDevice\\Common\\Thing" FILES
    Source/W3DDevice/Common/Thing/W3DModuleFactory.cpp
    Source/W3DDevice/Common/Thing/W3DThingFactory.cpp
)

source_group("W3DDevice\\GameClient" REGULAR_EXPRESSION "Source/W3DDevice/GameClient/[^/]+\\.cpp$")
source_group("W3DDevice\\GameClient\\Shadow" REGULAR_EXPRESSION "Source/W3DDevice/GameClient/Shadow/.*\\.cpp$")
source_group("W3DDevice\\GameClient\\Water" REGULAR_EXPRESSION "Source/W3DDevice/GameClient/Water/.*\\.cpp$")
source_group("W3DDevice\\GameClient\\GUI" REGULAR_EXPRESSION "Source/W3DDevice/GameClient/GUI/[^/]+\\.cpp$")
source_group("W3DDevice\\GameClient\\GUI\\Gadget" REGULAR_EXPRESSION "Source/W3DDevice/GameClient/GUI/Gadget/.*\\.cpp$")
source_group("W3DDevice\\GameClient\\GUI\\GUICallbacks" REGULAR_EXPRESSION "Source/W3DDevice/GameClient/GUI/GUICallbacks/.*\\.cpp$")
source_group("W3DDevice\\GameClient\\Drawable\\Draw" REGULAR_EXPRESSION "Source/W3DDevice/GameClient/Drawable/Draw/.*\\.cpp$")
source_group("W3DDevice\\GameLogic" REGULAR_EXPRESSION "Source/W3DDevice/GameLogic/.*\\.cpp$")

source_group("Win32Device\\Common" REGULAR_EXPRESSION "Source/Win32Device/Common/.*\\.cpp$")
source_group("Win32Device\\GameClient" REGULAR_EXPRESSION "Source/Win32Device/GameClient/.*\\.cpp$")

source_group("VideoDevice\\Bink" FILES
    Source/VideoDevice/Bink/BinkVideoPlayer.cpp
)

source_group("MilesAudioDevice" FILES
    Source/MilesAudioDevice/MilesAudioManager.cpp
)

# ==============================================================================
# Installation
# ==============================================================================

install(TARGETS GameEngineDevice
    ARCHIVE DESTINATION lib
)

install(DIRECTORY Include/
    DESTINATION include/GameEngineDevice
    FILES_MATCHING PATTERN "*.h"
)

# ==============================================================================
# Summary
# ==============================================================================

message(STATUS "Configured GameEngineDevice library:")
message(STATUS "  - W3DDevice sources:        ${CMAKE_CURRENT_LIST_DIR}/Source/W3DDevice/")
message(STATUS "  - Win32Device sources:      ${CMAKE_CURRENT_LIST_DIR}/Source/Win32Device/")
message(STATUS "  - VideoDevice sources:      ${CMAKE_CURRENT_LIST_DIR}/Source/VideoDevice/")
message(STATUS "  - MilesAudioDevice sources: ${CMAKE_CURRENT_LIST_DIR}/Source/MilesAudioDevice/")
message(STATUS "  - Output library:           ${CMAKE_CURRENT_SOURCE_DIR}/Lib/GameEngineDevice.lib")
