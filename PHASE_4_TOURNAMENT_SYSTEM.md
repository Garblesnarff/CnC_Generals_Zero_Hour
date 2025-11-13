# Phase 4: Tournament System

**Goal**: Automate AI-vs-AI matches and collect comprehensive match statistics.

**Time Estimate**: 5-7 days
**Dependencies**: Phase 1 (headless mode), Phase 2 (decision logging)
**Risk Level**: Low (orchestration layer over existing systems)
**Priority**: High (enables large-scale AI research)

## Overview

The Tournament System automates competitive matches between AI personalities:
- **Match Scheduling**: Run hundreds of matches unattended
- **Bracket Management**: Round-robin, single/double elimination
- **Result Aggregation**: Win rates, statistics, patterns
- **Deterministic Testing**: Reproducible matches with fixed seeds
- **Parallel Execution**: Multiple matches simultaneously (future)

This transforms manual playtesting into **large-scale AI research**.

## Architecture

```
┌─────────────────────────────────────────────────────┐
│              Tournament Configuration                │
│  (JSON: personalities, maps, match count)            │
└──────────────────────┬──────────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────────┐
│           TournamentScheduler                        │
│  • Generate match pairings                           │
│  • Manage bracket progression                        │
│  • Track completed matches                           │
└──────────────────────┬──────────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────────┐
│            MatchOrchestrator                         │
│  • Launch game instance                              │
│  • Configure AI players                              │
│  • Monitor match progress                            │
│  • Detect win conditions                             │
│  • Collect results                                   │
└──────────────────────┬──────────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────────┐
│             Match Results Database                   │
│  • Match logs (decisions, kills, builds)             │
│  • Final statistics (winner, duration, etc.)         │
│  • Replay files (for analysis)                       │
└─────────────────────────────────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────────┐
│          StatisticalAnalyzer                         │
│  • Win rate by personality                           │
│  • Average match duration                            │
│  • Strategy patterns                                 │
│  • Generated reports (HTML, Markdown)                │
└─────────────────────────────────────────────────────┘
```

## Tournament Types

### 1. Round-Robin Tournament

Every personality plays every other personality N times.

**Example**: 4 personalities, 25 matches per pairing
```
Aggressive vs Economic:   25 matches
Aggressive vs Defensive:  25 matches
Aggressive vs Balanced:   25 matches
Economic vs Defensive:    25 matches
Economic vs Balanced:     25 matches
Defensive vs Balanced:    25 matches
─────────────────────────────────────
Total:                    150 matches
```

**Advantages**:
- Complete data for all matchups
- Statistical significance
- No bracket luck

### 2. Single Elimination Bracket

Standard tournament bracket.

**Example**: 8 AI instances
```
Round 1:  8 → 4  (4 matches)
Round 2:  4 → 2  (2 matches)
Finals:   2 → 1  (1 match)
─────────────────────────
Total:           7 matches
```

**Advantages**:
- Fast (fewer matches)
- Clear champion
- Exciting progression

### 3. Swiss System

Pair competitors with similar records.

**Advantages**:
- Balance between completeness and speed
- More fair than single elimination
- Suitable for large participant pools

### 4. Repeated Single-Map Training

Same map, same matchup, 100+ iterations to watch learning stabilize.

**Purpose**: Study convergence of learning algorithm.

## Configuration Format

**File**: `/tournaments/personality_championship.json`

```json
{
  "name": "Personality Championship - Round Robin",
  "type": "round_robin",
  "description": "Testing all 4 AI personalities against each other",

  "participants": [
    {
      "id": "aggressive_1",
      "personality": "AI_PERSONALITY_AGGRESSIVE",
      "faction": "USA",
      "starting_money": 10000,
      "starting_units": "default"
    },
    {
      "id": "economic_1",
      "personality": "AI_PERSONALITY_ECONOMIC",
      "faction": "China",
      "starting_money": 10000,
      "starting_units": "default"
    },
    {
      "id": "defensive_1",
      "personality": "AI_PERSONALITY_DEFENSIVE",
      "faction": "GLA",
      "starting_money": 10000,
      "starting_units": "default"
    },
    {
      "id": "balanced_1",
      "personality": "AI_PERSONALITY_BALANCED",
      "faction": "USA",
      "starting_money": 10000,
      "starting_units": "default"
    }
  ],

  "maps": [
    "maps/tournament_arena.map",
    "maps/desert_combat.map",
    "maps/winter_assault.map"
  ],

  "match_settings": {
    "matches_per_pairing": 25,
    "starting_money": 10000,
    "time_limit_minutes": 30,
    "deterministic": true,
    "seed_base": 10000
  },

  "execution": {
    "headless": true,
    "speed_multiplier": 100,
    "log_decisions": true,
    "log_path": "./tournament_results/personality_championship/",
    "parallel_matches": 1
  },

  "output": {
    "results_database": "tournament_results.db",
    "generate_html_report": true,
    "generate_markdown_summary": true,
    "save_replays": true
  }
}
```

## Implementation

### Core Classes

#### 1. TournamentScheduler

**File**: `/GeneralsMD/Code/Tools/TournamentRunner/TournamentScheduler.h`

```cpp
#pragma once

#include "Common/RTS/RTS.h"
#include "Common/STL/Vector.h"
#include "Common/STL/String.h"

//=============================================================================
// TournamentScheduler
//
// Manages tournament bracket and match scheduling.
// Generates pairings and tracks completion.
//=============================================================================

enum TournamentType
{
    TOURNAMENT_ROUND_ROBIN,
    TOURNAMENT_SINGLE_ELIMINATION,
    TOURNAMENT_DOUBLE_ELIMINATION,
    TOURNAMENT_SWISS,
};

struct TournamentParticipant
{
    String id;
    String personalityName;
    AIPersonality personality;
    String faction;
    Int startingMoney;
    String startingUnits;

    // Tournament statistics
    Int wins;
    Int losses;
    Int draws;
    Real totalPlayTime;
};

struct MatchPairing
{
    Int matchID;
    TournamentParticipant* player1;
    TournamentParticipant* player2;
    String map;
    UInt32 randomSeed;
    bool completed;
    String resultPath;              // Path to match log directory
};

struct MatchResult
{
    Int matchID;
    TournamentParticipant* winner;
    TournamentParticipant* loser;
    Real durationSeconds;
    String winCondition;            // "destruction", "timeout", "resignation"
    String logPath;
};

class TournamentScheduler
{
public:
    TournamentScheduler();
    ~TournamentScheduler();

    // Configuration
    bool LoadFromJSON(const String& configPath);
    bool SaveToJSON(const String& configPath);

    // Tournament management
    void GeneratePairings();
    MatchPairing* GetNextMatch();
    void RecordMatchResult(const MatchResult& result);
    bool IsComplete() const;

    // Statistics
    Int GetTotalMatches() const { return m_totalMatches; }
    Int GetCompletedMatches() const { return m_completedMatches; }
    Real GetProgress() const { return (Real)m_completedMatches / m_totalMatches; }

    // Results export
    void GenerateReport(const String& outputPath);
    void ExportToCSV(const String& outputPath);

private:
    // Configuration
    String m_name;
    TournamentType m_type;
    Vector<TournamentParticipant> m_participants;
    Vector<String> m_maps;
    Int m_matchesPerPairing;
    UInt32 m_seedBase;

    // Match scheduling
    Vector<MatchPairing> m_pairings;
    Int m_currentMatchIndex;
    Int m_totalMatches;
    Int m_completedMatches;

    // Results
    Vector<MatchResult> m_results;

    // Internal helpers
    void GenerateRoundRobinPairings();
    void GenerateSingleEliminationPairings();
    void UpdateParticipantStats(const MatchResult& result);
};
```

**File**: `/GeneralsMD/Code/Tools/TournamentRunner/TournamentScheduler.cpp`

```cpp
#include "TournamentRunner/TournamentScheduler.h"
#include <fstream>
#include <json/json.h>  // Using nlohmann/json

TournamentScheduler::TournamentScheduler()
    : m_type(TOURNAMENT_ROUND_ROBIN)
    , m_matchesPerPairing(1)
    , m_seedBase(10000)
    , m_currentMatchIndex(0)
    , m_totalMatches(0)
    , m_completedMatches(0)
{
}

TournamentScheduler::~TournamentScheduler()
{
}

//=============================================================================
// Configuration Loading
//=============================================================================

bool TournamentScheduler::LoadFromJSON(const String& configPath)
{
    std::ifstream file(configPath.c_str());
    if (!file.is_open()) {
        DBG_LOG("ERROR: Failed to open tournament config: %s", configPath.c_str());
        return false;
    }

    nlohmann::json config;
    file >> config;

    // Parse basic info
    m_name = config["name"].get<std::string>();

    String typeStr = config["type"].get<std::string>();
    if (typeStr == "round_robin") {
        m_type = TOURNAMENT_ROUND_ROBIN;
    } else if (typeStr == "single_elimination") {
        m_type = TOURNAMENT_SINGLE_ELIMINATION;
    }
    // ... other types ...

    // Parse participants
    for (const auto& pJson : config["participants"]) {
        TournamentParticipant participant;
        participant.id = pJson["id"].get<std::string>();
        participant.personalityName = pJson["personality"].get<std::string>();
        participant.personality = ParsePersonality(participant.personalityName);
        participant.faction = pJson["faction"].get<std::string>();
        participant.startingMoney = pJson["starting_money"].get<int>();
        participant.wins = 0;
        participant.losses = 0;
        participant.draws = 0;
        participant.totalPlayTime = 0.0f;

        m_participants.push_back(participant);
    }

    // Parse maps
    for (const auto& mapJson : config["maps"]) {
        m_maps.push_back(mapJson.get<std::string>());
    }

    // Parse match settings
    m_matchesPerPairing = config["match_settings"]["matches_per_pairing"].get<int>();
    m_seedBase = config["match_settings"]["seed_base"].get<unsigned int>();

    DBG_LOG("Loaded tournament config: %s", m_name.c_str());
    DBG_LOG("  Participants: %d", m_participants.size());
    DBG_LOG("  Maps: %d", m_maps.size());
    DBG_LOG("  Matches per pairing: %d", m_matchesPerPairing);

    return true;
}

//=============================================================================
// Pairing Generation
//=============================================================================

void TournamentScheduler::GeneratePairings()
{
    m_pairings.clear();

    switch (m_type) {
        case TOURNAMENT_ROUND_ROBIN:
            GenerateRoundRobinPairings();
            break;

        case TOURNAMENT_SINGLE_ELIMINATION:
            GenerateSingleEliminationPairings();
            break;

        default:
            DBG_LOG("ERROR: Unsupported tournament type");
            break;
    }

    m_totalMatches = (Int)m_pairings.size();
    m_completedMatches = 0;
    m_currentMatchIndex = 0;

    DBG_LOG("Generated %d match pairings", m_totalMatches);
}

void TournamentScheduler::GenerateRoundRobinPairings()
{
    Int matchID = 1;

    // For each pair of participants
    for (Int i = 0; i < (Int)m_participants.size(); ++i) {
        for (Int j = i + 1; j < (Int)m_participants.size(); ++j) {
            TournamentParticipant* p1 = &m_participants[i];
            TournamentParticipant* p2 = &m_participants[j];

            // Play N matches per pairing
            for (Int matchNum = 0; matchNum < m_matchesPerPairing; ++matchNum) {
                // For each map
                for (const String& map : m_maps) {
                    MatchPairing pairing;
                    pairing.matchID = matchID++;
                    pairing.player1 = p1;
                    pairing.player2 = p2;
                    pairing.map = map;
                    pairing.randomSeed = m_seedBase + pairing.matchID;
                    pairing.completed = false;

                    m_pairings.push_back(pairing);
                }
            }
        }
    }
}

void TournamentScheduler::GenerateSingleEliminationPairings()
{
    // TODO: Implement bracket-style pairing
    // This requires tracking rounds and updating pairings as matches complete
}

//=============================================================================
// Match Management
//=============================================================================

MatchPairing* TournamentScheduler::GetNextMatch()
{
    if (m_currentMatchIndex >= (Int)m_pairings.size()) {
        return nullptr;  // Tournament complete
    }

    MatchPairing* pairing = &m_pairings[m_currentMatchIndex];
    if (!pairing->completed) {
        return pairing;
    }

    // Move to next match
    ++m_currentMatchIndex;
    return GetNextMatch();  // Recurse to find next incomplete match
}

void TournamentScheduler::RecordMatchResult(const MatchResult& result)
{
    // Find the pairing
    for (MatchPairing& pairing : m_pairings) {
        if (pairing.matchID == result.matchID) {
            pairing.completed = true;
            pairing.resultPath = result.logPath;
            break;
        }
    }

    // Update statistics
    UpdateParticipantStats(result);

    // Store result
    m_results.push_back(result);
    ++m_completedMatches;

    DBG_LOG("Match %d complete: %s defeats %s (%.1fs)",
            result.matchID,
            result.winner->id.c_str(),
            result.loser->id.c_str(),
            result.durationSeconds);
}

bool TournamentScheduler::IsComplete() const
{
    return m_completedMatches >= m_totalMatches;
}

void TournamentScheduler::UpdateParticipantStats(const MatchResult& result)
{
    if (result.winner) {
        result.winner->wins++;
        result.winner->totalPlayTime += result.durationSeconds;
    }

    if (result.loser) {
        result.loser->losses++;
        result.loser->totalPlayTime += result.durationSeconds;
    }
}

//=============================================================================
// Report Generation
//=============================================================================

void TournamentScheduler::GenerateReport(const String& outputPath)
{
    // Generate HTML report with win rates, statistics, etc.
    // (Full implementation in separate file for brevity)

    std::ofstream html(outputPath + "/report.html");

    html << "<html><head><title>" << m_name << " - Results</title></head>\n";
    html << "<body>\n";
    html << "<h1>" << m_name << "</h1>\n";

    // Win rate table
    html << "<h2>Win Rates</h2>\n";
    html << "<table border='1'>\n";
    html << "<tr><th>Participant</th><th>Wins</th><th>Losses</th><th>Win Rate</th></tr>\n";

    for (const TournamentParticipant& p : m_participants) {
        Int totalGames = p.wins + p.losses;
        Real winRate = (totalGames > 0) ? ((Real)p.wins / totalGames * 100.0f) : 0.0f;

        html << "<tr>\n";
        html << "  <td>" << p.id << " (" << p.personalityName << ")</td>\n";
        html << "  <td>" << p.wins << "</td>\n";
        html << "  <td>" << p.losses << "</td>\n";
        html << "  <td>" << winRate << "%</td>\n";
        html << "</tr>\n";
    }

    html << "</table>\n";

    // Match history
    html << "<h2>Match History</h2>\n";
    html << "<table border='1'>\n";
    html << "<tr><th>Match</th><th>Winner</th><th>Loser</th><th>Duration</th><th>Map</th></tr>\n";

    for (const MatchResult& result : m_results) {
        html << "<tr>\n";
        html << "  <td>" << result.matchID << "</td>\n";
        html << "  <td>" << result.winner->id << "</td>\n";
        html << "  <td>" << result.loser->id << "</td>\n";
        html << "  <td>" << (Int)result.durationSeconds << "s</td>\n";
        html << "  <td>-</td>\n";  // Map name (TODO: store in MatchPairing)
        html << "</tr>\n";
    }

    html << "</table>\n";
    html << "</body></html>\n";

    html.close();

    DBG_LOG("Tournament report generated: %s", (outputPath + "/report.html").c_str());
}

void TournamentScheduler::ExportToCSV(const String& outputPath)
{
    // Export results to CSV for analysis in Excel/Python
    std::ofstream csv(outputPath + "/results.csv");

    csv << "match_id,winner_id,winner_personality,loser_id,loser_personality,duration_seconds,win_condition\n";

    for (const MatchResult& result : m_results) {
        csv << result.matchID << ","
            << result.winner->id << ","
            << result.winner->personalityName << ","
            << result.loser->id << ","
            << result.loser->personalityName << ","
            << result.durationSeconds << ","
            << result.winCondition << "\n";
    }

    csv.close();

    DBG_LOG("Results exported to CSV: %s", (outputPath + "/results.csv").c_str());
}
```

#### 2. MatchOrchestrator

**File**: `/GeneralsMD/Code/Tools/TournamentRunner/MatchOrchestrator.h`

```cpp
#pragma once

#include "TournamentScheduler.h"

//=============================================================================
// MatchOrchestrator
//
// Executes individual matches:
// - Configure game settings
// - Set up AI players
// - Monitor win conditions
// - Collect results
//=============================================================================

class MatchOrchestrator
{
public:
    MatchOrchestrator(TournamentScheduler* scheduler);
    ~MatchOrchestrator();

    // Match execution
    bool StartMatch(MatchPairing* pairing);
    void UpdateMatch();
    bool IsMatchComplete() const { return m_matchComplete; }
    MatchResult GetResult() const { return m_result; }

private:
    TournamentScheduler* m_scheduler;
    MatchPairing* m_currentPairing;
    bool m_matchComplete;
    MatchResult m_result;

    Real m_matchStartTime;
    Real m_timeLimit;

    // Internal
    void ConfigureGameForMatch(MatchPairing* pairing);
    void CheckWinConditions();
    void FinalizeMatch();
};
```

### Python Wrapper Script

**File**: `/Scripts/run_tournament.py`

```python
#!/usr/bin/env python3
"""
Tournament runner script.

Launches the game multiple times to run a complete tournament.
Monitors progress and handles failures gracefully.
"""

import subprocess
import json
import time
import sys
from pathlib import Path

def load_tournament_config(config_path):
    """Load tournament configuration from JSON."""
    with open(config_path, 'r') as f:
        return json.load(f)

def run_match(match_id, player1, player2, map_path, seed, speed_multiplier=100):
    """Run a single match."""
    print(f"Starting Match {match_id}: {player1['id']} vs {player2['id']} on {map_path}")

    # Build command line
    cmd = [
        "./RTS.exe",
        "--headless",
        "--speed-multiplier", str(speed_multiplier),
        "--deterministic", str(seed),
        "--laboratory",
        "--ai-log-path", f"./tournament_results/match_{match_id}/",
        "--map", map_path,
        "--player1-ai", player1['personality'],
        "--player2-ai", player2['personality'],
        "--match-id", str(match_id)
    ]

    # Run the match
    start_time = time.time()
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=600)
        duration = time.time() - start_time

        if result.returncode == 0:
            print(f"  ✓ Match {match_id} completed in {duration:.1f}s")
            return parse_match_result(match_id)
        else:
            print(f"  ✗ Match {match_id} failed with code {result.returncode}")
            print(f"    stderr: {result.stderr}")
            return None

    except subprocess.TimeoutExpired:
        print(f"  ✗ Match {match_id} timed out (10 minutes)")
        return None

def parse_match_result(match_id):
    """Parse match result from output files."""
    result_file = Path(f"./tournament_results/match_{match_id}/result.json")
    if not result_file.exists():
        return None

    with open(result_file, 'r') as f:
        return json.load(f)

def run_tournament(config_path):
    """Run complete tournament."""
    config = load_tournament_config(config_path)

    print(f"=== {config['name']} ===")
    print(f"Participants: {len(config['participants'])}")
    print(f"Maps: {len(config['maps'])}")
    print(f"Matches per pairing: {config['match_settings']['matches_per_pairing']}")
    print()

    # Generate match pairings
    participants = config['participants']
    maps = config['maps']
    matches_per_pairing = config['match_settings']['matches_per_pairing']

    match_id = 1
    total_matches = 0
    completed_matches = 0

    # Round-robin: every participant vs every other
    for i in range(len(participants)):
        for j in range(i + 1, len(participants)):
            p1 = participants[i]
            p2 = participants[j]

            for match_num in range(matches_per_pairing):
                for map_path in maps:
                    total_matches += 1

    print(f"Total matches to run: {total_matches}\n")

    # Run matches
    match_id = 1
    seed_base = config['match_settings']['seed_base']
    speed = config['execution']['speed_multiplier']

    for i in range(len(participants)):
        for j in range(i + 1, len(participants)):
            p1 = participants[i]
            p2 = participants[j]

            for match_num in range(matches_per_pairing):
                for map_path in maps:
                    seed = seed_base + match_id
                    result = run_match(match_id, p1, p2, map_path, seed, speed)

                    if result:
                        completed_matches += 1

                    progress = (completed_matches / total_matches) * 100
                    print(f"Progress: {completed_matches}/{total_matches} ({progress:.1f}%)\n")

                    match_id += 1

    print("=== Tournament Complete ===")
    print(f"Completed: {completed_matches}/{total_matches} matches")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python run_tournament.py <config.json>")
        sys.exit(1)

    config_path = sys.argv[1]
    run_tournament(config_path)
```

## Usage Examples

### Running a Tournament

```bash
# Generate tournament configuration
python Scripts/generate_tournament_config.py --type round_robin --participants 4 --matches 25

# Run the tournament
python Scripts/run_tournament.py tournaments/personality_championship.json

# View results
open tournament_results/personality_championship/report.html
```

### In-Game Tournament Mode

```bash
# Run tournament directly (C++ orchestrator)
./RTS.exe --tournament --config tournaments/quick_test.json
```

## Testing Plan

### Test 1: Single Match
Run one match to ensure orchestration works.

```bash
python Scripts/run_tournament.py tournaments/single_match_test.json
```

### Test 2: Mini Tournament
4 participants, 1 match per pairing.

**Expected**: 6 matches complete, HTML report generated.

### Test 3: Deterministic Verification
Run same match twice with same seed.

**Expected**: Identical results (same winner, same duration ±1 frame).

### Test 4: Full Tournament
4 personalities, 25 matches per pairing, 3 maps.

**Expected**: 450 matches complete in ~8 hours (headless 100x speed).

## Success Criteria

- ✅ Tournament configuration loads from JSON
- ✅ Match pairings generated correctly
- ✅ Matches execute unattended
- ✅ Results collected and aggregated
- ✅ HTML reports generated with statistics
- ✅ CSV export for external analysis
- ✅ Deterministic matches are reproducible
- ✅ Progress monitoring works

## Next Phase

Once Phase 4 is complete, proceed to [Phase 5: Analysis Tools](./PHASE_5_ANALYSIS_TOOLS.md) to extract insights from tournament data.
