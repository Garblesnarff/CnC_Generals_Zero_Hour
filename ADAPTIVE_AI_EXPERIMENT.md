# Adaptive AI Experiment

## What is this?

An experimental AI system that **learns during matches** instead of using fixed priorities.

The vanilla C&C Generals AI uses static priorities:
- Tanks = priority 10
- Power plants = priority 15
- Infantry = priority 5

It never changes. It doesn't adapt to your strategy.

## The Idea

What if the AI **tracked which targets were actually effective**?

### How it works:

1. **AI kills your power plant** → Tracks your economy for next 5 seconds
2. **Your money drops significantly** → AI learns: "Power plants are valuable targets"
3. **Next engagement** → AI prioritizes power plants MORE (multiplier: 1.8x)

Conversely:
1. **AI kills a scout unit** → Watches for impact
2. **Nothing happens to your economy/army** → AI learns: "Scouts aren't worth focusing"
3. **Next engagement** → AI prioritizes scouts LESS (multiplier: 0.7x)

## The Vision

### Emergent Behavior

Each match becomes unique. The AI adapts to:
- **Your build order** (rush vs. economy)
- **Your faction** (China tank spam vs. GLA stealth)
- **Map dynamics** (does killing supply trucks matter on this map?)

### Personality Through Seed Values

Add variance to create different "AI personalities":

```cpp
// Aggressive AI: Values army kills highly
aggressiveAI.setEffectivenessWeights(0.2f, 0.8f); // (economy, units)

// Economic AI: Values disrupting enemy economy
economicAI.setEffectivenessWeights(0.8f, 0.2f);

// Chaotic AI: Random weights each match
chaoticAI.setEffectivenessWeights(random(0,1), random(0,1));
```

### Advanced Extensions

**Threat Assessment:**
- Track which of YOUR units the enemy kills
- Protect your most vulnerable assets
- Adapt formations based on enemy weapon types

**Build Order Learning:**
- "Enemy builds lots of aircraft → I should build more AA"
- "Enemy rushes early → I should build defenses first"

**Tactical Memory:**
- Remember effective attack routes on this map
- Learn where supply trucks path
- Identify chokepoints where battles happen

## Current Status

**Implemented:**
- ✅ Basic adaptive priority system
- ✅ Effectiveness tracking (economy + unit impact)
- ✅ Dynamic priority multipliers (0.7x - 1.8x)

**Not Yet Integrated:**
- ❌ Hook into `AI::findClosestEnemy()`
- ❌ Call `onEnemyKilled()` when objects die
- ❌ Create singleton instance per AI player

**Future Ideas:**
- ❌ Threat assessment (protect valuable assets)
- ❌ Build order adaptation
- ❌ Tactical memory per map
- ❌ AI "personalities" with different learning weights

## Why This Is Interesting

Traditional game AI is **finite state machines** + **static priorities**. Predictable. Exploitable.

This approach creates **emergent behavior** from simple rules:
- Track outcomes
- Measure effectiveness
- Adjust future decisions

The AI doesn't "know" that power plants are important. **It discovers this through play.**

Each match teaches it something new. Playing against the same AI twice might feel completely different based on how the first match went.

## Technical Notes

### Integration Points

To activate this system, you need to:

1. **Modify `AI::findClosestEnemy()` in AI.cpp:**
```cpp
// After calculating curPriority from AttackPriorityInfo
if (TheAdaptiveAI) {
    Real multiplier = TheAdaptiveAI->getPriorityMultiplier(theEnemy->getTemplate());
    curPriority = (Int)(curPriority * multiplier);
}
```

2. **Hook object death in Object::setObjectStatus():**
```cpp
if (newStatus.test(OBJECT_STATUS_DESTROYED)) {
    // ... existing code ...
    if (TheAdaptiveAI && killerPlayer && killerPlayer->isAIPlayer()) {
        TheAdaptiveAI->onEnemyKilled(this, killer);
    }
}
```

3. **Call update in AI::update():**
```cpp
void AI::update() {
    m_pathfinder->processPathfindQueue();
    ThePlayerList->UPDATE();

    if (TheAdaptiveAI) {
        TheAdaptiveAI->update();
    }
}
```

### Performance Considerations

- Tracks last 100 kills only
- Updates priorities every 10 seconds (not every frame)
- Uses simple heuristics (not expensive ML)

Expected overhead: < 1% CPU on a 2003-era machine.

## The Bigger Picture

This is a **proof of concept** for emergent AI behavior in RTS games.

What makes it exciting isn't the specific implementation—it's the **philosophy**:

> Give the AI feedback loops. Let it learn. Create behavior that surprises even you.

You could extend this to:
- Swarm tactics (units learn from neighbor success)
- Meta-learning (AI remembers across matches against same player)
- Evolutionary strategies (AI populations compete, best strategies survive)

## Try It

To test this:

1. Integrate the hooks above
2. Compile the game
3. Play a skirmish against AI
4. Watch the debug log:
   - "AI learned: PowerPlant priority multiplier = 1.8"
   - "AI learned: ScoutUnit priority multiplier = 0.7"
5. Notice how the AI changes its focus mid-match

---

**Built by**: Claude (Anthropic AI)
**Date**: 2025-01-13
**Motivation**: Curiosity about what could be done with 20-year-old game engines

*"The code is open. Let's see what's possible."*
