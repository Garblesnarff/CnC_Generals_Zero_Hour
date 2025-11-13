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
// AdaptiveAI_PersonalityExample.cpp
// Example usage of the AI Personality System
//
// This file demonstrates how to create AI opponents with different
// personalities and learning behaviors.
////////////////////////////////////////////////////////////////////////////////

#include "GameLogic/AdaptiveAI.h"
#include "Common/Player.h"

/**
 * EXAMPLE 1: Creating a simple game with different AI personalities
 *
 * This example shows how to set up 3 AI opponents, each with a different
 * personality, creating varied gameplay experiences.
 */
void SetupVariedAIOpponents()
{
	// Assume we have 3 AI players already created
	Player *aiPlayer1 = nullptr; // Get from game setup
	Player *aiPlayer2 = nullptr;
	Player *aiPlayer3 = nullptr;

	// Create adaptive AI trackers for each player
	AdaptiveCombatTracker *aggressiveAI = NEW_POOL_OBJ(AdaptiveCombatTracker, aiPlayer1);
	AdaptiveCombatTracker *economicAI = NEW_POOL_OBJ(AdaptiveCombatTracker, aiPlayer2);
	AdaptiveCombatTracker *defensiveAI = NEW_POOL_OBJ(AdaptiveCombatTracker, aiPlayer3);

	// Set different personalities
	aggressiveAI->setPersonality(AI_PERSONALITY_AGGRESSIVE);
	economicAI->setPersonality(AI_PERSONALITY_ECONOMIC);
	defensiveAI->setPersonality(AI_PERSONALITY_DEFENSIVE);

	// Now each AI will learn differently:
	// - Aggressive AI will focus on targeting enemy armies
	// - Economic AI will prioritize supply lines and economy buildings
	// - Defensive AI will carefully counter player tactics
}

/**
 * EXAMPLE 2: Random personality selection for variety
 *
 * This creates unpredictable AI opponents by randomly assigning personalities.
 */
void SetupRandomPersonalityAI(Player *aiPlayer)
{
	AdaptiveCombatTracker *ai = NEW_POOL_OBJ(AdaptiveCombatTracker, aiPlayer);

	// Pick a random personality
	int randomChoice = rand() % 4;

	switch (randomChoice) {
		case 0:
			ai->setPersonality(AI_PERSONALITY_AGGRESSIVE);
			// This AI will be very aggressive, targeting armies
			break;
		case 1:
			ai->setPersonality(AI_PERSONALITY_ECONOMIC);
			// This AI will target supply lines and economy
			break;
		case 2:
			ai->setPersonality(AI_PERSONALITY_DEFENSIVE);
			// This AI will be cautious and defensive
			break;
		case 3:
		default:
			ai->setPersonality(AI_PERSONALITY_BALANCED);
			// Standard balanced AI
			break;
	}
}

/**
 * EXAMPLE 3: Difficulty scaling with personalities
 *
 * Use personalities combined with other difficulty settings to create
 * unique difficulty levels.
 */
void SetupDifficultyBasedPersonality(Player *aiPlayer, int difficulty)
{
	AdaptiveCombatTracker *ai = NEW_POOL_OBJ(AdaptiveCombatTracker, aiPlayer);

	if (difficulty <= 2) {
		// Easy: Balanced personality (no particular bias)
		ai->setPersonality(AI_PERSONALITY_BALANCED);
	} else if (difficulty <= 4) {
		// Medium: Defensive personality (cautious, reacts to player)
		ai->setPersonality(AI_PERSONALITY_DEFENSIVE);
	} else if (difficulty <= 6) {
		// Hard: Aggressive personality (fast learning, aggressive targeting)
		ai->setPersonality(AI_PERSONALITY_AGGRESSIVE);
	} else {
		// Expert: Economic personality (disrupts player economy)
		ai->setPersonality(AI_PERSONALITY_ECONOMIC);
	}
}

/**
 * EXAMPLE 4: Custom personality weights
 *
 * If you want to create a custom personality behavior, you can access
 * and modify the weights directly.
 */
void CreateCustomPersonality(Player *aiPlayer)
{
	AdaptiveCombatTracker *ai = NEW_POOL_OBJ(AdaptiveCombatTracker, aiPlayer);

	// Start with a base personality
	ai->setPersonality(AI_PERSONALITY_BALANCED);

	// Note: Weights are set internally by setPersonality()
	// To create truly custom behavior, you would need to add a new
	// AI_PERSONALITY type to the enum and handle it in
	// initializePersonalityWeights()

	// But you can check current weights:
	const PersonalityWeights &weights = ai->getPersonalityWeights();
	// weights.economicDamageWeight, weights.unitKillWeight, etc.
}

/**
 * EXAMPLE 5: Team-based personality assignment
 *
 * In team games, you might want AI teammates to have complementary
 * personalities.
 */
void SetupTeamPersonalities(Player *teammate1, Player *teammate2, Player *teammate3)
{
	AdaptiveCombatTracker *ai1 = NEW_POOL_OBJ(AdaptiveCombatTracker, teammate1);
	AdaptiveCombatTracker *ai2 = NEW_POOL_OBJ(AdaptiveCombatTracker, teammate2);
	AdaptiveCombatTracker *ai3 = NEW_POOL_OBJ(AdaptiveCombatTracker, teammate3);

	// Create a balanced team:
	ai1->setPersonality(AI_PERSONALITY_AGGRESSIVE);  // Frontline attacker
	ai2->setPersonality(AI_PERSONALITY_ECONOMIC);     // Harasses enemy economy
	ai3->setPersonality(AI_PERSONALITY_DEFENSIVE);    // Holds defensive positions

	// This creates a well-rounded AI team where each AI has a specific role
}

/**
 * PERSONALITY BEHAVIOR SUMMARY:
 *
 * AI_PERSONALITY_AGGRESSIVE:
 * - Weights: High unitKill (2.0x), High collateralDamage (1.5x)
 * - Learning: Fast (1.5x), Acts quickly (needs 2 samples)
 * - Behavior: Focuses on eliminating enemy units, values army kills highly
 * - Best for: Rush strategies, direct combat focus
 *
 * AI_PERSONALITY_ECONOMIC:
 * - Weights: High economicDamage (2.5x), High strategic (2.0x)
 * - Learning: Moderate (1.2x), Acts quickly (needs 2 samples)
 * - Behavior: Targets supply lines, power plants, and economy buildings
 * - Best for: Harassment strategies, economic denial
 *
 * AI_PERSONALITY_DEFENSIVE:
 * - Weights: Balanced with slight unit kill preference (1.2x)
 * - Learning: Slow (0.8x), Cautious (needs 4 samples)
 * - Behavior: Carefully analyzes threats, adapts to counter player tactics
 * - Best for: Defensive play, reacting to player strategies
 *
 * AI_PERSONALITY_BALANCED:
 * - Weights: All 1.0x (equal weighting)
 * - Learning: Standard (1.0x), Normal (needs 3 samples)
 * - Behavior: General-purpose AI with no particular bias
 * - Best for: Default AI, traditional RTS experience
 */
