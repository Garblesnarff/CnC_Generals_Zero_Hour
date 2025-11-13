# CMake Quick Reference for Generals Zero Hour

## Quick Build Commands

### Windows (Visual Studio)
```cmd
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A Win32
cmake --build . --config Release -j8
```

### Linux/macOS
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

## Common CMake Options

| Option | Description | Default |
|--------|-------------|---------|
| `-DCMAKE_BUILD_TYPE=<type>` | Build type: Debug, Release, RelWithDebInfo | Release |
| `-DBUILD_TOOLS=ON/OFF` | Build development tools | OFF |
| `-DBUILD_TESTS=ON/OFF` | Build test executables | OFF |
| `-DUSE_PRECOMPILED_HEADERS=ON/OFF` | Use PCH for faster builds | ON |
| `-DENABLE_PROFILE=ON/OFF` | Enable profiling support | OFF |

## Build Configurations

| Configuration | Optimization | Debug Info | Use Case |
|--------------|--------------|------------|----------|
| Debug | None (`/Od`) | Full (`/ZI`) | Development & debugging |
| Release | Maximum (`/O2`) | Minimal (`/Zi`) | Final builds, performance |
| RelWithDebInfo | Optimized (`/O2`) | Full (`/Zi`) | Profiling, debugging issues |

## Helper Functions for Subdirectories

### Add a Static Library
```cmake
generals_add_library(
    NAME MyLibrary
    SOURCES file1.cpp file2.cpp
    HEADERS file1.h file2.h
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include
    DEPENDS OtherLibrary
    DEFINES MY_DEFINE
    PCH PrecompiledHeader.h
)
```

### Add an Executable
```cmake
generals_add_executable(
    NAME MyApp
    SOURCES main.cpp app.cpp
    HEADERS app.h
    DEPENDS MyLibrary OtherLibrary
    LIBS ${PLATFORM_LIBS}
    WIN32  # Windows GUI app
)
```

### Add a Development Tool
```cmake
generals_add_tool(
    NAME MyTool
    SOURCES tool.cpp
    DEPENDS GameEngine GameEngineDevice
    LIBS ${PLATFORM_LIBS}
)
```

### Glob Source Files
```cmake
generals_glob_sources(ALL_SOURCES)
generals_add_library(NAME MyLib SOURCES ${ALL_SOURCES})
```

## Directory Structure

```
Code/
├── CMakeLists.txt              # Root (you are here)
├── cmake/
│   ├── config.h.in            # Configuration template
│   └── GeneralsHelpers.cmake   # Helper functions
├── Libraries/Source/           # Layer 1 libraries
├── GameEngine/                 # Layer 2
├── GameEngineDevice/           # Layer 2
└── Main/                       # Layer 3 - RTS executable
```

## Build Targets

| Target | Command | Description |
|--------|---------|-------------|
| `all` | `cmake --build .` | Build everything |
| `RTS` | `cmake --build . --target RTS` | Build only game executable |
| `run` | `cmake --build . --target run` | Build and run game |
| `clean` | `cmake --build . --target clean` | Clean build files |
| `clean-all` | `cmake --build . --target clean-all` | Clean everything |
| `docs` | `cmake --build . --target docs` | Generate documentation |

## Output Locations

| Type | Location | Example |
|------|----------|---------|
| Executables | `../Run/` | `RTS.exe`, `RTSD.exe` |
| Libraries | `Libraries/Lib/` | `GameEngine.lib` |
| Debug Symbols | `../Run/` | `RTS.pdb` |

## Common Build Scenarios

### Clean Rebuild
```bash
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j8
```

### Debug Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```

### Build with Tools
```bash
cmake .. -DBUILD_TOOLS=ON
cmake --build .
```

### Parallel Build
```bash
# Specify number of cores
cmake --build . -j8

# Or use all cores
cmake --build . -j$(nproc)  # Linux
cmake --build . -j$(sysctl -n hw.ncpu)  # macOS
```

### Verbose Output
```bash
cmake --build . --verbose
# or
make VERBOSE=1
# or
ninja -v
```

## Troubleshooting

### "Target not found"
- Check build order in root CMakeLists.txt
- Ensure dependencies are built first

### "Header not found"
- Add include directory to `GENERALS_COMMON_INCLUDES`
- Or use `target_include_directories()`

### PCH Errors
```bash
cmake .. -DUSE_PRECOMPILED_HEADERS=OFF
```

### Slow Builds
```bash
# Use Ninja
cmake .. -G Ninja

# Enable ccache
cmake .. -DCMAKE_CXX_COMPILER_LAUNCHER=ccache

# Parallel builds
cmake --build . -j8
```

## IDE-Specific Tips

### Visual Studio
- Open folder in VS 2019+
- Or generate .sln: `cmake .. -G "Visual Studio 17 2022"`
- Set working directory for debugging

### CLion
- Open `Code` directory
- Auto-detects CMakeLists.txt
- Configure toolchain in Settings

### VS Code
- Install "CMake Tools" extension
- `Ctrl+Shift+P` → "CMake: Configure"
- `F7` to build

## Variables Available in Subdirectories

| Variable | Description |
|----------|-------------|
| `GENERALS_COMMON_INCLUDES` | Common include directories |
| `PLATFORM_LIBS` | Platform-specific system libraries |
| `PLATFORM_WINDOWS/LINUX/MACOS` | Platform detection flags |
| `USE_PRECOMPILED_HEADERS` | Whether to use PCH |
| `BUILD_TOOLS` | Whether tools are being built |

## Example Subdirectory CMakeLists.txt

### Simple Library
```cmake
# Libraries/Source/WWVegas/WWMath/CMakeLists.txt
generals_glob_sources(MATH_SOURCES)
generals_add_library(
    NAME WWMath
    SOURCES ${MATH_SOURCES}
)
```

### Library with Dependencies
```cmake
# GameEngine/CMakeLists.txt
file(GLOB_RECURSE ENGINE_SOURCES "*.cpp" "*.c")
file(GLOB_RECURSE ENGINE_HEADERS "*.h")

generals_add_library(
    NAME GameEngine
    SOURCES ${ENGINE_SOURCES}
    HEADERS ${ENGINE_HEADERS}
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/Include
    DEPENDS Benchmark Compression EABrowserDispatch
    PCH Include/Precompiled/PreRTS.h
)
```

### Main Executable
```cmake
# Main/CMakeLists.txt
generals_add_executable(
    NAME RTS
    SOURCES WinMain.cpp
    HEADERS WinMain.h resource.h
    DEPENDS
        GameEngine
        GameEngineDevice
        WWMath
        WWLib
        # ... other dependencies
    LIBS ${PLATFORM_LIBS}
    WIN32
)

# Set working directory for debugging
generals_set_debug_working_dir(RTS "${CMAKE_SOURCE_DIR}/../Run")

# Copy DLLs
if(WIN32)
    generals_copy_runtime_files(RTS
        "${CMAKE_SOURCE_DIR}/Libraries/Bin/binkw32.dll"
        "${CMAKE_SOURCE_DIR}/Libraries/Bin/mss32.dll"
    )
endif()
```

## Platform-Specific Code

```cmake
# Windows-only
if(WIN32)
    target_sources(MyTarget PRIVATE windows_specific.cpp)
    target_link_libraries(MyTarget PRIVATE winmm)
endif()

# Linux-only
if(UNIX AND NOT APPLE)
    target_sources(MyTarget PRIVATE linux_specific.cpp)
    target_link_libraries(MyTarget PRIVATE pthread dl)
endif()

# macOS-only
if(APPLE)
    target_sources(MyTarget PRIVATE macos_specific.cpp)
    target_link_libraries(MyTarget PRIVATE "-framework Cocoa")
endif()
```

## Useful CMake Commands

```bash
# Configure only (don't build)
cmake ..

# Build only (no configure)
cmake --build .

# Clean build
cmake --build . --clean-first

# Install
cmake --install . --prefix /usr/local

# Run tests
ctest

# Generate compile_commands.json
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

## Links

- [Full Build Guide](CMAKE_BUILD_GUIDE.md)
- [CMake Documentation](https://cmake.org/documentation/)
- [GeneralsHelpers.cmake](cmake/GeneralsHelpers.cmake)

---
**Quick Start:** `mkdir build && cd build && cmake .. && cmake --build .`
