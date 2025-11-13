================================================================================
GameSpy SDK Stub Headers
Command & Conquer Generals Zero Hour
================================================================================

OVERVIEW
--------
This directory contains stub implementations of the GameSpy SDK headers
required to compile the Generals Zero Hour networking code without the
proprietary GameSpy SDK.

These are STUB implementations only - they allow compilation but do NOT
provide functional multiplayer networking. All functions return error codes
or do nothing to prevent crashes.

IMPLEMENTATION STATUS
---------------------
Goal: Allow code to compile with disabled multiplayer
Status: COMPLETE - Core headers created

LAN multiplayer may be restored later by implementing alternative backends
for the required functionality.

FILES CREATED
-------------

Common/Base Headers:
  nonport.h          - Platform-specific types and definitions
  gstypes.h          - Common GameSpy type definitions

GameSpy Presence & Messaging (GP) SDK:
  GP/GP.h            - Profile management, buddy lists, presence
                       * Handles user profiles and authentication
                       * Manages buddy/friend lists
                       * Reports online status

GameSpy Peer Chat SDK:
  Peer/Peer.h        - Chat rooms, lobbies, game staging
  peer/peer.h        - Lowercase alias (case-sensitivity)
                       * Manages chat channels/rooms
                       * Handles game lobbies
                       * Coordinates staging rooms for games
                       * Includes Query & Reporting (QR2) integration

GameSpy HTTP Client:
  ghttp/ghttp.h      - HTTP requests (patches, MOTD, config)
                       * Downloads game patches
                       * Fetches message of the day
                       * Retrieves configuration files
                       * Checks server status

GameSpy Stats & Persistent Storage:
  gstats/gpersist.h  - Persistent player stats storage
                       * Stores player statistics
                       * Manages persistent game data
                       * Handles leaderboards/rankings

USAGE PATTERNS FOUND IN CODEBASE
---------------------------------

Most Common Includes:
  1. GameSpy/Peer/Peer.h (4 occurrences)
  2. GameSpy/ghttp/ghttp.h (3 occurrences)
  3. GameSpy/GP/GP.h (3 occurrences)
  4. GameSpy/gstats/gpersist.h (1 occurrence)

Files Referencing GameSpy:
  - ~160+ source files
  - ~11,000 lines of code
  - Concentrated in GameNetwork/GameSpy/ directory

Primary Use Cases:
  - Online multiplayer lobbies
  - Player authentication
  - Buddy/friend lists
  - Chat functionality
  - Game browser/listing
  - Stats tracking
  - Patch checking
  - MOTD display

KEY TYPES AND FUNCTIONS
------------------------

GP (Presence) SDK:
  Types: GPConnection, GPProfile, GPEnum, GPResult
  Functions: gpInitialize, gpConnect, gpProcess, gpSendBuddyMessage,
             gpSetStatus, gpGetInfo, gpSetCallback

Peer (Chat) SDK:
  Types: PEER, RoomType (TitleRoom, GroupRoom, StagingRoom), MessageType
  Functions: peerConnect, peerJoinGroupRoom, peerJoinStagingRoom,
             peerCreateStagingRoomWithSocket, peerMessageRoom,
             peerStartListingGames, peerEnumPlayers, peerThink

GHTTP (HTTP) SDK:
  Types: GHTTPRequest, GHTTPResult, GHTTPState
  Functions: ghttpGet, ghttpHead, ghttpPost, ghttpThink, ghttpSetProxy

GPersist (Stats) SDK:
  Types: persisttype_t, PersistDataType
  Functions: gpGetPersistentData, gpSetPersistentData

STUB BEHAVIOR
-------------

All stub functions:
  1. Return safe error codes (GP_NETWORK_ERROR, PEERConnectFailed, etc.)
  2. Set output parameters to safe defaults
  3. Do not access network or perform I/O
  4. May invoke callbacks immediately with failure results
  5. Are marked with (void) parameter annotations to prevent warnings

This ensures:
  - Code compiles without errors
  - No crashes due to NULL pointers or uninitialized data
  - Graceful degradation (multiplayer features disabled)
  - Clear indication that network features are unavailable

BUILDING WITH STUBS
--------------------

To use these stubs, ensure the include path contains:
  GeneralsMD/Code/Libraries/Source

The code should #include headers like:
  #include "GameSpy/Peer/Peer.h"
  #include "GameSpy/GP/GP.h"
  #include "GameSpy/ghttp/ghttp.h"

No link libraries are needed - these are header-only stubs.

FUTURE WORK
-----------

Potential enhancements:
  1. Implement LAN-only Peer functionality
  2. Create local profile storage (no GameSpy server)
  3. Add LAN discovery for game browsers
  4. Implement local stats storage (file-based)
  5. Remove GameSpy dependencies entirely and refactor

TECHNICAL NOTES
---------------

QR2 (Query & Reporting 2):
  - Not a separate header
  - Integrated into Peer SDK
  - Used for game server queries
  - Handles NAT traversal

GT2 (GameSpy Transport 2):
  - Not found in includes
  - May be internal to Peer SDK
  - Not needed for compilation

ServerBrowser:
  - References found in code
  - Likely part of Peer SDK
  - Not a separate include

COMPATIBILITY
-------------

These stubs are compatible with:
  - Windows (MSVC, MinGW)
  - Linux (GCC, Clang)
  - Original C&C Generals code structure

Platform differences handled through:
  - SOCKET type definitions
  - Conditional includes for winsock2.h vs sys/socket.h
  - Standard C types where possible

LICENSE
-------

All stub implementations are released under GPLv3, matching the
main Generals Zero Hour project license.

================================================================================
End of GameSpy SDK Stub Documentation
================================================================================
