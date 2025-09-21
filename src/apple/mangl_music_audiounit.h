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


#if MANGL_MUSIC_AUDIOUNIT

#include <CoreFoundation/CoreFoundation.h>


namespace mangl {

class MusicEngineDevice {
    using TrackId = MusicTrackId;
    
public:
    bool playing();
    bool paused() { return paused_; }
    
    void play(MusicTrackContextPtr track);
    void load(MusicTrackContextPtr track) {}
    void setVolumeRampTime(Timestamp time);
    void setVolume(Real volume, Real ramp);
    void stop();
    
    void onNotify(){};
    void onTimer(Timestamp timestamp);

    void pause();
    void resume();
    
    
private:
    MusicTrackContextPtr currentTrack_;
    bool paused_{};
    bool stopped_{true};
    
    //
    // Scheduling
    //
    ScheduleTriggerTimed volumeScheduled_, playScheduled_, queueDisposeScheduled_;
    MusicTrackContextPtr scheduledTrack_;
    Real scheduledVolume_, scheduledRamp_;


    void playTrackBody(MusicTrackContextPtr track);
    
    bool isRunning(); 
    
    
    void handleAudioQueueWarnings(OSStatus result, int line = 0);
    const char* audioQueueErrorString(OSStatus result);
    
    
    
    template<class... Args>
    void assertResult(OSStatus result, const char *fmt, Args&&... args) {
        if (result == 0)
            return;

        MDebugThrowError("AudioQueue::%s:%s: %s",
                         apl::extractFileName(currentTrack_->filename),
                         audioQueueErrorString(result),
                         apl::sformat(fmt, std::move(args)...));
    }

    
};

}  // namespace mangl

#endif
