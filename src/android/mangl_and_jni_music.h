/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once


#include "../core/mangl_setup.h"
#include "../core/audio/mangl_music.h"
#include "mangl_and_jni.h"

#if MANGL_MUSIC_JNI

namespace mangl {

class MusicEngineJni: public MusicEngineBase {
public:
    void play(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Real volume) override;
    void playCrossfade(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Timestamp gap, Real volume) override;
    void setVolume(Real volume, Real ramp) override;
    void stop(Timestamp delay, Timestamp ramp) override;

    void pause() override;
    void resume() override;
    void shutdownContext() override;
    void restoreContext() override;
    void shutdown() override;

};

} // namespace mangl

#endif


