/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_sound.h"

#include "../mangl_framework.h"


#if MANGL_AUDIO_OPENAL

#include "mangl_sound_openal.h"
namespace mangl {
using SfxEngine = OpenALEngine;
}  // namespace mangl

#elif MANGL_AUDIO_DIRECTSOUND

#include "../../windows/mangl_win_directsound.h"
namespace mangl {
using SfxEngine = SfxEngineDirectSound;
}  // namespace mangl

//#elif MANGL_AUDIO_AVAUDIOENGINE
//#include "../../apple/mangl_avaudio_engine.h"
//namespace mangl {
//class SfxEngine: public AVAudioEngineSfxEngine{};
//}  // namespace mangl
//#endif

#elif MANGL_AUDIO_OBOE

#include "../../android/mangl_and_oboe.h"

namespace mangl {
using SfxEngine = SfxEngineOboe;
}  // namespace mangl


#elif MANGL_AUDIO_OPENSL

#include "../../android/mangl_and_opensl.h"
namespace mangl {
using SfxEngine = SfxEngineOpenSL;
}  // namespace mangl

#else

namespace mangl {

class SfxEngineDummy {
    using Ctx = SfxContext;
    using Ctxp = SfxContextPtr;

public:
    void release(Ctxp &ctx){}
    void release(){}

    void shutdownContext() {}
    void restoreContext() {}

    void clearErrors() {}

    bool load(Ctxp &ctx, StrView filename){return true;}
    bool play(Ctxp &ctx, Real volume){return true;}

    void setVolume(Ctxp &ctx, Real volume){}
};

using SfxEngine = SfxEngineDummy;

} // namespace mangl
#endif



#if MANGL_CAPTURE_AUDIO
#include "../game/game_capture_audio.h"
#endif



#include <apeal/file.hpp>

namespace mangl {


static SfxEngine engine_{};

Setting<bool> Sfx::settingEnabled{"mangl_sfx_enabled", true};
Setting<Real> Sfx::settingVolume{"mangl_sfx_volume", 1};
std::vector<Sfx::ScheduleItem> Sfx::scheduledItems_{};



Sfx::SoundsContainer Sfx::soundsById_{};
StrValCont<SfxId> Sfx::soundsByFn_{};
Sfx::Mutex Sfx::containerMutex_{};



Real Sfx::volume_{1};
bool Sfx::enabled_{true};
bool Sfx::soundsPaused_{};
VectorCont<SfxId> Sfx::pausedSounds_;

#if MANGL_CAPTURE_AUDIO
CaptureAudio* Sfx::capture_{};
#endif

void Sfx::initialize() {

#if MANGL_CAPTURE_AUDIO
    capture_ = MCaptureAudio::instance();
#endif

    setEnabled(settingEnabled.get(true));
    setVolume(settingVolume.get(1));
}

void Sfx::startAudio() {
}

void Sfx::stopAudio() {
    clearThread();

    engine_.release();
}

void Sfx::destroy() {
}

void Sfx::shutdownContextThread() {
    engine_.shutdownContext();
}

void Sfx::restoreContextThread() {
    engine_.restoreContext();
}


void Sfx::clearThread()
{
    {
        Lock lock{containerMutex_};

        // Release all the audio data
        for (auto &s: soundsById_) {
            engine_.release(s.second);
        }

        soundsById_.clear();
        soundsByFn_.clear();

        pausedSounds_.clear();
        soundsPaused_ = false;
    }

    engine_.clearErrors();
}

void Sfx::releaseBody(SfxId soundId)
{
    SfxContextPtr ctx{};

    {
        Lock lock{containerMutex_};

        auto it = soundsById_.find(soundId);
        if (it == soundsById_.end())
            return;

        ctx = it->second;
    }

    engine_.release(ctx);

    {
        Lock lock{containerMutex_};
        soundsById_.erase(soundId);
        soundsByFn_.erase(ctx->filename);
    }
}


void Sfx::loadThread(SfxId soundId, StrView filename)
{
    auto fnv = apl::extractFileName(filename); // keep only the file name
    
    String fn{fnv};

    // TODO: container mutex

    // If the sound is already loaded, check if we want to load a different sound
    bool needRelease{false};

    { 
        Lock lock{containerMutex_};

        if (auto soundIt = soundsById_.find(soundId); soundIt != soundsById_.end()) {
            if (soundIt->second->filename == fn)  // Ignore duplicate calls
                return;
            MDebugFunc("Loading same Id from different file: %d: %s -> %s", soundId, soundIt->second->filename, fn);
            needRelease = true;
            //releaseId = soundId;
            //releaseBody(soundId);
        }
    }

    if (needRelease)
        releaseBody(soundId);

    // Check if the sound with such file name was already loaded
    // Just reuse the context. Nothing to do
    { 
        Lock lock{containerMutex_};

        if (auto it = soundsByFn_.find(fn); it != soundsByFn_.end()) {
            //soundInsertHint_ = soundsById_.emplace_hint(soundInsertHint_, id, soundsById_[it->second]);
            return;
        }
    }

    auto ctx = makeSfxContextPtr();

    ctx->filename = fn;

    if (!engine_.load(ctx, filename)) {
#if _DEBUG
        mthrowError("SoundFX: Cannot load: %s", fn);
#else
        return;
#endif
    }

    // TODO: bring back hint
    //soundInsertHint_ = soundsById_.emplace_hint(soundInsertHint_, id, ctx);
    { 
        Lock lock{containerMutex_};
        soundsById_.emplace(soundId, ctx);
    }

    //return this;
}

// Slow operation
SfxId Sfx::findByFn(StrView path)
{
    auto fn = apl::extractFileName(path); // keep only the file name

    Lock lock{containerMutex_};

    if (auto it = soundsByFn_.find(fn); it != soundsByFn_.end())
        return it->second;

    return 0;
}

void Sfx::playThread(SfxId id, Real volume) 
{
    SfxContextPtr ctx{};

    { 
        Lock lock{containerMutex_};

        auto it = soundsById_.find(id);

        if (it == soundsById_.end()) {
            MDebugFunc("Sound resource not found: %d", id);
            return;
        }

        ctx = it->second;
    }


    engine_.play(ctx, volume >= 0 ? volume : volume_);

#if MANGL_CAPTURE_AUDIO
    capture_->capture(s->second.filename);
#endif
}

void Sfx::setVolume(Real volume)
{
    volume_ = volume;

    //for ( auto &s : sounds_ )
    //	device_.setVolume ( s.second.engineContext, volume ) ;
}

void Sfx::setVolumeBody(SfxId soundId, Real volume)
{
    SfxContextPtr ctx{};

    {
        Lock lock{containerMutex_};

        auto it = soundsById_.find(soundId);

        if (it == soundsById_.end()) {
            MDebugFunc("Sound not found: %d", soundId);
            return;
        }

        ctx = it->second;
    }

    engine_.setVolume(ctx, volume);
}


//
// pausing/resuming
//
void Sfx::pauseSounds()
{
    //throw std::runtime_error("deal with it later") ;

    /*
    if ( soundsPaused_ )
        return this ;
    pausedSounds_.reserve(sounds_.size()) ;

    pausedSounds_.clear() ;
    pausedSounds_.reserve(sounds_.size()) ;
    for ( auto s_it = sounds_.begin(), s_end = sounds_.end() ; s_it != s_end ; ++s_it ) {
        pausedSounds_.resize(pausedSounds_.size()+1) ;
        auto &ps = pausedSounds_.back() ;

        ps.soundId = s_it->first ;
        ps.filename = s_it->second.filename ;

        device_.release(s_it->second.sound) ;
    }

    sounds_.clear() ;
    soundsPaused_ = true ;
     */
}

void Sfx::resumeSounds()
{
    //throw std::runtime_error("deal with it later") ;

    /*
    if ( !soundsPaused_ )
        return this ;
    for ( auto &s: pausedSounds_ )
        load ( s.soundId, s.filename ) ;
    pausedSounds_.clear() ;
    soundsPaused_ = false ;
     */
}

//
// User setting updates
//

static Timestamp lastUserSettingPlayed_{};
static bool userSettingPlayScheduled_{};
static Timestamp userSettingPlayTimestamp_{};
static Real userSettingPlayVolume_{};
static SfxId userSettingPlaySfxId_{};

void Sfx::updateEnabledSetting(bool enabled) {
    auto sampleId = Framework::appParams_.settingsVolumeSound;
    MDebugTrapAssert(sampleId != 0);
    updateUserEnabledSetting(enabled, sampleId);
}

void Sfx::updateUserVolumePosition(Real volume) {
    auto sampleId = Framework::appParams_.settingsVolumeSound;
    MDebugTrapAssert(sampleId != 0);
    updateUserVolumePositionSample(volume, sampleId);
}

void Sfx::updateUserVolumeEnd(Real volume) {
    auto sampleId = Framework::appParams_.settingsVolumeSound;
    MDebugTrapAssert(sampleId != 0);
    updateUserVolumeEndSample(volume, sampleId);
}




void Sfx::processUserSettingsVolumeThread(Real volume, SfxId sampleId) {
    auto timestamp = Framework::timestampAtomic();
    auto delta = timestamp - lastUserSettingPlayed_;

    userSettingPlayVolume_ = volume;
    //setVolumeBody(volume);

    if (delta >= userSettingPlayGap_) {
        lastUserSettingPlayed_ = timestamp;
        playThread(sampleId);
        return;
    }

    // Schedule the sound play
    if (!userSettingPlayScheduled_) {
        userSettingPlayScheduled_ = true;
        userSettingPlayTimestamp_ = timestamp + userSettingPlayGap_ - delta;
        userSettingPlaySfxId_ = sampleId;
    }

}


void Sfx::processUserSettingsVolumeEndThread(Real volume, SfxId sampleId) {

    playThread(sampleId);

    userSettingPlayScheduled_ = false;
}



//
// Scheduled playing
//
void Sfx::playDelayedThread(SfxId soundId, Timestamp when, Real volume)
{
    auto &it = scheduledItems_.emplace_back();
    it.valid = true;
    it.soundId = soundId;
    it.playTimestamp = Framework::timestampAtomic() + when;
    it.volume = volume;
}

void Sfx::processTimer(Timestamp timestamp)
{
    if (!scheduledItems_.empty()) {
        auto found = false;
        for (auto &i: scheduledItems_) {
            if (!i.valid)
                continue;
            found = true;

            if (timestamp >= i.playTimestamp) {
                playThread(i.soundId, i.volume);
                i.valid = false;
            }
        }
        if (!found)
            scheduledItems_.clear();
    }

    if (userSettingPlayScheduled_ && timestamp >= userSettingPlayTimestamp_) {
        userSettingPlayScheduled_ = false;

        //setVolume(userSettingPlayVolume_);
        playThread(userSettingPlaySfxId_, userSettingPlayVolume_);
    }
}

}  // namespace mangl
