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
// GameSpy SDK Stub - nonport.h
// Common non-portable definitions and platform-specific types
//
// STUB IMPLEMENTATION - Functions are stubs for compilation only
// Multiplayer functionality will not work with these stubs
////////////////////////////////////////////////////////////////////////////////

#ifndef __NONPORT_H__
#define __NONPORT_H__

#ifdef __cplusplus
extern "C" {
#endif

// Platform detection
#ifdef _WIN32
	#define _WIN32
	#ifdef _WIN64
		#define _X86_64
	#else
		#define _X86
	#endif
#endif

// Common GameSpy types
typedef int gsi_bool;
typedef int gsi_time;

// Boolean values
#ifndef GS_FALSE
	#define GS_FALSE 0
	#define GS_TRUE 1
#endif

// Character types
#ifdef GSI_UNICODE
	typedef unsigned short gsi_char;
	#define __TEXT(x) L##x
#else
	typedef char gsi_char;
	#define __TEXT(x) x
#endif

#define _T(x) __TEXT(x)
#define _TS(x) __TEXT(x)

// Network types
#ifdef _WIN32
	typedef unsigned int SOCKET;
	#define INVALID_SOCKET ((SOCKET)~0)
	#define SOCKET_ERROR (-1)
#else
	typedef int SOCKET;
	#define INVALID_SOCKET (-1)
	#define SOCKET_ERROR (-1)
#endif

// Common macros
#ifndef max
	#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
	#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

// Memory functions (stub implementations in actual SDK)
#define gsimalloc malloc
#define gsifree free
#define gsirealloc realloc

// String functions
#define gsiStringLen strlen
#define gsiStringCopy strcpy
#define gsiStringCompare strcmp

// Current time stub
static inline gsi_time current_time()
{
	return (gsi_time)time(NULL);
}

#ifdef __cplusplus
}
#endif

#endif // __NONPORT_H__
