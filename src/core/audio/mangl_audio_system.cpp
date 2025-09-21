/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_audio_system.h"

#include "../mangl_framework.h"
#include "mangl_sound.h"
#include "mangl_music.h"

#include <thread>

namespace mangl {

namespace mangl_impl_ {

using Mutex = std::mutex;
using Lock = std::scoped_lock<Mutex>;

static std::atomic_bool queueEmpty_{};  // Use atomic bool for speed
static Mutex queueMutex_{};
static apl::RingBuffer<AudioCommandQueueItem> queue_{};

static std::atomic_bool threadExit_{};


static bool activeMusicPlaying_{};
static bool activeMusicPaused_{};
static MusicTrackId activeMusicTrack_{};

}  // namespace mangl_impl_

using namespace mangl_impl_;

bool AudioSystem::otherAudioPlaying_{};
bool AudioSystem::interruptionActive_{};


//std::atomic_bool AudioSystem::queueEmpty_{};  // Use atomic bool for speed
//AudioSystem::Mutex AudioSystem::queueMutex_{};
//apl::RingBuffer<AudioCommandQueueItem> AudioSystem::queue_{};
//
//std::atomic_bool AudioSystem::threadExit_{};



void AudioSystem::initialize() {
    
    initializePlatform();

    Sfx::initialize();
    Music::initialize();

    std::thread th([] {
        threadFunc();
    });
    th.detach();
}


void AudioSystem::onBecomeActive()
{

    Sfx::restoreContext();
    Music::restoreContext();

    // Decide whether we need to resume playing the music
    if (AudioSystem::detectOtherAudioPlaying()) {
        Music::disable();
        return;
    }
    
    if (!Music::settingEnabled()) {
        Music::disable();
        return;
    }
    
    Music::enable();

    if (!activeMusicPlaying_ || !activeMusicTrack_)
        return;
    
    if (AudioSystem::isInterruption())
        return;
    
    // Start playing the soundtrack
    Music::play(activeMusicTrack_);
    

    // Handle the paused music case
    if (activeMusicPaused_) {
        Music::pause();
    }

//    // This must be called before any sound can be made
//    if (AudioSystem::detectOtherAudioPlaying()) {
//        AudioSystem::setOtherAudioPlaying(true);
//        Music::enable(false);
//    } else {
//        AudioSystem::setOtherAudioPlaying(false);
//
//        // Play last played audio
//        if (Music::settingEnabled() && Music::enabled() && musicPlaying_ && (!Music::playing() || Music::paused() || AudioSystem::isInterruption())) {
//
//            if (!musicPaused_) {
//                if (!Music::playing()) {
//                    if (musicCurrentId_ != 0)
//                        Music::play(musicCurrentId_);
//                } else {
//                    Music::resume();
//                }
//            }
//        }
//    }


}


void AudioSystem::onResignActive()
{
    // Remember the state of the playing music
    // But only if we were the ones playing the music
    if (!AudioSystem::detectOtherAudioPlaying()) {
        activeMusicPlaying_ = Music::playing();
        activeMusicTrack_ = Music::currentId();
        activeMusicPaused_ = Music::paused();
    }
    
    
    Sfx::shutdownContext();
    Music::shutdownContext();
}




//
// Audio thread
//

void AudioSystem::scheduleCommandItem(AudioCommandQueueItem item) {
    {
        Lock lock{queueMutex_};

        if (queue_.full()) {
            //MDebugFunc("Queue is full");
            queue_.reserve(queue_.max_size() * 2);
        }

        if (!item.str.empty()) {
            item.strValue.assign(item.str.data(), item.str.size());
        }

        queue_.emplace(std::move(item));
    }

    queueEmpty_ = false;
}


void AudioSystem::threadFunc() 
{
    //MDebugPrint("AudioSystem::Thread Id: %ul", M::debugGetCurrentThreadId());

    threadPlatformInit();

    Sfx::startAudio();
    Music::startAudio();

    AudioCommandQueueItem item{};


    while(!threadExit_) {
        
        try {
            //
            // Check for the commands in queue
            //

            auto timestamp = Framework::timestampAtomic();
            
            // Execute all the pending commands
            while (!queueEmpty_) {
                {
                    Lock lock{queueMutex_};
                    if (queue_.empty()) {
                        queueEmpty_ = true;
                        break;
                    }
                    
                    item = queue_.top();
                    queue_.pop();
                }
                
                // Execute the item command
                timestamp = Framework::timestampAtomic();
                threadCommandProcess(item, timestamp);
            }
            
            
            // Sound effects and music timers
            Sfx::processTimer(timestamp);
            Music::processTimer(timestamp);
            
            // Run platform code
            threadPlatformTimer(timestamp);
            
            // Wait a little
            //uint64_t delayAmount = fpsRecipTimer_ - elapsedInt;
            uint64_t delayAmount = 33; // Roughly 30 FPS
            std::this_thread::sleep_for(std::chrono::milliseconds(delayAmount));
        }
        catch(const std::exception& e) {
            // TODO: repot analythics
            MDebugFunc("Audio exception: %s", e.what());
            M::sleep(2);
        }
        catch(...) {
            MDebugFunc("Audio unhandled exception");
            M::sleep(2);
        }
    }

    Sfx::stopAudio();
    Music::stopAudio();

    threadPlatformDestroy();
}


void AudioSystem::threadCommandProcess(const AudioCommandQueueItem& item, Timestamp timestamp) {
    switch (item.cmd) {
    using enum AudioCommand;


    case SfxShutdownContext: Sfx::shutdownContextThread(); break;
    case SfxRestoreContext: Sfx::restoreContextThread(); break;

    case SfxLoad: Sfx::loadThread(item.id, item.strValue); break;
    case SfxClear: Sfx::clearThread(); break;

    case SfxPlay: Sfx::playThread(item.id, item.volume); break;
    case SfxPlayDelayed: Sfx::playDelayedThread(item.id, item.timestamp, item.volume); break;
    case SfxUserSettingsVolume: Sfx::processUserSettingsVolumeThread(item.volume, item.id); break;
    case SfxUserSettingsVolumeEnd: Sfx::processUserSettingsVolumeEndThread(item.volume, item.id); break;


    case MusicLoad: Music::loadThread(item.id, item.strValue); break;

    case MusicSetVolume: Music::setVolumeBody(item.volume, item.ramp); break;
    case MusicPlay: Music::playThread(item.id, item.timestamp, item.ramp, item.volume); break;
    case MusicPlayCrossfade: Music::playCrossfadeBody(item.id, item.timestamp, item.ramp, item.real, item.volume); break;

    //case MusicPlayDelayed: Music::playScheduleBody(item.id, item.timestamp, item.ramp, item.volume); break;

    case MusicPause: Music::pauseBody(); break;
    case MusicResume: Music::resumeBody(); break;

    case MusicStop: Music::stopBody(item.timestamp, item.ramp); break;

    default:
        MDebugTrap();
        MDebugFunc("Warning: Unprocessed Audio Thread Command: %d", (int)item.cmd);
        break;
    }
}


}  // namespace mangl

