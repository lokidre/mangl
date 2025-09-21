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

namespace mangl {

struct SfxContext: apl::NonCopyable {
    String filename;
    std::shared_ptr<void> engineContext; // anything the engine wants to attach
};

using SfxContextPtr = std::shared_ptr<SfxContext>;

template <class... Args>
auto makeSfxContextPtr(Args&&... args) {
    return std::make_shared<SfxContext>(std::forward<Args>(args)...);
}


class Sfx {
    friend class AudioSystem;

    using Self = Sfx;
    using SelfRet = Self*;

public:
    using Id = SfxId;
    using SoundsContainer = KeyValCont<SfxId, SfxContextPtr>;

    static Setting<bool> settingEnabled;
    static Setting<Real> settingVolume;

private:
    using Mutex = std::mutex;
    using Lock = std::scoped_lock<Mutex>;

    static SoundsContainer soundsById_;
    static StrValCont<SfxId> soundsByFn_;
    static Mutex containerMutex_;

    static Real volume_;

//    static SfxEngine* engine_;
    static bool enabled_;

    static bool soundsPaused_;
    static VectorCont<Id> pausedSounds_;



    static void initialize();
    static void startAudio();  // Should be called from the audio thread
    static void stopAudio();  // Should be called from the audio thread
    static void destroy();

    static void shutdownContextThread();
    static void restoreContextThread();

    static void loadThread(SfxId soundId, StrView filename);
    static void releaseBody(SfxId soundId);
    static void playThread(SfxId soundId, Real volume = -1);
    static void playDelayedThread(SfxId soundId, Timestamp when, Real volume = -1);
    static void clearThread();
    static void setVolumeBody(SfxId soundId, Real volume);


public:
    static void clear() {
        AudioSystem::scheduleCommandItem({.cmd = AudioCommand::SfxClear});
    }
    //static void release();

    static void shutdownContext() {
        AudioSystem::scheduleCommandItem({.cmd = AudioCommand::SfxShutdownContext});
    }

    static void restoreContext() {
        AudioSystem::scheduleCommandItem({.cmd = AudioCommand::SfxRestoreContext});
    }


    // Loading the sounds
    /*
        Loading the sound with the same id and same filename will be ignored
        Loading the same id with different file name will produce warning on the console, but
       permitted
        TODO: test
            Loading new id with already loaded file is permitted and optimized.
            The memory from previous loaded sound will be used.
    */
    template <typename SfxT>
    static void load(SfxT soundId, StrView filename) {
        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::SfxLoad,
            .id = static_cast<CommonId>(soundId),
            .str = filename,
        });
        //loadThread(static_cast<SfxId>(soundId), filename);
    }


    static auto setEnabled(bool flag = true) { enabled_ = flag; }
    static bool isEnabled() noexcept { return enabled_; }

    static auto enable(bool flag = true) { enabled_ = flag; }
    static bool enabled() { return enabled_; }

    static void setVolume(Real volume);
    static void setVolume(SfxId soundId, Real volume) {
        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::SfxSetVolume,
            .id = static_cast<CommonId>(soundId),
            .volume = volume,
        });
    }

    static void release(SfxId soundId) {
        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::SfxRelease,
            .id = static_cast<CommonId>(soundId),
        });
    }

    static SfxId findByFn(StrView filename); // finds the sound id for particular filename


    template <typename SfxT>
    static void play(SfxT soundId, Real volume = -1) {
        if (!enabled_ || soundsPaused_)
            return;

        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::SfxPlay,
            .id = static_cast<CommonId>(soundId),
            .volume = volume >= 0 ? volume : volume_,
        });

        //playThread(static_cast<SfxId>(id), volume);
    }

    static SfxId maxId() noexcept {
        Lock lock{containerMutex_};

        SfxId max{};
        for (auto& s: soundsById_)
            if (s.first > max)
                max = s.first;
        return max;
    }

    static SfxId firstId() noexcept { 
        Lock lock{containerMutex_};

        return soundsById_.empty() ? 0 : soundsById_.begin()->first; 
    }


    // standby mode
    static void pauseSounds();
    static void resumeSounds();

    //
    // Scheduled playing
    //
    template <class SfxIdT>
    static void playDelayed(SfxIdT soundId, Timestamp when, Real volume = -1) {
        if (!enabled_ || soundsPaused_)
            return;

        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::SfxPlayDelayed,
            .id = static_cast<CommonId>(soundId),
            .timestamp = when,
            .volume = volume >= 0 ? volume : volume_,
        });

    }




private:
    //
    // Commonly used sequence of sound settings
    //
    constexpr static Timestamp userSettingPlayGap_ = 0.5;

    static void scheduleUserSettingsVolume(Real volume, SfxId sampleId) {
        setVolume(volume);

        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::SfxUserSettingsVolume,
            .id = static_cast<CommonId>(sampleId),
            .volume = volume,
        });
    }


    static void processUserSettingsVolumeThread(Real volume, SfxId sampleId);
    static void processUserSettingsVolumeEndThread(Real volume, SfxId sampleId);


public:
    template <class SfxIdT>
    static void updateUserEnabledSetting(bool enabled, SfxIdT sampleId) {
        settingEnabled = enabled;
        setEnabled(enabled);
        play(sampleId);
    }

    static void updateEnabledSetting(bool enabled);


    template <class SfxIdT>
    static void updateUserVolumePositionSample(Real volume, SfxIdT sampleId) {
        setVolume(volume);

        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::SfxUserSettingsVolume,
            .id = static_cast<CommonId>(sampleId),
            .volume = volume,
        });
    }

    static void updateUserVolumePosition(Real volume);


    template <class SfxIdT>
    static void updateUserVolumeEndSample(Real volume, SfxIdT sampleId) {
        setVolume(volume);
        settingVolume = volume;

        AudioSystem::scheduleCommandItem({
            .cmd = AudioCommand::SfxUserSettingsVolumeEnd,
            .id = static_cast<CommonId>(sampleId),
            .volume = volume,
        });
    }

    static void updateUserVolumeEnd(Real volume);




protected:
    // Should be called from the audio thread
    static void processTimer(Timestamp);

public:
    struct ScheduleItem {
        bool valid{};
        Timestamp playTimestamp{};
        Id soundId{};
        Real volume{};
    };

private:
    static std::vector<ScheduleItem> scheduledItems_;


#if MANGL_CAPTURE_AUDIO
    static CaptureAudio* capture_;
#endif
};

//using SfxP = Sfx*;

} // namespace mangl
