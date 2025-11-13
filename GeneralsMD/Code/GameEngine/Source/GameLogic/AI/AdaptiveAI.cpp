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
// AdaptiveAI.cpp
// Experimental adaptive AI implementation
////////////////////////////////////////////////////////////////////////////////

#include "PreRTS.h"
#include "GameLogic/AdaptiveAI.h"
#include "GameLogic/Object.h"
#include "GameLogic/GameLogic.h"
#include "Common/Player.h"
#include "Common/PlayerList.h"
#include "Common/ThingTemplate.h"

// Global instance
AdaptiveCombatTracker *TheAdaptiveAI = NULL;

/**
 * Constructor
 */
AdaptiveCombatTracker::AdaptiveCombatTracker(Player *owner)
	: m_owner(owner)
{
	reset();
}

/**
 * Destructor
 */
AdaptiveCombatTracker::~AdaptiveCombatTracker()
{
	m_recentKills.clear();
	m_targetStats.clear();
}

/**
 * Reset all learned data
 */
void AdaptiveCombatTracker::reset()
{
	m_recentKills.clear();
	m_targetStats.clear();
}

/**
 * Called when we destroy an enemy object.
 * Start tracking to see if this kill was valuable.
 */
void AdaptiveCombatTracker::onEnemyKilled(const Object *victim, const Object *killer)
{
	if (!victim || !m_owner) return;

	Player *enemy = getEnemyPlayer();
	if (!enemy) return;

	// Create a kill record
	TargetKillRecord record;
	record.targetType = victim->getTemplate()->getName();
	record.frameKilled = TheGameLogic->getFrame();
	record.enemyMoneyBefore = enemy->getMoney()->countMoney();
	record.enemyMoneyAfter = 0; // Will be filled in by update()
	record.enemyUnitsCountBefore = 0;
	record.enemyUnitsCountAfter = 0;
	record.effectivenessScore = 0.0f;

	// Count enemy units before the kill
	Object *obj;
	for (obj = TheGameLogic->getFirstObject(); obj; obj = obj->getNextObject()) {
		if (obj->getControllingPlayer() == enemy && !obj->getStatusBits().test(OBJECT_STATUS_DESTROYED)) {
			record.enemyUnitsCountBefore++;
		}
	}

	m_recentKills.push_back(record);

	// Keep only recent kills (last 100)
	if (m_recentKills.size() > 100) {
		m_recentKills.erase(m_recentKills.begin());
	}
}

/**
 * Update: Analyze recent kills to see their impact
 */
void AdaptiveCombatTracker::update()
{
	if (!m_owner) return;

	Player *enemy = getEnemyPlayer();
	if (!enemy) return;

	UnsignedInt currentFrame = TheGameLogic->getFrame();

	// Update recent kill records (check impact after a few seconds)
	for (size_t i = 0; i < m_recentKills.size(); ++i) {
		TargetKillRecord &record = m_recentKills[i];

		// Wait ~5 seconds after kill to measure impact
		UnsignedInt framesElapsed = currentFrame - record.frameKilled;
		if (framesElapsed == 5 * LOGICFRAMES_PER_SECOND) {
			// Measure state after kill
			record.enemyMoneyAfter = enemy->getMoney()->countMoney();

			// Count enemy units after kill
			record.enemyUnitsCountAfter = 0;
			Object *obj;
			for (obj = TheGameLogic->getFirstObject(); obj; obj = obj->getNextObject()) {
				if (obj->getControllingPlayer() == enemy && !obj->getStatusBits().test(OBJECT_STATUS_DESTROYED)) {
					record.enemyUnitsCountAfter++;
				}
			}

			// Calculate effectiveness
			record.effectivenessScore = calculateEffectiveness(record);

			// Update stats for this target type
			AsciiString typeName = record.targetType;
			if (m_targetStats.find(typeName) == m_targetStats.end()) {
				// New type
				TargetTypeStats stats;
				stats.typeName = typeName;
				stats.timesKilled = 0;
				stats.avgEffectiveness = 0.0f;
				stats.priorityMultiplier = 1.0f;
				m_targetStats[typeName] = stats;
			}

			TargetTypeStats &stats = m_targetStats[typeName];
			stats.timesKilled++;

			// Running average of effectiveness
			stats.avgEffectiveness = (stats.avgEffectiveness * (stats.timesKilled - 1) + record.effectivenessScore) / stats.timesKilled;
		}
	}

	// Every 10 seconds, update priorities based on learned data
	if (currentFrame % (10 * LOGICFRAMES_PER_SECOND) == 0) {
		updatePriorities();
	}
}

/**
 * Calculate how effective a kill was.
 * Returns 0.0 to 2.0+ (0 = no impact, 1.0 = normal, 2.0 = very effective)
 */
Real AdaptiveCombatTracker::calculateEffectiveness(const TargetKillRecord &record)
{
	Real effectiveness = 0.5f; // Base value

	// Did we hurt their economy?
	Int moneyDelta = record.enemyMoneyBefore - record.enemyMoneyAfter;
	if (moneyDelta > 500) {
		effectiveness += 0.3f; // Significant economic impact
	} else if (moneyDelta > 1000) {
		effectiveness += 0.6f; // Major economic damage
	}

	// Did we reduce their unit count significantly?
	Int unitDelta = record.enemyUnitsCountBefore - record.enemyUnitsCountAfter;
	if (unitDelta > 3) {
		effectiveness += 0.4f; // Killed a valuable unit that took others with it
	} else if (unitDelta > 1) {
		effectiveness += 0.2f; // Some collateral benefit
	}

	// Bonus for certain strategic target types
	AsciiString targetType = record.targetType;
	if (targetType.find("Power") != std::string::npos) {
		effectiveness += 0.3f; // Power plants are strategic
	} else if (targetType.find("Supply") != std::string::npos ||
	           targetType.find("Dock") != std::string::npos) {
		effectiveness += 0.4f; // Economy targets are valuable
	} else if (targetType.find("Factory") != std::string::npos ||
	           targetType.find("Barracks") != std::string::npos ||
	           targetType.find("Airfield") != std::string::npos) {
		effectiveness += 0.5f; // Production facilities are critical
	}

	return effectiveness;
}

/**
 * Update priority multipliers based on learned effectiveness
 */
void AdaptiveCombatTracker::updatePriorities()
{
	// Adjust multipliers based on average effectiveness
	for (std::map<AsciiString, TargetTypeStats>::iterator it = m_targetStats.begin();
	     it != m_targetStats.end(); ++it) {
		TargetTypeStats &stats = it->second;

		// Only adjust if we have enough data (at least 3 kills)
		if (stats.timesKilled < 3) continue;

		// Map effectiveness to multiplier
		// avgEffectiveness: 0.0-0.5 = bad target (multiply by 0.7)
		// avgEffectiveness: 0.5-1.0 = normal (multiply by 1.0)
		// avgEffectiveness: 1.0-2.0+ = great target (multiply by 1.5-2.0)

		if (stats.avgEffectiveness < 0.5f) {
			stats.priorityMultiplier = 0.7f; // Deprioritize
		} else if (stats.avgEffectiveness > 1.5f) {
			stats.priorityMultiplier = 1.8f; // High priority!
		} else {
			// Linear interpolation between 0.8 and 1.5
			stats.priorityMultiplier = 0.8f + (stats.avgEffectiveness - 0.5f) * 0.7f;
		}
	}
}

/**
 * Get the learned priority multiplier for a target type.
 * Returns 1.0 by default (no change), higher if we've learned it's valuable.
 */
Real AdaptiveCombatTracker::getPriorityMultiplier(const ThingTemplate *targetTemplate) const
{
	if (!targetTemplate) return 1.0f;

	AsciiString typeName = targetTemplate->getName();
	std::map<AsciiString, TargetTypeStats>::const_iterator it = m_targetStats.find(typeName);

	if (it != m_targetStats.end()) {
		return it->second.priorityMultiplier;
	}

	return 1.0f; // Default: no adjustment
}

/**
 * Get the enemy player (simplified - just gets first enemy we find)
 */
Player *AdaptiveCombatTracker::getEnemyPlayer() const
{
	if (!m_owner) return NULL;

	// Find any enemy player
	for (Player *player = ThePlayerList->getFirstPlayer(); player; player = player->getNextPlayer()) {
		if (player != m_owner && !player->isPlayerAlly(m_owner)) {
			return player;
		}
	}

	return NULL;
}
