/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_music.h"

#include "../mangl_framework.h"


#if MANGL_MUSIC

#if MANGL_MUSIC_AUDIOQUEUE

#include "../../apple/mangl_music_audioqueue.h"

namespace mangl {
static MusicEngineAudioQueue device_{};
}

#elif MANGL_MUSIC_AUDIOUNIT

#include "../../apple/mangl_music_audiounit.h"


#elif MANGL_MUSIC_MCI

#include "../../windows/mangl_win_music_mci.h"

namespace mangl {
static MusicEngineMCI device_{};
}


#elif MANGL_MUSIC_DIRECTSOUND

#include "../../windows/mangl_music_directsound.h"

#elif MANGL_MUSIC_OBOE

#include "../../android/mangl_and_oboe_music.h"
namespace mangl {
static MusicEngineOboe device_{};
}

#elif MANGL_MUSIC_JNI

#include "../../android/mangl_and_jni_music.h"
namespace mangl {
static MusicEngineJni device_{};
}

#else // No music

#include "mangl_music_dummy.h"

namespace mangl {
static MusicEngineDummy device_{};
}  // namespace mangl

#endif


#else  // If no music

#include "mangl_music_dummy.h"

namespace mangl {
static MusicEngineDummy device_{};
}  // namespace mangl


#endif


namespace mangl {

Setting<bool> Music::settingEnabled{"mangl_music_enabled", true};
Setting<Real> Music::settingVolume{"mangl_music_volume", 1};


Music::Callback Music::onStart{};  // Track finished playing
Music::Callback Music::onPause{};  // Track paused playing
Music::Callback Music::onResume{};  // Track resumed playing
Music::Callback Music::onFinish{};  // Track finished playing
Music::Callback Music::onStop{};  // Track stopped playing


Music::Callback Music::onGameMusicStart{};  // Track finished playing
Music::Callback Music::onGameMusicPause{};  // Track paused playing
Music::Callback Music::onGameMusicResume{};  // Track resumed playing
Music::Callback Music::onGameMusicFinish{};  // Track finished playing
Music::Callback Music::onGameMusicStop{};  // Track stopped playing


bool Music::userEnabled_{true};  // Music enabled in general
bool Music::userStopped_{false}; // stop() was called, but the music might still be playing and
                             // "onFinished" will be called

bool Music::userPaused_{false};  // Music enabled in general

bool Music::statusPlaying_{false};
bool Music::statusPaused_{false};

Real Music::userVolume_{1};
Real Music::userRamp_{1};

Music::Mutex Music::containerMutex_{};
std::map<MusicTrackId, MusicTrackContextPtr> Music::tracks_{};

MusicTrackId Music::currentTrackId_{};



void Music::initialize() {
    device_.initialize();

    setEnabled(settingEnabled.get(true));
    setVolume(settingVolume.get(1));
}

void Music::destroy() {
}

void Music::startAudio() {
}

void Music::stopAudio() {
    device_.shutdown();
}

void Music::shutdownContext() {
    device_.shutdownContext();
}

void Music::restoreContext() {
    device_.restoreContext();
}



void Music::loadThread(MusicTrackId id, StrView filename) {

    auto track = makeMusicTrackContextPtr(id, filename);

    device_.load(track.get());

    {
        Lock lock{containerMutex_};
        tracks_[id] = track;
    }
}

void Music::playThread(MusicTrackId id, Timestamp delay, Timestamp ramp, Real volume) 
{
    MusicTrackContextP track{};

    {
        Lock lock{containerMutex_};
    
        auto trackIt = tracks_.find(id);

        if (trackIt == tracks_.end()) {
             mthrowError("AudioPlayback::play()::Track not found: %d", id);
             return;
        }
        
        track = trackIt->second.get();
    }

    device_.play(track, delay, ramp, volume);
}

void Music::playCrossfadeBody(MusicTrackId id, Timestamp delay, Timestamp ramp, Timestamp gap, Real volume) {
    MusicTrackContextP track{};

    {
        Lock lock{containerMutex_};
    
        auto trackIt = tracks_.find(id);

        if (trackIt == tracks_.end()) {
             mthrowError("AudioPlayback::play()::Track not found: %d", id);
             return;
        }
        
        track = trackIt->second.get();
    }

    device_.playCrossfade(track, delay, ramp, gap, volume);
}




// Real MMusic::volume() const {   return device_->volume() ;  }

void Music::setVolumeBody(Real volume, Real ramp) {
    //volume_ = volume;
    //ramp_ = ramp;

    //if (!enabled_)
    //    return;

    device_.setVolume(volume, ramp);
}

void Music::pauseBody() { 
    device_.pause();
}
void Music::resumeBody() { 
    device_.resume();
}


void Music::stopBody(Timestamp delay, Timestamp ramp) {
    device_.stop(delay, ramp);
}

void Music::enable(bool flag) {
    if (flag == false)
        stop();
    
    userEnabled_ = flag;
}



static Timestamp lastUserSettingPlayed_{};
static bool userSettingPlayScheduled_{};
static Timestamp userSettingPlayTimestamp_{};
static Real userSettingPlayVolume_{};

void Music::updateUserVolumePlay(Real volume, TrackId sampleId) {
    auto timestamp = Framework::timestamp();
    auto delta = timestamp - lastUserSettingPlayed_;

    if (delta >= userSettingPlayGap_) {
        lastUserSettingPlayed_ = timestamp;
        setVolume(volume);
        return;
    }

    // Schedule the sound play
    if (!userSettingPlayScheduled_) {
        userSettingPlayScheduled_ = true;
        userSettingPlayTimestamp_ = timestamp + userSettingPlayGap_ - delta;
        userSettingPlayVolume_ = volume;
    }

}


void Music::processTimer(Timestamp timestamp)
{
    if (userSettingPlayScheduled_ && timestamp >= userSettingPlayTimestamp_) {
        userSettingPlayScheduled_ = false;

        setVolume(userSettingPlayVolume_);
    }
    
    device_.processTimer(timestamp);
}


} // namespace mangl
