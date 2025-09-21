/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_music.h"


namespace mangl {

// Dummy Music engine
class MusicEngineDummy {
public:
    void initialize(){}
    void shutdown(){}
    void shutdownContext(){}
    void restoreContext(){}

    void play(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Real volume){}
    void playCrossfade(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Timestamp gap, Real volume){}
    void load(MusicTrackContextP track) {}
    void setVolume(Real volume, Real ramp){}
    void stop(Timestamp delay, Timestamp ramp){}

    void pause(){}
    void resume(){}

    void processTimer(Timestamp timestamp){}
};

}  // namespace mangl
