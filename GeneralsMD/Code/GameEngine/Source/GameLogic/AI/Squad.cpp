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
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// FILE: Squad.cpp 
/*---------------------------------------------------------------------------*/
/* EA Pacific                                                                */
/* Confidential Information	                                                 */
/* Copyright (C) 2001 - All Rights Reserved                                  */
/* DO NOT DISTRIBUTE                                                         */
/*---------------------------------------------------------------------------*/
/* Project:    RTS3                                                          */
/* File name:  Squad.cpp                                                      */
/* Created:    John K. McDonald, Jr., 4/19/2002                               */
/* Desc:       // @todo                                                      */
/* Revision History:                                                         */
/*		4/19/2002 : Initial creation                                          */
/*---------------------------------------------------------------------------*/
#include "PreRTS.h"	// This must go first in EVERY cpp file int the GameEngine

#include "GameLogic/Squad.h"

#include "Common/GameState.h"
#include "Common/Team.h"
#include "Common/Xfer.h"

#include "GameLogic/AI.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/Object.h"
#include "GameLogic/ScriptEngine.h"
#include "GameLogic/Module/AIUpdate.h"
#include "GameLogic/PartitionManager.h"

#ifdef _INTERNAL
// for occasional debugging...
//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif

// addObject //////////////////////////////////////////////////////////////////////////////////////
void Squad::addObject(Object *objectToAdd)
{
	if (objectToAdd) {
		m_objectIDs.push_back(objectToAdd->getID());
	}
}

// addObjectID ////////////////////////////////////////////////////////////////////////////////////
void Squad::addObjectID(ObjectID objectID) {
	m_objectIDs.push_back(objectID);
}

// removeObject ///////////////////////////////////////////////////////////////////////////////////
void Squad::removeObject(Object *objectToRemove)
{
	if (objectToRemove) {
		ObjectID objID;
		objID = objectToRemove->getID();
		VecObjectIDIt it = std::find(m_objectIDs.begin(), m_objectIDs.end(), objID);
		if (it != m_objectIDs.end()) {
			m_objectIDs.erase(it);
		}
	}
}

// clearSquad /////////////////////////////////////////////////////////////////////////////////////
void Squad::clearSquad() {
	m_objectIDs.clear();
	m_objectsCached.clear();
}

// getAllObjects //////////////////////////////////////////////////////////////////////////////////
const VecObjectPtr& Squad::getAllObjects(void) // Not a const function cause we clear away dead object here too
{
	// prunes all NULL objects
	m_objectsCached.clear();
	for (VecObjectIDIt it = m_objectIDs.begin(); it != m_objectIDs.end(); ) {
		Object *obj = TheGameLogic->findObjectByID(*it);
		if (obj) {
			m_objectsCached.push_back(obj);
			++it;
		} else {
			it = m_objectIDs.erase(it);
		}
	}

	return m_objectsCached;
}

// getLiveObjects /////////////////////////////////////////////////////////////////////////////////
const VecObjectPtr& Squad::getLiveObjects(void) 
{
	// first get all the objects.
	// cheat, since we are a member function, and just use m_objectsCached
	getAllObjects();
	for (VecObjectPtrIt it = m_objectsCached.begin(); it != m_objectsCached.end(); ) {
		if (!(*it)->isSelectable()) {
			it = m_objectsCached.erase(it);		
		} else {
			++it;
		}
	}
	
	return m_objectsCached;
}

// getSizeOfGroup /////////////////////////////////////////////////////////////////////////////////
Int Squad::getSizeOfGroup(void) const
{
	return m_objectIDs.size();
}

// isOnSquad //////////////////////////////////////////////////////////////////////////////////////
Bool Squad::isOnSquad(const Object *objToTest) const
{
	// @todo need a faster way to do this. Perhaps a more efficient data structure?
	ObjectID objID = objToTest->getID();
	for (VecObjectID::const_iterator cit = m_objectIDs.begin(); cit != m_objectIDs.end(); ++cit) {
		if (objID == (*cit)) {
			return true;
		}
	}

	return false;
}

/**
 * There should never be a TeamFromSqaud as Teams are entirely a construct to work with the AI. 
 * Since things can only be on one Team at a time, creating a Team from an arbitrary Squad will 
 * cause weird, difficult to reproduce bugs. Please don't do it.
 */

// squadFromTeam //////////////////////////////////////////////////////////////////////////////////
void Squad::squadFromTeam(const Team* fromTeam, Bool clearSquadFirst)
{
	if (!fromTeam) {
		return;
	}

	if (clearSquadFirst) {
		m_objectIDs.clear();
	}

	for (DLINK_ITERATOR<Object> iter = fromTeam->iterate_TeamMemberList(); !iter.done(); iter.advance()) {
		Object *obj = iter.cur();
		m_objectIDs.push_back(obj->getID());
	}
}

// squadFromAIGroup ///////////////////////////////////////////////////////////////////////////////
void Squad::squadFromAIGroup(const AIGroup* fromAIGroup, Bool clearSquadFirst)
{
	if (!fromAIGroup) {
		return;
	}

	if (clearSquadFirst) {
		m_objectIDs.clear();
	}

	m_objectIDs = fromAIGroup->getAllIDs();
}

// aiGroupFromSquad ///////////////////////////////////////////////////////////////////////////////
void Squad::aiGroupFromSquad(AIGroup* aiGroupToFill)
{
	if (!aiGroupToFill) {
		return;
	}
	
	// cheat, since we are a member function, and just use m_objectsCached
	getLiveObjects();
	for (VecObjectPtr::iterator it = m_objectsCached.begin(); it != m_objectsCached.end(); ++it) {
		aiGroupToFill->add((*it));
	}
}

// ------------------------------------------------------------------------------------------------
/** Focus Fire Enhancement - Find best target for squad coordination
 *  This enhances squad AI by helping units focus fire on high-value targets
 *  that multiple squad members can attack together.
 */
// ------------------------------------------------------------------------------------------------
Object* Squad::findBestFocusFireTarget(Real maxRange, const AttackPriorityInfo* attackInfo) const
{
	const VecObjectPtr& liveObjects = const_cast<Squad*>(this)->getLiveObjects();
	if (liveObjects.empty()) {
		return NULL;
	}

	// Get reference squad member position for distance calculations
	Object* firstMember = liveObjects[0];
	if (!firstMember) {
		return NULL;
	}

	// Find all potential targets within range
	std::vector<Object*> potentialTargets;

	// Use partition manager to efficiently find enemies in range
	PartitionFilterRelationship filterEnemy(firstMember, PartitionFilterRelationship::ALLOW_ENEMIES);
	PartitionFilterPossibleToAttack filterAttackable(ATTACK_NEW_TARGET, firstMember, CMD_FROM_AI);

	PartitionData data;
	data.pos = *firstMember->getPosition();
	data.radius = maxRange;

	ThePartitionManager->findObjectsInRange(&data, &filterEnemy, &filterAttackable);

	if (data.objectList.empty()) {
		return NULL;
	}

	// Score each target based on focus fire potential
	Object* bestTarget = NULL;
	Real bestScore = -1.0f;

	for (ListObjectPtr::iterator it = data.objectList.begin(); it != data.objectList.end(); ++it) {
		Object* target = *it;
		if (!target || target->isEffectivelyDead()) {
			continue;
		}

		Real score = calculateFocusFireScore(target, attackInfo);
		if (score > bestScore) {
			bestScore = score;
			bestTarget = target;
		}
	}

	return bestTarget;
}

// ------------------------------------------------------------------------------------------------
/** Count how many squad members can attack a given target
 */
// ------------------------------------------------------------------------------------------------
Int Squad::countSquadMembersCanAttack(const Object* target) const
{
	if (!target) {
		return 0;
	}

	const VecObjectPtr& liveObjects = const_cast<Squad*>(this)->getLiveObjects();
	Int count = 0;

	for (VecObjectPtr::const_iterator it = liveObjects.begin(); it != liveObjects.end(); ++it) {
		Object* member = *it;
		if (!member || member->isEffectivelyDead()) {
			continue;
		}

		// Check if this member can attack the target
		CanAttackResult result = member->getAbleToAttackSpecificObject(ATTACK_NEW_TARGET, target, CMD_FROM_AI);
		if (result == ATTACKRESULT_POSSIBLE || result == ATTACKRESULT_POSSIBLE_AFTER_MOVING) {
			count++;
		}
	}

	return count;
}

// ------------------------------------------------------------------------------------------------
/** Calculate focus fire score for a potential target
 *  Higher score = better target for coordinated attack
 *
 *  Score factors:
 *  - Number of squad members that can attack (encourages focus fire)
 *  - Attack priority (respects existing priority system)
 *  - Distance (closer targets score higher)
 *  - Target health (wounded targets score higher to finish them off)
 */
// ------------------------------------------------------------------------------------------------
Real Squad::calculateFocusFireScore(const Object* target, const AttackPriorityInfo* attackInfo) const
{
	if (!target) {
		return 0.0f;
	}

	// Base score starts at 1.0
	Real score = 1.0f;

	// Factor 1: Number of squad members that can attack this target
	// This is the core of focus fire - prioritize targets multiple units can hit
	Int attackerCount = countSquadMembersCanAttack(target);
	if (attackerCount == 0) {
		return 0.0f; // Can't attack, invalid target
	}

	// Scale score by number of attackers (more attackers = better target)
	// Use square root to avoid extreme bias towards single targets
	score *= (1.0f + sqrtf((Real)attackerCount));

	// Factor 2: Attack priority from attack priority info
	if (attackInfo) {
		Int priority = attackInfo->getPriority(target->getTemplate());
		if (priority > 0) {
			// Higher priority targets get better scores
			score *= (1.0f + (Real)priority * 0.2f);
		}
	}

	// Factor 3: Distance - closer targets are preferred
	const VecObjectPtr& liveObjects = const_cast<Squad*>(this)->getLiveObjects();
	if (!liveObjects.empty() && liveObjects[0]) {
		Real distSqr = liveObjects[0]->getPosition()->distance2DSquared(*target->getPosition());
		// Inverse distance factor (closer = higher score)
		// Add small value to avoid division by zero
		Real distFactor = 1000.0f / (distSqr + 100.0f);
		score *= distFactor;
	}

	// Factor 4: Target health - prefer wounded targets to finish them off
	const BodyModuleInterface* body = target->getBodyModuleInterface();
	if (body) {
		Real healthPercent = body->getHealthPercent();
		// Bonus for targets below 50% health (finish them off!)
		if (healthPercent < 0.5f) {
			score *= (1.0f + (0.5f - healthPercent));
		}
	}

	// Factor 5: Bonus for high-value targets (structures, special units)
	KindOfMaskType highValueTypes;
	highValueTypes.set(KINDOF_STRUCTURE);
	highValueTypes.set(KINDOF_HERO);
	highValueTypes.set(KINDOF_HUGE_VEHICLE);
	if (target->isAnyKindOf(highValueTypes)) {
		score *= 1.3f;
	}

	return score;
}

// ------------------------------------------------------------------------------------------------
/** CRC */
// ------------------------------------------------------------------------------------------------
void Squad::crc( Xfer *xfer )
{

}  // end crc

// ------------------------------------------------------------------------------------------------
/** Xfer method
	* Version Info:
	* 1: Initial version */
// ------------------------------------------------------------------------------------------------
void Squad::xfer( Xfer *xfer )
{

	// version
	XferVersion currentVersion = 1;
	XferVersion version = currentVersion;
	xfer->xferVersion( &version, currentVersion );

	// length of object ID list
	UnsignedShort objectCount = m_objectIDs.size();
	xfer->xferUnsignedShort( &objectCount );

	// object id elements
	ObjectID objectID;
	if( xfer->getXferMode() == XFER_SAVE )
	{

		// save each object id
		VecObjectIDIt it;
		for( it = m_objectIDs.begin(); it != m_objectIDs.end(); ++it )
		{

			// save object ID
			objectID = *it;
			xfer->xferObjectID( &objectID );

		}  // end for, it

	}  // end if, save
	else
	{
		
		// the cached objects list should be empty
		if( m_objectsCached.size() != 0 )
		{

			DEBUG_CRASH(( "Squad::xfer - m_objectsCached should be emtpy, but is not\n" ));
			throw SC_INVALID_DATA;

		}  // end of

		// read all items
		for( UnsignedShort i = 0; i < objectCount; ++i )
		{

			// read id
			xfer->xferObjectID( &objectID );

			// put on list
			m_objectIDs.push_back( objectID );

		}  // end for, i

	}  // end else, load

}  // end xfer

// ------------------------------------------------------------------------------------------------
/** Load post process */
// ------------------------------------------------------------------------------------------------
void Squad::loadPostProcess( void )
{

}  // end loadPostProcess
