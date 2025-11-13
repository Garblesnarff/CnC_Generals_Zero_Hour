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
// GameSpy SDK Stub - gstypes.h
// Common GameSpy type definitions
//
// STUB IMPLEMENTATION - Functions are stubs for compilation only
// Multiplayer functionality will not work with these stubs
////////////////////////////////////////////////////////////////////////////////

#ifndef __GSTYPES_H__
#define __GSTYPES_H__

#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// Common result codes
typedef enum
{
	GSResultSection_None,
	GSResultSection_Network,
	GSResultSection_Memory,
	GSResultSection_File,
	GSResultSection_State,
	GSResultSection_AuthService,
	GSResultSection_Util
} GSResultSection;

typedef enum
{
	GSResultCode_Success = 0,
	GSResultCode_NetworkError,
	GSResultCode_OutOfMemory,
	GSResultCode_InvalidParameters,
	GSResultCode_Timeout,
	GSResultCode_Max
} GSResultCode;

// Boolean type
#ifndef gsi_bool
typedef int gsi_bool;
#endif

#ifndef gsi_true
	#define gsi_true 1
	#define gsi_false 0
#endif

// Callback result type
typedef struct GSResult
{
	GSResultSection section;
	GSResultCode code;
} GSResult;

// Memory allocation function types
typedef void* (*GSMemoryAllocFunc)(size_t size);
typedef void (*GSMemoryFreeFunc)(void* ptr);

#ifdef __cplusplus
}
#endif

#endif // __GSTYPES_H__
