# Phase 3: Visualization Tools

**Goal**: Make AI learning visible in real-time through debug overlays and graphs.

**Time Estimate**: 7-10 days
**Dependencies**: Phase 2 (needs logged decision data)
**Risk Level**: Medium (UI/rendering integration complexity)
**Priority**: High (makes AI behavior understandable)

## Overview

The Visualization System provides real-time insight into AI decision-making:
- **Live Graphs**: Priority multipliers changing over time
- **Heat Maps**: Which target types are focused by AI
- **Learning Curves**: Effectiveness scores trending upward
- **Target Indicators**: Visual markers showing what AI is thinking
- **Decision Log Overlay**: Scrolling text showing recent decisions

This transforms invisible AI logic into a **watchable learning process**.

## Visual Elements

### 1. Priority Multiplier Graphs

Real-time line graphs showing how target priorities evolve.

```
┌─────────────────────────────────────────┐
│ Economic AI - Target Priority Multipliers│
├─────────────────────────────────────────┤
│   3.0x │                            ████ │ Supply Truck
│        │                      ██████     │
│   2.0x │            ████████             │ Power Plant
│        │      ██████                     │
│   1.0x │██████──────────────────────────│ Tank
│        │───────────────────────────────│ Infantry
│   0.0  └────────────────────────────────┘
│        0s      50s     100s    150s  200s│
└─────────────────────────────────────────┘
```

**Features**:
- Tracks up to 10 target types
- Color-coded lines (red=high priority, blue=low)
- Updates in real-time (every second)
- Shows last 200 seconds of data

### 2. Target Selection Heat Map

Shows which unit types are being targeted most frequently.

```
┌────────────────────────────────────────┐
│  Target Selection Heat Map (Last 60s)  │
├────────────────────────────────────────┤
│  Supply Truck   ████████████████  145  │
│  Power Plant    ██████████        78   │
│  War Factory    ████████          62   │
│  Tank           ████              31   │
│  Infantry       ██                15   │
│  Building       █                 8    │
└────────────────────────────────────────┘
```

**Features**:
- Horizontal bar chart
- Auto-scales to fit data
- Shows last 60 seconds of decisions
- Updates every 5 seconds

### 3. Learning Event Feed

Scrolling log of significant AI insights.

```
┌────────────────────────────────────────┐
│        Learning Events                 │
├────────────────────────────────────────┤
│ [03:25] Economic AI discovered Supply  │
│         Trucks are high-value targets  │
│         (multiplier 1.0 → 2.1)         │
│                                        │
│ [02:48] Aggressive AI adapted focus to │
│         Power Plants after losing base │
│                                        │
│ [01:32] Economic AI learned Worker     │
│         harassment is effective        │
└────────────────────────────────────────┘
```

**Features**:
- Shows last 10 events
- Timestamp in MM:SS format
- Color-coded by event type
- Fades out old events

### 4. Unit Target Indicators

3D floating markers above units showing AI targeting.

```
     [PRIORITY: 2.3x]
          ▼
        ┌───┐
        │ T │  <-- Supply Truck being targeted
        └───┘
```

**Features**:
- Red crosshair over high-priority targets
- Yellow marker for medium priority
- Green for low priority
- Fades after 2 seconds

### 5. AI Personality Dashboard

Stats panel showing current AI state.

```
┌─────────────────────────────────────┐
│ USA Player 1 - Economic AI          │
├─────────────────────────────────────┤
│ Total Kills:        87              │
│ Learning Samples:   245             │
│ Avg Effectiveness:  0.78            │
│ Top Priority:       Supply Trucks   │
│ Learning Rate:      0.15            │
│ Adaptation Events:  12              │
└─────────────────────────────────────┘
```

## Technical Implementation

### Architecture

```
┌──────────────────────────────────────────┐
│         Rendering Pipeline               │
├──────────────────────────────────────────┤
│  1. Render World (normal game graphics)  │
│  2. Render UI (HUD, buttons, etc.)       │
│  3. Render Debug Overlay (if enabled)    │
│     └─→ VisualizationOverlay::Render()   │
└──────────────────────────────────────────┘
```

### Core Class: VisualizationOverlay

**File**: `/GeneralsMD/Code/GameEngine/Include/Renderer/VisualizationOverlay.h`

```cpp
#pragma once

#include "Common/RTS/RTS.h"
#include "Common/STL/Vector.h"
#include "Common/STL/Deque.h"
#include "Common/STL/Map.h"
#include "GameLogic/Player.h"

//=============================================================================
// VisualizationOverlay
//
// Real-time debug visualization for AI Laboratory mode.
// Renders graphs, heat maps, and status overlays on top of game world.
//=============================================================================

class VisualizationOverlay
{
public:
    VisualizationOverlay();
    ~VisualizationOverlay();

    // Initialization
    bool Init();
    void Shutdown();

    // Update and render
    void Update();                       // Update data structures
    void Render();                       // Draw overlay to screen

    // Control visibility
    void SetEnabled(bool enabled) { m_enabled = enabled; }
    bool IsEnabled() const { return m_enabled; }

    // Toggle individual visualizations
    void TogglePriorityGraphs() { m_showPriorityGraphs = !m_showPriorityGraphs; }
    void ToggleHeatMaps() { m_showHeatMaps = !m_showHeatMaps; }
    void ToggleLearningFeed() { m_showLearningFeed = !m_showLearningFeed; }
    void ToggleTargetIndicators() { m_showTargetIndicators = !m_showTargetIndicators; }
    void TogglePersonalityDashboard() { m_showDashboard = !m_showDashboard; }

    // Data input (called by DecisionLogger)
    void NotifyTargetSelection(const Object* attacker, const Object* target, Real priority);
    void NotifyPriorityUpdate(const ObjectTemplate* targetType, Real oldValue, Real newValue);
    void NotifyLearningEvent(const String& description);

private:
    // State
    bool m_enabled;
    bool m_showPriorityGraphs;
    bool m_showHeatMaps;
    bool m_showLearningFeed;
    bool m_showTargetIndicators;
    bool m_showDashboard;

    // Priority graph data
    struct PriorityDataPoint
    {
        Real timeSeconds;
        Real multiplier;
    };

    struct PriorityHistory
    {
        const ObjectTemplate* targetType;
        Deque<PriorityDataPoint> dataPoints;
        static const Int MAX_HISTORY = 200;  // 200 seconds of data
    };

    Map<const ObjectTemplate*, PriorityHistory> m_priorityHistories;

    // Heat map data
    struct HeatMapEntry
    {
        const ObjectTemplate* targetType;
        Int count;
        Real lastUpdate;
    };

    Vector<HeatMapEntry> m_heatMapData;
    static const Int HEATMAP_WINDOW = 60;   // Last 60 seconds

    // Learning event feed
    struct LearningEventEntry
    {
        Real timestamp;
        String description;
        Real fadeAge;                        // For fade-out effect
    };

    Deque<LearningEventEntry> m_learningEvents;
    static const Int MAX_LEARNING_EVENTS = 10;

    // Target indicators (active targets being attacked)
    struct TargetIndicator
    {
        const Object* target;
        Real priority;
        Real expiryTime;
    };

    Vector<TargetIndicator> m_activeTargets;

    // Rendering helpers
    void RenderPriorityGraphs();
    void RenderHeatMap();
    void RenderLearningFeed();
    void RenderTargetIndicators();
    void RenderDashboard();

    // Utility
    void PruneOldData();
    Color GetPriorityColor(Real multiplier);
    Vector2 WorldToScreen(const Vector3& worldPos);
};

// Global singleton
extern VisualizationOverlay* TheVisualizationOverlay;
```

**File**: `/GeneralsMD/Code/GameEngine/Source/Renderer/VisualizationOverlay.cpp`

```cpp
#include "Renderer/VisualizationOverlay.h"
#include "Renderer/Renderer.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/AdaptiveAI.h"
#include "Common/RTS/GlobalData.h"

VisualizationOverlay* TheVisualizationOverlay = nullptr;

//=============================================================================
// Constructor / Destructor
//=============================================================================

VisualizationOverlay::VisualizationOverlay()
    : m_enabled(false)
    , m_showPriorityGraphs(true)
    , m_showHeatMaps(true)
    , m_showLearningFeed(true)
    , m_showTargetIndicators(true)
    , m_showDashboard(true)
{
}

VisualizationOverlay::~VisualizationOverlay()
{
    Shutdown();
}

//=============================================================================
// Initialization
//=============================================================================

bool VisualizationOverlay::Init()
{
    DBG_LOG("VisualizationOverlay::Init()");

    // Initialize data structures
    m_priorityHistories.clear();
    m_heatMapData.clear();
    m_learningEvents.clear();
    m_activeTargets.clear();

    m_enabled = true;
    return true;
}

void VisualizationOverlay::Shutdown()
{
    if (m_enabled) {
        DBG_LOG("VisualizationOverlay::Shutdown()");
        m_enabled = false;
    }
}

//=============================================================================
// Update
//=============================================================================

void VisualizationOverlay::Update()
{
    if (!m_enabled) return;

    // Prune old data to prevent unbounded growth
    PruneOldData();

    // Update current frame time for fading effects
    Real currentTime = TheGameLogic->getFrame() / 30.0f;

    // Update learning event fade
    for (LearningEventEntry& event : m_learningEvents) {
        event.fadeAge = currentTime - event.timestamp;
    }
}

//=============================================================================
// Render
//=============================================================================

void VisualizationOverlay::Render()
{
    if (!m_enabled) return;

    // Render in screen-space overlay mode (after world rendering)
    TheRenderer->SetOverlayMode(true);

    // Render enabled visualizations
    if (m_showPriorityGraphs) {
        RenderPriorityGraphs();
    }

    if (m_showHeatMaps) {
        RenderHeatMap();
    }

    if (m_showLearningFeed) {
        RenderLearningFeed();
    }

    if (m_showTargetIndicators) {
        RenderTargetIndicators();
    }

    if (m_showDashboard) {
        RenderDashboard();
    }

    TheRenderer->SetOverlayMode(false);
}

//=============================================================================
// Data Input Notifications
//=============================================================================

void VisualizationOverlay::NotifyTargetSelection(const Object* attacker, const Object* target, Real priority)
{
    if (!m_enabled || !target) return;

    Real currentTime = TheGameLogic->getFrame() / 30.0f;

    // Add to heat map
    const ObjectTemplate* targetType = target->getTemplate();
    bool found = false;

    for (HeatMapEntry& entry : m_heatMapData) {
        if (entry.targetType == targetType) {
            entry.count++;
            entry.lastUpdate = currentTime;
            found = true;
            break;
        }
    }

    if (!found) {
        HeatMapEntry newEntry;
        newEntry.targetType = targetType;
        newEntry.count = 1;
        newEntry.lastUpdate = currentTime;
        m_heatMapData.push_back(newEntry);
    }

    // Add target indicator
    TargetIndicator indicator;
    indicator.target = target;
    indicator.priority = priority;
    indicator.expiryTime = currentTime + 2.0f;  // Visible for 2 seconds
    m_activeTargets.push_back(indicator);
}

void VisualizationOverlay::NotifyPriorityUpdate(const ObjectTemplate* targetType, Real oldValue, Real newValue)
{
    if (!m_enabled || !targetType) return;

    Real currentTime = TheGameLogic->getFrame() / 30.0f;

    // Add data point to priority history
    PriorityHistory& history = m_priorityHistories[targetType];
    history.targetType = targetType;

    PriorityDataPoint point;
    point.timeSeconds = currentTime;
    point.multiplier = newValue;
    history.dataPoints.push_back(point);

    // Limit history size
    if (history.dataPoints.size() > PriorityHistory::MAX_HISTORY) {
        history.dataPoints.pop_front();
    }
}

void VisualizationOverlay::NotifyLearningEvent(const String& description)
{
    if (!m_enabled) return;

    Real currentTime = TheGameLogic->getFrame() / 30.0f;

    LearningEventEntry event;
    event.timestamp = currentTime;
    event.description = description;
    event.fadeAge = 0.0f;
    m_learningEvents.push_back(event);

    // Limit event log size
    if (m_learningEvents.size() > MAX_LEARNING_EVENTS) {
        m_learningEvents.pop_front();
    }
}

//=============================================================================
// Rendering Implementation
//=============================================================================

void VisualizationOverlay::RenderPriorityGraphs()
{
    // Graph position and size
    Int graphX = 20;
    Int graphY = 100;
    Int graphWidth = 400;
    Int graphHeight = 200;

    // Background
    TheRenderer->DrawRect(graphX, graphY, graphWidth, graphHeight, Color(0, 0, 0, 180));

    // Title
    TheRenderer->DrawText("Target Priority Multipliers", graphX + 10, graphY + 10, Color(255, 255, 255, 255));

    // Draw axes
    TheRenderer->DrawLine(graphX, graphY + graphHeight, graphX + graphWidth, graphY + graphHeight, Color(255, 255, 255, 128));  // X axis
    TheRenderer->DrawLine(graphX, graphY, graphX, graphY + graphHeight, Color(255, 255, 255, 128));  // Y axis

    // Draw grid lines
    for (Int i = 1; i <= 3; ++i) {
        Int y = graphY + (graphHeight * i / 4);
        TheRenderer->DrawLine(graphX, y, graphX + graphWidth, y, Color(128, 128, 128, 64));
    }

    // Y axis labels (0.0, 1.0, 2.0, 3.0)
    for (Int i = 0; i <= 3; ++i) {
        Real value = i * 1.0f;
        Int y = graphY + graphHeight - (Int)(graphHeight * value / 3.0f);
        String label = String::Format("%.1f", value);
        TheRenderer->DrawText(label, graphX - 30, y - 5, Color(200, 200, 200, 255));
    }

    // Draw priority history lines
    Int colorIndex = 0;
    for (auto& pair : m_priorityHistories) {
        const PriorityHistory& history = pair.second;

        if (history.dataPoints.size() < 2) continue;

        // Get color for this line
        Color lineColor = GetPriorityColor(history.dataPoints.back().multiplier);

        // Draw line segments
        for (Int i = 0; i < (Int)history.dataPoints.size() - 1; ++i) {
            const PriorityDataPoint& p1 = history.dataPoints[i];
            const PriorityDataPoint& p2 = history.dataPoints[i + 1];

            // Map data to screen coordinates
            Real xRatio1 = (Real)i / PriorityHistory::MAX_HISTORY;
            Real xRatio2 = (Real)(i + 1) / PriorityHistory::MAX_HISTORY;

            Int x1 = graphX + (Int)(graphWidth * xRatio1);
            Int x2 = graphX + (Int)(graphWidth * xRatio2);

            Int y1 = graphY + graphHeight - (Int)(graphHeight * p1.multiplier / 3.0f);
            Int y2 = graphY + graphHeight - (Int)(graphHeight * p2.multiplier / 3.0f);

            TheRenderer->DrawLine(x1, y1, x2, y2, lineColor);
        }

        // Draw legend label
        String label = history.targetType->getName();
        Int legendY = graphY + 30 + (colorIndex * 15);
        TheRenderer->DrawRect(graphX + graphWidth - 150, legendY, 10, 10, lineColor);
        TheRenderer->DrawText(label, graphX + graphWidth - 135, legendY, Color(255, 255, 255, 255));

        colorIndex++;
        if (colorIndex >= 10) break;  // Max 10 lines
    }
}

void VisualizationOverlay::RenderHeatMap()
{
    // Heat map position
    Int mapX = 20;
    Int mapY = 320;
    Int mapWidth = 400;
    Int barHeight = 20;

    // Background
    Int mapHeight = (Int)m_heatMapData.size() * (barHeight + 5) + 30;
    TheRenderer->DrawRect(mapX, mapY, mapWidth, mapHeight, Color(0, 0, 0, 180));

    // Title
    TheRenderer->DrawText("Target Selection Heat Map (Last 60s)", mapX + 10, mapY + 10, Color(255, 255, 255, 255));

    // Sort by count (descending)
    Vector<HeatMapEntry> sortedData = m_heatMapData;
    std::sort(sortedData.begin(), sortedData.end(), [](const HeatMapEntry& a, const HeatMapEntry& b) {
        return a.count > b.count;
    });

    // Find max count for scaling
    Int maxCount = 1;
    for (const HeatMapEntry& entry : sortedData) {
        if (entry.count > maxCount) maxCount = entry.count;
    }

    // Draw bars
    for (Int i = 0; i < (Int)sortedData.size() && i < 10; ++i) {
        const HeatMapEntry& entry = sortedData[i];

        Int barY = mapY + 30 + i * (barHeight + 5);
        Int barWidth = (Int)(350.0f * entry.count / maxCount);

        // Bar
        Color barColor = Color(255, (255 - entry.count * 2), 0, 200);
        TheRenderer->DrawRect(mapX + 10, barY, barWidth, barHeight, barColor);

        // Label
        String label = String::Format("%s (%d)", entry.targetType->getName().c_str(), entry.count);
        TheRenderer->DrawText(label, mapX + 15, barY + 3, Color(255, 255, 255, 255));
    }
}

void VisualizationOverlay::RenderLearningFeed()
{
    // Feed position
    Int feedX = TheRenderer->GetScreenWidth() - 450;
    Int feedY = 100;
    Int feedWidth = 430;

    // Background
    Int feedHeight = MAX_LEARNING_EVENTS * 40 + 30;
    TheRenderer->DrawRect(feedX, feedY, feedWidth, feedHeight, Color(0, 0, 0, 180));

    // Title
    TheRenderer->DrawText("Learning Events", feedX + 10, feedY + 10, Color(255, 255, 255, 255));

    // Draw events (newest at top)
    Int yOffset = 30;
    for (auto it = m_learningEvents.rbegin(); it != m_learningEvents.rend(); ++it) {
        const LearningEventEntry& event = *it;

        // Calculate fade alpha
        Real fadeAlpha = 255.0f;
        if (event.fadeAge > 10.0f) {
            fadeAlpha = 255.0f * (1.0f - (event.fadeAge - 10.0f) / 20.0f);
            if (fadeAlpha < 0) fadeAlpha = 0;
        }

        // Timestamp
        Int minutes = (Int)event.timestamp / 60;
        Int seconds = (Int)event.timestamp % 60;
        String timestamp = String::Format("[%02d:%02d]", minutes, seconds);

        TheRenderer->DrawText(timestamp, feedX + 10, feedY + yOffset, Color(200, 200, 100, (Int)fadeAlpha));

        // Description (word-wrapped if needed)
        TheRenderer->DrawText(event.description, feedX + 70, feedY + yOffset, Color(255, 255, 255, (Int)fadeAlpha));

        yOffset += 40;
    }
}

void VisualizationOverlay::RenderTargetIndicators()
{
    Real currentTime = TheGameLogic->getFrame() / 30.0f;

    for (const TargetIndicator& indicator : m_activeTargets) {
        if (currentTime > indicator.expiryTime) continue;
        if (!indicator.target || indicator.target->isDestroyed()) continue;

        // Get target's screen position
        Vector3 worldPos = indicator.target->getPosition();
        worldPos.z += 50.0f;  // Offset above unit

        Vector2 screenPos = WorldToScreen(worldPos);

        // Draw crosshair
        Color color = GetPriorityColor(indicator.priority);
        Int size = 20;

        TheRenderer->DrawLine(screenPos.x - size, screenPos.y, screenPos.x + size, screenPos.y, color);
        TheRenderer->DrawLine(screenPos.x, screenPos.y - size, screenPos.x, screenPos.y + size, color);

        // Draw priority text
        String priorityText = String::Format("%.1fx", indicator.priority);
        TheRenderer->DrawText(priorityText, screenPos.x + 25, screenPos.y - 10, color);
    }
}

void VisualizationOverlay::RenderDashboard()
{
    // Dashboard position (top-right)
    Int dashX = TheRenderer->GetScreenWidth() - 450;
    Int dashY = 20;
    Int dashWidth = 430;
    Int dashHeight = 150;

    // Background
    TheRenderer->DrawRect(dashX, dashY, dashWidth, dashHeight, Color(0, 0, 0, 180));

    // Get AI player (assume first AI player for now)
    const Player* aiPlayer = nullptr;
    for (const Player* player : TheGameLogic->getPlayers()) {
        if (player && player->isAI()) {
            aiPlayer = player;
            break;
        }
    }

    if (!aiPlayer) return;

    // Title
    String title = String::Format("%s - %s AI", aiPlayer->getName().c_str(),
                                   GetPersonalityName(aiPlayer->getAIPersonality()).c_str());
    TheRenderer->DrawText(title, dashX + 10, dashY + 10, Color(255, 255, 100, 255));

    // Stats
    Int yOffset = 35;
    Int lineHeight = 18;

    // Get stats from AdaptiveAI
    if (TheAdaptiveAI) {
        // Total kills, learning samples, etc.
        // (These accessors would need to be added to AdaptiveAI)

        TheRenderer->DrawText(String::Format("Total Kills: %d", 0 /* TODO */),
                             dashX + 15, dashY + yOffset, Color(255, 255, 255, 255));
        yOffset += lineHeight;

        TheRenderer->DrawText(String::Format("Learning Samples: %d", 0 /* TODO */),
                             dashX + 15, dashY + yOffset, Color(255, 255, 255, 255));
        yOffset += lineHeight;

        TheRenderer->DrawText(String::Format("Avg Effectiveness: %.2f", 0.0f /* TODO */),
                             dashX + 15, dashY + yOffset, Color(255, 255, 255, 255));
        yOffset += lineHeight;

        // Top priority target
        TheRenderer->DrawText("Top Priority: Supply Trucks" /* TODO: dynamic */,
                             dashX + 15, dashY + yOffset, Color(255, 200, 100, 255));
    }
}

//=============================================================================
// Utility Functions
//=============================================================================

void VisualizationOverlay::PruneOldData()
{
    Real currentTime = TheGameLogic->getFrame() / 30.0f;

    // Prune heat map data older than window
    m_heatMapData.erase(
        std::remove_if(m_heatMapData.begin(), m_heatMapData.end(),
            [currentTime](const HeatMapEntry& entry) {
                return (currentTime - entry.lastUpdate) > HEATMAP_WINDOW;
            }),
        m_heatMapData.end()
    );

    // Prune expired target indicators
    m_activeTargets.erase(
        std::remove_if(m_activeTargets.begin(), m_activeTargets.end(),
            [currentTime](const TargetIndicator& indicator) {
                return currentTime > indicator.expiryTime;
            }),
        m_activeTargets.end()
    );

    // Prune very old learning events (>30 seconds)
    m_learningEvents.erase(
        std::remove_if(m_learningEvents.begin(), m_learningEvents.end(),
            [currentTime](const LearningEventEntry& event) {
                return (currentTime - event.timestamp) > 30.0f;
            }),
        m_learningEvents.end()
    );
}

Color VisualizationOverlay::GetPriorityColor(Real multiplier)
{
    if (multiplier >= 2.0f) {
        return Color(255, 0, 0, 255);      // Red - very high
    } else if (multiplier >= 1.5f) {
        return Color(255, 128, 0, 255);    // Orange - high
    } else if (multiplier >= 1.0f) {
        return Color(255, 255, 0, 255);    // Yellow - medium
    } else {
        return Color(128, 255, 128, 255);  // Light green - low
    }
}

Vector2 VisualizationOverlay::WorldToScreen(const Vector3& worldPos)
{
    // Use camera system to project world position to screen
    const Camera* camera = TheGameLogic->getCamera();
    if (!camera) return Vector2(0, 0);

    return camera->ProjectToScreen(worldPos);
}
```

### Integration with DecisionLogger

**Modify**: `/GeneralsMD/Code/GameEngine/Source/GameLogic/AI/DecisionLogger.cpp`

```cpp
void DecisionLogger::LogTargetSelection(/* ... parameters ... */)
{
    // ... existing logging code ...

    // Notify visualization overlay
    if (TheVisualizationOverlay) {
        TheVisualizationOverlay->NotifyTargetSelection(attacker, target, priorityScore);
    }
}

void DecisionLogger::LogPriorityUpdate(/* ... parameters ... */)
{
    // ... existing logging code ...

    // Notify visualization overlay
    if (TheVisualizationOverlay) {
        TheVisualizationOverlay->NotifyPriorityUpdate(targetType, oldMultiplier, newMultiplier);
    }
}

void DecisionLogger::LogLearningEvent(/* ... parameters ... */)
{
    // ... existing logging code ...

    // Notify visualization overlay
    if (TheVisualizationOverlay) {
        TheVisualizationOverlay->NotifyLearningEvent(description);
    }
}
```

### Keyboard Controls

**File**: `/GeneralsMD/Code/GameEngine/Source/Input/InputHandler.cpp`

```cpp
void InputHandler::ProcessKeyPress(Int key)
{
    // ... existing key handling ...

    // Visualization toggle keys (when laboratory mode enabled)
    if (g_theWriteableGlobalData->m_laboratoryMode) {
        if (key == KEY_F1) {
            if (TheVisualizationOverlay) {
                TheVisualizationOverlay->TogglePriorityGraphs();
            }
        }
        else if (key == KEY_F2) {
            if (TheVisualizationOverlay) {
                TheVisualizationOverlay->ToggleHeatMaps();
            }
        }
        else if (key == KEY_F3) {
            if (TheVisualizationOverlay) {
                TheVisualizationOverlay->ToggleLearningFeed();
            }
        }
        else if (key == KEY_F4) {
            if (TheVisualizationOverlay) {
                TheVisualizationOverlay->ToggleTargetIndicators();
            }
        }
        else if (key == KEY_F5) {
            if (TheVisualizationOverlay) {
                TheVisualizationOverlay->TogglePersonalityDashboard();
            }
        }
        else if (key == KEY_F12) {
            // Toggle entire overlay
            if (TheVisualizationOverlay) {
                TheVisualizationOverlay->SetEnabled(!TheVisualizationOverlay->IsEnabled());
            }
        }
    }
}
```

## Testing Plan

### Test 1: Overlay Renders
```bash
./RTS.exe --laboratory
```

Press F12 to toggle overlay. Should see graphs, heat maps, dashboard.

### Test 2: Live Data Updates
Play match, watch priority graphs update as AI learns.

**Expected**: Lines move upward for effective targets.

### Test 3: Target Indicators
Watch AI units attack enemies. Crosshairs should appear over targeted units.

**Expected**: High-priority targets have red crosshairs.

### Test 4: Learning Feed
AI discovers effective strategy.

**Expected**: Learning event appears in feed with timestamp.

### Test 5: Performance
Measure FPS with/without visualization enabled.

**Expected**: <5ms overhead per frame (60 FPS → 58 FPS acceptable).

## Performance Optimization

### Rendering Budget
- Max 5ms per frame for all visualizations
- Use batched rendering (group draw calls)
- Limit graph data points (200 max)
- Prune expired indicators

### Data Structures
- Use fixed-size circular buffers for history
- Pre-allocate string buffers
- Cache screen-space conversions

### Conditional Rendering
- Only render visible elements
- Skip off-screen indicators
- Update graphs at 5 FPS (not 30 FPS)

## CMake Changes

**File**: `/GeneralsMD/Code/GameEngine/CMakeLists.txt`

```cmake
set(RENDERER_SOURCES
    ${RENDERER_SOURCES}
    Source/Renderer/VisualizationOverlay.cpp
)
```

## Success Criteria

- ✅ All visualizations render correctly
- ✅ Live data updates in real-time
- ✅ Performance overhead <5ms per frame
- ✅ Keyboard controls work
- ✅ Overlays are clear and readable
- ✅ Learning events are informative
- ✅ Target indicators are accurate

## Next Phase

Once Phase 3 is complete, proceed to [Phase 4: Tournament System](./PHASE_4_TOURNAMENT_SYSTEM.md) to automate AI-vs-AI matches.
