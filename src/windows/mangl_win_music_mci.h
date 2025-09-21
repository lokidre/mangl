/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/audio/mangl_music.h"

#include <apeal/buffer.hpp>

#if MANGL_MUSIC_MCI

#include <Mmsystem.h>

namespace mangl {

class MusicEngineMCI {
    friend class ManglMainWindow;

    using Self = MusicEngineMCI;

public:

    void initialize();
    void shutdown();
    void shutdownContext(){}
    void restoreContext(){}


    void load(MusicTrackContextP track) {}

    void stop(Timestamp when, Timestamp ramp);
    void setVolume(Real volume, Real ramp);
    void play(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Real volume);
    void playCrossfade(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Timestamp gap, Real volume);
    bool playing();
    bool paused() { return false; }
    void pause();
    void resume();

    void setVolumeRampTime(Real) {}
    //void setRepeat ( bool f ) { repeat_ = f ;  }
    auto volume() const { return volume_; }


    void processTimer(Timestamp);
    void onNotify();

private:
    void onWndMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    MusicTrackId currentTrackId_{};
    Real volume_{1};

    bool mciCheckResult(int code);

    std::atomic_bool scheduleClose_{};

    MCIDEVICEID mciDeviceId_{};

    bool mciCmd(UINT uMsg, DWORD_PTR dwParam1, void* dwParam2);

    void mciCmdPlay(StrView filename);
    void mciCmdStop();
    void mciCmdClose();

};

}  // namespace mangl

#endif

