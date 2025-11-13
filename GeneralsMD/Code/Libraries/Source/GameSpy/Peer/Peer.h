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
// GameSpy SDK Stub - Peer/Peer.h (GameSpy Peer Chat SDK)
// Handles chat rooms, lobbies, and game staging
//
// STUB IMPLEMENTATION - Functions are stubs for compilation only
// Multiplayer functionality will not work with these stubs
////////////////////////////////////////////////////////////////////////////////

#ifndef __PEER_H__
#define __PEER_H__

#include "../nonport.h"
#include "../gstypes.h"
#include "../GP/GP.h"

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// Types
//============================================================================

// Peer connection handle
typedef struct PEERPeer * PEER;

// Boolean values
#define PEERFalse 0
#define PEERTrue 1

//============================================================================
// Room Types
//============================================================================

typedef enum
{
	TitleRoom = 0,
	GroupRoom,
	StagingRoom,
	NumRooms
} RoomType;

//============================================================================
// Message Types
//============================================================================

typedef enum
{
	NormalMessage = 0,
	ActionMessage,
	NoticeMessage
} MessageType;

//============================================================================
// Player Flags
//============================================================================

#define PEER_FLAG_READY       0x0001
#define PEER_FLAG_PLAYING     0x0002
#define PEER_FLAG_AWAY        0x0004
#define PEER_FLAG_HOST        0x0008
#define PEER_FLAG_OP          0x0010
#define PEER_FLAG_STAGING     0x0020
#define PEER_FLAG_TALKING     0x0040
#define PEER_FLAG_NO_CROSSPLAY 0x0080

//============================================================================
// Game Reporting Flags
//============================================================================

#define PEER_KEEP_REPORTING  0
#define PEER_STOP_REPORTING  1

//============================================================================
// Result Codes
//============================================================================

typedef enum
{
	PEERSuccess = 0,
	PEEROutOfMemory,
	PEERBadParameter,
	PEERTimedOut,
	PEERHostNotFound,
	PEERConnectFailed,
	PEERNoConnection,
	PEERMax
} PEERBool;

//============================================================================
// Server Address Structure
//============================================================================

typedef struct
{
	unsigned int ip;
	unsigned int privateIp;
	unsigned short port;
	unsigned short privatePort;
	unsigned int queryPort;
} SBServer;

//============================================================================
// Callback Types
//============================================================================

// Generic callback
typedef void (* peerCallback)(PEER peer, PEERBool success, void * param);

// Nick error callback
typedef void (* peerNickErrorCallback)(PEER peer, int type, const char * nick,
	int numSuggestedNicks, const char ** suggestedNicks, void * param);

// Room callback
typedef void (* peerJoinRoomCallback)(PEER peer, PEERBool success,
	PEERBool result, RoomType roomType, void * param);

// Player callback
typedef void (* peerEnumPlayersCallback)(PEER peer, PEERBool success,
	RoomType roomType, int index, const char * nick, int flags, void * param);

// Listing callback
typedef void (* peerListingGamesCallback)(PEER peer, PEERBool success,
	const char * name, SBServer server, PEERBool staging,
	int msg, int progress, void * param);

// Room key callback
typedef void (* peerGetRoomKeysCallback)(PEER peer, PEERBool success,
	RoomType roomType, const char * nick, int num,
	const char ** keys, const char ** values, void * param);

// Global key callback
typedef void (* peerGetGlobalKeysCallback)(PEER peer, PEERBool success,
	const char * nick, int num, const char ** keys,
	const char ** values, void * param);

// CD key auth callback
typedef void (* peerAuthenticateCDKeyCallback)(PEER peer, int result,
	const char * message, void * param);

// Profile ID callback
typedef void (* peerGetPlayerProfileIDCallback)(PEER peer, PEERBool success,
	const char * nick, GPProfile profileID, void * param);

//============================================================================
// API Functions (Stubs)
//============================================================================

// Initialize/Shutdown
PEERBool peerInitialize(PEER * peer, int maxPlayers);
void peerShutdown(PEER peer);

// Connection
PEERBool peerConnect(
	PEER peer,
	const char * nick,
	int profileID,
	peerNickErrorCallback nickErrorCallback,
	peerCallback connectCallback,
	void * param,
	PEERBool blocking
);

void peerDisconnect(PEER peer);
void peerThink(PEER peer);

// Room Management
void peerJoinGroupRoom(
	PEER peer,
	int groupID,
	peerJoinRoomCallback callback,
	void * param,
	PEERBool blocking
);

void peerJoinStagingRoom(
	PEER peer,
	SBServer server,
	const char * password,
	peerJoinRoomCallback callback,
	void * param,
	PEERBool blocking
);

void peerCreateStagingRoomWithSocket(
	PEER peer,
	const char * name,
	int maxPlayers,
	const char * password,
	SOCKET socket,
	unsigned short port,
	peerJoinRoomCallback callback,
	void * param,
	PEERBool blocking
);

void peerLeaveRoom(
	PEER peer,
	RoomType roomType,
	const char * reason
);

void peerListGroupRooms(
	PEER peer,
	const char * fields,
	peerCallback callback,
	void * param,
	PEERBool blocking
);

// Messaging
void peerMessagePlayer(
	PEER peer,
	const char * nick,
	const char * message,
	MessageType messageType
);

void peerMessageRoom(
	PEER peer,
	RoomType roomType,
	const char * message,
	MessageType messageType
);

void peerUTMPlayer(
	PEER peer,
	const char * nick,
	const char * command,
	const char * parameters,
	PEERBool authenticated
);

void peerUTMRoom(
	PEER peer,
	RoomType roomType,
	const char * command,
	const char * parameters,
	PEERBool authenticated
);

// Player Enumeration
void peerEnumPlayers(
	PEER peer,
	RoomType roomType,
	peerEnumPlayersCallback callback,
	void * param
);

// Game Listing
void peerStartListingGames(
	PEER peer,
	const char ** fields,
	int numFields,
	const char * filter,
	peerListingGamesCallback callback,
	void * param
);

void peerStopListingGames(PEER peer);
void peerUpdateGame(PEER peer, SBServer server, PEERBool state);

// Game State
void peerStartGame(PEER peer, const char * message, int reportingOptions);
void peerStopGame(PEER peer);
void peerStateChanged(PEER peer);

// Keys Management
void peerSetGlobalKeys(
	PEER peer,
	int num,
	const char ** keys,
	const char ** values
);

void peerSetRoomKeys(
	PEER peer,
	RoomType roomType,
	const char * nick,
	int num,
	const char ** keys,
	const char ** values
);

void peerGetGlobalKeys(
	PEER peer,
	const char * nick,
	int num,
	const char ** keys,
	peerGetGlobalKeysCallback callback,
	void * param,
	PEERBool blocking
);

void peerGetRoomKeys(
	PEER peer,
	RoomType roomType,
	const char * nick,
	int num,
	const char ** keys,
	peerGetRoomKeysCallback callback,
	void * param,
	PEERBool blocking
);

void peerSetGlobalWatchKeys(
	PEER peer,
	RoomType roomType,
	int num,
	const char ** keys,
	PEERBool addKeys
);

void peerSetRoomWatchKeys(
	PEER peer,
	RoomType roomType,
	int num,
	const char ** keys,
	PEERBool addKeys
);

// CD Key Authentication
void peerAuthenticateCDKey(
	PEER peer,
	const char * cdkey,
	peerAuthenticateCDKeyCallback callback,
	void * param,
	PEERBool blocking
);

// Query Response
void peerParseQuery(
	PEER peer,
	const char * query,
	int len,
	struct sockaddr * sender
);

// Profile ID
void peerGetPlayerProfileID(
	PEER peer,
	const char * nick,
	peerGetPlayerProfileIDCallback callback,
	void * param,
	PEERBool blocking
);

// Retry with different nick
void peerRetryWithNick(PEER peer, const char * nick);

//============================================================================
// Stub Implementations
//============================================================================

#ifdef GAMESPY_STUB_IMPLEMENTATION

PEERBool peerInitialize(PEER * peer, int maxPlayers)
{
	(void)maxPlayers;
	if (peer) *peer = (PEER)1;
	return PEERSuccess;
}

void peerShutdown(PEER peer)
{
	(void)peer;
}

PEERBool peerConnect(PEER peer, const char * nick, int profileID,
	peerNickErrorCallback nickErrorCallback, peerCallback connectCallback,
	void * param, PEERBool blocking)
{
	(void)peer; (void)nick; (void)profileID; (void)nickErrorCallback;
	(void)connectCallback; (void)param; (void)blocking;
	return PEERConnectFailed;
}

void peerDisconnect(PEER peer)
{
	(void)peer;
}

void peerThink(PEER peer)
{
	(void)peer;
}

void peerJoinGroupRoom(PEER peer, int groupID, peerJoinRoomCallback callback,
	void * param, PEERBool blocking)
{
	(void)peer; (void)groupID; (void)callback; (void)param; (void)blocking;
}

void peerJoinStagingRoom(PEER peer, SBServer server, const char * password,
	peerJoinRoomCallback callback, void * param, PEERBool blocking)
{
	(void)peer; (void)server; (void)password; (void)callback; (void)param; (void)blocking;
}

void peerCreateStagingRoomWithSocket(PEER peer, const char * name, int maxPlayers,
	const char * password, SOCKET socket, unsigned short port,
	peerJoinRoomCallback callback, void * param, PEERBool blocking)
{
	(void)peer; (void)name; (void)maxPlayers; (void)password; (void)socket;
	(void)port; (void)callback; (void)param; (void)blocking;
}

void peerLeaveRoom(PEER peer, RoomType roomType, const char * reason)
{
	(void)peer; (void)roomType; (void)reason;
}

void peerListGroupRooms(PEER peer, const char * fields, peerCallback callback,
	void * param, PEERBool blocking)
{
	(void)peer; (void)fields; (void)callback; (void)param; (void)blocking;
}

void peerMessagePlayer(PEER peer, const char * nick, const char * message,
	MessageType messageType)
{
	(void)peer; (void)nick; (void)message; (void)messageType;
}

void peerMessageRoom(PEER peer, RoomType roomType, const char * message,
	MessageType messageType)
{
	(void)peer; (void)roomType; (void)message; (void)messageType;
}

void peerUTMPlayer(PEER peer, const char * nick, const char * command,
	const char * parameters, PEERBool authenticated)
{
	(void)peer; (void)nick; (void)command; (void)parameters; (void)authenticated;
}

void peerUTMRoom(PEER peer, RoomType roomType, const char * command,
	const char * parameters, PEERBool authenticated)
{
	(void)peer; (void)roomType; (void)command; (void)parameters; (void)authenticated;
}

void peerEnumPlayers(PEER peer, RoomType roomType, peerEnumPlayersCallback callback,
	void * param)
{
	(void)peer; (void)roomType; (void)callback; (void)param;
}

void peerStartListingGames(PEER peer, const char ** fields, int numFields,
	const char * filter, peerListingGamesCallback callback, void * param)
{
	(void)peer; (void)fields; (void)numFields; (void)filter;
	(void)callback; (void)param;
}

void peerStopListingGames(PEER peer)
{
	(void)peer;
}

void peerUpdateGame(PEER peer, SBServer server, PEERBool state)
{
	(void)peer; (void)server; (void)state;
}

void peerStartGame(PEER peer, const char * message, int reportingOptions)
{
	(void)peer; (void)message; (void)reportingOptions;
}

void peerStopGame(PEER peer)
{
	(void)peer;
}

void peerStateChanged(PEER peer)
{
	(void)peer;
}

void peerSetGlobalKeys(PEER peer, int num, const char ** keys, const char ** values)
{
	(void)peer; (void)num; (void)keys; (void)values;
}

void peerSetRoomKeys(PEER peer, RoomType roomType, const char * nick,
	int num, const char ** keys, const char ** values)
{
	(void)peer; (void)roomType; (void)nick; (void)num; (void)keys; (void)values;
}

void peerGetGlobalKeys(PEER peer, const char * nick, int num, const char ** keys,
	peerGetGlobalKeysCallback callback, void * param, PEERBool blocking)
{
	(void)peer; (void)nick; (void)num; (void)keys; (void)callback; (void)param; (void)blocking;
}

void peerGetRoomKeys(PEER peer, RoomType roomType, const char * nick, int num,
	const char ** keys, peerGetRoomKeysCallback callback, void * param, PEERBool blocking)
{
	(void)peer; (void)roomType; (void)nick; (void)num; (void)keys;
	(void)callback; (void)param; (void)blocking;
}

void peerSetGlobalWatchKeys(PEER peer, RoomType roomType, int num,
	const char ** keys, PEERBool addKeys)
{
	(void)peer; (void)roomType; (void)num; (void)keys; (void)addKeys;
}

void peerSetRoomWatchKeys(PEER peer, RoomType roomType, int num,
	const char ** keys, PEERBool addKeys)
{
	(void)peer; (void)roomType; (void)num; (void)keys; (void)addKeys;
}

void peerAuthenticateCDKey(PEER peer, const char * cdkey,
	peerAuthenticateCDKeyCallback callback, void * param, PEERBool blocking)
{
	(void)peer; (void)cdkey; (void)callback; (void)param; (void)blocking;
}

void peerParseQuery(PEER peer, const char * query, int len, struct sockaddr * sender)
{
	(void)peer; (void)query; (void)len; (void)sender;
}

void peerGetPlayerProfileID(PEER peer, const char * nick,
	peerGetPlayerProfileIDCallback callback, void * param, PEERBool blocking)
{
	(void)peer; (void)nick; (void)callback; (void)param; (void)blocking;
}

void peerRetryWithNick(PEER peer, const char * nick)
{
	(void)peer; (void)nick;
}

#endif // GAMESPY_STUB_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // __PEER_H__
