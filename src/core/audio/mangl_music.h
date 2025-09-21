/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_common.h"
#include "../mangl_storage.h"
#include "mangl_audio_system.h"

#include "../mangl_dispatch.h"

namespace mangl {

struct MusicTrackContext: apl::NonCopyable {
    MusicTrackId trackId;
    String filename;
    Timestamp duration{};
    std::shared_ptr<void> engineContext; // anything the engine wants to attach
    MusicTrackContext(MusicTrackId i, StrView fn): trackId{i}, filename{fn} {}
};

using MusicTrackContextP = MusicTrackContext*;

using MusicTrackContextPtr = std::shared_ptr<MusicTrackContext>;

template <class... Args>
MusicTrackContextPtr makeMusicTrackContextPtr(Args&&... args) {
    return std::make_shared<MusicTrackContext>(std::forward<Args>(args)...);
}

class MusicEngineBase {
public:
    virtual ~MusicEngineBase() = default;

    virtual void initialize(){}
    virtual void shutdown(){}
    virtual void shutdownContext(){}
    virtual void restoreContext(){}

    virtual void play(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Real volume){}
    virtual void playCrossfade(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Timestamp gap, Real volume){}
    virtual void load(MusicTrackContextP track) {}

    virtual void setVolume(Real volume, Real ramp){}
    virtual void stop(Timestamp delay, Timestamp ramp){}

    virtual void pause(){}
    virtual void resume(){}

    virtual void processTimer(Timestamp timestamp){}

};


class Music {
    friend class AudioSystem;
    friend class MusicEngineDevice;
    friend class GameMusic;

    friend class MusicEngineAudioQueue;
    friend class MusicEngineMCI;
    friend class MusicEngineOboe;

    using Self = Music;
    using SelfP = Self*;

    using SelfRet = Music*;

public:
    using TrackId = MusicTrackId;
    using Callback = std::function<void (MusicTrackId)>;



    // User settings
    static Setting<bool> settingEnabled;
    static Setting<Real> settingVolume;


    //
    // Callbacks
    //
    static Callback onStart;  // Track finished playing
    static Callback onPause;  // Track paused playing
    static Callback onResume;  // Track resume playing
    static Callback onFinish;  // Track finished playing
    static Callback onStop;  // Track stopped playing

    static void clearCallbacks() noexcept {
        onStart = {};
        onPause = {};
        onResume = {};
        onFinish = {};
        onStop = {};
    }


    // Game Music callbacks
private:
    static Callback onGameMusicStart;  // Track finished playing
    static Callback onGameMusicPause;  // Track finished playing
    static Callback onGameMusicResume;  // Track finished playing
    static Callback onGameMusicFinish;  // Track finished playing
    static Callback onGameMusicStop;  // Track stopped playing

    static void clearGameMusicCallbacks() noexcept {
        onGameMusicStart = {};
        onGameMusicPause = {};
        onGameMusicResume = {};
        onGameMusicFinish = {};
        onGameMusicStop = {};
    }

    
public:
    static void processTrackStart(TrackId track) {
        Dispatch::asyncMain([track] {
            statusPlaying_ = true;
            statusPaused_ = false;
            if (onGameMusicStart) onGameMusicStart(track);
            if (onStart) onStart(track);
        });
    }
    
    static void processTrackPause(TrackId track) {
        Dispatch::asyncMain([track] {
            statusPaused_ = true;
            if (onGameMusicStart) onGameMusicStart(track);
            if (onStart) onStart(track);
        });
    }

    static void processTrackResume(TrackId track) {
        Dispatch::asyncMain([track] {
            statusPaused_ = false;
            if (onGameMusicStart) onGameMusicStart(track);
            if (onStart) onStart(track);
        });
    }


    static void processTrackStop(TrackId track) {
        Dispatch::asyncMain([track] {
            statusPlaying_ = false;
            statusPaused_ = false;
            if (onGameMusicFinish) onGameMusicFinish(track);
            if (onFinish) onFinish(track);
        });
    }

    static void processTrackFinish(TrackId track) {
        Dispatch::asyncMain([track] {
            statusPlaying_ = false;
            statusPaused_ = false;
            if (onGameMusicFinish) onGameMusicFinish(track);
            if (onFinish) onFinish(track);
        });
    }


private:
    // Music status
    static bool userEnabled_; //{true};  // Music enabled in general
    static bool userStopped_; //{false};  // stop() was called, but the music might still be playing and
                          //"onFinished" will be called
    static bool userPaused_;
    static Real userVolume_;  //{1};
    static Real userRamp_;    //{1};

    static bool statusPlaying_;
    static bool statusPaused_;

    using Mutex = std::mutex;
    using Lock = std::scoped_lock<Mutex>;

    static std::map<MusicTrackId, MusicTrackContextPtr> tracks_;
    static Mutex containerMutex_;
    //static MusicTrackContextPtr currentTrack_;
    static MusicTrackId currentTrackId_;

    static ScheduleTriggerTimed fadeScheduled_, stopScheduled_, playScheduled_;
    static Real playScheduleVolume_, playScheduleRamp_;
    static MusicTrackContextPtr playScheduledTrack_;
    static Real scheduledFadeVolume_, scheduledFadeRamp_;


    static void initialize();  // Called from the main thread
    static void startAudio();  // Called from the audio thread
    static void stopAudio();  // Called from the audio thread
    static void destroy();  // Called from the main thread

    // Implementations
    static void loadThread(MusicTrackId id, StrView filename);
    static void playThread(MusicTrackId id, Timestamp delay, Timestamp ramp, Real volume);
    static void playCrossfadeBody(MusicTrackId id, Timestamp delay, Timestamp ramp, Timestamp gap, Real volume);
    //static void playThread(MusicTrackId id);

    static void pauseBody();
    static void resumeBody();

    static void stopBody(Timestamp when, Timestamp ramp);

    static void setVolumeBody(Real volume, Real ramp);



    template <typename TrackT>
    static void schedulePlay(TrackT trackId, Timestamp when, Real ramp, Real volume) {
        currentTrackId_ = static_cast<TrackId>(trackId);

        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::MusicPlay,
            .id = static_cast<CommonId>(trackId),
            .timestamp = when,
            .volume = volume >= 0 ? volume : userVolume_,
            .ramp = ramp,
        });
    }

    template <typename TrackT>
    static void schedulePlayCrossfade(TrackT trackId, Timestamp delay, Real ramp, Timestamp gap, Real volume) {
        currentTrackId_ = static_cast<TrackId>(trackId);

        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::MusicPlayCrossfade,
            .id = static_cast<CommonId>(trackId),
            .timestamp = delay,
            .volume = volume >= 0 ? volume : userVolume_,
            .ramp = ramp,
            .real = gap,
        });
    }




public:
    static void enable(bool flag = true);
    static void disable() { enable(false); }
    static void setEnabled(bool flag = true) { enable(flag); }

    [[nodiscard]] static auto isEnabled() noexcept { return userEnabled_; }
    [[nodiscard]] static auto enabled() noexcept { return userEnabled_; }
    
    
    static void shutdownContext();
    static void restoreContext();

    static void setVolume(Real volume, Real ramp = 0) {
        userVolume_ = volume;
        userRamp_ = ramp;

        if (!userEnabled_)
            return;

        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::MusicSetVolume,
            .volume = volume, 
            .ramp = ramp,
        });
    }

    [[nodiscard]] static auto volume() noexcept { return userVolume_; }
    [[nodiscard]] static auto getVolume() noexcept { return userVolume_; }


    template <typename TrackT>
    static void load(TrackT trackId, StrView filename) {
        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::MusicLoad,
            .id = static_cast<CommonId>(trackId),
            .str = filename, 
        });
    }

    template <typename TrackT>
    static void play(TrackT trackId, Timestamp ramp = 0) {
        userStopped_ = false;
        userPaused_ = false;

        if (!userEnabled_)
            return;
        schedulePlay(trackId, 0, ramp, userVolume_);
    }

    template <typename TrackT>
    static void playDelayed(TrackT trackId, Timestamp delay, Timestamp ramp = 0) {
        userStopped_ = false;
        userPaused_ = false;
        if (!userEnabled_)
            return;
        schedulePlay(trackId, delay, ramp, userVolume_);
    }


    template <typename TrackT>
    static void playCrossfade(TrackT trackId, Timestamp ramp, Timestamp gap = 0) {
        userStopped_ = false;
        if (!userEnabled_)
            return;
        schedulePlayCrossfade(trackId, 0, ramp, gap, userVolume_);
    }

    template <typename TrackT>
    static void playCrossfadeDelayed(TrackT trackId, Timestamp delay, Timestamp ramp, Timestamp gap = 0) {
        userStopped_ = false;
        if (!userEnabled_)
            return;
        schedulePlayCrossfade(trackId, delay, ramp, gap, userVolume_);
    }



    static void pause() {
        if (!userEnabled_)
            return;
        userPaused_ = true;
        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::MusicPause,
        });
    }

    static void resume() {
        userPaused_ = false;
        if (!userEnabled_)
            return;

        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::MusicResume,
        });
    }

    static void stop(Timestamp ramp = 0) {
        userStopped_ = true;
        userPaused_ = false;

        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::MusicStop,
            .ramp = ramp,
        });
    }

    static void stopDelayed(Timestamp delay, Timestamp ramp = 0) noexcept {
        userStopped_ = true;
        userPaused_ = false;

        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::MusicStop,
            .timestamp = delay,
            .ramp = ramp,
        });
    }

    [[nodiscard]] static bool stopped() noexcept { return userStopped_; }

    [[nodiscard]] static bool playing() noexcept { return statusPlaying_; }
    [[nodiscard]] static bool paused() noexcept { return statusPaused_; }

    [[nodiscard]] static TrackId currentId() noexcept {
        return currentTrackId_;
    }

    template <class TrackT>
    static StrView getFileName(TrackT trackId) {
        Lock lock{containerMutex_};
        auto it = tracks_.find(static_cast<TrackId>(trackId));
        if (it == tracks_.end())
            return {};
        return it->second->filename;
    }

    //[[nodiscard]] static TrackId firstId() noexcept {
    //    Lock lock{containerMutex_};
    //    return tracks_.empty() ? 0 : tracks_.begin()->first;
    //}
    //[[nodiscard]] static TrackId lastId() noexcept {
    //    Lock lock{containerMutex_};
    //    return tracks_.empty() ? 0 : tracks_.rbegin()->first;
    //}

    //void onNotify(); // Need to remove. it's for windows. come up with something better




private:
    //
    // Commonly used sequence of sound settings
    //
    constexpr static Timestamp userSettingPlayGap_ = 0.1;

    static void updateUserVolumePlay(Real volume, TrackId sampleId);

public:

    template <typename TrackIdT>
    static void updateUserEnabledSetting(bool enabled, TrackIdT sampleId) {
        settingEnabled = enabled;
        setEnabled(enabled);
        play(sampleId);
    }

    template <class TrackIdT>
    static void updateUserVolumePosition(Real volume, TrackIdT sampleId) {
        setVolume(volume);
        updateUserVolumePlay(volume, (MusicTrackId)sampleId);
    }

    template <class TrackIdT>
    static void updateUserVolumeEnd(Real volume, TrackIdT sampleId) {
        setVolume(volume);
        settingVolume = volume;
        updateUserVolumePlay(volume, (MusicTrackId)sampleId);
    }



protected:
    static void processTimer(Timestamp);

};

} // namespace mangl
