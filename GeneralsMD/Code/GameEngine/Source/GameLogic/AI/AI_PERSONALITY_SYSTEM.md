# AI Personality System for Adaptive AI

## Overview

The AI Personality System extends the AdaptiveAI framework to give different AI opponents unique learning behaviors and target priorities. This creates more varied and interesting gameplay where each AI opponent plays differently.

## Files Modified/Created

1. **AdaptiveAI.h** - Header file with personality enums and structures
2. **AdaptiveAI.cpp** - Implementation of personality-based learning
3. **AdaptiveAI_PersonalityExample.cpp** - Usage examples and documentation

## Personality Types

### 1. AI_PERSONALITY_AGGRESSIVE
**Focus:** Killing enemy units and armies

**Weights:**
- Economic Damage: 0.3x (low interest)
- Unit Kill: 2.0x (highly values)
- Strategic Target: 0.5x (some interest)
- Collateral Damage: 1.5x (values)
- Learning Rate: 1.5x (learns quickly)
- Min Sample Size: 2 kills

**Behavior:**
- Aggressively targets enemy combat units (tanks, infantry, aircraft)
- Learns quickly from combat encounters
- Values destroying multiple units at once (collateral damage)
- Less interested in economy disruption
- Acts on limited data (makes quick tactical adjustments)

**Best for:** Rush strategies, direct combat-focused AI

---

### 2. AI_PERSONALITY_ECONOMIC
**Focus:** Disrupting enemy economy and resources

**Weights:**
- Economic Damage: 2.5x (highly values)
- Unit Kill: 0.4x (low interest)
- Strategic Target: 2.0x (highly values)
- Collateral Damage: 0.5x (minimal interest)
- Learning Rate: 1.2x (moderately fast)
- Min Sample Size: 2 kills

**Behavior:**
- Prioritizes supply trucks, docks, and resource buildings
- Targets power plants to disable base defenses
- Attacks production facilities (barracks, factories, airfields)
- Less interested in direct unit combat
- Learns from economic impact rather than kill counts

**Best for:** Harassment strategies, economic denial tactics

---

### 3. AI_PERSONALITY_DEFENSIVE
**Focus:** Protecting own assets and countering player tactics

**Weights:**
- Economic Damage: 0.8x (moderate interest)
- Unit Kill: 1.2x (values eliminating threats)
- Strategic Target: 1.0x (balanced)
- Collateral Damage: 1.0x (balanced)
- Learning Rate: 0.8x (learns slowly/cautiously)
- Min Sample Size: 4 kills

**Behavior:**
- Carefully analyzes threats before committing
- Requires more data before changing priorities (cautious)
- Adapts slowly but reliably to player strategies
- Balanced approach to targeting
- Good at countering repeated player tactics

**Best for:** Defensive play, reacting to player strategies

---

### 4. AI_PERSONALITY_BALANCED
**Focus:** General-purpose AI with no bias

**Weights:**
- Economic Damage: 1.0x (normal)
- Unit Kill: 1.0x (normal)
- Strategic Target: 1.0x (normal)
- Collateral Damage: 1.0x (normal)
- Learning Rate: 1.0x (standard)
- Min Sample Size: 3 kills

**Behavior:**
- Standard adaptive AI behavior
- No particular preference for any target type
- Balanced learning rate
- Traditional RTS AI experience

**Best for:** Default AI, standard gameplay

---

## How It Works

### 1. Effectiveness Calculation (Personality-Aware)

When the AI destroys an enemy target, it calculates effectiveness using personality-specific weights:

```cpp
Real effectiveness = 0.5f; // Base value

// Economic damage (weighted by personality)
if (moneyDelta > 1000) {
    effectiveness += 0.6f * m_weights.economicDamageWeight;
}

// Unit kills (weighted by personality)
if (unitDelta == 1) {
    effectiveness += 0.3f * m_weights.unitKillWeight;
}

// Strategic targets (weighted by personality)
if (targetType == "Power Plant") {
    effectiveness += 0.3f * m_weights.strategicTargetWeight;
}
```

**Result:** Different personalities value the same kill differently!
- Aggressive AI: Tank kill = HIGH effectiveness
- Economic AI: Supply truck kill = HIGH effectiveness
- Defensive AI: Balanced evaluation

### 2. Learning Rate (Adaptation Speed)

Each personality has a different learning rate that affects how quickly they adapt priorities:

```cpp
Real delta = targetMultiplier - stats.priorityMultiplier;
stats.priorityMultiplier += delta * m_weights.learningRate;
```

**Examples:**
- **Aggressive (1.5x):** Adapts very quickly to new information
- **Balanced (1.0x):** Standard adaptation speed
- **Defensive (0.8x):** Adapts cautiously, more conservative

### 3. Sample Size Requirements

Each personality requires different amounts of data before changing priorities:

```cpp
if (stats.timesKilled < (Int)m_weights.minSampleSize) continue;
```

**Examples:**
- **Aggressive (2 kills):** Acts quickly on limited data
- **Balanced (3 kills):** Standard threshold
- **Defensive (4 kills):** Needs more evidence before changing tactics

---

## Usage Examples

### Example 1: Setting up varied AI opponents

```cpp
// Create three AI players with different personalities
AdaptiveCombatTracker *ai1 = NEW_POOL_OBJ(AdaptiveCombatTracker, aiPlayer1);
AdaptiveCombatTracker *ai2 = NEW_POOL_OBJ(AdaptiveCombatTracker, aiPlayer2);
AdaptiveCombatTracker *ai3 = NEW_POOL_OBJ(AdaptiveCombatTracker, aiPlayer3);

ai1->setPersonality(AI_PERSONALITY_AGGRESSIVE);
ai2->setPersonality(AI_PERSONALITY_ECONOMIC);
ai3->setPersonality(AI_PERSONALITY_DEFENSIVE);

// Each AI will now learn and behave differently!
```

### Example 2: Random personality for variety

```cpp
int randomChoice = rand() % 4;
AIPersonality personality = (AIPersonality)randomChoice;
ai->setPersonality(personality);
```

### Example 3: Difficulty-based personalities

```cpp
if (difficulty == EASY) {
    ai->setPersonality(AI_PERSONALITY_BALANCED);
} else if (difficulty == HARD) {
    ai->setPersonality(AI_PERSONALITY_AGGRESSIVE);
} else if (difficulty == EXPERT) {
    ai->setPersonality(AI_PERSONALITY_ECONOMIC);
}
```

---

## Integration Points

### When AI destroys enemy unit:

```cpp
void onUnitDestroyed(Object *victim, Object *killer) {
    if (adaptiveAI) {
        adaptiveAI->onEnemyKilled(victim, killer);
    }
}
```

### Every frame:

```cpp
void updateAI() {
    if (adaptiveAI) {
        adaptiveAI->update();
    }
}
```

### When AI selects targets:

```cpp
Real priority = basePriority * adaptiveAI->getPriorityMultiplier(targetTemplate);
```

---

## Debugging

A debug display function is included that shows:
- Current personality type
- Learned priority multipliers for different targets
- Recent kills and their effectiveness scores
- Sample size requirements

Enable with: `TheGlobalData->m_debugAI = true`

The debug display updates every 3 seconds and shows:
```
=== ADAPTIVE AI DEBUG ===
Personality: AGGRESSIVE (Values unit kills)
--- Learned Priorities ---
EnemyTank: 1.8x (kills:5, eff:1.6)
EnemyInfantry: 1.5x (kills:7, eff:1.3)
SupplyTruck: 0.7x (kills:3, eff:0.4)
--- Recent Kills ---
EnemyTank: eff=1.8 (dmg:$200, units:-1)
=========================
```

---

## Technical Details

### Data Structures

**PersonalityWeights:**
- `economicDamageWeight`: Multiplier for economic impact evaluation
- `unitKillWeight`: Multiplier for unit kill valuation
- `strategicTargetWeight`: Multiplier for strategic target evaluation
- `collateralDamageWeight`: Multiplier for collateral damage value
- `learningRate`: Speed of priority adaptation (0.8-1.5)
- `minSampleSize`: Kills required before adjusting priorities (2-4)

**TargetKillRecord:**
- Tracks each kill and its impact on enemy economy/army
- Measured 5 seconds after kill to assess effectiveness
- Stores money delta, unit count delta, and calculated effectiveness

**TargetTypeStats:**
- Per-target-type statistics
- Running average of effectiveness
- Dynamic priority multiplier (0.5-2.5 range)

### Algorithm Flow

1. **Kill Occurs** → Record kill and game state
2. **5 Seconds Later** → Measure impact (money/units lost)
3. **Calculate Effectiveness** → Use personality weights
4. **Every 10 Seconds** → Update priority multipliers based on learned effectiveness
5. **Apply Learning Rate** → Adjust priorities at personality-specific speed
6. **Target Selection** → Use learned multipliers to prioritize targets

---

## Balancing Notes

### Aggressive AI
- **Strengths:** Fast adaptation, good at direct combat
- **Weaknesses:** Vulnerable to economic strategies, may over-commit
- **Counter:** Defensive play, economy focus

### Economic AI
- **Strengths:** Excellent at disruption, weakens enemy over time
- **Weaknesses:** Poor at direct combat, slow army growth
- **Counter:** Aggressive rush, strong army focus

### Defensive AI
- **Strengths:** Reliable, hard to exploit with repeated tactics
- **Weaknesses:** Slow to adapt, can be overwhelmed by variety
- **Counter:** Mix strategies frequently, aggressive early game

### Balanced AI
- **Strengths:** Well-rounded, no exploitable weaknesses
- **Weaknesses:** No particular strengths, predictable
- **Counter:** Specialist strategies (pure rush or pure economy)

---

## Future Enhancements

Possible additions to the personality system:

1. **Dynamic Personality Switching:** AI changes personality mid-game based on situation
2. **Team Coordination:** Different personalities coordinate (Aggressive + Economic)
3. **Custom Personalities:** Players create custom personality weight profiles
4. **Personality Drift:** AI gradually evolves personality based on success/failure
5. **Counter-Personality:** AI detects player style and chooses counter-personality

---

## Conclusion

The AI Personality System creates diverse and engaging AI opponents by giving each AI a unique "playstyle" and learning behavior. Players will face:

- **Aggressive AIs** that pressure armies relentlessly
- **Economic AIs** that cripple supply lines
- **Defensive AIs** that adapt to counter strategies
- **Balanced AIs** for traditional gameplay

This system makes each match feel different and encourages players to adapt their strategies based on the AI opponent they're facing.
