# CMake Build System for Command & Conquer: Generals Zero Hour

This directory contains a modern CMake build system that replaces the original Visual Studio 6 project files.

## Quick Start

### Windows
```cmd
validate_cmake.bat           REM Validate configuration
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A Win32
cmake --build . --config Release -j8
```

### Linux/macOS
```bash
./validate_cmake.sh          # Validate configuration
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

## Documentation

| File | Description |
|------|-------------|
| **[CMAKE_QUICK_REFERENCE.md](CMAKE_QUICK_REFERENCE.md)** | Quick commands and cheat sheet |
| **[CMAKE_BUILD_GUIDE.md](CMAKE_BUILD_GUIDE.md)** | Comprehensive build guide |
| **[CMAKE_IMPLEMENTATION_SUMMARY.md](CMAKE_IMPLEMENTATION_SUMMARY.md)** | Implementation details and status |

## Files in This Directory

### Build System Core
- **CMakeLists.txt** - Root build configuration (463 lines)
- **cmake/GeneralsHelpers.cmake** - Helper functions for subdirectories
- **cmake/config.h.in** - Configuration header template

### Documentation
- **CMAKE_BUILD_GUIDE.md** - Full build instructions and troubleshooting
- **CMAKE_QUICK_REFERENCE.md** - Quick reference for common tasks
- **CMAKE_IMPLEMENTATION_SUMMARY.md** - Technical implementation details
- **README_CMAKE.md** - This file

### Validation Scripts
- **validate_cmake.sh** - Linux/macOS validation script
- **validate_cmake.bat** - Windows validation script

## Features

- **Modern CMake 3.15+** with target-based configuration
- **Cross-platform** support (Windows, Linux, macOS)
- **Multi-config generators** (Visual Studio, Xcode, Makefiles, Ninja)
- **Original VS6 compatibility** - matches compiler flags and defines
- **Helper functions** for easy subdirectory configuration
- **IDE integration** (Visual Studio, CLion, VS Code, Qt Creator)
- **Precompiled headers** for faster compilation
- **Custom targets** (run, clean-all, docs)

## Build Options

```bash
-DCMAKE_BUILD_TYPE=<type>        # Debug, Release, RelWithDebInfo
-DBUILD_TOOLS=ON                 # Build development tools
-DBUILD_TESTS=ON                 # Build test executables
-DUSE_PRECOMPILED_HEADERS=ON     # Use PCH (default: ON)
-DENABLE_PROFILE=ON              # Enable profiling
```

## Project Structure

```
Code/
├── CMakeLists.txt              # Root build configuration
├── cmake/                      # CMake modules and configuration
├── Libraries/Source/           # Layer 1: Base libraries
│   ├── Benchmark/
│   ├── Compression/
│   ├── WWVegas/               # Westwood Vegas libraries
│   └── GameSpy/               # GameSpy SDK
├── GameEngine/                 # Layer 2: Game engine
├── GameEngineDevice/           # Layer 2: Device layer
├── Main/                       # Layer 3: RTS executable
└── Tools/                      # Layer 4: Development tools (optional)
```

## Dependencies

| Layer | Components |
|-------|------------|
| **1** | Benchmark, Compression, WWVegas, GameSpy, debug, profile |
| **2** | GameEngine, GameEngineDevice (depend on Layer 1) |
| **3** | RTS executable (depends on Layers 1 & 2) |
| **4** | Tools (optional, depend on Layers 1-3) |

## Output Directories

| Type | Location | Example |
|------|----------|---------|
| Executables | `../Run/` | `RTS.exe` |
| Libraries | `Libraries/Lib/` | `GameEngine.lib` |
| Debug Symbols | `../Run/` | `RTS.pdb` |

## Configuration Types

| Type | Optimization | Debug Info | Executable |
|------|--------------|------------|------------|
| Debug | None | Full | `RTSD.exe` |
| Release | Maximum | Minimal | `RTS.exe` |
| Internal | Medium | Full | `RTSI.exe` |
| Profile | Full | Full | `RTSP.exe` |

## Compiler Flags

### MSVC (Windows)
- **Release**: `/G6 /MD /W3 /WX /O2 /GL /Zi`
- **Debug**: `/G6 /MDd /W3 /WX /Od /ZI /RTC1`

### GCC/Clang (Linux/macOS)
- **Release**: `-O3 -march=native -Wall -Werror`
- **Debug**: `-O0 -g3 -Wall -Werror -fno-inline`

## Global Definitions

```cmake
Z_PREFIX                # zlib namespace
WIN32, _WINDOWS        # Windows platform
WINVER=0x0400          # Windows 95+
WIN32_LEAN_AND_MEAN    # Minimal headers
NOMINMAX               # No min/max macros
```

## Troubleshooting

### Configuration fails
```bash
./validate_cmake.sh    # Check for issues
cmake .. --debug-output
```

### Build fails
```bash
cmake --build . --verbose  # See full commands
```

### Missing dependencies
```bash
cmake .. -DCMAKE_PREFIX_PATH="/path/to/deps"
```

### PCH issues
```bash
cmake .. -DUSE_PRECOMPILED_HEADERS=OFF
```

## Support

1. **Read the docs**: Start with [CMAKE_QUICK_REFERENCE.md](CMAKE_QUICK_REFERENCE.md)
2. **Validate setup**: Run `validate_cmake.sh` or `validate_cmake.bat`
3. **Check logs**: Enable verbose output with `--verbose`
4. **Review guide**: See [CMAKE_BUILD_GUIDE.md](CMAKE_BUILD_GUIDE.md) for detailed help

## Status

- [x] Root CMakeLists.txt (complete)
- [x] Helper functions (complete)
- [x] Configuration header (complete)
- [x] Documentation (complete)
- [~] Subdirectory CMakeLists.txt (partial - some exist, others needed)
- [ ] Full build testing (pending)

See [CMAKE_IMPLEMENTATION_SUMMARY.md](CMAKE_IMPLEMENTATION_SUMMARY.md) for detailed status.

## Contributing

When adding new components:

1. Use helper functions from `cmake/GeneralsHelpers.cmake`
2. Follow existing patterns in subdirectories
3. Update documentation
4. Test on all platforms

## Example Subdirectory CMakeLists.txt

```cmake
# Simple library
generals_glob_sources(MY_SOURCES)
generals_add_library(
    NAME MyLibrary
    SOURCES ${MY_SOURCES}
)

# Library with dependencies
generals_add_library(
    NAME MyComplexLib
    SOURCES file1.cpp file2.cpp
    HEADERS file1.h file2.h
    DEPENDS OtherLib
    DEFINES MY_DEFINE
    PCH pch.h
)

# Executable
generals_add_executable(
    NAME MyApp
    SOURCES main.cpp
    DEPENDS MyLibrary
    LIBS ${PLATFORM_LIBS}
    WIN32
)
```

## Resources

- [CMake Documentation](https://cmake.org/documentation/)
- [Modern CMake Guide](https://cliutils.gitlab.io/modern-cmake/)
- Original VS6 project: `RTS.dsw`

---

**Version**: 1.0.0
**CMake Required**: 3.15+
**C++ Standard**: C++17
**Last Updated**: 2025-11-13
