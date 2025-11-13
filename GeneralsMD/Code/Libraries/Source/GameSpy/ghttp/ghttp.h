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
// GameSpy SDK Stub - ghttp/ghttp.h (GameSpy HTTP Client)
// Handles HTTP requests for patches, MOTD, config files, etc.
//
// STUB IMPLEMENTATION - Functions are stubs for compilation only
// Multiplayer functionality will not work with these stubs
////////////////////////////////////////////////////////////////////////////////

#ifndef __GHTTP_H__
#define __GHTTP_H__

#include "../nonport.h"
#include "../gstypes.h"

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// Types
//============================================================================

// HTTP request handle
typedef int GHTTPRequest;

// Boolean values
#define GHTTPFalse 0
#define GHTTPTrue 1

//============================================================================
// HTTP Result Codes
//============================================================================

typedef enum
{
	GHTTPSuccess = 0,
	GHTTPOutOfMemory,
	GHTTPBufferOverflow,
	GHTTPParseURLFailed,
	GHTTPHostLookupFailed,
	GHTTPSocketFailed,
	GHTTPConnectFailed,
	GHTTPBadResponse,
	GHTTPRequestCancelled,
	GHTTPUnauthorized,
	GHTTPForbidden,
	GHTTPFileNotFound,
	GHTTPServerError,
	GHTTPFileWriteFailed,
	GHTTPFileReadFailed,
	GHTTPFileIncomplete,
	GHTTPFileToBig,
	GHTTPEncryptionError,
	GHTTPRequestTimeout,
	GHTTPMax
} GHTTPResult;

//============================================================================
// HTTP States
//============================================================================

typedef enum
{
	GHTTPWaiting = 0,
	GHTTPHostLookup,
	GHTTPConnecting,
	GHTTPSecuringSession,
	GHTTPSendingRequest,
	GHTTPPosting,
	GHTTPWaitingForResponse,
	GHTTPReceivingStatus,
	GHTTPReceivingHeaders,
	GHTTPReceivingFile,
	GHTTPComplete,
	GHTTPMax_State
} GHTTPState;

//============================================================================
// Callback Types
//============================================================================

// Completion callback
typedef int (* ghttpCompletedCallback)(
	GHTTPRequest request,
	GHTTPResult result,
	char * buffer,
	int bufferLen,
	void * param
);

// Progress callback
typedef void (* ghttpProgressCallback)(
	GHTTPRequest request,
	GHTTPState state,
	const char * buffer,
	int bufferLen,
	int bytesReceived,
	int totalSize,
	void * param
);

// Post callback
typedef void (* ghttpPostCallback)(
	GHTTPRequest request,
	int bytesPosted,
	int totalBytes,
	int objectsPosted,
	int totalObjects,
	void * param
);

//============================================================================
// API Functions (Stubs)
//============================================================================

// HTTP GET request
GHTTPRequest ghttpGet(
	const char * URL,
	int blocking,
	ghttpCompletedCallback completedCallback,
	void * param
);

// HTTP HEAD request
GHTTPRequest ghttpHead(
	const char * URL,
	int blocking,
	ghttpCompletedCallback completedCallback,
	void * param
);

// HTTP POST request
GHTTPRequest ghttpPost(
	const char * URL,
	int blocking,
	const char * data,
	int dataLen,
	ghttpCompletedCallback completedCallback,
	void * param
);

// Process HTTP requests
void ghttpThink(void);

// Cancel request
void ghttpCancelRequest(GHTTPRequest request);

// Set proxy
void ghttpSetProxy(const char * proxy);

// Get status
GHTTPState ghttpGetState(GHTTPRequest request);

// Get response
const char * ghttpGetResponseStatus(GHTTPRequest request);

// Get headers
const char * ghttpGetHeaders(GHTTPRequest request);
const char * ghttpGetHeader(GHTTPRequest request, const char * name);

// Stream to file
GHTTPRequest ghttpSaveEx(
	const char * URL,
	const char * filename,
	const char * headers,
	ghttpProgressCallback progressCallback,
	ghttpCompletedCallback completedCallback,
	void * param
);

GHTTPRequest ghttpSave(
	const char * URL,
	const char * filename,
	int blocking,
	ghttpCompletedCallback completedCallback,
	void * param
);

//============================================================================
// Stub Implementations
//============================================================================

#ifdef GAMESPY_STUB_IMPLEMENTATION

static int ghttpRequestCounter = 1;

GHTTPRequest ghttpGet(const char * URL, int blocking,
	ghttpCompletedCallback completedCallback, void * param)
{
	(void)URL; (void)blocking;
	// Immediately call callback with failure
	if (completedCallback)
		completedCallback(ghttpRequestCounter, GHTTPHostLookupFailed, NULL, 0, param);
	return ghttpRequestCounter++;
}

GHTTPRequest ghttpHead(const char * URL, int blocking,
	ghttpCompletedCallback completedCallback, void * param)
{
	(void)URL; (void)blocking;
	// Immediately call callback with failure
	if (completedCallback)
		completedCallback(ghttpRequestCounter, GHTTPHostLookupFailed, NULL, 0, param);
	return ghttpRequestCounter++;
}

GHTTPRequest ghttpPost(const char * URL, int blocking, const char * data, int dataLen,
	ghttpCompletedCallback completedCallback, void * param)
{
	(void)URL; (void)blocking; (void)data; (void)dataLen;
	// Immediately call callback with failure
	if (completedCallback)
		completedCallback(ghttpRequestCounter, GHTTPHostLookupFailed, NULL, 0, param);
	return ghttpRequestCounter++;
}

void ghttpThink(void)
{
	// Stub: Nothing to do
}

void ghttpCancelRequest(GHTTPRequest request)
{
	(void)request;
	// Stub: Nothing to do
}

void ghttpSetProxy(const char * proxy)
{
	(void)proxy;
	// Stub: Nothing to do
}

GHTTPState ghttpGetState(GHTTPRequest request)
{
	(void)request;
	return GHTTPComplete;
}

const char * ghttpGetResponseStatus(GHTTPRequest request)
{
	(void)request;
	return "404 Not Found";
}

const char * ghttpGetHeaders(GHTTPRequest request)
{
	(void)request;
	return "";
}

const char * ghttpGetHeader(GHTTPRequest request, const char * name)
{
	(void)request; (void)name;
	return NULL;
}

GHTTPRequest ghttpSaveEx(const char * URL, const char * filename,
	const char * headers, ghttpProgressCallback progressCallback,
	ghttpCompletedCallback completedCallback, void * param)
{
	(void)URL; (void)filename; (void)headers; (void)progressCallback;
	// Immediately call callback with failure
	if (completedCallback)
		completedCallback(ghttpRequestCounter, GHTTPFileNotFound, NULL, 0, param);
	return ghttpRequestCounter++;
}

GHTTPRequest ghttpSave(const char * URL, const char * filename, int blocking,
	ghttpCompletedCallback completedCallback, void * param)
{
	(void)URL; (void)filename; (void)blocking;
	// Immediately call callback with failure
	if (completedCallback)
		completedCallback(ghttpRequestCounter, GHTTPFileNotFound, NULL, 0, param);
	return ghttpRequestCounter++;
}

#endif // GAMESPY_STUB_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // __GHTTP_H__
