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
// GameSpy SDK Stub - GP/GP.h (GameSpy Presence SDK)
// Handles profiles, buddies, and presence information
//
// STUB IMPLEMENTATION - Functions are stubs for compilation only
// Multiplayer functionality will not work with these stubs
////////////////////////////////////////////////////////////////////////////////

#ifndef __GP_H__
#define __GP_H__

#include "../nonport.h"
#include "../gstypes.h"

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// Types
//============================================================================

// Profile ID type
typedef int GPProfile;

// Enum type (used for status, etc.)
typedef int GPEnum;

// Connection handle
typedef struct GPIConnection * GPConnection;

// Boolean values
#define GP_FALSE 0
#define GP_TRUE 1

// Blocking/non-blocking modes
#define GP_NON_BLOCKING 0
#define GP_BLOCKING 1

//============================================================================
// Result Codes
//============================================================================

typedef enum
{
	GP_NO_ERROR = 0,
	GP_MEMORY_ERROR,
	GP_PARAMETER_ERROR,
	GP_NETWORK_ERROR,
	GP_SERVER_ERROR,
	GP_MAX_ERROR
} GPResult;

//============================================================================
// Connection Status
//============================================================================

typedef enum
{
	GP_OFFLINE = 0,
	GP_ONLINE,
	GP_PLAYING,
	GP_STAGING,
	GP_CHATTING,
	GP_AWAY,
	GP_MAX_STATUS
} GPStatus;

//============================================================================
// Info Masks
//============================================================================

#define GP_MASK_NONE          0x00000000
#define GP_MASK_HOMEPAGE      0x00000001
#define GP_MASK_ZIPCODE       0x00000002
#define GP_MASK_COUNTRYCODE   0x00000004
#define GP_MASK_BIRTHDAY      0x00000008
#define GP_MASK_SEX           0x00000010
#define GP_MASK_EMAIL         0x00000020
#define GP_MASK_ALL           0xFFFFFFFF

//============================================================================
// Cache Control
//============================================================================

#define GP_CHECK_CACHE   0x0001
#define GP_NO_CACHE      0x0000

//============================================================================
// Callback Types
//============================================================================

typedef enum
{
	GP_ERROR = 0,
	GP_RECV_BUDDY_MESSAGE,
	GP_RECV_BUDDY_REQUEST,
	GP_RECV_BUDDY_STATUS,
	GP_RECV_BUDDY_AUTH,
	GP_TRANSFER_CALLBACK,
	GP_NEW_PROFILE,
	GP_PROFILE_SEARCH,
	GP_GET_INFO,
	GP_CONNECT,
	GP_MAX_CALLBACK
} GPCallbackType;

//============================================================================
// Callback Argument Structures
//============================================================================

// Error callback
typedef struct
{
	GPResult result;
	GPEnum fatal;
	char * errorString;
	int errorCode;
} GPErrorArg;

// Connect response
typedef struct
{
	GPResult result;
	GPProfile profile;
	const char * uniquenick;
} GPConnectResponseArg;

// Buddy message
typedef struct
{
	GPProfile profile;
	unsigned int date;
	const char * message;
} GPRecvBuddyMessageArg;

// Buddy request
typedef struct
{
	GPProfile profile;
	unsigned int date;
	const char * reason;
} GPRecvBuddyRequestArg;

// Buddy status
typedef struct
{
	GPProfile profile;
	GPEnum status;
	const char * statusString;
	const char * locationString;
	unsigned int ip;
	int port;
	GPEnum quietModeFlags;
	int index;
} GPRecvBuddyStatusArg;

// Get info response
typedef struct
{
	GPResult result;
	GPProfile profile;
	const char * nick;
	const char * uniquenick;
	const char * email;
	const char * firstname;
	const char * lastname;
	const char * homepage;
	int icquin;
	const char * zipcode;
	const char * countrycode;
	float longitude;
	float latitude;
	const char * place;
	int birthday;
	int birthmonth;
	int birthyear;
	int sex;
	int publicmask;
	const char * aimname;
	int pic;
	int occupationid;
	int industryid;
	int incomeid;
	int marriedid;
	int childcount;
	int interests1;
	const char * ownership1;
	int conntypeid;
} GPGetInfoResponseArg;

// Generic callback type
typedef void (* GPCallback)(
	GPConnection * connection,
	void * arg,
	void * param
);

//============================================================================
// API Functions (Stubs)
//============================================================================

// Initialize/Destroy
GPResult gpInitialize(GPConnection * connection, int productID);
void gpDestroy(GPConnection * connection);

// Connection
GPResult gpConnect(
	GPConnection * connection,
	const char nick[],
	const char email[],
	const char password[],
	GPEnum firewall,
	GPEnum blocking,
	GPCallback callback,
	void * param
);

GPResult gpDisconnect(GPConnection * connection);
GPResult gpProcess(GPConnection * connection);

// Profile Management
GPResult gpNewProfile(
	GPConnection * connection,
	const char nick[],
	const char uniquenick[],
	const char email[],
	const char password[],
	GPEnum firewall,
	GPEnum blocking,
	GPCallback callback,
	void * param
);

GPResult gpDeleteProfile(GPConnection * connection);

// Buddy Management
GPResult gpSendBuddyRequest(
	GPConnection * connection,
	GPProfile profile,
	const char reason[]
);

GPResult gpAuthBuddyRequest(
	GPConnection * connection,
	GPProfile profile
);

GPResult gpDenyBuddyRequest(
	GPConnection * connection,
	GPProfile profile
);

GPResult gpDeleteBuddy(
	GPConnection * connection,
	GPProfile profile
);

GPResult gpSendBuddyMessage(
	GPConnection * connection,
	GPProfile profile,
	const char message[]
);

GPResult gpGetBuddyStatus(
	GPConnection * connection,
	int index,
	GPEnum * status
);

// Status
GPResult gpSetStatus(
	GPConnection * connection,
	GPEnum status,
	const char statusString[],
	const char locationString[]
);

GPResult gpSetInfoMask(
	GPConnection * connection,
	int mask
);

// Profile Info
GPResult gpGetInfo(
	GPConnection * connection,
	GPProfile profile,
	int checkCache,
	int blocking,
	GPCallback callback,
	void * param
);

// Callbacks
GPResult gpSetCallback(
	GPConnection * connection,
	GPCallbackType func,
	GPCallback callback,
	void * param
);

//============================================================================
// Stub Implementations
//============================================================================

#ifdef GAMESPY_STUB_IMPLEMENTATION

GPResult gpInitialize(GPConnection * connection, int productID)
{
	(void)connection; (void)productID;
	// Stub: Set connection to dummy value
	if (connection) *connection = (GPConnection)1;
	return GP_NO_ERROR;
}

void gpDestroy(GPConnection * connection)
{
	(void)connection;
	// Stub: Nothing to do
}

GPResult gpConnect(GPConnection * connection, const char nick[], const char email[],
	const char password[], GPEnum firewall, GPEnum blocking, GPCallback callback, void * param)
{
	(void)connection; (void)nick; (void)email; (void)password;
	(void)firewall; (void)blocking; (void)callback; (void)param;
	// Stub: Pretend connection failed
	return GP_NETWORK_ERROR;
}

GPResult gpDisconnect(GPConnection * connection)
{
	(void)connection;
	return GP_NO_ERROR;
}

GPResult gpProcess(GPConnection * connection)
{
	(void)connection;
	return GP_NO_ERROR;
}

GPResult gpNewProfile(GPConnection * connection, const char nick[], const char uniquenick[],
	const char email[], const char password[], GPEnum firewall, GPEnum blocking,
	GPCallback callback, void * param)
{
	(void)connection; (void)nick; (void)uniquenick; (void)email;
	(void)password; (void)firewall; (void)blocking; (void)callback; (void)param;
	return GP_NETWORK_ERROR;
}

GPResult gpDeleteProfile(GPConnection * connection)
{
	(void)connection;
	return GP_NO_ERROR;
}

GPResult gpSendBuddyRequest(GPConnection * connection, GPProfile profile, const char reason[])
{
	(void)connection; (void)profile; (void)reason;
	return GP_NETWORK_ERROR;
}

GPResult gpAuthBuddyRequest(GPConnection * connection, GPProfile profile)
{
	(void)connection; (void)profile;
	return GP_NO_ERROR;
}

GPResult gpDenyBuddyRequest(GPConnection * connection, GPProfile profile)
{
	(void)connection; (void)profile;
	return GP_NO_ERROR;
}

GPResult gpDeleteBuddy(GPConnection * connection, GPProfile profile)
{
	(void)connection; (void)profile;
	return GP_NO_ERROR;
}

GPResult gpSendBuddyMessage(GPConnection * connection, GPProfile profile, const char message[])
{
	(void)connection; (void)profile; (void)message;
	return GP_NETWORK_ERROR;
}

GPResult gpGetBuddyStatus(GPConnection * connection, int index, GPEnum * status)
{
	(void)connection; (void)index;
	if (status) *status = GP_OFFLINE;
	return GP_NO_ERROR;
}

GPResult gpSetStatus(GPConnection * connection, GPEnum status,
	const char statusString[], const char locationString[])
{
	(void)connection; (void)status; (void)statusString; (void)locationString;
	return GP_NO_ERROR;
}

GPResult gpSetInfoMask(GPConnection * connection, int mask)
{
	(void)connection; (void)mask;
	return GP_NO_ERROR;
}

GPResult gpGetInfo(GPConnection * connection, GPProfile profile, int checkCache,
	int blocking, GPCallback callback, void * param)
{
	(void)connection; (void)profile; (void)checkCache;
	(void)blocking; (void)callback; (void)param;
	return GP_NETWORK_ERROR;
}

GPResult gpSetCallback(GPConnection * connection, GPCallbackType func,
	GPCallback callback, void * param)
{
	(void)connection; (void)func; (void)callback; (void)param;
	return GP_NO_ERROR;
}

#endif // GAMESPY_STUB_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // __GP_H__
