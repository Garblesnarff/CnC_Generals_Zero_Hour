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

/*******************************************************************************
**
**  Miles Sound System - MP3 Decoder (ASI) - STUB HEADER
**
**  This is a stub implementation of the Miles ASI (Audio Stream Interface)
**  MP3 decoder header. It provides type definitions and function declarations
**  needed for compilation of MP3 streaming code.
**
**  These stubs allow the game code to compile on modern systems without
**  the proprietary Miles Sound System library.
**
*******************************************************************************/

#ifndef MP3DEC_H
#define MP3DEC_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
** Include Dependencies
*******************************************************************************/

#include "mss.h"

/*******************************************************************************
** Constants
*******************************************************************************/

// Stream buffer size for MP3 decoding
#define STREAM_BUFSIZE  8192

/*******************************************************************************
** Type Definitions
*******************************************************************************/

// ASI stream handle types
typedef void*              HASISTREAM;

// Callback function type for fetching stream data
typedef S32 (AILCALLBACK *AILSTREAMCB)(U32 user, void FAR *dest, S32 bytes, S32 offset);

/*******************************************************************************
** ASI Stream Structure (STUB)
*******************************************************************************/

// STUB: Internal structure for ASI stream state
typedef struct _ASISTREAM {
    // Stream state
    void*          user_data;
    AILSTREAMCB    fetch_CB;

    // Decoding state (stub fields)
    S32            output_cursor;
    S32            frame_size;
    S32            input_cursor;

    // Audio format info
    S32            rate;
    S32            bits;
    S32            channels;

    // Internal buffers (not actually used in stub)
    void*          internal_buffer;

} ASISTREAM;

/*******************************************************************************
** ASI MP3 Decoder Functions (STUB)
*******************************************************************************/

// Initialize the ASI system
DXDEC void       AILCALL ASI_startup (void);

// Shutdown the ASI system
DXDEC void       AILCALL ASI_shutdown (void);

// Open an MP3 stream for decoding
// Parameters:
//   user      - User data passed to callback
//   callback  - Function to fetch compressed data
//   total_size - Total size of compressed data (0 if unknown)
// Returns:
//   HASISTREAM handle, or NULL on failure
DXDEC HASISTREAM AILCALL ASI_stream_open (U32 user, AILSTREAMCB callback, S32 total_size);

// Close an MP3 stream
// Parameters:
//   stream - Stream handle to close
DXDEC void       AILCALL ASI_stream_close (HASISTREAM stream);

// Process (decode) MP3 stream data
// Parameters:
//   stream     - Stream handle
//   buffer     - Output buffer for PCM data
//   buffer_size - Size of output buffer
// Returns:
//   Number of bytes written to buffer, or 0 if no data available
DXDEC S32        AILCALL ASI_stream_process (HASISTREAM stream, void *buffer, S32 buffer_size);

// Seek to position in stream (STUB - not implemented)
DXDEC S32        AILCALL ASI_stream_seek (HASISTREAM stream, S32 offset);

// Get stream property (STUB - not implemented)
DXDEC S32        AILCALL ASI_stream_property (HASISTREAM stream, U32 property, void *before_value, void const *new_value, void *after_value);

#ifdef __cplusplus
}
#endif

#endif // MP3DEC_H
