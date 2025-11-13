/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
// AdaptiveAI.h
// Experimental adaptive AI that learns during matches
// Tracks combat effectiveness and adjusts targeting priorities dynamically
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _ADAPTIVE_AI_H_
#define _ADAPTIVE_AI_H_

#include "Common/STLTypedefs.h"
#include "Common/GameMemory.h"
#include "Common/GameType.h"

class Object;
class Player;
class ThingTemplate;

/**
 * Tracks the effectiveness of destroying different target types.
 *
 * The idea: Instead of fixed priorities, learn which targets matter.
 * - Did killing their power plant slow them down?
 * - Did destroying supply trucks hurt their economy?
 * - Should we focus fire on their strongest units?
 *
 * This creates emergent behavior where the AI adapts to each match.
 */
class AdaptiveCombatTracker : public MemoryPoolObject
{
	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(AdaptiveCombatTracker, "AdaptiveCombatTracker")

public:
	AdaptiveCombatTracker(Player *owner);
	~AdaptiveCombatTracker();

	/**
	 * Called when we destroy an enemy object.
	 * Records what we killed and starts tracking the impact.
	 */
	void onEnemyKilled(const Object *victim, const Object *killer);

	/**
	 * Called every frame to analyze game state changes.
	 * Did destroying that target actually help us?
	 */
	void update();

	/**
	 * Get the learned priority multiplier for a target type.
	 * Returns 1.0 by default, higher if we've learned this target is valuable.
	 */
	Real getPriorityMultiplier(const ThingTemplate *targetTemplate) const;

	/**
	 * Reset all learned data (new match, new opponent).
	 */
	void reset();

private:
	struct TargetKillRecord
	{
		AsciiString targetType;				// What did we kill?
		UnsignedInt frameKilled;			// When?
		Int enemyMoneyBefore;				// Enemy resources before kill
		Int enemyMoneyAfter;					// Enemy resources after kill (did we hurt their economy?)
		Int enemyUnitsCountBefore;		// Number of enemy units before
		Int enemyUnitsCountAfter;			// Number of enemy units after
		Real effectivenessScore;			// Calculated impact (0.0 = no impact, 1.0 = significant)
	};

	struct TargetTypeStats
	{
		AsciiString typeName;
		Int timesKilled;
		Real avgEffectiveness;
		Real priorityMultiplier;			// Dynamic multiplier (starts at 1.0)
	};

	Player *m_owner;									// The AI player who owns this tracker

	// Recent kills we're tracking
	std::vector<TargetKillRecord> m_recentKills;

	// Learned statistics per target type
	std::map<AsciiString, TargetTypeStats> m_targetStats;

	/**
	 * Calculate how effective a kill was.
	 * Did it hurt their economy? Reduce their army?
	 */
	Real calculateEffectiveness(const TargetKillRecord &record);

	/**
	 * Update learned priorities based on accumulated data.
	 */
	void updatePriorities();

	/**
	 * Get enemy player (for tracking their resources/units).
	 */
	Player *getEnemyPlayer() const;
};

/**
 * Global adaptive AI tracking.
 * Each AI player gets one of these to learn during matches.
 */
extern AdaptiveCombatTracker *TheAdaptiveAI;

#endif // _ADAPTIVE_AI_H_
