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

#include <apeal/buffer.hpp>
#include <apeal/file.hpp>


#if MANGL_MUSIC_AUDIOQUEUE

#include <AudioToolbox/AudioQueue.h>
#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>

namespace mangl {

class MusicEngineAudioQueue {
public:
    using TrackId = MusicTrackId;

    void initialize(){}
    void shutdown();
    void shutdownContext();
    void restoreContext();
    

    void play(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Real volume);
    void playCrossfade(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Timestamp gap, Real volume);
    void load(MusicTrackContextP track) {}
    void setVolume(Real volume, Real ramp);
    void stop(Timestamp delay, Timestamp ramp);

    void pause();
    void resume();

    void processTimer(Timestamp timestamp);


private:
    MusicTrackContextP currentTrack_{};
    bool stopped_{true};
    bool paused_{false};


    void aqPlayTrack(MusicTrackContextP track, Timestamp ramp, Real volume);

    bool isQueueRunning();
    void aqDisposeQueue(OSStatus status = 0);
    void aqCloseFile();
    
    void aqPause();
    void aqResume();
    
    void aqStop();
    void aqSetVolumeGain(Real volume, Timestamp ramp);
    void aqSetVolume(Real volume, Timestamp ramp);

    //
    // Error handling
    //
    const char *audioQueueErrorString(OSStatus result);
    void handleAudioQueueWarnings(OSStatus result, int line = 0);

    template<class... Args>
    void assertResult(OSStatus result, const char *fmt, Args&&... args) {
        if (result == 0)
            return;
        
        auto filename  = currentTrack_ ? apl::extractFileName(currentTrack_->filename) : "";
        auto errorDesc = apl::sformat(fmt, std::move(args)...);
        auto message = apl::sformat("AudioQueue::%s:%s :%s", filename, audioQueueErrorString(result), errorDesc);
        MDebugPrint(message);
//        MDebugThrowError(message);
        mthrowError(message);
    }

    void deriveAudioQueueBufferSize(AudioStreamBasicDescription &asbd, UInt32 maxPacketSize, Float64 seconds);


public:   // Callback
    void onAudioQueueOutputBuffer(AudioQueueRef aq, AudioQueueBufferRef buffer);
};

}   // namespace mangl

#endif
