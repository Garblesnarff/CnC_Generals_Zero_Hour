# Miles Sound System Stub Headers

This directory contains stub header files for the Miles Sound System (MSS) API. These headers allow the Command & Conquer Generals Zero Hour source code to compile on modern systems without requiring the proprietary Miles Sound System library.

## Files

### mss.h
The main Miles Sound System header file containing:
- Type definitions (HSAMPLE, H3DSAMPLE, HSTREAM, HAUDIO, HDIGDRIVER, HPROVIDER, etc.)
- Function declarations for all AIL_* API functions
- Constants (speaker types, sample processor types, etc.)
- MSADPCM coefficient table for audio decompression
- Platform-specific calling conventions (AILCALL, AILCALLBACK, DXDEC)

### mp3dec.h
The Miles ASI (Audio Stream Interface) MP3 decoder header containing:
- ASI stream types (HASISTREAM, ASISTREAM structure)
- MP3 decoder function declarations (ASI_*)
- Stream callback types
- Constants (STREAM_BUFSIZE)

## Purpose

These are **STUB HEADERS ONLY**. They provide:
1. **Type definitions** - All handle types, structures, and enums needed for compilation
2. **Function declarations** - All function signatures with correct prototypes
3. **Constants** - Required constant values and tables

These headers do NOT include:
- Actual implementations (no function bodies)
- Working audio functionality
- The proprietary Miles Sound System library

## Usage

The headers are designed to allow the game code to compile. To actually run the game with audio, you would need either:
1. The original Miles Sound System library (proprietary, not included)
2. A replacement audio system implementation
3. Stub implementations that return safe default values

## Integration

The headers are included in the codebase via:
- Direct path: `GeneralsMD/Code/Libraries/Source/WWVegas/Miles6/`
- Symlinked path: `GeneralsMD/Code/Libraries/Source/WPAudio/asimp3/`

Source files include them as:
```c
#include "mss.h"           // From WWVegas path
#include "mp3dec.h"        // From WWVegas path
#include "asimp3/mss.h"    // From WPAudio path
#include "asimp3/mp3dec.h" // From WPAudio path
```

## API Coverage

The stub headers cover approximately 211 Miles API functions across these categories:

### System Functions
- Initialization/shutdown (AIL_startup, AIL_shutdown, AIL_quick_startup, etc.)
- Version information (AIL_MSS_version)
- Error handling (AIL_last_error)
- Timer functions (AIL_get_timer_highest_delay)

### 2D Audio
- Sample allocation/release (AIL_allocate_sample_handle, AIL_release_sample_handle)
- Playback control (AIL_start_sample, AIL_stop_sample, AIL_resume_sample)
- Volume/pan control (AIL_set_sample_volume_pan, AIL_sample_volume_pan)
- Playback rate (AIL_set_sample_playback_rate, AIL_sample_playback_rate)
- Callbacks (AIL_register_EOS_callback)
- User data (AIL_set_sample_user_data, AIL_sample_user_data)

### 3D Audio
- Provider enumeration (AIL_enumerate_3D_providers)
- Provider management (AIL_open_3D_provider, AIL_close_3D_provider)
- Listener control (AIL_open_3D_listener, AIL_close_3D_listener)
- 3D sample management (AIL_allocate_3D_sample_handle, AIL_release_3D_sample_handle)
- 3D positioning (AIL_set_3D_position, AIL_set_3D_orientation)
- 3D sample playback (AIL_start_3D_sample, AIL_stop_3D_sample)
- Distance/occlusion (AIL_set_3D_sample_distances, AIL_set_3D_sample_occlusion)
- Speaker configuration (AIL_set_3D_speaker_type)

### Streaming Audio
- Stream management (AIL_open_stream, AIL_close_stream)
- Stream playback (AIL_start_stream, AIL_pause_stream)
- Stream position (AIL_stream_ms_position)
- Stream volume (AIL_set_stream_volume_pan, AIL_stream_volume_pan)
- Stream callbacks (AIL_register_stream_callback)

### Quick Audio
- Simple playback (AIL_quick_load_and_play, AIL_quick_halt, AIL_quick_unload)
- Quick volume control (AIL_quick_set_volume)

### Filters/Effects
- Filter enumeration (AIL_enumerate_filters)
- Sample processors (AIL_set_sample_processor)
- Filter preferences (AIL_set_filter_sample_preference)

### File I/O
- Custom file callbacks (AIL_set_file_callbacks)

### DirectSound Integration
- DirectSound info retrieval (AIL_get_DirectSound_info)

### Audio Data Processing
- WAV file parsing (AIL_WAV_info)
- ADPCM decompression (AIL_decompress_ADPCM)

### Memory Management
- Locked memory allocation (AIL_mem_alloc_lock, AIL_mem_free_lock)

### MP3 Decoding (ASI)
- MP3 system initialization (ASI_startup, ASI_shutdown)
- Stream operations (ASI_stream_open, ASI_stream_close, ASI_stream_process)

## Platform Compatibility

The headers use preprocessor directives to ensure cross-platform compilation:
- Windows: Uses `__stdcall` calling convention and `__declspec(dllexport)`
- Linux/Other: Uses standard C calling conventions

## License

Copyright 2025 Electronic Arts Inc.
Licensed under GNU General Public License v3.0 or later.
See LICENSE.md for details.

## Notes

- These headers are designed for COMPILATION ONLY
- They do NOT provide working audio functionality
- All function declarations return appropriate types but have no implementations
- Safe default values should be returned by stub implementations (NULL for pointers, 0 for integers, etc.)
- The MSADPCM_StdCoef table is provided for audio decompression algorithms
