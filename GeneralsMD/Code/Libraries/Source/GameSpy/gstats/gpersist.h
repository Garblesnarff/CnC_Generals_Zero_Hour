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
// GameSpy SDK Stub - gstats/gpersist.h (GameSpy Persistent Storage)
// Handles persistent player stats and data storage
//
// STUB IMPLEMENTATION - Functions are stubs for compilation only
// Multiplayer functionality will not work with these stubs
////////////////////////////////////////////////////////////////////////////////

#ifndef __GPERSIST_H__
#define __GPERSIST_H__

#include "../nonport.h"
#include "../gstypes.h"
#include "../GP/GP.h"

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// Types
//============================================================================

// Persistent data handle
typedef struct GPIPersistDataBuffer * persisttype_t;

//============================================================================
// Data Types
//============================================================================

typedef enum
{
	pd_private_ro = 0,    // Private read-only
	pd_private_rw,        // Private read-write
	pd_public_ro,         // Public read-only
	pd_public_rw          // Public read-write
} PersistDataType;

//============================================================================
// Result Codes
//============================================================================

typedef enum
{
	GP_PERSIST_SUCCESS = 0,
	GP_PERSIST_ERROR,
	GP_PERSIST_ERROR_NOT_SET,
	GP_PERSIST_ERROR_DATABASE_ERROR,
	GP_PERSIST_ERROR_DATABASE_FULL,
	GP_PERSIST_MAX
} GPPersistResult;

//============================================================================
// Callback Types
//============================================================================

// Callback for data operations
typedef void (* GPCallback)(
	GPConnection * connection,
	void * arg,
	void * param
);

// Data callback argument
typedef struct
{
	GPResult result;
	GPProfile profile;
	int index;
	int modified;
	const char * data;
	int dataLen;
} GPGetPersistDataArg;

typedef struct
{
	GPResult result;
} GPSetPersistDataArg;

//============================================================================
// API Functions (Stubs)
//============================================================================

// Get persistent data
GPResult gpGetPersistentData(
	GPConnection * connection,
	GPProfile profile,
	int index,
	PersistDataType dataType,
	GPCallback callback,
	void * param
);

// Set persistent data
GPResult gpSetPersistentData(
	GPConnection * connection,
	GPProfile profile,
	int index,
	PersistDataType dataType,
	const char * data,
	int dataLen,
	GPCallback callback,
	void * param
);

// Free persistent data buffer
void gpFreePersistentData(persisttype_t * data);

//============================================================================
// Stub Implementations
//============================================================================

#ifdef GAMESPY_STUB_IMPLEMENTATION

GPResult gpGetPersistentData(GPConnection * connection, GPProfile profile,
	int index, PersistDataType dataType, GPCallback callback, void * param)
{
	(void)connection; (void)profile; (void)index; (void)dataType;
	(void)callback; (void)param;
	// Stub: Return error
	return GP_NETWORK_ERROR;
}

GPResult gpSetPersistentData(GPConnection * connection, GPProfile profile,
	int index, PersistDataType dataType, const char * data, int dataLen,
	GPCallback callback, void * param)
{
	(void)connection; (void)profile; (void)index; (void)dataType;
	(void)data; (void)dataLen; (void)callback; (void)param;
	// Stub: Return error
	return GP_NETWORK_ERROR;
}

void gpFreePersistentData(persisttype_t * data)
{
	(void)data;
	// Stub: Nothing to do
}

#endif // GAMESPY_STUB_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // __GPERSIST_H__
