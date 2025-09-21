/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once


#include "../mangl_types.h"


namespace mangl {

enum class AudioCommand: int {
    Invalid,

    SfxInit,
    SfxDestroy,
    SfxShutdownContext,
    SfxRestoreContext,
    SfxSetVolume,
    SfxLoad,
    SfxRelease,
    SfxPlay,
    SfxPlayDelayed,
    SfxClear,
    SfxUserSettingsVolume,
    SfxUserSettingsVolumeEnd,


    MusicInit,
    MusicDestroy,
    MusicLoad,

    MusicPlay,
    MusicPlayCrossfade,

    MusicPause,
    MusicResume,

    MusicStop,

    MusicSetVolume,
};

struct AudioCommandQueueItem {
    AudioCommand cmd;
    CommonId id;  // Sfx or Music track id
    Timestamp timestamp;  // Timestamp parameter
    Real volume;  // Volume parameter
    Real ramp;  // Ramp parameter
    StrView str;  // String parameter
    Real real;  // Real parameters (gap for crossfade)


    String strValue;
};


}  // namespace mangl
