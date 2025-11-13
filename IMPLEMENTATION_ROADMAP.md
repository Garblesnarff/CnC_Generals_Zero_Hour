# Implementation Roadmap: AI Laboratory System

**Document Version**: 1.0
**Last Updated**: 2025-01-13
**Status**: Ready for Implementation

## Overview

This roadmap provides practical guidance for implementing all 5 phases of the AI Laboratory System for Claude & Conquer. It includes timelines, dependencies, milestones, and risk mitigation strategies.

## Quick Reference

| Phase | Duration | Dependencies | Risk | Start After |
|-------|----------|--------------|------|-------------|
| Phase 1: Headless Mode | 3-5 days | None | Low | Immediately |
| Phase 2: Decision Logging | 4-6 days | Phase 1 (optional) | Low | Phase 1 complete |
| Phase 3: Visualization | 7-10 days | Phase 2 | Medium | Phase 2 complete |
| Phase 4: Tournament System | 5-7 days | Phase 1, 2 | Low | Phase 2 complete |
| Phase 5: Analysis Tools | 7-10 days | Phase 2, 4 | Medium | Phase 4 complete |

**Total Timeline**:
- **Minimum**: 26 days (everything goes perfectly)
- **Realistic**: 35-40 days (with testing and iteration)
- **Comfortable**: 50 days (with polish and unexpected issues)

## Dependency Graph

```
Phase 1: Headless Mode
    │
    ├──→ Phase 2: Decision Logging
    │       │
    │       ├──→ Phase 3: Visualization
    │       │
    │       └──→ Phase 4: Tournament System
    │               │
    │               └──→ Phase 5: Analysis Tools
    │
    └──→ Phase 4: Tournament System (dependency)
```

**Key Insight**: Phase 1 and 2 unlock everything else. Prioritize these.

## Implementation Strategy

### Strategy A: Sequential (Safest)

Build each phase completely before starting the next.

**Timeline**: 40-50 days

**Pros**:
- Each phase is fully tested before moving on
- Lower risk of cascading issues
- Easier to manage for solo developer

**Cons**:
- Slower time to first results
- Less parallel progress

**Recommended for**: Solo developers, first-time implementations

### Strategy B: Parallel (Fastest)

Work on multiple phases simultaneously.

**Timeline**: 25-35 days

**Pros**:
- Faster completion
- Can leverage multiple developers

**Cons**:
- Integration challenges
- Higher coordination overhead
- More complex debugging

**Recommended for**: Team of 2-3 developers with clear module ownership

### Strategy C: MVP First (Pragmatic)

Build minimum viable version of all phases, then iterate.

**Timeline**: 20 days MVP + 20 days polish = 40 days total

**Pros**:
- End-to-end working system quickly
- Can generate initial results while polishing
- Easier to validate approach

**Cons**:
- Initial results may be rough
- Requires discipline to go back and polish

**Recommended for**: Research-focused projects, proof-of-concept

## Phase-by-Phase Implementation Guide

### Phase 1: Headless Mode (Days 1-5)

**Goal**: Game runs without rendering, 50x+ faster.

**Implementation Steps**:

**Day 1**: Foundation
- [ ] Create `HeadlessVideoDevice.cpp` and `.h`
- [ ] Add headless mode fields to `GlobalData.h`
- [ ] Update CMakeLists.txt
- [ ] Test compilation

**Day 2**: Integration
- [ ] Implement video device factory selection
- [ ] Add command-line argument parsing
- [ ] Wire up video device creation
- [ ] Test headless mode launches

**Day 3**: Speed Control
- [ ] Implement speed multiplier in game loop
- [ ] Disable unnecessary systems in headless mode
- [ ] Add progress logging
- [ ] Test 10x speed

**Day 4**: Determinism
- [ ] Implement deterministic RNG
- [ ] Replace rand() calls in AI code
- [ ] Test reproducibility
- [ ] Document RNG usage

**Day 5**: Testing & Polish
- [ ] Run full match headless
- [ ] Verify 50x+ speedup achieved
- [ ] Test deterministic mode (same seed = same result)
- [ ] Write tests and documentation

**Milestone**: Headless match runs 50x faster than real-time.

**Gotchas**:
- Audio system may still run (not critical for Phase 1)
- Some systems may still poll for rendering
- Win detection might not work initially

**Exit Criteria**:
- ✅ Headless mode runs without crashing
- ✅ Speed multiplier of 50x achieved
- ✅ Deterministic mode produces identical results
- ✅ Normal rendering mode still works

### Phase 2: Decision Logging (Days 6-11)

**Goal**: Comprehensive AI decision data captured to files.

**Implementation Steps**:

**Day 6**: Core Logger
- [ ] Create `DecisionLogger.h` and `.cpp`
- [ ] Implement file I/O infrastructure
- [ ] Add buffering system
- [ ] Test file creation

**Day 7**: Event Types
- [ ] Implement target selection logging
- [ ] Implement priority update logging
- [ ] Add JSON formatting helpers
- [ ] Test JSON output validity

**Day 8**: Integration Points
- [ ] Add logging to AI target selection
- [ ] Add logging to adaptive learning
- [ ] Add logging to unit production (if time)
- [ ] Test data is being written

**Day 9**: CSV Export
- [ ] Implement CSV writers for each event type
- [ ] Test CSV loading in Python/pandas
- [ ] Verify data integrity
- [ ] Performance test (overhead <5%)

**Day 10**: Match Metadata
- [ ] Implement match metadata JSON
- [ ] Capture win conditions
- [ ] Record match duration
- [ ] Test end-to-end logging

**Day 11**: Testing & Polish
- [ ] Run multiple matches, verify logs
- [ ] Test with Phase 1 headless mode
- [ ] Load data in Python for validation
- [ ] Document data formats

**Milestone**: Complete match logged with 1000+ decision events.

**Gotchas**:
- Buffer overflow if events come too fast
- File I/O can slow headless mode (use buffering!)
- JSON formatting errors (use helper functions)

**Exit Criteria**:
- ✅ All AI decisions logged to JSON and CSV
- ✅ Performance overhead <5%
- ✅ Data loads correctly in Python
- ✅ Files are human-readable

### Phase 3: Visualization (Days 12-21)

**Goal**: Real-time overlay showing AI learning.

**Implementation Steps**:

**Day 12-13**: Core Overlay System
- [ ] Create `VisualizationOverlay.h` and `.cpp`
- [ ] Implement rendering integration
- [ ] Add keyboard toggle controls
- [ ] Test basic overlay draws

**Day 14-15**: Priority Graphs
- [ ] Implement graph rendering
- [ ] Add data buffering for history
- [ ] Test real-time updates
- [ ] Polish appearance

**Day 16-17**: Heat Maps & Feed
- [ ] Implement heat map rendering
- [ ] Add learning event feed
- [ ] Test fade-out animations
- [ ] Polish layout

**Day 18-19**: Target Indicators
- [ ] Implement 3D target markers
- [ ] Add priority color coding
- [ ] Test world-to-screen projection
- [ ] Optimize rendering

**Day 20**: Dashboard
- [ ] Implement personality dashboard
- [ ] Wire up AI statistics
- [ ] Test layout and readability
- [ ] Polish fonts and colors

**Day 21**: Testing & Polish
- [ ] Performance test (<5ms overhead)
- [ ] Test all keyboard shortcuts
- [ ] Watch full match with overlay
- [ ] Create demo video/screenshots

**Milestone**: Watch AI learn in real-time with graphs updating.

**Gotchas**:
- Rendering performance can be tricky
- World-to-screen projection needs camera integration
- Too much data can clutter the screen
- Font rendering may require library

**Exit Criteria**:
- ✅ All visualizations render correctly
- ✅ Performance overhead <5ms per frame
- ✅ Keyboard controls work
- ✅ Overlay is clear and informative

### Phase 4: Tournament System (Days 22-28)

**Goal**: Automated AI-vs-AI tournaments with result aggregation.

**Implementation Steps**:

**Day 22-23**: Scheduler
- [ ] Create `TournamentScheduler.h` and `.cpp`
- [ ] Implement JSON config loading
- [ ] Add pairing generation (round-robin)
- [ ] Test pairing logic

**Day 24-25**: Orchestrator
- [ ] Create `MatchOrchestrator.h` and `.cpp`
- [ ] Implement match launching
- [ ] Add win condition detection
- [ ] Test single match execution

**Day 26**: Python Wrapper
- [ ] Create `run_tournament.py`
- [ ] Implement match runner
- [ ] Add progress tracking
- [ ] Test tournament execution

**Day 27**: Result Aggregation
- [ ] Implement HTML report generation
- [ ] Add CSV export
- [ ] Calculate statistics
- [ ] Test report accuracy

**Day 28**: Testing & Polish
- [ ] Run mini tournament (6 matches)
- [ ] Verify determinism across runs
- [ ] Test parallel execution (if implemented)
- [ ] Document tournament configuration

**Milestone**: 25-match tournament completes unattended.

**Gotchas**:
- Match crashes can halt tournament (need error handling)
- Win detection may not work in all scenarios
- Seeding must be deterministic
- File path management across platforms

**Exit Criteria**:
- ✅ Tournament runs unattended
- ✅ Results are accurate
- ✅ HTML reports generated
- ✅ Deterministic matches work

### Phase 5: Analysis Tools (Days 29-38)

**Goal**: Automated insights from match data.

**Implementation Steps**:

**Day 29-30**: Data Loader
- [ ] Create `data_loader.py`
- [ ] Implement JSON/CSV parsing
- [ ] Add tournament data loading
- [ ] Test with Phase 4 output

**Day 31-32**: Strategy Detector
- [ ] Create `strategy_detector.py`
- [ ] Implement pattern detection algorithms
- [ ] Test against known strategies
- [ ] Validate accuracy

**Day 33-34**: Learning Analyzer
- [ ] Create `learning_analyzer.py`
- [ ] Implement convergence analysis
- [ ] Add comparative statistics
- [ ] Test with tournament data

**Day 35-36**: Report Generator
- [ ] Create `report_generator.py`
- [ ] Implement markdown generation
- [ ] Add visualization charts
- [ ] Test full report generation

**Day 37**: Integration
- [ ] Create `analyze_tournament.py` script
- [ ] Wire up all analyzers
- [ ] Test end-to-end pipeline
- [ ] Generate sample reports

**Day 38**: Testing & Polish
- [ ] Validate analysis accuracy
- [ ] Improve report formatting
- [ ] Add more visualizations
- [ ] Document analysis methods

**Milestone**: Full tournament analysis report with insights.

**Gotchas**:
- Statistical analysis requires understanding
- Visualization libraries can be finicky
- Report formatting takes time
- Need sufficient data for meaningful analysis

**Exit Criteria**:
- ✅ Analysis runs on tournament data
- ✅ Strategy detection is accurate
- ✅ Reports are comprehensive
- ✅ Visualizations are publication-quality

## Testing Strategy

### Unit Tests

**Phase 1**:
- Test headless video device initialization
- Test speed multiplier calculations
- Test deterministic RNG output

**Phase 2**:
- Test event buffering and flushing
- Test JSON formatting
- Test CSV generation

**Phase 3**:
- Test data structure pruning
- Test color selection logic
- Test coordinate transformations

**Phase 4**:
- Test pairing generation
- Test result aggregation
- Test report generation

**Phase 5**:
- Test data loading
- Test strategy classification
- Test statistical calculations

### Integration Tests

**Phases 1+2**: Run headless match with logging enabled
**Phases 2+3**: Watch match with visualization showing logged data
**Phases 1+2+4**: Run 6-match tournament headless
**Phases 2+4+5**: Run tournament, generate analysis report

### End-to-End Tests

**Full System Test**:
1. Configure 4-personality tournament (25 matches)
2. Run tournament headless (100x speed)
3. Generate analysis report
4. Verify insights are meaningful

**Expected Duration**: 2-3 hours for 25 matches

## Risk Management

### High-Risk Areas

**Phase 1**: Rendering/logic separation
- **Mitigation**: Test frequently, keep changes minimal
- **Fallback**: Implement speed multiplier without full headless

**Phase 3**: Rendering performance
- **Mitigation**: Profile early, optimize aggressively
- **Fallback**: Reduce visualization complexity

**Phase 5**: Analysis accuracy
- **Mitigation**: Validate against manual analysis
- **Fallback**: Start with simple statistics, add complexity later

### Common Pitfalls

1. **Scope Creep**: "Let's add just one more feature..."
   - **Solution**: Stick to phase specifications, track "nice-to-have" separately

2. **Premature Optimization**: Optimizing before measuring
   - **Solution**: Profile first, then optimize bottlenecks

3. **Integration Issues**: Phases don't work together
   - **Solution**: Test integration points early and often

4. **Data Format Changes**: Breaking changes to log formats
   - **Solution**: Version your data formats, write migration scripts

5. **Insufficient Testing**: Bugs discovered late
   - **Solution**: Test each phase before moving to next

## Progress Tracking

### Daily Standup Questions

1. What did I complete yesterday?
2. What will I complete today?
3. What blockers do I have?
4. Am I on track for phase deadline?

### Weekly Milestones

**Week 1**: Phase 1 complete, headless mode working
**Week 2**: Phase 2 complete, decision logging working
**Week 3**: Phase 3 in progress, basic visualization working
**Week 4**: Phase 3 complete, Phase 4 in progress
**Week 5**: Phase 4 complete, tournaments running
**Week 6**: Phase 5 in progress, analysis tools working
**Week 7**: Phase 5 complete, full system operational
**Week 8**: Buffer for polish and documentation

### Success Metrics

**Phase 1**:
- ✅ Headless match runs 50x faster
- ✅ Deterministic matches are reproducible

**Phase 2**:
- ✅ 1000+ events logged per match
- ✅ Data loads in Python without errors

**Phase 3**:
- ✅ Can watch AI learning in real-time
- ✅ Overhead <5ms per frame

**Phase 4**:
- ✅ 25-match tournament completes unattended
- ✅ Win rates calculated correctly

**Phase 5**:
- ✅ Strategies correctly identified (>80% accuracy)
- ✅ Reports generate insights

## MVP Definition

**Minimum Viable Product** (for research purposes):

1. **Phase 1**: Headless mode with 20x speed (not 50x)
2. **Phase 2**: Target selection logging only (skip builds)
3. **Phase 3**: Priority graphs only (skip heat maps, indicators)
4. **Phase 4**: Round-robin only (skip other tournament types)
5. **Phase 5**: Basic statistics only (skip ML, advanced analysis)

**MVP Timeline**: 20-25 days

This MVP is sufficient to:
- Run tournaments quickly
- Collect learning data
- Generate basic insights
- Validate the approach

## Post-Implementation

### Validation Experiments

Once complete, run these experiments to validate the system:

**Experiment 1: Personality Comparison**
- Run 100 matches (4 personalities, 25 matches per pairing)
- Verify different win rates emerge
- Confirm strategies differ

**Experiment 2: Learning Verification**
- Run single personality vs itself (control)
- Verify priorities change over time
- Confirm learning stabilizes

**Experiment 3: Map Analysis**
- Run same personalities on 3 different maps
- Verify map affects win rates
- Identify map-specific strategies

**Experiment 4: Determinism Test**
- Run same match 10 times with same seed
- Verify identical outcomes
- Confirm reproducibility

### Future Enhancements

**Phase 1+**:
- Multi-threaded match execution (parallel tournaments)
- Save/load game states for replay analysis
- GPU-accelerated physics (if bottleneck)

**Phase 2+**:
- Real-time streaming to database
- Log compression (gzip old logs)
- More event types (diplomacy, tech research)

**Phase 3+**:
- VR visualization mode
- Web-based dashboard (view matches remotely)
- Video export of visualization

**Phase 4+**:
- Swiss-system tournaments
- Ladder/ranking system
- Matchmaking based on skill

**Phase 5+**:
- Machine learning predictions
- Causal inference (what caused wins?)
- Meta-game tracking over time
- Transfer learning experiments

## Resources Needed

### Development Environment
- C++ compiler (GCC 7+, Clang 9+, or MSVC 2019+)
- CMake 3.15+
- Python 3.8+
- Git for version control

### Libraries (already in codebase or easy to add)
- nlohmann/json (JSON parsing)
- STL (C++ standard library)

### Python Packages
```bash
pip install pandas numpy matplotlib seaborn scipy jupyter
```

### Hardware Recommendations
- **Minimum**: 4-core CPU, 8GB RAM
- **Recommended**: 8-core CPU, 16GB RAM, SSD
- **Optimal**: 16-core CPU, 32GB RAM, NVMe SSD

### Time Investment
- **Solo Developer**: 40-50 days full-time (8 hours/day)
- **Part-Time**: 3-4 months (2-3 hours/day)
- **Team of 3**: 20-25 days (parallel work)

## Getting Started

### Immediate Next Steps

1. **Read all phase documents** (this + 5 phase docs)
2. **Choose implementation strategy** (Sequential/Parallel/MVP)
3. **Set up development environment** (build tools, libraries)
4. **Create project branch** (git branch ai-laboratory-phase1)
5. **Start Phase 1** (follow PHASE_1_HEADLESS_MODE.md)

### First Week Goals

- [ ] Complete Phase 1 implementation
- [ ] Test headless mode with 50x speed
- [ ] Verify deterministic matches work
- [ ] Document any deviations from spec
- [ ] Begin Phase 2 planning

## Questions & Support

### Common Questions

**Q: Can I skip phases?**
A: Phase 1 and 2 are required. Phase 3 is optional (but valuable). Phase 4 and 5 require Phase 2.

**Q: Can I change the order?**
A: No. Dependencies are strict. Phase 2 needs Phase 1 benefits. Phase 4 needs Phase 2 data.

**Q: What if I get stuck?**
A: Refer to phase documents, check integration points, add debug logging, test in isolation.

**Q: How do I know if I'm done?**
A: Each phase has "Exit Criteria" checklist. All items must pass.

**Q: Can I use this for other RTS games?**
A: Yes! The architecture is RTS-agnostic. Adapt the integration points.

## Conclusion

This roadmap provides everything needed to implement the AI Laboratory System. Follow the phases sequentially, test thoroughly, and stick to the specifications.

The result will be a **research-grade platform** for studying emergent AI behavior in RTS games.

**Now go build something amazing.** 🚀

---

## Document History

- **v1.0** (2025-01-13): Initial roadmap created
- Document all changes here as implementation progresses
