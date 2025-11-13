# Claude & Conquer: AI Laboratory System

**Vision**: Transform C&C Generals Zero Hour into a research platform for emergent AI behavior through adaptive learning systems.

## Executive Summary

This document outlines the complete AI Laboratory system - a suite of tools and features designed to enable AI-vs-AI experimentation, real-time learning visualization, and emergent strategy discovery.

### Core Philosophy

Instead of hand-coding "smart" AI, we create **feedback loops** that allow AI to discover effective strategies through experience. The AI Laboratory makes this learning process *visible*, *measurable*, and *reproducible*.

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    AI Laboratory System                     │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │   Phase 1    │  │   Phase 2    │  │   Phase 3    │     │
│  │   Headless   │→ │   Decision   │→ │ Visualization│     │
│  │    Mode      │  │   Logging    │  │    Tools     │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
│         │                  │                  │            │
│         └──────────────────┴──────────────────┘            │
│                            │                               │
│         ┌──────────────────┴──────────────────┐            │
│         │                                     │            │
│  ┌──────────────┐                    ┌──────────────┐     │
│  │   Phase 4    │                    │   Phase 5    │     │
│  │  Tournament  │←───────────────────│   Analysis   │     │
│  │   System     │                    │    Tools     │     │
│  └──────────────┘                    └──────────────┘     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

## The Five Phases

### Phase 1: Headless Mode (Foundation)
**Goal**: Enable fast, GPU-free simulation for AI training

**Deliverables**:
- Simulation engine runs without rendering
- 10x-100x speed multiplier support
- Command-line configuration
- Deterministic replay capability

**Time Estimate**: 3-5 days
**Dependencies**: None (builds on current codebase)
**Risk Level**: Low (well-understood separation of logic/rendering)

### Phase 2: Decision Logging System (Instrumentation)
**Goal**: Capture AI decision-making data during matches

**Deliverables**:
- Structured logging of AI decisions
- Priority calculation tracking
- Learning event recording
- JSON/CSV export formats

**Time Estimate**: 4-6 days
**Dependencies**: Phase 1 (benefits from fast simulation)
**Risk Level**: Low (additive logging system)

### Phase 3: Visualization Tools (Understanding)
**Goal**: Make AI learning visible in real-time

**Deliverables**:
- Real-time graph overlays
- Heat maps of target priorities
- Learning curve visualization
- Debug overlay system

**Time Estimate**: 7-10 days
**Dependencies**: Phase 2 (needs logged data)
**Risk Level**: Medium (UI/rendering integration)

### Phase 4: Tournament System (Automation)
**Goal**: Run AI-vs-AI matches automatically

**Deliverables**:
- Automated match scheduler
- Bracket system for personalities
- Result aggregation
- Statistical analysis

**Time Estimate**: 5-7 days
**Dependencies**: Phase 1, 2 (needs headless + logging)
**Risk Level**: Low (orchestration layer)

### Phase 5: Analysis Tools (Insight)
**Goal**: Extract meaning from AI match data

**Deliverables**:
- Replay analysis tools
- Strategy emergence detection
- Automated report generation
- Comparative analysis

**Time Estimate**: 7-10 days
**Dependencies**: Phase 2, 4 (needs data corpus)
**Risk Level**: Medium (requires ML/statistics)

## Total Timeline

- **Minimum Viable**: 26 days (assuming no blockers)
- **Realistic**: 35-40 days (with testing and iteration)
- **Comfortable**: 50 days (with polish and documentation)

## Key Technologies

### Existing Systems (Leverage)
- **AdaptiveAI**: Learning system with personality support
- **Focus Fire**: Squad coordination
- **W3D Engine**: Rendering (to be bypassed in headless mode)
- **Game Logic**: Deterministic simulation

### New Systems (Build)
- **Headless Simulator**: Simulation without rendering
- **Decision Logger**: Structured event recording
- **Viz Overlay**: Real-time debug visualization
- **Tournament Runner**: Match orchestration
- **Analysis Engine**: Data processing and insights

### Supporting Libraries (Integrate)
- **JSON**: nlohmann/json for data export
- **CSV**: Fast CSV parser for analysis
- **Plotting**: ImGui + ImPlot for visualization
- **Statistics**: Basic statistical analysis (mean, stddev, regression)

## Data Flow

```
┌──────────────┐
│ Game Match   │
│  Simulation  │
└──────┬───────┘
       │
       ├─→ [Decision Logger] ──→ match_12345.json
       │                          ├─ ai_decisions.csv
       │                          ├─ learning_events.csv
       │                          └─ kill_records.csv
       │
       ├─→ [Real-time Viz] ──────→ Screen Overlay
       │                          ├─ Priority Graphs
       │                          ├─ Heat Maps
       │                          └─ Learning Curves
       │
       └─→ [Tournament System] ──→ tournament_results/
                                   ├─ match_logs/
                                   ├─ statistics/
                                   └─ reports/
           │
           ├─→ [Analysis Tools] ──→ Insights
                                   ├─ Strategy Detection
                                   ├─ Meta-Game Analysis
                                   └─ Generated Reports
```

## Success Metrics

### Technical Metrics
- **Performance**: Headless mode runs 50x+ faster than real-time
- **Data Quality**: 99%+ of AI decisions logged successfully
- **Automation**: Tournament system runs 100+ matches unattended
- **Visualization**: <16ms overhead for debug overlay

### Research Metrics
- **Learning Detection**: System identifies when AI adapts strategy
- **Emergence**: AI discovers strategies not explicitly programmed
- **Reproducibility**: Same initial conditions → same outcome
- **Insights**: Generate novel findings about AI behavior

### User Experience Metrics
- **Accessibility**: Non-programmers can run tournaments
- **Clarity**: Visualizations make AI reasoning understandable
- **Documentation**: Complete examples and tutorials
- **Extensibility**: Easy to add new analysis metrics

## Use Cases

### 1. AI Researcher
*"I want to study emergent behavior in competitive multi-agent systems"*

**Workflow**:
1. Configure tournament (4 personalities, 25 matches each)
2. Run overnight in headless mode
3. Review generated reports for patterns
4. Drill into specific matches using replay analyzer
5. Extract insights about learning dynamics

### 2. Game Designer
*"I want to balance the Economic personality against Aggressive"*

**Workflow**:
1. Run Economic vs Aggressive tournament (100 matches)
2. Review win rate statistics
3. Use visualization to see where Economic struggles
4. Adjust personality weights
5. Re-run tournament to validate changes

### 3. Strategy Enthusiast
*"I want to see if AI can discover the 'Tank Rush' strategy"*

**Workflow**:
1. Watch live match with visualization overlay
2. See AI learning in real-time (graphs updating)
3. Observe moment when AI discovers tank effectiveness
4. Review decision log to understand the discovery
5. Watch replay to see strategy execution

### 4. Content Creator
*"I want to create 'AI Tournament Championship' videos"*

**Workflow**:
1. Run tournament with commentator-friendly settings
2. Export match replays
3. Use visualization mode for dramatic effect
4. Generate statistics graphics from reports
5. Narrate the "story" of AI learning

## File Organization

```
/GeneralsMD/Code/
├── GameEngine/
│   ├── Include/GameLogic/
│   │   ├── AdaptiveAI.h                    [Existing - Enhanced]
│   │   ├── AILaboratory.h                  [New - Phase 4]
│   │   └── DecisionLogger.h                [New - Phase 2]
│   ├── Source/GameLogic/AI/
│   │   ├── AdaptiveAI.cpp                  [Existing - Enhanced]
│   │   ├── DecisionLogger.cpp              [New - Phase 2]
│   │   └── AILaboratory.cpp                [New - Phase 4]
│   └── Source/Renderer/
│       └── VisualizationOverlay.cpp        [New - Phase 3]
├── GameEngineDevice/
│   └── Source/VideoDevice/
│       └── HeadlessVideoDevice.cpp         [New - Phase 1]
├── Tools/
│   ├── AnalysisEngine/
│   │   ├── StrategyDetector.cpp            [New - Phase 5]
│   │   ├── ReplayAnalyzer.cpp              [New - Phase 5]
│   │   └── ReportGenerator.cpp             [New - Phase 5]
│   └── TournamentRunner/
│       ├── TournamentScheduler.cpp         [New - Phase 4]
│       └── MatchOrchestrator.cpp           [New - Phase 4]
└── Scripts/
    ├── run_tournament.py                   [New - Phase 4]
    ├── analyze_results.py                  [New - Phase 5]
    └── visualize_learning.py               [New - Phase 3]
```

## Integration Points

### 1. Game Initialization
```cpp
// In GameLogic::Init()
if (g_theWriteableGlobalData->m_laboratoryMode) {
    TheDecisionLogger = new DecisionLogger();
    TheAILaboratory = new AILaboratory();
}
```

### 2. Main Loop
```cpp
// In GameLogic::Update()
if (TheDecisionLogger) {
    TheDecisionLogger->flushBufferedEvents();
}
if (g_theWriteableGlobalData->m_headlessMode) {
    // Skip rendering, run faster
}
```

### 3. AI Decision Making
```cpp
// In AI::findClosestEnemy()
if (TheDecisionLogger) {
    TheDecisionLogger->logTargetSelection(this, target, priority, reason);
}
```

## Configuration

### Command Line Options
```bash
# Run headless with 50x speed
./RTS.exe --headless --speed-multiplier 50 --map maps/tournament_arena.map

# Enable decision logging
./RTS.exe --log-decisions --log-path ./ai_data/match_001/

# Run with visualization overlay
./RTS.exe --ai-viz --show-graphs --show-heatmaps

# Tournament mode
./RTS.exe --tournament --config tournaments/personalities_vs.json
```

### Configuration Files
```json
// tournament_config.json
{
  "name": "Personality Championship",
  "matches_per_pairing": 25,
  "participants": [
    {"name": "Aggressive", "personality": "AI_PERSONALITY_AGGRESSIVE"},
    {"name": "Economic", "personality": "AI_PERSONALITY_ECONOMIC"},
    {"name": "Defensive", "personality": "AI_PERSONALITY_DEFENSIVE"},
    {"name": "Balanced", "personality": "AI_PERSONALITY_BALANCED"}
  ],
  "maps": ["tournament_arena", "winter_assault", "desert_combat"],
  "headless": true,
  "speed_multiplier": 100,
  "log_decisions": true,
  "output_path": "./tournament_results/"
}
```

## Data Formats

### Decision Log (JSON)
```json
{
  "match_id": "match_20250113_001",
  "timestamp": "2025-01-13T10:30:00Z",
  "decisions": [
    {
      "frame": 1500,
      "time_seconds": 50.0,
      "ai_player": "USA_Player_1",
      "personality": "AI_PERSONALITY_ECONOMIC",
      "event_type": "target_selection",
      "attacker_unit": "M1A2_Abrams_0x1234",
      "target_unit": "Supply_Truck_0x5678",
      "target_type": "SupplyTruck",
      "priority_score": 2.3,
      "adaptive_multiplier": 2.1,
      "distance": 150.0,
      "reason": "High economic value, learned from 12 previous kills"
    }
  ]
}
```

### Learning Events (CSV)
```csv
match_id,frame,time_seconds,ai_player,personality,event_type,target_type,old_multiplier,new_multiplier,effectiveness_score,sample_count
match_001,2000,66.7,USA_Player_1,ECONOMIC,priority_update,SupplyTruck,1.0,2.1,0.85,12
match_001,3500,116.7,USA_Player_1,ECONOMIC,priority_update,PowerPlant,1.0,1.6,0.72,8
```

## Next Steps

1. **Review & Feedback**: Read through all phase documents
2. **Prioritize**: Confirm phase order or adjust based on needs
3. **Start Phase 1**: Begin with headless mode implementation
4. **Iterate**: Build, test, refine each phase
5. **Document**: Capture learnings and discoveries along the way

## Questions to Answer Through This System

1. **Learning Speed**: How many kills before AI adapts priorities?
2. **Personality Viability**: Which personalities win on which maps?
3. **Strategy Emergence**: What unexpected tactics emerge?
4. **Meta-Game**: Do counter-strategies develop between personalities?
5. **Learning Stability**: Do priorities converge or oscillate?
6. **Transfer Learning**: Does learning from one match help in the next?
7. **Overfitting**: Can AI over-specialize and become predictable?
8. **Personality Crossover**: Do personalities develop similar strategies over time?

## The Vision

When complete, Claude & Conquer will be the first RTS where:
- You can watch AI learn in real-time
- AI discovers strategies through experience, not programming
- Every match generates research-quality data
- The "meta-game" emerges from AI competition

**Not just a game. A laboratory for emergent intelligence.**

---

## Detailed Phase Documentation

See individual phase documents for complete implementation specifications:
- [Phase 1: Headless Mode](./PHASE_1_HEADLESS_MODE.md)
- [Phase 2: Decision Logging](./PHASE_2_DECISION_LOGGING.md)
- [Phase 3: Visualization](./PHASE_3_VISUALIZATION.md)
- [Phase 4: Tournament System](./PHASE_4_TOURNAMENT_SYSTEM.md)
- [Phase 5: Analysis Tools](./PHASE_5_ANALYSIS_TOOLS.md)
- [Implementation Roadmap](./IMPLEMENTATION_ROADMAP.md)
