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
#include "GameLogic/ScriptEngine.h"
#include "Common/Player.h"
#include "Common/PlayerList.h"
#include "Common/ThingTemplate.h"
#include "Common/GlobalData.h"

// Global instance
AdaptiveCombatTracker *TheAdaptiveAI = NULL;

/**
 * Constructor
 */
AdaptiveCombatTracker::AdaptiveCombatTracker(Player *owner)
	: m_owner(owner)
	, m_personality(AI_PERSONALITY_BALANCED)
{
	reset();
	initializePersonalityWeights();
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

	// Display debug information if enabled (every 3 seconds)
	displayDebugInfo();
}

/**
 * Calculate how effective a kill was.
 * Returns 0.0 to 2.0+ (0 = no impact, 1.0 = normal, 2.0 = very effective)
 * NOW PERSONALITY-AWARE: Uses personality weights to evaluate effectiveness
 */
Real AdaptiveCombatTracker::calculateEffectiveness(const TargetKillRecord &record)
{
	Real effectiveness = 0.5f; // Base value

	// Economic damage (weighted by personality)
	Int moneyDelta = record.enemyMoneyBefore - record.enemyMoneyAfter;
	if (moneyDelta > 1000) {
		effectiveness += 0.6f * m_weights.economicDamageWeight; // Major economic damage
	} else if (moneyDelta > 500) {
		effectiveness += 0.3f * m_weights.economicDamageWeight; // Significant economic impact
	}

	// Unit kills and collateral damage (weighted by personality)
	Int unitDelta = record.enemyUnitsCountBefore - record.enemyUnitsCountAfter;
	if (unitDelta > 3) {
		// Killed a valuable unit that took others with it
		effectiveness += 0.4f * m_weights.collateralDamageWeight;
	} else if (unitDelta > 1) {
		// Some collateral benefit
		effectiveness += 0.2f * m_weights.collateralDamageWeight;
	} else if (unitDelta == 1) {
		// Single unit kill - valued by aggressive personalities
		effectiveness += 0.3f * m_weights.unitKillWeight;
	}

	// Strategic target bonuses (weighted by personality)
	AsciiString targetType = record.targetType;
	if (targetType.find("Power") != std::string::npos) {
		// Power plants - strategic and economic
		effectiveness += 0.3f * m_weights.strategicTargetWeight;
	} else if (targetType.find("Supply") != std::string::npos ||
	           targetType.find("Dock") != std::string::npos) {
		// Economy targets - very valuable for economic personalities
		effectiveness += 0.4f * m_weights.economicDamageWeight;
		effectiveness += 0.2f * m_weights.strategicTargetWeight;
	} else if (targetType.find("Factory") != std::string::npos ||
	           targetType.find("Barracks") != std::string::npos ||
	           targetType.find("Airfield") != std::string::npos) {
		// Production facilities - critical strategic targets
		effectiveness += 0.5f * m_weights.strategicTargetWeight;
	} else if (targetType.find("Tank") != std::string::npos ||
	           targetType.find("Vehicle") != std::string::npos ||
	           targetType.find("Infantry") != std::string::npos ||
	           targetType.find("Aircraft") != std::string::npos) {
		// Combat units - valued by aggressive personalities
		effectiveness += 0.4f * m_weights.unitKillWeight;
	}

	return effectiveness;
}

/**
 * Update priority multipliers based on learned effectiveness.
 * NOW PERSONALITY-AWARE: Uses personality-specific learning rates and sample sizes.
 */
void AdaptiveCombatTracker::updatePriorities()
{
	// Adjust multipliers based on average effectiveness
	for (std::map<AsciiString, TargetTypeStats>::iterator it = m_targetStats.begin();
	     it != m_targetStats.end(); ++it) {
		TargetTypeStats &stats = it->second;

		// Only adjust if we have enough data (personality-specific threshold)
		if (stats.timesKilled < (Int)m_weights.minSampleSize) continue;

		// Map effectiveness to multiplier
		// avgEffectiveness: 0.0-0.5 = bad target (multiply by 0.7)
		// avgEffectiveness: 0.5-1.0 = normal (multiply by 1.0)
		// avgEffectiveness: 1.0-2.0+ = great target (multiply by 1.5-2.0)

		Real targetMultiplier;
		if (stats.avgEffectiveness < 0.5f) {
			targetMultiplier = 0.7f; // Deprioritize
		} else if (stats.avgEffectiveness > 1.5f) {
			targetMultiplier = 1.8f; // High priority!
		} else {
			// Linear interpolation between 0.8 and 1.5
			targetMultiplier = 0.8f + (stats.avgEffectiveness - 0.5f) * 0.7f;
		}

		// Apply learning rate: how quickly we adapt to new information
		// Higher learning rate = faster adaptation (aggressive)
		// Lower learning rate = slower, more cautious adaptation (defensive)
		Real delta = targetMultiplier - stats.priorityMultiplier;
		stats.priorityMultiplier += delta * m_weights.learningRate;

		// Clamp to reasonable range
		if (stats.priorityMultiplier < 0.5f) stats.priorityMultiplier = 0.5f;
		if (stats.priorityMultiplier > 2.5f) stats.priorityMultiplier = 2.5f;
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
 * Set the AI personality type.
 * This changes how the AI evaluates targets and learns from combat.
 */
void AdaptiveCombatTracker::setPersonality(AIPersonality personality)
{
	m_personality = personality;
	initializePersonalityWeights();

	// Reset learned data when changing personality
	// (different personality = different evaluation criteria)
	reset();
}

/**
 * Initialize personality-specific weight parameters.
 * Each personality type emphasizes different aspects of combat.
 */
void AdaptiveCombatTracker::initializePersonalityWeights()
{
	switch (m_personality) {
		case AI_PERSONALITY_AGGRESSIVE:
			// Aggressive AI: Focus on killing enemy units and armies
			m_weights.economicDamageWeight = 0.3f;			// Less interested in economy
			m_weights.unitKillWeight = 2.0f;						// Highly values unit kills
			m_weights.strategicTargetWeight = 0.5f;			// Some interest in strategic targets
			m_weights.collateralDamageWeight = 1.5f;		// Values collateral damage
			m_weights.learningRate = 1.5f;							// Learns quickly (aggressive adaptation)
			m_weights.minSampleSize = 2.0f;							// Acts on less data
			break;

		case AI_PERSONALITY_ECONOMIC:
			// Economic AI: Focus on disrupting enemy economy
			m_weights.economicDamageWeight = 2.5f;			// Highly values economic damage
			m_weights.unitKillWeight = 0.4f;						// Less interested in unit kills
			m_weights.strategicTargetWeight = 2.0f;			// Highly values strategic targets
			m_weights.collateralDamageWeight = 0.5f;		// Doesn't care much about collateral
			m_weights.learningRate = 1.2f;							// Learns moderately fast
			m_weights.minSampleSize = 2.0f;							// Acts on less data
			break;

		case AI_PERSONALITY_DEFENSIVE:
			// Defensive AI: Focus on countering player tactics
			m_weights.economicDamageWeight = 0.8f;			// Moderate interest in economy
			m_weights.unitKillWeight = 1.2f;						// Values eliminating threats
			m_weights.strategicTargetWeight = 1.0f;			// Balanced strategic interest
			m_weights.collateralDamageWeight = 1.0f;		// Balanced collateral interest
			m_weights.learningRate = 0.8f;							// Learns more slowly (cautious)
			m_weights.minSampleSize = 4.0f;							// Needs more data before acting
			break;

		case AI_PERSONALITY_BALANCED:
		default:
			// Balanced AI: No particular bias, standard learner
			m_weights.economicDamageWeight = 1.0f;
			m_weights.unitKillWeight = 1.0f;
			m_weights.strategicTargetWeight = 1.0f;
			m_weights.collateralDamageWeight = 1.0f;
			m_weights.learningRate = 1.0f;								// Normal learning rate
			m_weights.minSampleSize = 3.0f;							// Standard sample size
			break;
	}
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

/**
 * Display debug information about adaptive AI learning progress.
 * Shows what the AI has learned and current priorities.
 * Only displays when TheGlobalData->m_debugAI is enabled.
 */
void AdaptiveCombatTracker::displayDebugInfo()
{
	// Only display debug info if AI debugging is enabled
	if (!TheGlobalData || !TheGlobalData->m_debugAI) {
		return;
	}

	// Only output every few seconds to avoid spam
	UnsignedInt currentFrame = TheGameLogic->getFrame();
	if (currentFrame % (3 * LOGICFRAMES_PER_SECOND) != 0) {
		return;
	}

	// Build debug message
	AsciiString debugMsg;

	// Header: AI Personality Type
	debugMsg.concat("=== ADAPTIVE AI DEBUG ===");
	TheScriptEngine->AppendDebugMessage(debugMsg, false);

	// Show personality type
	debugMsg.clear();
	debugMsg.concat("Personality: ");
	switch (m_personality) {
		case AI_PERSONALITY_AGGRESSIVE:
			debugMsg.concat("AGGRESSIVE (Values unit kills)");
			break;
		case AI_PERSONALITY_ECONOMIC:
			debugMsg.concat("ECONOMIC (Targets economy)");
			break;
		case AI_PERSONALITY_DEFENSIVE:
			debugMsg.concat("DEFENSIVE (Counter tactics)");
			break;
		case AI_PERSONALITY_BALANCED:
		default:
			debugMsg.concat("BALANCED (Standard learner)");
			break;
	}
	TheScriptEngine->AppendDebugMessage(debugMsg, false);

	// Show learned priority multipliers (top priorities only)
	if (!m_targetStats.empty()) {
		debugMsg.clear();
		debugMsg.concat("--- Learned Priorities ---");
		TheScriptEngine->AppendDebugMessage(debugMsg, false);

		// Find targets with interesting multipliers (not 1.0)
		Int displayCount = 0;
		for (std::map<AsciiString, TargetTypeStats>::const_iterator it = m_targetStats.begin();
		     it != m_targetStats.end() && displayCount < 5; ++it) {
			const TargetTypeStats &stats = it->second;

			// Only show if we have enough data and multiplier is not default
			if (stats.timesKilled >= (Int)m_weights.minSampleSize) {
				debugMsg.clear();

				// Truncate long names
				AsciiString shortName = stats.typeName;
				if (shortName.length() > 20) {
					shortName = shortName.substr(0, 17);
					shortName.concat("...");
				}

				// Format: "TargetName: 1.8x (kills:5, eff:1.6)"
				char buffer[128];
				sprintf(buffer, "%s: %.1fx (kills:%d, eff:%.1f)",
				        shortName.str(),
				        stats.priorityMultiplier,
				        stats.timesKilled,
				        stats.avgEffectiveness);
				debugMsg.concat(buffer);

				TheScriptEngine->AppendDebugMessage(debugMsg, false);
				displayCount++;
			}
		}

		if (displayCount == 0) {
			debugMsg.clear();
			debugMsg.concat("(Not enough data yet - need ");
			char buffer[32];
			sprintf(buffer, "%d", (Int)m_weights.minSampleSize);
			debugMsg.concat(buffer);
			debugMsg.concat(" kills per target)");
			TheScriptEngine->AppendDebugMessage(debugMsg, false);
		}
	} else {
		debugMsg.clear();
		debugMsg.concat("(No targets analyzed yet)");
		TheScriptEngine->AppendDebugMessage(debugMsg, false);
	}

	// Show recent kills (last 3)
	if (!m_recentKills.empty()) {
		debugMsg.clear();
		debugMsg.concat("--- Recent Kills ---");
		TheScriptEngine->AppendDebugMessage(debugMsg, false);

		Int killCount = 0;
		for (Int i = (Int)m_recentKills.size() - 1; i >= 0 && killCount < 3; --i) {
			const TargetKillRecord &record = m_recentKills[i];

			// Only show analyzed kills (with effectiveness score)
			if (record.effectivenessScore > 0.0f) {
				debugMsg.clear();

				// Truncate long names
				AsciiString shortName = record.targetType;
				if (shortName.length() > 20) {
					shortName = shortName.substr(0, 17);
					shortName.concat("...");
				}

				// Format: "TargetName: eff=1.2 (dmg:$500, units:-2)"
				Int moneyDelta = record.enemyMoneyBefore - record.enemyMoneyAfter;
				Int unitDelta = record.enemyUnitsCountBefore - record.enemyUnitsCountAfter;

				char buffer[128];
				sprintf(buffer, "%s: eff=%.1f (dmg:$%d, units:%d)",
				        shortName.str(),
				        record.effectivenessScore,
				        moneyDelta,
				        unitDelta);
				debugMsg.concat(buffer);

				TheScriptEngine->AppendDebugMessage(debugMsg, false);
				killCount++;
			}
		}

		if (killCount == 0) {
			debugMsg.clear();
			debugMsg.concat("(No analyzed kills yet)");
			TheScriptEngine->AppendDebugMessage(debugMsg, false);
		}
	}

	// Footer
	debugMsg.clear();
	debugMsg.concat("=========================");
	TheScriptEngine->AppendDebugMessage(debugMsg, false);

	// Also log to debug output (for file logs)
	DEBUG_LOG(("Adaptive AI: %d target types learned, %d recent kills tracked\n",
	           (Int)m_targetStats.size(), (Int)m_recentKills.size()));
}
