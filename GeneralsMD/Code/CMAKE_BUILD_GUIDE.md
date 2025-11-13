# CMake Build System Guide for Command & Conquer: Generals Zero Hour

## Overview

This document describes the modern CMake build system for Command & Conquer: Generals Zero Hour. The build system has been designed to replace the original Visual Studio 6 project files while maintaining full compatibility with the original codebase.

## Prerequisites

### Required Tools

- **CMake 3.15 or later** - [Download](https://cmake.org/download/)
- **C++17 compatible compiler**:
  - Windows: Visual Studio 2017 or later (2019/2022 recommended)
  - Linux: GCC 7+ or Clang 5+
  - macOS: Xcode 10+ with Apple Clang

### Optional Tools

- **Ninja** - Fast build system (recommended for all platforms)
- **ccache** - Compiler cache for faster rebuilds
- **Doxygen** - For generating documentation

## Quick Start

### Windows (Visual Studio)

```cmd
cd GeneralsMD\Code
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A Win32
cmake --build . --config Release
```

### Windows (Ninja)

```cmd
cd GeneralsMD\Code
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Linux

```bash
cd GeneralsMD/Code
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

### macOS

```bash
cd GeneralsMD/Code
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(sysctl -n hw.ncpu)
```

## Build Configurations

### Configuration Types

The build system supports standard CMake configuration types:

- **Debug** - No optimization, full debug symbols, runtime checks
- **Release** - Full optimization, minimal debug info
- **RelWithDebInfo** - Optimized with debug symbols (recommended for profiling)
- **MinSizeRel** - Optimized for size

### Single-Config Generators (Makefiles, Ninja)

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

### Multi-Config Generators (Visual Studio, Xcode)

```bash
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

## Build Options

The build system provides several options that can be enabled/disabled:

```bash
# Build development tools (WorldBuilder, ParticleEditor, etc.)
cmake .. -DBUILD_TOOLS=ON

# Enable testing support
cmake .. -DBUILD_TESTS=ON

# Use precompiled headers (faster compilation)
cmake .. -DUSE_PRECOMPILED_HEADERS=ON

# Enable profiling support
cmake .. -DENABLE_PROFILE=ON
```

### Combining Options

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TOOLS=ON -DUSE_PRECOMPILED_HEADERS=ON
```

## Project Structure

The build system respects the original project architecture:

```
Code/
├── CMakeLists.txt                 # Root build configuration (THIS FILE)
├── cmake/
│   ├── config.h.in               # Configuration header template
│   └── GeneralsHelpers.cmake     # Helper functions
├── Libraries/
│   └── Source/
│       ├── Benchmark/            # Performance benchmarking
│       ├── Compression/          # zlib compression
│       ├── EABrowserDispatch/    # EA browser integration
│       ├── WWVegas/              # Westwood Vegas engine libraries
│       │   ├── WWMath/           # Math library
│       │   ├── WWUtil/           # Utility library
│       │   ├── WWDebug/          # Debug utilities
│       │   ├── WWLib/            # Core library
│       │   ├── WWSaveLoad/       # Save/Load system
│       │   ├── WW3D2/            # 3D rendering engine
│       │   ├── wwshade/          # Shader system
│       │   └── WWDownload/       # Download manager
│       ├── GameSpy/              # GameSpy SDK
│       ├── debug/                # Debug library
│       └── profile/              # Profiling library
├── GameEngine/                   # Core game engine
├── GameEngineDevice/             # Device abstraction layer
├── Main/                         # Game executable (WinMain)
└── Tools/                        # Development tools (optional)
```

## Build Order and Dependencies

The CMake build system follows this dependency hierarchy:

### Layer 1: Base Libraries (No Dependencies)
- Benchmark
- Compression
- EABrowserDispatch
- WWMath, WWUtil, WWDebug, WWLib, WWSaveLoad, WW3D2, wwshade, WWDownload
- GameSpy libraries (ghttp, peer, gp, gstats)
- debug, profile

### Layer 2: Game Engine (Depends on Layer 1)
- GameEngine
- GameEngineDevice

### Layer 3: Main Executable (Depends on Layers 1 & 2)
- RTS (Main game executable)

### Layer 4: Tools (Optional, Depends on Layers 1-3)
- WorldBuilder
- ParticleEditor
- GUIEdit
- ImagePacker
- MapCacheBuilder
- etc.

## Compiler Flags

### Windows (MSVC)

The build system configures MSVC with flags matching the original VS6 settings:

**Release:**
- `/W3` - Warning level 3
- `/WX` - Warnings as errors
- `/O2` - Maximize speed
- `/Ob2` - Inline expansion
- `/GL` - Whole program optimization
- `/MD` - Multi-threaded DLL runtime

**Debug:**
- `/W3` - Warning level 3
- `/WX` - Warnings as errors
- `/Od` - Disable optimization
- `/ZI` - Edit and Continue debug info
- `/MDd` - Multi-threaded Debug DLL runtime
- `/RTC1` - Runtime checks

### Linux/macOS (GCC/Clang)

**Release:**
- `-O3` - Maximum optimization
- `-march=native` - CPU-specific optimizations
- `-Wall -Werror` - All warnings as errors
- `-fexceptions` - Enable exceptions

**Debug:**
- `-O0` - No optimization
- `-g3` - Full debug info
- `-Wall -Werror` - All warnings as errors
- `-fno-inline` - Disable inlining

## Global Definitions

The following preprocessor definitions are set globally:

### All Platforms
- `Z_PREFIX` - zlib namespace prefix

### Windows-Specific
- `WIN32`, `_WINDOWS`
- `WINVER=0x0400` - Target Windows 95/NT 4.0+
- `_WIN32_WINNT=0x0400`
- `WIN32_LEAN_AND_MEAN` - Exclude rarely-used Windows headers
- `NOMINMAX` - Prevent min/max macro conflicts
- `_MBCS` - Multi-byte character set

### Configuration-Specific
- `_DEBUG` - Debug builds only
- `NDEBUG` - Release builds
- `_RELEASE` - Release builds
- `IG_DEBUG_STACKTRACE` - Stack trace in release builds

## Output Directories

The build system mirrors the original project structure:

- **Executables:** `../Run/` (RTS.exe, tools)
- **Static Libraries:** `Libraries/Lib/` (.lib/.a files)
- **Debug Symbols:** `../Run/` (.pdb files on Windows)

## Advanced Usage

### Custom Build Types

Create a custom build configuration:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Profile \
         -DCMAKE_CXX_FLAGS_PROFILE="-O2 -g -pg" \
         -DCMAKE_C_FLAGS_PROFILE="-O2 -g -pg"
```

### Cross-Compilation

Example for Windows from Linux (MinGW):

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-mingw.cmake
```

### Using ccache

Speed up rebuilds with compiler caching:

```bash
cmake .. -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
```

### Verbose Build Output

```bash
cmake --build . --verbose
# or
make VERBOSE=1
# or
ninja -v
```

### Parallel Builds

```bash
# Makefiles
cmake --build . -j8

# Ninja (parallel by default)
ninja

# Visual Studio
cmake --build . --parallel 8
```

## Custom Targets

The build system provides several custom targets:

### Run the Game

```bash
cmake --build . --target run
```

### Clean All Artifacts

```bash
cmake --build . --target clean-all
```

### Generate Documentation

```bash
cmake --build . --target docs
```

## Troubleshooting

### Missing Dependencies

If CMake can't find dependencies, specify paths manually:

```bash
cmake .. -DCMAKE_PREFIX_PATH="/path/to/dependencies"
```

### Precompiled Header Issues

If you encounter PCH errors, disable them:

```bash
cmake .. -DUSE_PRECOMPILED_HEADERS=OFF
```

### Linking Errors on Windows

Ensure you're using the correct runtime library (MD/MDd):

```bash
cmake .. -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
```

### Platform-Specific Issues

**Windows:**
- Use Visual Studio Developer Command Prompt
- Ensure Windows SDK is installed

**Linux:**
- Install development packages: `build-essential`, `libx11-dev`, etc.

**macOS:**
- Install Xcode Command Line Tools: `xcode-select --install`

## IDE Integration

### Visual Studio

1. Open the folder in Visual Studio (2019+)
2. Visual Studio will automatically detect CMakeLists.txt
3. Use the CMake menu to configure and build

Or generate a solution file:
```cmd
cmake .. -G "Visual Studio 17 2022"
```

### CLion

CLion has native CMake support:
1. Open the `Code` directory
2. CLion will automatically configure the project
3. Use the Run/Debug configurations

### VS Code

1. Install the CMake Tools extension
2. Open the `Code` directory
3. Press `Ctrl+Shift+P` → "CMake: Configure"
4. Press `F7` to build

### Qt Creator

1. File → Open File or Project
2. Select `CMakeLists.txt`
3. Configure build settings
4. Build with `Ctrl+B`

## Migration from VS6

### Key Differences

1. **No .dsp/.dsw files** - All configuration is in CMakeLists.txt
2. **Out-of-source builds** - Build files are separate from source
3. **Cross-platform** - Same build files work on Windows/Linux/macOS
4. **Modern C++** - Uses C++17 instead of C++98

### Conversion Notes

The CMake build system:
- Preserves original compiler flags where possible
- Maintains the same library dependency order
- Uses modern equivalents for deprecated flags (e.g., `/GX` → exceptions enabled)
- Adds cross-platform support while maintaining Windows behavior

## Performance Tips

1. **Use Ninja** - Faster than makefiles/msbuild
2. **Enable ccache** - Dramatically speeds up rebuilds
3. **Use precompiled headers** - Enabled by default
4. **Parallel builds** - Use `-j` flag
5. **RAM disk** - Put build directory on RAM disk for maximum speed

## Contributing

When adding new source files or libraries:

1. Add them to the appropriate `CMakeLists.txt`
2. Update dependencies if needed
3. Test on all target platforms
4. Update this documentation

## Resources

- [CMake Documentation](https://cmake.org/documentation/)
- [Modern CMake Best Practices](https://cliutils.gitlab.io/modern-cmake/)
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)

## Support

For build system issues:
1. Check this guide first
2. Review CMake error messages carefully
3. Enable verbose output for detailed diagnostics
4. Consult the CMake documentation

## License

This CMake build system follows the same license as the main project.

---

**Last Updated:** 2025-11-13
**CMake Version:** 3.15+
**Build System Version:** 1.0.0
