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
**  Miles Sound System (MSS) - STUB HEADER
**
**  This is a stub implementation of the Miles Sound System API header.
**  It provides type definitions and function declarations needed for
**  compilation, but does not implement actual audio functionality.
**
**  These stubs allow the game code to compile on modern systems without
**  the proprietary Miles Sound System library.
**
*******************************************************************************/

#ifndef MSS_H
#define MSS_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
** Platform and Compiler Definitions
*******************************************************************************/

#ifndef FAR
#define FAR
#endif

#ifndef AILCALLBACK
  #ifdef _WIN32
    #define AILCALLBACK __stdcall
  #else
    #define AILCALLBACK
  #endif
#endif

#ifndef AILCALL
  #ifdef _WIN32
    #define AILCALL __stdcall
  #else
    #define AILCALL
  #endif
#endif

#ifndef DXDEC
  #ifdef _WIN32
    #define DXDEC __declspec(dllexport)
  #else
    #define DXDEC
  #endif
#endif

/*******************************************************************************
** Basic Type Definitions
*******************************************************************************/

typedef signed char        S8;
typedef unsigned char      U8;
typedef signed short       S16;
typedef unsigned short     U16;
typedef signed int         S32;
typedef unsigned int       U32;
typedef float              F32;
typedef double             F64;

/*******************************************************************************
** Handle Type Definitions (STUB: All handles are void pointers)
*******************************************************************************/

typedef void*              HSAMPLE;          // 2D sample handle
typedef void*              H3DSAMPLE;        // 3D sample handle
typedef void*              HSTREAM;          // Stream handle
typedef void*              HAUDIO;           // Generic audio handle
typedef void*              HDIGDRIVER;       // Digital driver handle
typedef void*              HPROVIDER;        // 3D provider handle
typedef void*              HPROENUM;         // Provider enumeration handle
typedef void*              HTIMER;           // Timer handle

/*******************************************************************************
** Constants
*******************************************************************************/

#define HPROENUM_FIRST     ((HPROENUM)0)

// Sample processor types
#define DP_FILTER          0x1000

// Speaker type constants
#define AIL_3D_2_SPEAKER   0
#define AIL_3D_HEADPHONE   1
#define AIL_3D_4_SPEAKER   2
#define AIL_3D_SURROUND    3
#define AIL_3D_51_SPEAKER  4
#define AIL_3D_71_SPEAKER  5

/*******************************************************************************
** Structure Definitions
*******************************************************************************/

// STUB: WAV file info structure
typedef struct {
    S32 format;
    void* data_ptr;
    U32 data_len;
    S32 rate;
    S32 bits;
    S32 channels;
    U32 samples;
    U32 block_size;
} AILSOUNDINFO;

/*******************************************************************************
** MS ADPCM Constants
*******************************************************************************/

// Microsoft ADPCM standard coefficient table
// Used for MS ADPCM decompression
static const S16 MSADPCM_StdCoef[7][2] = {
    { 256,   0},
    { 512,-256},
    {   0,   0},
    { 192,  64},
    { 240,   0},
    { 460,-208},
    { 392,-232}
};

/*******************************************************************************
** Callback Function Types
*******************************************************************************/

// File I/O callbacks
typedef U32  (AILCALLBACK *AIL_file_open_callback)  (char const *filename, U32 *file_handle);
typedef void (AILCALLBACK *AIL_file_close_callback) (U32 file_handle);
typedef S32  (AILCALLBACK *AIL_file_seek_callback)  (U32 file_handle, S32 offset, U32 type);
typedef U32  (AILCALLBACK *AIL_file_read_callback)  (U32 file_handle, void *buffer, U32 bytes);

/*******************************************************************************
** Miles Sound System Initialization/Shutdown Functions (STUB)
*******************************************************************************/

DXDEC void       AILCALL AIL_set_redist_directory (const char *dir);
DXDEC void       AILCALL AIL_startup (void);
DXDEC void       AILCALL AIL_shutdown (void);
DXDEC S32        AILCALL AIL_quick_startup (S32 use_digital, S32 use_MIDI, U32 output_rate, S32 output_bits, S32 output_channels);
DXDEC void       AILCALL AIL_quick_shutdown (void);
DXDEC void       AILCALL AIL_quick_handles (HDIGDRIVER *pdig, void *pmdi, void *pdls);
DXDEC char *     AILCALL AIL_last_error (void);
DXDEC void       AILCALL AIL_set_preference (U32 pref, S32 value);
DXDEC S32        AILCALL AIL_get_preference (U32 pref);
DXDEC void       AILCALL AIL_MSS_version (char *buffer, S32 buffer_size);

/*******************************************************************************
** Timer Functions (STUB)
*******************************************************************************/

DXDEC S32        AILCALL AIL_get_timer_highest_delay (void);

/*******************************************************************************
** File Callback Functions (STUB)
*******************************************************************************/

DXDEC void       AILCALL AIL_set_file_callbacks (
    AIL_file_open_callback  opencb,
    AIL_file_close_callback closecb,
    AIL_file_seek_callback  seekcb,
    AIL_file_read_callback  readcb);

/*******************************************************************************
** DirectSound Integration Functions (STUB)
*******************************************************************************/

DXDEC void       AILCALL AIL_get_DirectSound_info (void *sample, void **lplpDS, void **lplpDSB);

/*******************************************************************************
** 2D Sample Functions (STUB)
*******************************************************************************/

DXDEC HSAMPLE    AILCALL AIL_allocate_sample_handle (HDIGDRIVER dig);
DXDEC void       AILCALL AIL_release_sample_handle (HSAMPLE sample);
DXDEC void       AILCALL AIL_init_sample (HSAMPLE sample);
DXDEC S32        AILCALL AIL_set_sample_file (HSAMPLE sample, void const *file_image, S32 block);
DXDEC void       AILCALL AIL_start_sample (HSAMPLE sample);
DXDEC void       AILCALL AIL_stop_sample (HSAMPLE sample);
DXDEC void       AILCALL AIL_resume_sample (HSAMPLE sample);
DXDEC void       AILCALL AIL_set_sample_volume_pan (HSAMPLE sample, F32 volume, F32 pan);
DXDEC void       AILCALL AIL_sample_volume_pan (HSAMPLE sample, F32 *volume, F32 *pan);
DXDEC void       AILCALL AIL_set_sample_playback_rate (HSAMPLE sample, S32 rate);
DXDEC S32        AILCALL AIL_sample_playback_rate (HSAMPLE sample);
DXDEC void       AILCALL AIL_set_sample_loop_count (HSAMPLE sample, S32 count);
DXDEC S32        AILCALL AIL_sample_loop_count (HSAMPLE sample);
DXDEC void       AILCALL AIL_set_sample_user_data (HSAMPLE sample, U32 index, S32 value);
DXDEC S32        AILCALL AIL_sample_user_data (HSAMPLE sample, U32 index);
DXDEC void       AILCALL AIL_register_EOS_callback (HSAMPLE sample, void (AILCALLBACK *callback)(HSAMPLE));
DXDEC S32        AILCALL AIL_sample_status (HSAMPLE sample);

/*******************************************************************************
** 3D Provider Functions (STUB)
*******************************************************************************/

DXDEC S32        AILCALL AIL_enumerate_3D_providers (HPROENUM *next, HPROVIDER *dest, char **name);
DXDEC S32        AILCALL AIL_open_3D_provider (HPROVIDER provider);
DXDEC void       AILCALL AIL_close_3D_provider (HPROVIDER provider);
DXDEC void       AILCALL AIL_set_3D_speaker_type (HPROVIDER provider, S32 speaker_type);
DXDEC void*      AILCALL AIL_open_3D_listener (HPROVIDER provider);
DXDEC void       AILCALL AIL_close_3D_listener (void *listener);
DXDEC void       AILCALL AIL_set_3D_position (void *obj, F32 x, F32 y, F32 z);
DXDEC void       AILCALL AIL_set_3D_orientation (void *obj, F32 x_face, F32 y_face, F32 z_face, F32 x_up, F32 y_up, F32 z_up);

/*******************************************************************************
** 3D Sample Functions (STUB)
*******************************************************************************/

DXDEC H3DSAMPLE  AILCALL AIL_allocate_3D_sample_handle (HPROVIDER provider);
DXDEC void       AILCALL AIL_release_3D_sample_handle (H3DSAMPLE sample);
DXDEC S32        AILCALL AIL_set_3D_sample_file (H3DSAMPLE sample, void const *file_image);
DXDEC void       AILCALL AIL_start_3D_sample (H3DSAMPLE sample);
DXDEC void       AILCALL AIL_stop_3D_sample (H3DSAMPLE sample);
DXDEC void       AILCALL AIL_resume_3D_sample (H3DSAMPLE sample);
DXDEC void       AILCALL AIL_set_3D_sample_volume (H3DSAMPLE sample, F32 volume);
DXDEC void       AILCALL AIL_set_3D_sample_playback_rate (H3DSAMPLE sample, S32 rate);
DXDEC S32        AILCALL AIL_3D_sample_playback_rate (H3DSAMPLE sample);
DXDEC void       AILCALL AIL_set_3D_sample_distances (H3DSAMPLE sample, F32 max_dist, F32 min_dist);
DXDEC void       AILCALL AIL_set_3D_sample_loop_count (H3DSAMPLE sample, S32 count);
DXDEC void       AILCALL AIL_set_3D_sample_occlusion (H3DSAMPLE sample, F32 occlusion);
DXDEC void       AILCALL AIL_set_3D_user_data (H3DSAMPLE sample, U32 index, S32 value);
DXDEC S32        AILCALL AIL_3D_user_data (H3DSAMPLE sample, U32 index);
DXDEC void       AILCALL AIL_register_3D_EOS_callback (H3DSAMPLE sample, void (AILCALLBACK *callback)(H3DSAMPLE));
DXDEC S32        AILCALL AIL_3D_sample_status (H3DSAMPLE sample);

/*******************************************************************************
** Stream Functions (STUB)
*******************************************************************************/

DXDEC HSTREAM    AILCALL AIL_open_stream (HDIGDRIVER dig, char const *filename, S32 stream_mem);
DXDEC void       AILCALL AIL_close_stream (HSTREAM stream);
DXDEC void       AILCALL AIL_start_stream (HSTREAM stream);
DXDEC void       AILCALL AIL_pause_stream (HSTREAM stream, S32 onoff);
DXDEC void       AILCALL AIL_set_stream_volume_pan (HSTREAM stream, F32 volume, F32 pan);
DXDEC void       AILCALL AIL_stream_volume_pan (HSTREAM stream, F32 *volume, F32 *pan);
DXDEC void       AILCALL AIL_set_stream_loop_count (HSTREAM stream, S32 count);
DXDEC S32        AILCALL AIL_stream_loop_count (HSTREAM stream);
DXDEC void       AILCALL AIL_stream_ms_position (HSTREAM stream, S32 *total_ms, S32 *current_ms);
DXDEC void       AILCALL AIL_register_stream_callback (HSTREAM stream, void (AILCALLBACK *callback)(HSTREAM));
DXDEC S32        AILCALL AIL_stream_status (HSTREAM stream);

/*******************************************************************************
** Quick Audio Functions (STUB)
*******************************************************************************/

DXDEC HAUDIO     AILCALL AIL_quick_load_and_play (char const *filename, U32 loop_count, S32 wait_request);
DXDEC void       AILCALL AIL_quick_halt (HAUDIO audio);
DXDEC void       AILCALL AIL_quick_unload (HAUDIO audio);
DXDEC void       AILCALL AIL_quick_set_volume (HAUDIO audio, F32 volume, F32 extravol);
DXDEC S32        AILCALL AIL_quick_status (HAUDIO audio);

/*******************************************************************************
** Filter/Effect Functions (STUB)
*******************************************************************************/

DXDEC S32        AILCALL AIL_enumerate_filters (HPROENUM *next, HPROVIDER *dest, char **name);
DXDEC S32        AILCALL AIL_set_sample_processor (HSAMPLE sample, S32 pipeline_stage, HPROVIDER provider);
DXDEC S32        AILCALL AIL_set_filter_sample_preference (HSAMPLE sample, char const *name, void const *val);

/*******************************************************************************
** WAV File Functions (STUB)
*******************************************************************************/

DXDEC S32        AILCALL AIL_WAV_info (void const *data, AILSOUNDINFO *info);
DXDEC S32        AILCALL AIL_decompress_ADPCM (AILSOUNDINFO const *info, void **outdata, U32 *outsize);

/*******************************************************************************
** Memory Functions (STUB)
*******************************************************************************/

DXDEC void FAR * AILCALL AIL_mem_alloc_lock (U32 size);
DXDEC void       AILCALL AIL_mem_free_lock (void FAR *ptr);

/*******************************************************************************
** CPU Detection Functions (STUB)
*******************************************************************************/

DXDEC U32        AILCALL AIL_MMX_available (void);

#ifdef __cplusplus
}
#endif

#endif // MSS_H
