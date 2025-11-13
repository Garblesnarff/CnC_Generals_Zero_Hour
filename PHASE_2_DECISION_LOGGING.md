# Phase 2: Decision Logging System

**Goal**: Capture comprehensive AI decision-making data during matches for analysis and visualization.

**Time Estimate**: 4-6 days
**Dependencies**: Phase 1 (benefits from headless mode for fast data generation)
**Risk Level**: Low
**Priority**: High (required for Phases 3, 4, 5)

## Overview

The Decision Logging System instruments the AI to record:
- **Target Selection**: Which unit chose which target and why
- **Priority Changes**: When adaptive multipliers change and by how much
- **Learning Events**: When AI discovers effective strategies
- **Build Orders**: What AI constructs and when
- **Strategic Decisions**: Base placement, expansion timing, tech choices

This data enables:
- Understanding AI behavior
- Debugging learning algorithms
- Visualizing decision-making
- Analyzing strategy emergence
- Comparing AI personalities

## Architecture

```
┌────────────────────────────────────────────────┐
│            AI Decision Points                  │
├────────────────────────────────────────────────┤
│  • Target Selection                            │
│  • Priority Updates                            │
│  • Unit Production                             │
│  • Building Placement                          │
│  • Tech Research                               │
│  • Attack/Defend                               │
└───────────────┬────────────────────────────────┘
                │
                ▼
┌────────────────────────────────────────────────┐
│         DecisionLogger (Singleton)             │
├────────────────────────────────────────────────┤
│  • Buffer events in memory                     │
│  • Flush periodically to disk                  │
│  • Format: JSON and CSV                        │
│  • Low overhead (<1% CPU)                      │
└───────────────┬────────────────────────────────┘
                │
                ▼
┌────────────────────────────────────────────────┐
│            Output Files                        │
├────────────────────────────────────────────────┤
│  match_20250113_001/                           │
│  ├── decisions.json      (all events)          │
│  ├── target_selection.csv (quick analysis)     │
│  ├── learning_events.csv  (priority changes)   │
│  ├── build_orders.csv     (production)         │
│  └── match_metadata.json  (context)            │
└────────────────────────────────────────────────┘
```

## Event Types

### 1. Target Selection Event

Logged when AI chooses an attack target.

```cpp
struct TargetSelectionEvent
{
    UInt32 frame;                    // Game frame
    Real timeSeconds;                // Simulated time
    const Player* aiPlayer;          // Which AI
    AIPersonality personality;       // AI personality type
    const Object* attackerUnit;      // Unit selecting target
    const Object* targetUnit;        // Selected target
    const ObjectTemplate* targetType;// Target's type
    Real priorityScore;              // Final priority score
    Real adaptiveMultiplier;         // Learned multiplier (1.0 = neutral)
    Real distance;                   // Distance to target
    String reason;                   // Human-readable explanation
};
```

**Example JSON**:
```json
{
  "type": "target_selection",
  "frame": 1500,
  "time_seconds": 50.0,
  "ai_player": "USA_Player_1",
  "personality": "ECONOMIC",
  "attacker_unit": "M1A2_Abrams_0x1234",
  "attacker_type": "M1A2Abrams",
  "target_unit": "Supply_Truck_0x5678",
  "target_type": "SupplyTruck",
  "priority_score": 2.3,
  "adaptive_multiplier": 2.1,
  "distance": 150.0,
  "reason": "High economic value, learned from 12 previous kills"
}
```

**CSV Format**:
```csv
frame,time_seconds,ai_player,personality,attacker_type,target_type,priority_score,adaptive_multiplier,distance
1500,50.0,USA_Player_1,ECONOMIC,M1A2Abrams,SupplyTruck,2.3,2.1,150.0
```

### 2. Priority Update Event

Logged when adaptive AI changes target priority multipliers.

```cpp
struct PriorityUpdateEvent
{
    UInt32 frame;
    Real timeSeconds;
    const Player* aiPlayer;
    AIPersonality personality;
    const ObjectTemplate* targetType;
    Real oldMultiplier;
    Real newMultiplier;
    Real effectivenessScore;         // What triggered the update
    Int sampleCount;                 // How many kills informed this
    String reason;
};
```

**Example JSON**:
```json
{
  "type": "priority_update",
  "frame": 2000,
  "time_seconds": 66.7,
  "ai_player": "USA_Player_1",
  "personality": "ECONOMIC",
  "target_type": "SupplyTruck",
  "old_multiplier": 1.0,
  "new_multiplier": 2.1,
  "effectiveness_score": 0.85,
  "sample_count": 12,
  "reason": "High effectiveness: destroyed 12 supply trucks, average economic damage $15,000"
}
```

### 3. Unit Production Event

Logged when AI builds units.

```cpp
struct UnitProductionEvent
{
    UInt32 frame;
    Real timeSeconds;
    const Player* aiPlayer;
    AIPersonality personality;
    const ObjectTemplate* unitType;
    const Object* producer;          // Factory/barracks that built it
    Int cost;
    Int supplyUsed;
    String reason;                   // Why AI chose to build this
};
```

### 4. Building Placement Event

Logged when AI places structures.

```cpp
struct BuildingPlacementEvent
{
    UInt32 frame;
    Real timeSeconds;
    const Player* aiPlayer;
    AIPersonality personality;
    const ObjectTemplate* buildingType;
    Vector3 position;
    Int cost;
    String reason;                   // Strategy reason
};
```

### 5. Learning Event

Logged when AI has a significant learning insight.

```cpp
struct LearningEvent
{
    UInt32 frame;
    Real timeSeconds;
    const Player* aiPlayer;
    AIPersonality personality;
    String eventType;                // "discovery", "adaptation", "convergence"
    String description;              // What was learned
    Real confidence;                 // 0.0-1.0
};
```

**Example**: "Discovered that Power Plants are high-value targets (multiplier 1.0 → 1.8)"

### 6. Kill Record Event

Logged when AI unit destroys enemy unit (already in AdaptiveAI, enhance it).

```cpp
struct KillRecordEvent
{
    UInt32 frame;
    Real timeSeconds;
    const Player* aiPlayer;
    AIPersonality personality;
    const Object* killerUnit;
    const ObjectTemplate* killerType;
    const Object* victimUnit;
    const ObjectTemplate* victimType;
    Int economicDamage;              // Cost of destroyed unit
    Int strategicValue;              // Was it a priority target?
    Real timeToKill;                 // How long engagement lasted
};
```

## Implementation

### DecisionLogger Class

**File**: `/GeneralsMD/Code/GameEngine/Include/GameLogic/DecisionLogger.h`

```cpp
#pragma once

#include "Common/RTS/RTS.h"
#include "Common/STL/Vector.h"
#include "Common/STL/String.h"
#include "GameLogic/Object.h"
#include "GameLogic/Player.h"
#include "GameLogic/AdaptiveAI.h"
#include <stdio.h>

//=============================================================================
// DecisionLogger
//
// Singleton that captures AI decision-making events for analysis.
// Buffers events in memory and flushes periodically to disk.
//=============================================================================

class DecisionLogger
{
public:
    DecisionLogger();
    ~DecisionLogger();

    // Initialization
    bool Init(const String& outputPath, const String& matchID);
    void Shutdown();

    // Event logging methods
    void LogTargetSelection(
        const Player* aiPlayer,
        const Object* attacker,
        const Object* target,
        Real priorityScore,
        Real adaptiveMultiplier,
        const String& reason
    );

    void LogPriorityUpdate(
        const Player* aiPlayer,
        const ObjectTemplate* targetType,
        Real oldMultiplier,
        Real newMultiplier,
        Real effectivenessScore,
        Int sampleCount
    );

    void LogUnitProduction(
        const Player* aiPlayer,
        const ObjectTemplate* unitType,
        const Object* producer,
        Int cost,
        Int supplyUsed,
        const String& reason
    );

    void LogBuildingPlacement(
        const Player* aiPlayer,
        const ObjectTemplate* buildingType,
        const Vector3& position,
        Int cost,
        const String& reason
    );

    void LogLearningEvent(
        const Player* aiPlayer,
        const String& eventType,
        const String& description,
        Real confidence
    );

    void LogKillRecord(
        const Player* aiPlayer,
        const Object* killer,
        const Object* victim,
        Int economicDamage,
        Int strategicValue,
        Real timeToKill
    );

    // Periodic maintenance
    void Update();                   // Call once per frame
    void Flush();                    // Force write to disk

    // Accessors
    bool IsLogging() const { return m_isLogging; }
    const String& GetOutputPath() const { return m_outputPath; }

private:
    // Internal event structure (generic)
    struct LogEvent
    {
        String type;
        UInt32 frame;
        Real timeSeconds;
        String data;                 // JSON string
    };

    // State
    bool m_isLogging;
    String m_outputPath;
    String m_matchID;
    UInt32 m_eventCount;

    // Event buffer
    Vector<LogEvent> m_eventBuffer;
    static const Int MAX_BUFFER_SIZE = 1000;

    // Output files
    FILE* m_jsonFile;                // All events
    FILE* m_targetSelectionCSV;
    FILE* m_learningEventsCSV;
    FILE* m_buildOrdersCSV;
    FILE* m_killRecordsCSV;

    // Helpers
    void WriteEventToJSON(const LogEvent& event);
    void WriteTargetSelectionToCSV(const LogEvent& event);
    void WriteLearningEventToCSV(const LogEvent& event);
    void FlushBuffer();
    String FormatJSON(const char* format, ...);
    String GetPersonalityName(AIPersonality personality);
};

// Global singleton
extern DecisionLogger* TheDecisionLogger;
```

**File**: `/GeneralsMD/Code/GameEngine/Source/GameLogic/AI/DecisionLogger.cpp`

```cpp
#include "GameLogic/DecisionLogger.h"
#include "Common/RTS/GlobalData.h"
#include "GameLogic/GameLogic.h"
#include <stdarg.h>
#include <time.h>

DecisionLogger* TheDecisionLogger = nullptr;

//=============================================================================
// Constructor / Destructor
//=============================================================================

DecisionLogger::DecisionLogger()
    : m_isLogging(false)
    , m_outputPath("")
    , m_matchID("")
    , m_eventCount(0)
    , m_jsonFile(nullptr)
    , m_targetSelectionCSV(nullptr)
    , m_learningEventsCSV(nullptr)
    , m_buildOrdersCSV(nullptr)
    , m_killRecordsCSV(nullptr)
{
}

DecisionLogger::~DecisionLogger()
{
    Shutdown();
}

//=============================================================================
// Initialization
//=============================================================================

bool DecisionLogger::Init(const String& outputPath, const String& matchID)
{
    m_outputPath = outputPath;
    m_matchID = matchID;

    // Create output directory
    String fullPath = outputPath + "/" + matchID + "/";

    #ifdef _WIN32
        CreateDirectoryA(fullPath.c_str(), nullptr);
    #else
        mkdir(fullPath.c_str(), 0755);
    #endif

    // Open JSON file (all events)
    String jsonPath = fullPath + "decisions.json";
    m_jsonFile = fopen(jsonPath.c_str(), "w");
    if (!m_jsonFile) {
        DBG_LOG("ERROR: Failed to open %s", jsonPath.c_str());
        return false;
    }

    // Write JSON header
    fprintf(m_jsonFile, "{\n");
    fprintf(m_jsonFile, "  \"match_id\": \"%s\",\n", matchID.c_str());
    fprintf(m_jsonFile, "  \"timestamp\": \"%s\",\n", GetTimestamp().c_str());
    fprintf(m_jsonFile, "  \"events\": [\n");

    // Open CSV files
    String targetSelectionPath = fullPath + "target_selection.csv";
    m_targetSelectionCSV = fopen(targetSelectionPath.c_str(), "w");
    if (m_targetSelectionCSV) {
        fprintf(m_targetSelectionCSV, "frame,time_seconds,ai_player,personality,attacker_type,target_type,priority_score,adaptive_multiplier,distance\n");
    }

    String learningEventsPath = fullPath + "learning_events.csv";
    m_learningEventsCSV = fopen(learningEventsPath.c_str(), "w");
    if (m_learningEventsCSV) {
        fprintf(m_learningEventsCSV, "frame,time_seconds,ai_player,personality,target_type,old_multiplier,new_multiplier,effectiveness_score,sample_count\n");
    }

    String buildOrdersPath = fullPath + "build_orders.csv";
    m_buildOrdersCSV = fopen(buildOrdersPath.c_str(), "w");
    if (m_buildOrdersCSV) {
        fprintf(m_buildOrdersCSV, "frame,time_seconds,ai_player,personality,unit_type,cost,supply_used\n");
    }

    String killRecordsPath = fullPath + "kill_records.csv";
    m_killRecordsCSV = fopen(killRecordsPath.c_str(), "w");
    if (m_killRecordsCSV) {
        fprintf(m_killRecordsCSV, "frame,time_seconds,ai_player,personality,killer_type,victim_type,economic_damage,strategic_value,time_to_kill\n");
    }

    m_isLogging = true;
    DBG_LOG("DecisionLogger initialized: %s", fullPath.c_str());
    return true;
}

void DecisionLogger::Shutdown()
{
    if (!m_isLogging) return;

    // Flush any remaining events
    FlushBuffer();

    // Close JSON file
    if (m_jsonFile) {
        fprintf(m_jsonFile, "\n  ]\n");
        fprintf(m_jsonFile, "}\n");
        fclose(m_jsonFile);
        m_jsonFile = nullptr;
    }

    // Close CSV files
    if (m_targetSelectionCSV) { fclose(m_targetSelectionCSV); m_targetSelectionCSV = nullptr; }
    if (m_learningEventsCSV) { fclose(m_learningEventsCSV); m_learningEventsCSV = nullptr; }
    if (m_buildOrdersCSV) { fclose(m_buildOrdersCSV); m_buildOrdersCSV = nullptr; }
    if (m_killRecordsCSV) { fclose(m_killRecordsCSV); m_killRecordsCSV = nullptr; }

    m_isLogging = false;
    DBG_LOG("DecisionLogger shut down");
}

//=============================================================================
// Event Logging
//=============================================================================

void DecisionLogger::LogTargetSelection(
    const Player* aiPlayer,
    const Object* attacker,
    const Object* target,
    Real priorityScore,
    Real adaptiveMultiplier,
    const String& reason)
{
    if (!m_isLogging || !aiPlayer || !attacker || !target) return;

    UInt32 frame = TheGameLogic->getFrame();
    Real timeSeconds = frame / 30.0f; // Assuming 30 FPS logic rate

    // Build JSON
    String json = FormatJSON(
        "\"type\":\"target_selection\","
        "\"frame\":%u,"
        "\"time_seconds\":%.2f,"
        "\"ai_player\":\"%s\","
        "\"personality\":\"%s\","
        "\"attacker_unit\":\"%s\","
        "\"attacker_type\":\"%s\","
        "\"target_unit\":\"%s\","
        "\"target_type\":\"%s\","
        "\"priority_score\":%.2f,"
        "\"adaptive_multiplier\":%.2f,"
        "\"distance\":%.1f,"
        "\"reason\":\"%s\"",
        frame,
        timeSeconds,
        aiPlayer->getName().c_str(),
        GetPersonalityName(aiPlayer->getAIPersonality()).c_str(),
        attacker->getName().c_str(),
        attacker->getTemplate()->getName().c_str(),
        target->getName().c_str(),
        target->getTemplate()->getName().c_str(),
        priorityScore,
        adaptiveMultiplier,
        (attacker->getPosition() - target->getPosition()).Length(),
        reason.c_str()
    );

    // Buffer event
    LogEvent event;
    event.type = "target_selection";
    event.frame = frame;
    event.timeSeconds = timeSeconds;
    event.data = json;
    m_eventBuffer.push_back(event);

    // Check if buffer needs flushing
    if (m_eventBuffer.size() >= MAX_BUFFER_SIZE) {
        FlushBuffer();
    }
}

void DecisionLogger::LogPriorityUpdate(
    const Player* aiPlayer,
    const ObjectTemplate* targetType,
    Real oldMultiplier,
    Real newMultiplier,
    Real effectivenessScore,
    Int sampleCount)
{
    if (!m_isLogging || !aiPlayer || !targetType) return;

    UInt32 frame = TheGameLogic->getFrame();
    Real timeSeconds = frame / 30.0f;

    String json = FormatJSON(
        "\"type\":\"priority_update\","
        "\"frame\":%u,"
        "\"time_seconds\":%.2f,"
        "\"ai_player\":\"%s\","
        "\"personality\":\"%s\","
        "\"target_type\":\"%s\","
        "\"old_multiplier\":%.2f,"
        "\"new_multiplier\":%.2f,"
        "\"effectiveness_score\":%.2f,"
        "\"sample_count\":%d",
        frame,
        timeSeconds,
        aiPlayer->getName().c_str(),
        GetPersonalityName(aiPlayer->getAIPersonality()).c_str(),
        targetType->getName().c_str(),
        oldMultiplier,
        newMultiplier,
        effectivenessScore,
        sampleCount
    );

    LogEvent event;
    event.type = "priority_update";
    event.frame = frame;
    event.timeSeconds = timeSeconds;
    event.data = json;
    m_eventBuffer.push_back(event);
}

// Similar implementations for LogUnitProduction, LogBuildingPlacement, LogLearningEvent, LogKillRecord...
// (Omitted for brevity - follow same pattern)

//=============================================================================
// Periodic Maintenance
//=============================================================================

void DecisionLogger::Update()
{
    // Flush buffer every 30 frames (1 second of game time)
    if (TheGameLogic->getFrame() % 30 == 0) {
        FlushBuffer();
    }
}

void DecisionLogger::Flush()
{
    FlushBuffer();
}

//=============================================================================
// Internal Helpers
//=============================================================================

void DecisionLogger::FlushBuffer()
{
    if (m_eventBuffer.empty()) return;

    for (const LogEvent& event : m_eventBuffer) {
        WriteEventToJSON(event);

        if (event.type == "target_selection") {
            WriteTargetSelectionToCSV(event);
        } else if (event.type == "priority_update") {
            WriteLearningEventToCSV(event);
        }
        // ... handle other types ...
    }

    // Flush file buffers
    if (m_jsonFile) fflush(m_jsonFile);
    if (m_targetSelectionCSV) fflush(m_targetSelectionCSV);
    if (m_learningEventsCSV) fflush(m_learningEventsCSV);
    if (m_buildOrdersCSV) fflush(m_buildOrdersCSV);
    if (m_killRecordsCSV) fflush(m_killRecordsCSV);

    m_eventBuffer.clear();
}

void DecisionLogger::WriteEventToJSON(const LogEvent& event)
{
    if (m_eventCount > 0) {
        fprintf(m_jsonFile, ",\n");
    }

    fprintf(m_jsonFile, "    {%s}", event.data.c_str());
    m_eventCount++;
}

void DecisionLogger::WriteTargetSelectionToCSV(const LogEvent& event)
{
    // Parse event.data and write to CSV
    // (For simplicity, store parsed fields in LogEvent structure)
    // This is a simplified example - real implementation would parse JSON

    if (m_targetSelectionCSV) {
        // CSV write implementation
        // fprintf(m_targetSelectionCSV, "...\n");
    }
}

String DecisionLogger::FormatJSON(const char* format, ...)
{
    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return String(buffer);
}

String DecisionLogger::GetPersonalityName(AIPersonality personality)
{
    switch (personality) {
        case AI_PERSONALITY_AGGRESSIVE: return "AGGRESSIVE";
        case AI_PERSONALITY_ECONOMIC: return "ECONOMIC";
        case AI_PERSONALITY_DEFENSIVE: return "DEFENSIVE";
        case AI_PERSONALITY_BALANCED: return "BALANCED";
        default: return "UNKNOWN";
    }
}

String DecisionLogger::GetTimestamp()
{
    time_t now = time(nullptr);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    return String(buffer);
}
```

### Integration Points

**1. Game Initialization** (`GameLogic::Init()`)

```cpp
bool GameLogic::Init()
{
    // ... existing initialization ...

    // Initialize decision logger if in laboratory mode
    if (g_theWriteableGlobalData->m_laboratoryMode) {
        String outputPath = g_theWriteableGlobalData->m_aiLogPath;
        if (outputPath.empty()) {
            outputPath = "./ai_logs";
        }

        // Generate match ID
        char matchID[64];
        time_t now = time(nullptr);
        strftime(matchID, sizeof(matchID), "match_%Y%m%d_%H%M%S", localtime(&now));

        TheDecisionLogger = new DecisionLogger();
        if (!TheDecisionLogger->Init(outputPath, matchID)) {
            DBG_LOG("WARNING: Failed to initialize DecisionLogger");
            delete TheDecisionLogger;
            TheDecisionLogger = nullptr;
        }
    }

    return true;
}
```

**2. Game Loop** (`GameLogic::Update()`)

```cpp
void GameLogic::Update()
{
    // ... existing update ...

    // Update decision logger
    if (TheDecisionLogger) {
        TheDecisionLogger->Update();
    }
}
```

**3. AI Target Selection** (`AI::findClosestEnemy()`)

```cpp
Object* AI::findClosestEnemy()
{
    // ... existing logic ...

    // Found target
    if (closestEnemy) {
        // Log the decision
        if (TheDecisionLogger) {
            Real adaptiveMultiplier = 1.0f;
            if (TheAdaptiveAI) {
                adaptiveMultiplier = TheAdaptiveAI->getPriorityMultiplier(closestEnemy->getTemplate());
            }

            String reason = "Closest valid target";
            if (adaptiveMultiplier > 1.5f) {
                reason = String::Format("High-priority learned target (%.1fx)", adaptiveMultiplier);
            }

            TheDecisionLogger->LogTargetSelection(
                getOwner(),
                this,
                closestEnemy,
                closestPriority,
                adaptiveMultiplier,
                reason
            );
        }
    }

    return closestEnemy;
}
```

**4. Adaptive AI Learning** (`AdaptiveCombatTracker::updatePriority()`)

```cpp
void AdaptiveCombatTracker::updatePriority(const ObjectTemplate* targetType)
{
    // ... existing priority calculation ...

    Real oldMultiplier = m_priorityMultipliers[targetType];
    Real newMultiplier = calculatedMultiplier;

    // Update the multiplier
    m_priorityMultipliers[targetType] = newMultiplier;

    // Log the learning event
    if (TheDecisionLogger) {
        TheDecisionLogger->LogPriorityUpdate(
            m_player,
            targetType,
            oldMultiplier,
            newMultiplier,
            effectivenessScore,
            sampleCount
        );
    }
}
```

## Testing Plan

### Test 1: Logging Initializes
```bash
./RTS.exe --laboratory --ai-log-path ./test_logs
```

**Expected**: Creates `./test_logs/match_YYYYMMDD_HHMMSS/` with CSV and JSON files.

### Test 2: Target Selection Logged
Play a match and verify `target_selection.csv` contains entries.

**Expected**: Each AI attack decision is logged with frame, units, priority.

### Test 3: Learning Events Logged
AI learns after 10+ kills of same target type.

**Expected**: `learning_events.csv` shows multiplier increasing from 1.0 to 2.0+.

### Test 4: Performance Impact
Run headless match with/without logging, compare speed.

**Expected**: Logging adds <5% overhead (buffering keeps it minimal).

### Test 5: Data Integrity
Load generated JSON into Python and verify it parses correctly.

```python
import json
with open('test_logs/match_001/decisions.json') as f:
    data = json.load(f)
    print(f"Logged {len(data['events'])} events")
```

## Data Analysis Examples

### Python: Load and Analyze

```python
import pandas as pd
import matplotlib.pyplot as plt

# Load target selection data
df = pd.read_csv('ai_logs/match_001/target_selection.csv')

# Find most-targeted unit types
target_counts = df['target_type'].value_counts()
print("Most targeted units:")
print(target_counts.head(10))

# Plot adaptive multiplier over time
economic_data = df[df['personality'] == 'ECONOMIC']
plt.plot(economic_data['time_seconds'], economic_data['adaptive_multiplier'])
plt.xlabel('Time (seconds)')
plt.ylabel('Adaptive Multiplier')
plt.title('Economic AI Learning Curve')
plt.show()
```

### Learning Event Analysis

```python
# Load learning events
learning = pd.read_csv('ai_logs/match_001/learning_events.csv')

# Find biggest priority changes
learning['delta'] = learning['new_multiplier'] - learning['old_multiplier']
learning_sorted = learning.sort_values('delta', ascending=False)

print("Biggest learning insights:")
for _, row in learning_sorted.head(5).iterrows():
    print(f"  {row['target_type']}: {row['old_multiplier']:.2f} → {row['new_multiplier']:.2f} "
          f"(effectiveness: {row['effectiveness_score']:.2f}, samples: {row['sample_count']})")
```

## Output File Formats

### decisions.json (Complete Log)
```json
{
  "match_id": "match_20250113_143022",
  "timestamp": "2025-01-13T14:30:22Z",
  "events": [
    {
      "type": "target_selection",
      "frame": 1500,
      "time_seconds": 50.0,
      "ai_player": "USA_Player_1",
      "personality": "ECONOMIC",
      "attacker_unit": "M1A2_Abrams_0x1234",
      "attacker_type": "M1A2Abrams",
      "target_unit": "Supply_Truck_0x5678",
      "target_type": "SupplyTruck",
      "priority_score": 2.3,
      "adaptive_multiplier": 2.1,
      "distance": 150.0,
      "reason": "High-priority learned target (2.1x)"
    },
    {
      "type": "priority_update",
      "frame": 2000,
      "time_seconds": 66.7,
      "ai_player": "USA_Player_1",
      "personality": "ECONOMIC",
      "target_type": "SupplyTruck",
      "old_multiplier": 2.0,
      "new_multiplier": 2.1,
      "effectiveness_score": 0.85,
      "sample_count": 13
    }
  ]
}
```

### match_metadata.json
```json
{
  "match_id": "match_20250113_143022",
  "start_time": "2025-01-13T14:30:22Z",
  "end_time": "2025-01-13T14:42:15Z",
  "duration_seconds": 715,
  "map": "tournament_arena",
  "players": [
    {
      "name": "USA_Player_1",
      "faction": "USA",
      "personality": "ECONOMIC",
      "is_ai": true
    },
    {
      "name": "China_Player_2",
      "faction": "China",
      "personality": "AGGRESSIVE",
      "is_ai": true
    }
  ],
  "winner": "USA_Player_1",
  "total_events_logged": 15432,
  "build_version": "claude-and-conquer-0.1.0"
}
```

## Performance Optimization

### Buffering Strategy
- Buffer 1000 events in memory before flushing
- Flush every 1 second of game time (30 frames)
- Prevents disk I/O from slowing simulation

### String Formatting
- Use stack buffers for JSON formatting (avoid heap allocation)
- Pre-allocate string buffers to avoid reallocation
- Use printf-style formatting (faster than string concatenation)

### File I/O
- Use buffered file streams
- Write CSV in addition to JSON for fast loading
- Compress old logs with gzip (Phase 5)

## Integration Checklist

- [ ] Create `DecisionLogger.h` and `.cpp`
- [ ] Add logging calls to AI target selection
- [ ] Add logging calls to adaptive learning
- [ ] Add logging calls to unit production
- [ ] Add logging calls to building placement
- [ ] Initialize logger in `GameLogic::Init()`
- [ ] Update logger in `GameLogic::Update()`
- [ ] Shutdown logger in `GameLogic::Shutdown()`
- [ ] Test with headless mode
- [ ] Verify JSON format is valid
- [ ] Verify CSV files load in Python/Excel
- [ ] Benchmark performance overhead
- [ ] Document data formats

## CMake Changes

**File**: `/GeneralsMD/Code/GameEngine/CMakeLists.txt`

```cmake
set(AI_SOURCES
    ${AI_SOURCES}
    Source/GameLogic/AI/DecisionLogger.cpp
)
```

## Success Criteria

- ✅ All AI decisions logged to JSON and CSV
- ✅ Files written to configurable output path
- ✅ Performance overhead <5%
- ✅ Data loads correctly in Python/pandas
- ✅ CSV format compatible with Excel
- ✅ JSON format valid and human-readable
- ✅ Buffer system prevents I/O bottlenecks
- ✅ Match metadata captured

## Next Phase

Once Phase 2 is complete, proceed to [Phase 3: Visualization Tools](./PHASE_3_VISUALIZATION.md) to make the logged data visible in real-time.
