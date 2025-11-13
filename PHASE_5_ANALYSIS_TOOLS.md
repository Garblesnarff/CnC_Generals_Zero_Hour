# Phase 5: Analysis Tools

**Goal**: Extract insights from AI match data through automated analysis and reporting.

**Time Estimate**: 7-10 days
**Dependencies**: Phase 2 (decision logs), Phase 4 (tournament data)
**Risk Level**: Medium (requires ML/statistics knowledge)
**Priority**: High (transforms data into insights)

## Overview

The Analysis System extracts meaning from raw match data:
- **Strategy Detection**: Identify emergent patterns (rush, turtle, harassment)
- **Learning Convergence**: When do priorities stabilize?
- **Comparative Analysis**: Which personality wins on which maps?
- **Replay Analysis**: Drill into specific matches
- **Report Generation**: Automated research summaries

This transforms **match logs into scientific findings**.

## Analysis Modules

### 1. Strategy Detection Engine

Identifies high-level strategies from low-level decisions.

**Detectable Patterns**:
- **Tank Rush**: Early tank production + aggressive positioning
- **Economic Boom**: Supply trucks prioritized, defensive stance
- **Worker Harassment**: Targeting construction vehicles
- **Base Turtling**: Heavy defensive structures, few attacks
- **Power Denial**: Power plants prioritized
- **Supply Line Disruption**: Supply trucks/depots targeted

**Algorithm**:
```python
def detect_strategy(match_log):
    # Parse decision log
    decisions = load_decisions(match_log)

    # Aggregate statistics
    target_priorities = count_target_types(decisions)
    build_order = extract_build_order(match_log)
    attack_timing = get_first_attack_time(decisions)

    # Classify strategy
    if attack_timing < 120 and 'Tank' in build_order[:3]:
        return 'TANK_RUSH'
    elif target_priorities['SupplyTruck'] > 0.3:
        return 'ECONOMIC_DENIAL'
    elif count_defensive_structures(build_order) > 5:
        return 'TURTLE'
    else:
        return 'BALANCED'
```

### 2. Learning Curve Analyzer

Measures how quickly AI adapts.

**Metrics**:
- **Time to First Adaptation**: How many kills before priority changes?
- **Learning Rate**: How fast do multipliers change?
- **Stability**: Do priorities converge or oscillate?
- **Overfitting**: Does AI over-specialize?

**Analysis**:
```python
def analyze_learning_curve(match_log):
    learning_events = load_learning_events(match_log)

    results = {
        'first_adaptation_frame': None,
        'total_adaptations': len(learning_events),
        'learning_velocity': [],
        'priority_volatility': {}
    }

    for event in learning_events:
        if results['first_adaptation_frame'] is None:
            results['first_adaptation_frame'] = event['frame']

        # Measure how large the change was
        delta = abs(event['new_multiplier'] - event['old_multiplier'])
        results['learning_velocity'].append(delta)

        # Track per-target volatility
        target = event['target_type']
        if target not in results['priority_volatility']:
            results['priority_volatility'][target] = []
        results['priority_volatility'][target].append(delta)

    # Compute statistics
    results['avg_learning_velocity'] = mean(results['learning_velocity'])
    results['learning_deceleration'] = is_decreasing(results['learning_velocity'])

    return results
```

### 3. Comparative Analyzer

Compares personalities across dimensions.

**Comparisons**:
- **Win Rate by Personality**: Which personalities dominate?
- **Win Rate by Map**: Do some personalities excel on certain maps?
- **Match Duration**: Do aggressive personalities end games faster?
- **Economic Efficiency**: Money spent vs damage dealt
- **Learning Speed**: Which personality adapts fastest?

**Visualization**:
```
Win Rate by Personality
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Aggressive    ████████████████ 64%
Economic      ████████████████████ 78%
Defensive     ██████████ 42%
Balanced      ███████████████ 58%
```

### 4. Replay Analyzer

Drill into specific matches to understand outcomes.

**Features**:
- **Timeline View**: Key events in chronological order
- **Decision Breakdown**: Why did AI choose this target?
- **What-If Scenarios**: What if priority was different?
- **Turning Points**: When did the match swing?

### 5. Report Generator

Automated research reports in Markdown/HTML.

**Report Structure**:
```markdown
# Tournament Analysis Report
**Tournament**: Personality Championship Round Robin
**Date**: 2025-01-13
**Matches**: 450 (4 personalities × 3 maps × 25 repetitions × 3 pairings)

## Executive Summary
- **Winner**: Economic personality (78% win rate)
- **Key Finding**: Economic AI discovered supply truck harassment
- **Surprise**: Aggressive AI struggled on open maps
- **Recommendation**: Defensive personality needs tuning

## Win Rates
| Personality | Wins | Losses | Win Rate |
|-------------|------|--------|----------|
| Economic    | 234  | 66     | 78%      |
| Aggressive  | 192  | 108    | 64%      |
| Balanced    | 174  | 126    | 58%      |
| Defensive   | 126  | 174    | 42%      |

## Strategy Analysis
### Economic Personality
- **Primary Strategy**: Supply line disruption (83% of matches)
- **Learning Speed**: Fast (first adaptation at 45 seconds avg)
- **Effectiveness**: High (0.82 avg effectiveness score)
- **Notable Pattern**: Prioritizes Supply Trucks (2.4x multiplier by mid-game)

### Aggressive Personality
- **Primary Strategy**: Tank rush (67% of matches)
- **Learning Speed**: Medium (first adaptation at 72 seconds avg)
- **Weakness**: Overcommits to attacks, vulnerable to counter
- **Notable Pattern**: Targets infantry heavily (2.1x multiplier)

## Map-Specific Insights
### Desert Combat (Open Map)
- **Advantage**: Economic (85% win rate)
- **Disadvantage**: Defensive (31% win rate)
- **Reason**: Open terrain favors mobility and harassment

### Winter Assault (Narrow Chokepoints)
- **Advantage**: Defensive (64% win rate)
- **Disadvantage**: Aggressive (48% win rate)
- **Reason**: Chokepoints favor defensive structures

## Learning Discoveries
1. **Economic AI Discovery**: Supply trucks are high-value targets
   - Learned after avg 12 kills
   - Multiplier increased from 1.0 → 2.4
   - Win rate increased 18% after this adaptation

2. **Aggressive AI Adaptation**: Switched from building focus to unit focus
   - Occurred in 42% of matches
   - Triggered by failing to break defenses
   - Mixed effectiveness (52% win rate after switch)

## Recommendations
1. **Balance**: Reduce Economic learning rate by 10%
2. **Defensive**: Increase aggressive trigger threshold
3. **Maps**: Add more varied terrain to prevent strategy dominance
4. **Future Work**: Test personality hybrids (Economic + Aggressive)
```

## Implementation

### Analysis Engine Architecture

```
┌──────────────────────────────────────────────┐
│        Match Data Corpus                     │
│  (100s of matches with full decision logs)   │
└────────────────┬─────────────────────────────┘
                 │
                 ▼
┌──────────────────────────────────────────────┐
│         DataLoader                           │
│  • Parse JSON/CSV logs                       │
│  • Build in-memory database                  │
│  • Index by personality, map, time           │
└────────────────┬─────────────────────────────┘
                 │
                 ├──────────────┬──────────────┬──────────────┐
                 ▼              ▼              ▼              ▼
         ┌──────────────┐ ┌──────────┐ ┌──────────┐ ┌──────────────┐
         │ Strategy     │ │ Learning │ │ Compare  │ │ Replay       │
         │ Detector     │ │ Analyzer │ │ Analyzer │ │ Analyzer     │
         └──────┬───────┘ └────┬─────┘ └────┬─────┘ └──────┬───────┘
                │              │            │              │
                └──────────────┴────────────┴──────────────┘
                               │
                               ▼
                  ┌────────────────────────┐
                  │   ReportGenerator      │
                  │  • Markdown            │
                  │  • HTML                │
                  │  • LaTeX (academic)    │
                  └────────────────────────┘
```

### Core Classes

#### 1. DataLoader

**File**: `/Scripts/analysis/data_loader.py`

```python
"""
Data loader for match analysis.
Parses decision logs and tournament results into analyzable format.
"""

import json
import pandas as pd
from pathlib import Path
from typing import List, Dict

class MatchData:
    """Represents a single match with all associated data."""
    def __init__(self, match_id: int, log_path: str):
        self.match_id = match_id
        self.log_path = Path(log_path)

        # Load data files
        self.decisions = self._load_decisions()
        self.learning_events = self._load_learning_events()
        self.build_orders = self._load_build_orders()
        self.metadata = self._load_metadata()

    def _load_decisions(self) -> pd.DataFrame:
        """Load target selection decisions."""
        csv_path = self.log_path / 'target_selection.csv'
        if csv_path.exists():
            return pd.read_csv(csv_path)
        return pd.DataFrame()

    def _load_learning_events(self) -> pd.DataFrame:
        """Load learning/adaptation events."""
        csv_path = self.log_path / 'learning_events.csv'
        if csv_path.exists():
            return pd.read_csv(csv_path)
        return pd.DataFrame()

    def _load_build_orders(self) -> pd.DataFrame:
        """Load unit/building production."""
        csv_path = self.log_path / 'build_orders.csv'
        if csv_path.exists():
            return pd.read_csv(csv_path)
        return pd.DataFrame()

    def _load_metadata(self) -> Dict:
        """Load match metadata (winner, duration, etc)."""
        json_path = self.log_path / 'match_metadata.json'
        if json_path.exists():
            with open(json_path, 'r') as f:
                return json.load(f)
        return {}

class TournamentDataLoader:
    """Loads all matches from a tournament."""
    def __init__(self, tournament_path: str):
        self.tournament_path = Path(tournament_path)
        self.matches: List[MatchData] = []
        self.results_df = None

    def load(self):
        """Load all match data."""
        # Load tournament results summary
        results_csv = self.tournament_path / 'results.csv'
        if results_csv.exists():
            self.results_df = pd.read_csv(results_csv)

        # Load individual match logs
        match_dirs = sorted(self.tournament_path.glob('match_*'))
        for match_dir in match_dirs:
            match_id = int(match_dir.name.split('_')[1])
            match_data = MatchData(match_id, str(match_dir))
            self.matches.append(match_data)

        print(f"Loaded {len(self.matches)} matches from {self.tournament_path}")

    def get_matches_by_personality(self, personality: str) -> List[MatchData]:
        """Filter matches by AI personality."""
        filtered = []
        for match in self.matches:
            if match.metadata.get('player1_personality') == personality or \
               match.metadata.get('player2_personality') == personality:
                filtered.append(match)
        return filtered

    def get_matches_by_map(self, map_name: str) -> List[MatchData]:
        """Filter matches by map."""
        return [m for m in self.matches if m.metadata.get('map') == map_name]
```

#### 2. Strategy Detector

**File**: `/Scripts/analysis/strategy_detector.py`

```python
"""
Detects high-level strategies from low-level decision logs.
"""

import pandas as pd
from enum import Enum

class Strategy(Enum):
    TANK_RUSH = "Tank Rush"
    ECONOMIC_DENIAL = "Economic Denial"
    TURTLE = "Turtle/Defensive"
    WORKER_HARASSMENT = "Worker Harassment"
    POWER_DENIAL = "Power Denial"
    BALANCED = "Balanced"
    UNKNOWN = "Unknown"

class StrategyDetector:
    """Identifies AI strategy from match data."""

    def detect(self, match_data) -> Strategy:
        """Classify the strategy used in this match."""
        decisions = match_data.decisions
        build_order = match_data.build_orders

        if decisions.empty or build_order.empty:
            return Strategy.UNKNOWN

        # Calculate metrics
        first_attack_time = decisions['time_seconds'].min() if not decisions.empty else float('inf')
        target_distribution = decisions['target_type'].value_counts(normalize=True)

        # Check for tank rush
        early_tanks = build_order[
            (build_order['time_seconds'] < 120) &
            (build_order['unit_type'].str.contains('Tank|Abrams', case=False, na=False))
        ]

        if len(early_tanks) >= 3 and first_attack_time < 120:
            return Strategy.TANK_RUSH

        # Check for economic denial
        if 'SupplyTruck' in target_distribution and target_distribution['SupplyTruck'] > 0.25:
            return Strategy.ECONOMIC_DENIAL

        # Check for power denial
        if 'PowerPlant' in target_distribution and target_distribution['PowerPlant'] > 0.20:
            return Strategy.POWER_DENIAL

        # Check for worker harassment
        if 'Worker' in target_distribution and target_distribution['Worker'] > 0.20:
            return Strategy.WORKER_HARASSMENT

        # Check for turtle
        defensive_buildings = build_order[
            build_order['unit_type'].str.contains('Bunker|Tower|Wall', case=False, na=False)
        ]

        if len(defensive_buildings) > 5 and first_attack_time > 300:
            return Strategy.TURTLE

        return Strategy.BALANCED

    def get_strategy_transitions(self, match_data) -> List[Dict]:
        """Detect if AI changed strategy mid-match."""
        decisions = match_data.decisions

        # Split match into time windows
        window_size = 120  # 2 minutes
        max_time = decisions['time_seconds'].max()

        strategies = []
        for start_time in range(0, int(max_time), window_size):
            window_decisions = decisions[
                (decisions['time_seconds'] >= start_time) &
                (decisions['time_seconds'] < start_time + window_size)
            ]

            if window_decisions.empty:
                continue

            # Classify this window
            target_dist = window_decisions['target_type'].value_counts(normalize=True)
            dominant_target = target_dist.index[0] if len(target_dist) > 0 else None

            strategies.append({
                'time_start': start_time,
                'time_end': start_time + window_size,
                'dominant_target': dominant_target,
                'target_distribution': target_dist.to_dict()
            })

        return strategies
```

#### 3. Learning Analyzer

**File**: `/Scripts/analysis/learning_analyzer.py`

```python
"""
Analyzes AI learning curves and adaptation patterns.
"""

import pandas as pd
import numpy as np
from typing import Dict, List

class LearningAnalyzer:
    """Analyzes how AI learns over time."""

    def analyze_match(self, match_data) -> Dict:
        """Analyze learning in a single match."""
        learning_events = match_data.learning_events

        if learning_events.empty:
            return self._empty_analysis()

        # Time to first adaptation
        first_adaptation = learning_events['time_seconds'].min()

        # Learning velocity (how much priorities change)
        learning_events['delta'] = abs(
            learning_events['new_multiplier'] - learning_events['old_multiplier']
        )

        # Convergence analysis
        convergence = self._analyze_convergence(learning_events)

        return {
            'first_adaptation_time': first_adaptation,
            'total_adaptations': len(learning_events),
            'avg_learning_velocity': learning_events['delta'].mean(),
            'max_learning_velocity': learning_events['delta'].max(),
            'convergence': convergence,
            'most_adapted_target': self._get_most_adapted_target(learning_events),
            'learning_timeline': self._build_timeline(learning_events)
        }

    def _analyze_convergence(self, learning_events: pd.DataFrame) -> Dict:
        """Determine if learning is converging or oscillating."""
        # Group by target type
        results = {}

        for target_type in learning_events['target_type'].unique():
            target_events = learning_events[learning_events['target_type'] == target_type]

            if len(target_events) < 3:
                continue

            # Check if changes are decreasing over time
            deltas = target_events['delta'].values
            is_converging = self._is_decreasing_trend(deltas)

            # Final multiplier value
            final_value = target_events.iloc[-1]['new_multiplier']

            results[target_type] = {
                'is_converging': is_converging,
                'final_multiplier': final_value,
                'adaptation_count': len(target_events),
                'volatility': np.std(deltas)
            }

        return results

    def _is_decreasing_trend(self, values: np.ndarray) -> bool:
        """Check if values show decreasing trend."""
        if len(values) < 2:
            return False

        # Simple linear regression slope
        x = np.arange(len(values))
        slope = np.polyfit(x, values, 1)[0]

        return slope < -0.01  # Negative slope = decreasing

    def _get_most_adapted_target(self, learning_events: pd.DataFrame) -> str:
        """Find which target type changed most."""
        if learning_events.empty:
            return None

        # Count adaptations per target
        target_counts = learning_events['target_type'].value_counts()
        return target_counts.index[0] if len(target_counts) > 0 else None

    def _build_timeline(self, learning_events: pd.DataFrame) -> List[Dict]:
        """Build chronological list of learning events."""
        timeline = []

        for _, event in learning_events.iterrows():
            timeline.append({
                'time': event['time_seconds'],
                'target_type': event['target_type'],
                'old_multiplier': event['old_multiplier'],
                'new_multiplier': event['new_multiplier'],
                'direction': 'up' if event['new_multiplier'] > event['old_multiplier'] else 'down'
            })

        return sorted(timeline, key=lambda x: x['time'])

    def _empty_analysis(self) -> Dict:
        """Return empty analysis structure."""
        return {
            'first_adaptation_time': None,
            'total_adaptations': 0,
            'avg_learning_velocity': 0.0,
            'max_learning_velocity': 0.0,
            'convergence': {},
            'most_adapted_target': None,
            'learning_timeline': []
        }

    def compare_personalities(self, tournament_data, personalities: List[str]) -> pd.DataFrame:
        """Compare learning characteristics across personalities."""
        results = []

        for personality in personalities:
            matches = tournament_data.get_matches_by_personality(personality)

            learning_stats = {
                'personality': personality,
                'avg_first_adaptation': [],
                'avg_total_adaptations': [],
                'avg_learning_velocity': []
            }

            for match in matches:
                analysis = self.analyze_match(match)
                if analysis['first_adaptation_time'] is not None:
                    learning_stats['avg_first_adaptation'].append(analysis['first_adaptation_time'])
                    learning_stats['avg_total_adaptations'].append(analysis['total_adaptations'])
                    learning_stats['avg_learning_velocity'].append(analysis['avg_learning_velocity'])

            results.append({
                'personality': personality,
                'avg_first_adaptation_time': np.mean(learning_stats['avg_first_adaptation']) if learning_stats['avg_first_adaptation'] else 0,
                'avg_total_adaptations': np.mean(learning_stats['avg_total_adaptations']) if learning_stats['avg_total_adaptations'] else 0,
                'avg_learning_velocity': np.mean(learning_stats['avg_learning_velocity']) if learning_stats['avg_learning_velocity'] else 0
            })

        return pd.DataFrame(results)
```

#### 4. Report Generator

**File**: `/Scripts/analysis/report_generator.py`

```python
"""
Generates analysis reports in multiple formats.
"""

from typing import Dict, List
from pathlib import Path
import matplotlib.pyplot as plt
import seaborn as sns

class ReportGenerator:
    """Generates comprehensive analysis reports."""

    def __init__(self, tournament_data, output_path: str):
        self.data = tournament_data
        self.output_path = Path(output_path)
        self.output_path.mkdir(parents=True, exist_ok=True)

    def generate_full_report(self):
        """Generate complete analysis report."""
        print("Generating tournament analysis report...")

        # Run all analyses
        strategy_analysis = self._analyze_strategies()
        learning_analysis = self._analyze_learning()
        comparative_analysis = self._analyze_comparisons()

        # Generate markdown report
        self._write_markdown_report(strategy_analysis, learning_analysis, comparative_analysis)

        # Generate visualizations
        self._generate_visualizations(comparative_analysis)

        print(f"Report generated: {self.output_path / 'report.md'}")

    def _write_markdown_report(self, strategy_analysis, learning_analysis, comparative_analysis):
        """Write comprehensive markdown report."""
        report_path = self.output_path / 'report.md'

        with open(report_path, 'w') as f:
            f.write("# Tournament Analysis Report\n\n")

            # Executive summary
            f.write("## Executive Summary\n\n")
            self._write_executive_summary(f, comparative_analysis)

            # Win rates
            f.write("\n## Win Rates\n\n")
            self._write_win_rates(f, comparative_analysis)

            # Strategy analysis
            f.write("\n## Strategy Analysis\n\n")
            self._write_strategy_analysis(f, strategy_analysis)

            # Learning analysis
            f.write("\n## Learning Analysis\n\n")
            self._write_learning_analysis(f, learning_analysis)

            # Map-specific insights
            f.write("\n## Map-Specific Insights\n\n")
            self._write_map_analysis(f)

            # Recommendations
            f.write("\n## Recommendations\n\n")
            self._write_recommendations(f, comparative_analysis)

    def _generate_visualizations(self, comparative_analysis):
        """Generate charts and graphs."""
        # Win rate bar chart
        plt.figure(figsize=(10, 6))
        sns.barplot(data=self.data.results_df, x='winner_personality', y='win_rate')
        plt.title('Win Rates by Personality')
        plt.xlabel('Personality')
        plt.ylabel('Win Rate (%)')
        plt.savefig(self.output_path / 'win_rates.png')
        plt.close()

        # More visualizations...
```

### Usage

```bash
# Run complete analysis on tournament results
python Scripts/analyze_tournament.py tournament_results/personality_championship/

# Generate report
python Scripts/generate_report.py tournament_results/personality_championship/ --format markdown

# Interactive analysis (Jupyter notebook)
jupyter notebook Scripts/interactive_analysis.ipynb
```

## Testing Plan

### Test 1: Data Loading
Load match data and verify parsing.

```python
from analysis.data_loader import TournamentDataLoader

loader = TournamentDataLoader('tournament_results/test_tournament/')
loader.load()

print(f"Loaded {len(loader.matches)} matches")
assert len(loader.matches) > 0
```

### Test 2: Strategy Detection
Verify strategies are correctly identified.

```python
from analysis.strategy_detector import StrategyDetector

detector = StrategyDetector()
strategy = detector.detect(loader.matches[0])
print(f"Detected strategy: {strategy}")
```

### Test 3: Learning Analysis
Analyze learning curves.

```python
from analysis.learning_analyzer import LearningAnalyzer

analyzer = LearningAnalyzer()
analysis = analyzer.analyze_match(loader.matches[0])
print(f"First adaptation: {analysis['first_adaptation_time']}s")
```

### Test 4: Report Generation
Generate full report and verify output files exist.

```bash
python Scripts/generate_report.py tournament_results/test/
ls tournament_results/test/report.md
ls tournament_results/test/*.png
```

## Success Criteria

- ✅ All match data loads correctly
- ✅ Strategy detection achieves >80% accuracy
- ✅ Learning curves are visualized clearly
- ✅ Reports are comprehensive and readable
- ✅ Visualizations are publication-quality
- ✅ Analysis runs in <5 minutes for 100 matches
- ✅ Insights are actionable

## Future Enhancements

1. **Machine Learning**: Train classifier to predict winner from early game
2. **Causal Analysis**: What actions caused wins/losses?
3. **Meta-Game Tracking**: How does the "meta" evolve over time?
4. **Transfer Learning**: Can AI learn from watching other matches?

## Conclusion

Phase 5 transforms raw match data into actionable intelligence. The analysis tools enable:
- Understanding what strategies work
- Identifying which personalities need balancing
- Discovering emergent AI behaviors
- Publishing research findings

Combined with Phases 1-4, this completes the **AI Laboratory System** for Claude & Conquer.
