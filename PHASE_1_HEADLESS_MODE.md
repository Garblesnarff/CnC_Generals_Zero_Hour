# Phase 1: Headless Mode

**Goal**: Enable fast, GPU-free simulation for AI training and automated tournaments.

**Time Estimate**: 3-5 days
**Dependencies**: None
**Risk Level**: Low
**Priority**: Critical (foundation for all other phases)

## Overview

Headless mode allows the game simulation to run without rendering graphics. This enables:
- **Speed**: 10x-100x faster than real-time (no GPU bottleneck)
- **Automation**: Run matches on servers without displays
- **Parallelization**: Multiple matches simultaneously
- **Resource Efficiency**: CPU-only operation

## Technical Architecture

### Current System (Coupled Rendering + Logic)

```
┌────────────────────────────────────────┐
│         Main Game Loop                 │
├────────────────────────────────────────┤
│  1. Process Input                      │
│  2. Update Game Logic  ◄───────────┐   │
│  3. Render Frame       ◄──────┐    │   │
│  4. Present to Screen         │    │   │
└───────────────────────────────┼────┼───┘
                                │    │
                        ┌───────┴────┴──────┐
                        │ Tightly Coupled!   │
                        │ Logic waits for    │
                        │ rendering to       │
                        │ complete           │
                        └────────────────────┘
```

### New System (Decoupled)

```
┌────────────────────────────────────────┐
│         Main Game Loop                 │
├────────────────────────────────────────┤
│  1. Process Input (optional)           │
│  2. Update Game Logic                  │
│  3. if (!headless) {                   │
│       Render Frame                     │
│       Present to Screen                │
│     }                                  │
└────────────────────────────────────────┘
            │
            ├── Normal Mode: 30-60 FPS
            │   (logic + rendering)
            │
            └── Headless Mode: 3000+ FPS
                (logic only)
```

## Implementation Steps

### Step 1: Create Headless Video Device (2-3 hours)

The W3D engine uses a `VideoDevice` abstraction. We create a no-op implementation.

**File**: `/GeneralsMD/Code/GameEngineDevice/Source/VideoDevice/HeadlessVideoDevice.cpp`

```cpp
#include "VideoDevice/VideoDevice.h"
#include "Common/RTS/GlobalData.h"

//=============================================================================
// HeadlessVideoDevice
//
// A video device that performs no actual rendering. Used for headless
// simulation mode where graphics are not needed.
//=============================================================================

class HeadlessVideoDevice : public VideoDevice
{
public:
    HeadlessVideoDevice();
    virtual ~HeadlessVideoDevice();

    // VideoDevice interface implementation
    virtual bool Init(HWND hWnd, Int width, Int height, Int bpp, bool fullscreen) override;
    virtual void Shutdown() override;
    virtual void Clear(UInt32 color) override;
    virtual void Present() override;
    virtual void SetResolution(Int width, Int height, Int bpp, bool fullscreen) override;

    // Texture management (no-ops)
    virtual TextureHandle CreateTexture(Int width, Int height, WW3DFormat format) override;
    virtual void ReleaseTexture(TextureHandle handle) override;
    virtual void* LockTexture(TextureHandle handle) override;
    virtual void UnlockTexture(TextureHandle handle) override;

    // Rendering (no-ops)
    virtual void SetRenderTarget(TextureHandle handle) override;
    virtual void DrawPrimitive(PrimitiveType type, const Vertex* vertices, Int count) override;
    virtual void DrawIndexedPrimitive(PrimitiveType type, const Vertex* vertices,
                                      Int vertexCount, const UInt16* indices, Int indexCount) override;

    // State management (tracked but not applied)
    virtual void SetTransform(TransformType type, const Matrix4& matrix) override;
    virtual void SetRenderState(RenderStateType state, UInt32 value) override;
    virtual void SetTexture(Int stage, TextureHandle handle) override;
    virtual void SetMaterial(const Material& material) override;
    virtual void SetLight(Int index, const Light& light) override;
    virtual void EnableLight(Int index, bool enable) override;

    // Capabilities
    virtual VideoDeviceType GetType() const override { return VIDEO_DEVICE_HEADLESS; }
    virtual const Char* GetName() const override { return "Headless Video Device"; }
    virtual bool SupportsShaderModel(Int major, Int minor) const override { return false; }

private:
    Int m_width;
    Int m_height;
    Int m_bpp;
    bool m_initialized;

    // Minimal state tracking (for queries)
    Matrix4 m_transforms[TRANSFORM_TYPE_COUNT];
    UInt32 m_renderStates[RENDER_STATE_COUNT];
};

//=============================================================================
// Implementation
//=============================================================================

HeadlessVideoDevice::HeadlessVideoDevice()
    : m_width(0)
    , m_height(0)
    , m_bpp(0)
    , m_initialized(false)
{
    // Initialize with identity/default states
    for (Int i = 0; i < TRANSFORM_TYPE_COUNT; ++i) {
        m_transforms[i].MakeIdentity();
    }

    memset(m_renderStates, 0, sizeof(m_renderStates));
}

HeadlessVideoDevice::~HeadlessVideoDevice()
{
    Shutdown();
}

bool HeadlessVideoDevice::Init(HWND hWnd, Int width, Int height, Int bpp, bool fullscreen)
{
    m_width = width;
    m_height = height;
    m_bpp = bpp;
    m_initialized = true;

    DBG_LOG("HeadlessVideoDevice::Init(%d x %d @ %d bpp)", width, height, bpp);
    return true;
}

void HeadlessVideoDevice::Shutdown()
{
    if (m_initialized) {
        DBG_LOG("HeadlessVideoDevice::Shutdown()");
        m_initialized = false;
    }
}

void HeadlessVideoDevice::Clear(UInt32 color)
{
    // No-op: nothing to clear
}

void HeadlessVideoDevice::Present()
{
    // No-op: nothing to present
}

void HeadlessVideoDevice::SetResolution(Int width, Int height, Int bpp, bool fullscreen)
{
    m_width = width;
    m_height = height;
    m_bpp = bpp;
}

TextureHandle HeadlessVideoDevice::CreateTexture(Int width, Int height, WW3DFormat format)
{
    // Return dummy handle (non-zero to indicate "success")
    static TextureHandle s_nextHandle = 1;
    return s_nextHandle++;
}

void HeadlessVideoDevice::ReleaseTexture(TextureHandle handle)
{
    // No-op
}

void* HeadlessVideoDevice::LockTexture(TextureHandle handle)
{
    // Return dummy buffer if anything tries to write to texture
    static Byte s_dummyBuffer[1024];
    return s_dummyBuffer;
}

void HeadlessVideoDevice::UnlockTexture(TextureHandle handle)
{
    // No-op
}

void HeadlessVideoDevice::SetRenderTarget(TextureHandle handle)
{
    // No-op
}

void HeadlessVideoDevice::DrawPrimitive(PrimitiveType type, const Vertex* vertices, Int count)
{
    // No-op: drop all draw calls
}

void HeadlessVideoDevice::DrawIndexedPrimitive(PrimitiveType type, const Vertex* vertices,
                                               Int vertexCount, const UInt16* indices, Int indexCount)
{
    // No-op: drop all draw calls
}

void HeadlessVideoDevice::SetTransform(TransformType type, const Matrix4& matrix)
{
    if (type >= 0 && type < TRANSFORM_TYPE_COUNT) {
        m_transforms[type] = matrix;
    }
}

void HeadlessVideoDevice::SetRenderState(RenderStateType state, UInt32 value)
{
    if (state >= 0 && state < RENDER_STATE_COUNT) {
        m_renderStates[state] = value;
    }
}

void HeadlessVideoDevice::SetTexture(Int stage, TextureHandle handle)
{
    // No-op
}

void HeadlessVideoDevice::SetMaterial(const Material& material)
{
    // No-op
}

void HeadlessVideoDevice::SetLight(Int index, const Light& light)
{
    // No-op
}

void HeadlessVideoDevice::EnableLight(Int index, bool enable)
{
    // No-op
}

//=============================================================================
// Factory function
//=============================================================================

VideoDevice* CreateHeadlessVideoDevice()
{
    return new HeadlessVideoDevice();
}
```

**Header**: `/GeneralsMD/Code/GameEngineDevice/Include/VideoDevice/HeadlessVideoDevice.h`

```cpp
#pragma once

#include "VideoDevice/VideoDevice.h"

// Factory function
VideoDevice* CreateHeadlessVideoDevice();

// Device type constant (add to VideoDevice.h enum)
// enum VideoDeviceType {
//     VIDEO_DEVICE_D3D,
//     VIDEO_DEVICE_OPENGL,
//     VIDEO_DEVICE_HEADLESS,  // <-- Add this
// };
```

### Step 2: Add Headless Mode Configuration (1 hour)

**File**: `/GeneralsMD/Code/GameEngine/Include/Common/RTS/GlobalData.h`

```cpp
// Add to WriteableGlobalData class:

class WriteableGlobalData
{
public:
    // ... existing fields ...

    // Laboratory Mode Settings
    bool m_headlessMode;              // Run without rendering
    Int m_speedMultiplier;            // Simulation speed (1 = normal, 100 = 100x faster)
    bool m_laboratoryMode;            // Enable AI laboratory features
    String m_aiLogPath;               // Where to write AI decision logs
    bool m_deterministicMode;         // Fixed RNG seed for reproducibility
    UInt32 m_randomSeed;              // Seed for deterministic mode

    // ... rest of class ...
};
```

**File**: `/GeneralsMD/Code/GameEngine/Source/Common/RTS/GlobalData.cpp`

```cpp
WriteableGlobalData::WriteableGlobalData()
    // ... existing initializers ...
    , m_headlessMode(false)
    , m_speedMultiplier(1)
    , m_laboratoryMode(false)
    , m_aiLogPath("")
    , m_deterministicMode(false)
    , m_randomSeed(0)
{
    // ... rest of constructor ...
}
```

### Step 3: Command Line Argument Parsing (2-3 hours)

**File**: `/GeneralsMD/Code/Main/Source/Main.cpp`

```cpp
#include "Common/RTS/GlobalData.h"
#include <cstring>

//=============================================================================
// Command line parsing for laboratory mode
//=============================================================================

void ParseLaboratoryArguments(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i) {
        const char* arg = argv[i];

        // Headless mode
        if (strcmp(arg, "--headless") == 0 || strcmp(arg, "-h") == 0) {
            g_theWriteableGlobalData->m_headlessMode = true;
            DBG_LOG("Headless mode enabled");
        }

        // Speed multiplier
        else if (strcmp(arg, "--speed-multiplier") == 0 || strcmp(arg, "-s") == 0) {
            if (i + 1 < argc) {
                int multiplier = atoi(argv[++i]);
                if (multiplier >= 1 && multiplier <= 1000) {
                    g_theWriteableGlobalData->m_speedMultiplier = multiplier;
                    DBG_LOG("Speed multiplier set to %dx", multiplier);
                } else {
                    DBG_LOG("WARNING: Speed multiplier must be between 1 and 1000");
                }
            }
        }

        // Laboratory mode
        else if (strcmp(arg, "--laboratory") == 0 || strcmp(arg, "-lab") == 0) {
            g_theWriteableGlobalData->m_laboratoryMode = true;
            DBG_LOG("Laboratory mode enabled");
        }

        // AI logging path
        else if (strcmp(arg, "--ai-log-path") == 0) {
            if (i + 1 < argc) {
                g_theWriteableGlobalData->m_aiLogPath = argv[++i];
                DBG_LOG("AI log path set to: %s", argv[i]);
            }
        }

        // Deterministic mode
        else if (strcmp(arg, "--deterministic") == 0 || strcmp(arg, "-d") == 0) {
            g_theWriteableGlobalData->m_deterministicMode = true;

            // Check for optional seed
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                g_theWriteableGlobalData->m_randomSeed = (UInt32)atoi(argv[++i]);
                DBG_LOG("Deterministic mode enabled with seed: %u", g_theWriteableGlobalData->m_randomSeed);
            } else {
                g_theWriteableGlobalData->m_randomSeed = 12345; // Default seed
                DBG_LOG("Deterministic mode enabled with default seed");
            }
        }

        // Map to load
        else if (strcmp(arg, "--map") == 0 || strcmp(arg, "-m") == 0) {
            if (i + 1 < argc) {
                // Store map path for auto-loading
                // (Will be implemented in Phase 4 for tournament mode)
                DBG_LOG("Map specified: %s", argv[i + 1]);
                i++;
            }
        }

        // Help
        else if (strcmp(arg, "--help") == 0) {
            printf("Claude & Conquer - AI Laboratory Mode\n");
            printf("\nUsage: RTS.exe [options]\n\n");
            printf("Options:\n");
            printf("  --headless, -h                  Run without rendering\n");
            printf("  --speed-multiplier N, -s N      Run N times faster (1-1000)\n");
            printf("  --laboratory, -lab              Enable AI laboratory features\n");
            printf("  --ai-log-path PATH              Set AI decision log output path\n");
            printf("  --deterministic, -d [SEED]      Deterministic RNG (optional seed)\n");
            printf("  --map PATH, -m PATH             Load specific map\n");
            printf("  --help                          Show this help\n");
            printf("\nExamples:\n");
            printf("  RTS.exe --headless --speed-multiplier 50\n");
            printf("  RTS.exe --laboratory --ai-log-path ./ai_data/\n");
            printf("  RTS.exe --deterministic 12345 --map maps/tournament.map\n");
            exit(0);
        }
    }

    // Auto-enable laboratory mode if headless
    if (g_theWriteableGlobalData->m_headlessMode) {
        g_theWriteableGlobalData->m_laboratoryMode = true;
    }
}

//=============================================================================
// Main entry point (modified)
//=============================================================================

int main(int argc, char* argv[])
{
    // Parse laboratory arguments FIRST
    ParseLaboratoryArguments(argc, argv);

    // ... rest of main() ...

    return 0;
}
```

### Step 4: Video Device Selection (1 hour)

**File**: `/GeneralsMD/Code/GameEngineDevice/Source/VideoDevice/VideoDevice.cpp`

```cpp
#include "VideoDevice/VideoDevice.h"
#include "VideoDevice/W3DVideoDevice.h"
#include "VideoDevice/HeadlessVideoDevice.h"
#include "Common/RTS/GlobalData.h"

VideoDevice* CreateVideoDevice()
{
    // Check if headless mode is requested
    if (g_theWriteableGlobalData && g_theWriteableGlobalData->m_headlessMode) {
        DBG_LOG("Creating headless video device");
        return CreateHeadlessVideoDevice();
    }

    // Normal mode: create D3D8 device (or OpenGL on Linux/Mac)
    #ifdef _WIN32
        DBG_LOG("Creating Direct3D video device");
        return CreateD3DVideoDevice();
    #else
        DBG_LOG("Creating OpenGL video device");
        return CreateOpenGLVideoDevice();
    #endif
}
```

### Step 5: Game Loop Speed Control (2-3 hours)

The game loop needs to respect the speed multiplier.

**File**: `/GeneralsMD/Code/GameEngine/Source/GameLogic/System/GameLogic.cpp`

```cpp
//=============================================================================
// Enhanced game loop with speed control
//=============================================================================

void GameLogic::Update()
{
    // Calculate how many logic updates to perform this frame
    Int updatesToPerform = 1;

    if (g_theWriteableGlobalData->m_headlessMode) {
        // In headless mode, perform multiple updates per "frame"
        updatesToPerform = g_theWriteableGlobalData->m_speedMultiplier;

        // Cap to prevent infinite loops if something goes wrong
        if (updatesToPerform > 1000) {
            updatesToPerform = 1000;
        }
    }

    // Perform logic updates
    for (Int i = 0; i < updatesToPerform; ++i) {
        UpdateSingleFrame();
    }
}

void GameLogic::UpdateSingleFrame()
{
    // Increment frame counter
    ++m_frame;

    // Update all game systems
    if (m_playerList) {
        m_playerList->Update();
    }

    if (m_partitionManager) {
        m_partitionManager->Update();
    }

    if (m_scriptEngine) {
        m_scriptEngine->Update();
    }

    // Update all game objects
    for (Object* obj = m_objectList; obj != nullptr; obj = obj->getNext()) {
        obj->update();
    }

    // Update AI systems
    if (TheAdaptiveAI) {
        TheAdaptiveAI->update();
    }

    // Check win conditions
    CheckWinConditions();

    // Log progress in headless mode
    if (g_theWriteableGlobalData->m_headlessMode && (m_frame % 3000) == 0) {
        Real timeSeconds = m_frame / 30.0f; // 30 FPS logic rate
        DBG_LOG("Headless: Frame %d (%.1f seconds simulated)", m_frame, timeSeconds);
    }
}
```

### Step 6: Disable Unnecessary Systems in Headless Mode (2 hours)

**File**: `/GeneralsMD/Code/GameEngine/Source/GameLogic/System/GameLogic.cpp`

```cpp
bool GameLogic::Init()
{
    // ... existing initialization ...

    if (g_theWriteableGlobalData->m_headlessMode) {
        DBG_LOG("Headless mode: Skipping rendering subsystems");

        // Don't initialize:
        // - Particle systems (purely visual)
        // - Terrain rendering
        // - Weather effects
        // - Camera system
        // - UI system

        // DO initialize:
        // - Game logic
        // - AI systems
        // - Physics/collision
        // - Networking (for multiplayer AI matches)

        m_skipRendering = true;
    } else {
        // Normal initialization
        InitRenderingSystems();
        m_skipRendering = false;
    }

    // ... rest of initialization ...

    return true;
}
```

**File**: `/GeneralsMD/Code/GameEngine/Source/Renderer/WorldRenderer.cpp`

```cpp
void WorldRenderer::Render()
{
    // Early exit in headless mode
    if (g_theWriteableGlobalData->m_headlessMode) {
        return;
    }

    // ... normal rendering code ...
}
```

### Step 7: Deterministic Random Number Generation (1-2 hours)

For reproducible experiments, we need deterministic RNG.

**File**: `/GeneralsMD/Code/GameEngine/Include/Common/RTS/Random.h`

```cpp
#pragma once

#include "Platform/Platform.h"

//=============================================================================
// DeterministicRNG
//
// Linear Congruential Generator for reproducible randomness
// Using parameters from Numerical Recipes
//=============================================================================

class DeterministicRNG
{
public:
    DeterministicRNG(UInt32 seed = 1) : m_state(seed) {}

    void SetSeed(UInt32 seed) { m_state = seed; }

    UInt32 Next()
    {
        // LCG: X(n+1) = (a * X(n) + c) mod m
        // Using a=1664525, c=1013904223, m=2^32
        m_state = 1664525 * m_state + 1013904223;
        return m_state;
    }

    Int NextInt(Int min, Int max)
    {
        if (min >= max) return min;
        UInt32 range = (UInt32)(max - min + 1);
        return min + (Int)(Next() % range);
    }

    Real NextReal(Real min = 0.0f, Real max = 1.0f)
    {
        Real normalized = (Real)Next() / (Real)0xFFFFFFFF;
        return min + normalized * (max - min);
    }

private:
    UInt32 m_state;
};

// Global RNG instance
extern DeterministicRNG* g_theRNG;
```

**File**: `/GeneralsMD/Code/GameEngine/Source/Common/RTS/Random.cpp`

```cpp
#include "Common/RTS/Random.h"
#include "Common/RTS/GlobalData.h"

DeterministicRNG* g_theRNG = nullptr;

void InitRNG()
{
    if (g_theWriteableGlobalData->m_deterministicMode) {
        UInt32 seed = g_theWriteableGlobalData->m_randomSeed;
        if (seed == 0) {
            seed = 12345; // Default seed
        }

        DBG_LOG("Initializing deterministic RNG with seed: %u", seed);
        g_theRNG = new DeterministicRNG(seed);
    } else {
        // Use system time as seed for non-deterministic mode
        UInt32 seed = (UInt32)time(nullptr);
        DBG_LOG("Initializing RNG with time-based seed: %u", seed);
        g_theRNG = new DeterministicRNG(seed);
    }
}

void ShutdownRNG()
{
    if (g_theRNG) {
        delete g_theRNG;
        g_theRNG = nullptr;
    }
}
```

**Usage**: Replace all calls to `rand()` with `g_theRNG->NextInt()` in AI code.

## Testing Plan

### Test 1: Headless Mode Launches
```bash
./RTS.exe --headless
```

**Expected**: Game initializes, no window appears, simulation runs.

### Test 2: Speed Multiplier Works
```bash
./RTS.exe --headless --speed-multiplier 10
```

**Expected**: Simulation runs 10x faster (check frame counter logs).

### Test 3: Deterministic Mode
```bash
./RTS.exe --headless --deterministic 12345 --map test.map
./RTS.exe --headless --deterministic 12345 --map test.map
```

**Expected**: Both runs produce identical game state at any given frame.

### Test 4: Normal Mode Still Works
```bash
./RTS.exe
```

**Expected**: Game runs normally with graphics.

### Test 5: Performance Benchmark
```bash
time ./RTS.exe --headless --speed-multiplier 100 --map benchmark.map
```

**Expected**: Completes 10-minute match in < 10 seconds.

## Performance Targets

| Mode | Target FPS | Target Speedup |
|------|-----------|----------------|
| Normal (with rendering) | 30-60 | 1x |
| Headless (multiplier=1) | 300-500 | 10x |
| Headless (multiplier=10) | 3000+ | 100x |
| Headless (multiplier=100) | 30000+ | 1000x |

**Note**: Actual speedup depends on CPU. Above targets assume modern multi-core CPU.

## Integration Checklist

- [ ] Create `HeadlessVideoDevice.cpp` and `.h`
- [ ] Add headless fields to `GlobalData.h`
- [ ] Implement command-line parsing in `Main.cpp`
- [ ] Modify video device factory to select headless mode
- [ ] Update game loop for speed multiplier
- [ ] Disable rendering subsystems in headless mode
- [ ] Implement deterministic RNG
- [ ] Replace `rand()` calls in AI code with `g_theRNG->NextInt()`
- [ ] Update CMakeLists.txt to include new files
- [ ] Test all modes (headless, normal, deterministic)
- [ ] Benchmark performance
- [ ] Document command-line options

## CMake Changes

**File**: `/GeneralsMD/Code/GameEngineDevice/CMakeLists.txt`

```cmake
# Add headless video device source
set(VIDEO_DEVICE_SOURCES
    ${VIDEO_DEVICE_SOURCES}
    Source/VideoDevice/HeadlessVideoDevice.cpp
)
```

**File**: `/GeneralsMD/Code/GameEngine/CMakeLists.txt`

```cmake
# Add RNG source
set(COMMON_SOURCES
    ${COMMON_SOURCES}
    Source/Common/RTS/Random.cpp
)
```

## Known Limitations

1. **Audio Still Runs**: Audio device still ticks even in headless mode. Not performance-critical, but could be optimized.
2. **Network Tick Rate**: Multiplayer matches still limited by network tick rate. Headless doesn't accelerate networked games.
3. **Pathfinding**: Complex pathfinding can still be CPU bottleneck at extreme speeds (100x+).
4. **Win Condition**: Need to ensure matches auto-exit when win condition met (for automated tournaments).

## Future Enhancements

1. **Headless Audio Device**: Similar to headless video, skip audio processing.
2. **Fast-Forward Pathfinding**: Cache pathfinding results for repeated scenarios.
3. **Multi-Threading**: Run multiple matches in parallel (Phase 4).
4. **Save States**: Snapshot game state for replay/analysis (Phase 5).

## Success Criteria

- ✅ Headless mode runs without crashing
- ✅ Speed multiplier achieves 50x+ speedup
- ✅ Deterministic mode produces identical results
- ✅ Normal mode unaffected
- ✅ Performance targets met
- ✅ Zero rendering overhead in headless mode

## Next Phase

Once Phase 1 is complete, proceed to [Phase 2: Decision Logging](./PHASE_2_DECISION_LOGGING.md) to instrument AI decision-making for analysis.
