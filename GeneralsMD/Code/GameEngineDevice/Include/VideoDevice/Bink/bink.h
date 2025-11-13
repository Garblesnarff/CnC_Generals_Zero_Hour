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
//                                                                            //
// BINK VIDEO SDK - STUB IMPLEMENTATION                                      //
//                                                                            //
// This is a stub header file for the Bink Video SDK.                        //
// It provides minimal definitions to allow compilation but does not         //
// implement actual video playback functionality.                            //
//                                                                            //
// The real Bink SDK is proprietary software from RAD Game Tools.            //
// This stub is provided for compilation purposes only.                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef __BINK_H__
#define __BINK_H__

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// Type Definitions
//============================================================================

// Bink handle structure - contains video stream state
typedef struct BINK {
    unsigned int Width;      // Width of video in pixels
    unsigned int Height;     // Height of video in pixels
    unsigned int Frames;     // Total number of frames
    unsigned int FrameNum;   // Current frame number (1-based)
} BINK;

// Bink handle type (pointer to BINK structure)
typedef BINK* HBINK;

// Sound system callback function pointer types (not used in stubs)
typedef void* BINKSNDOPEN;

//============================================================================
// Constants and Flags
//============================================================================

// BinkOpen flags
#define BINKPRELOADALL      0x00000001  // Preload entire video into memory

// BinkCopyToBuffer surface format flags
#define BINKSURFACE32       0x00000001  // 32-bit X8R8G8B8 format
#define BINKSURFACE24       0x00000002  // 24-bit R8G8B8 format
#define BINKSURFACE565      0x00000004  // 16-bit R5G6B5 format
#define BINKSURFACE555      0x00000008  // 16-bit X1R5G5B5 format

//============================================================================
// Function Declarations - All return safe default values
//============================================================================

// Opens a Bink file for playback
// STUB: Returns NULL (video will not play)
static inline HBINK BinkOpen(const char* name, unsigned int flags)
{
    (void)name;   // Unused parameter
    (void)flags;  // Unused parameter
    return NULL;  // STUB: Cannot open video
}

// Closes a Bink file
// STUB: No operation
static inline void BinkClose(HBINK bink)
{
    (void)bink;  // Unused parameter
    // STUB: Nothing to close
}

// Waits until it's time to decompress the next frame
// STUB: Returns 0 (always ready)
static inline int BinkWait(HBINK bink)
{
    (void)bink;  // Unused parameter
    return 0;    // STUB: Always ready
}

// Decompresses a frame of video
// STUB: No operation
static inline void BinkDoFrame(HBINK bink)
{
    (void)bink;  // Unused parameter
    // STUB: Nothing to decompress
}

// Advances to the next frame
// STUB: No operation
static inline void BinkNextFrame(HBINK bink)
{
    (void)bink;  // Unused parameter
    // STUB: No frame to advance
}

// Copies decompressed frame to a buffer
// STUB: No operation
static inline void BinkCopyToBuffer(HBINK bink, void* dest, int dest_pitch,
                                   int dest_height, unsigned int dest_x,
                                   unsigned int dest_y, unsigned int flags)
{
    (void)bink;        // Unused parameter
    (void)dest;        // Unused parameter
    (void)dest_pitch;  // Unused parameter
    (void)dest_height; // Unused parameter
    (void)dest_x;      // Unused parameter
    (void)dest_y;      // Unused parameter
    (void)flags;       // Unused parameter
    // STUB: No frame data to copy
}

// Goes to a specific frame
// STUB: No operation
static inline void BinkGoto(HBINK bink, unsigned int frame, int flags)
{
    (void)bink;  // Unused parameter
    (void)frame; // Unused parameter
    (void)flags; // Unused parameter
    // STUB: Cannot seek
}

// Sets the volume for a specific track
// STUB: No operation
static inline void BinkSetVolume(HBINK bink, unsigned int track_id, int volume)
{
    (void)bink;     // Unused parameter
    (void)track_id; // Unused parameter
    (void)volume;   // Unused parameter
    // STUB: No audio to set volume for
}

// Configures Bink to use DirectSound for audio output
// STUB: Returns 0 (failure)
static inline int BinkSoundUseDirectSound(void* driver)
{
    (void)driver;  // Unused parameter
    return 0;      // STUB: DirectSound not available
}

// Sets which audio tracks to play
// STUB: No operation
static inline void BinkSetSoundTrack(unsigned int total_tracks, unsigned int* tracks)
{
    (void)total_tracks; // Unused parameter
    (void)tracks;       // Unused parameter
    // STUB: No audio tracks to set
}

#ifdef __cplusplus
}
#endif

#endif // __BINK_H__
