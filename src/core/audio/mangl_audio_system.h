/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_audio_system_queue.h"

namespace mangl {

class AudioSystem {
public:
    static void initialize();
    static void destroy();

#if APEAL_OS_MAC
    static void initializePlatform();
    static void destroyPlatform();
#else
    static void initializePlatform(){}
    static void destroyPlatform(){}
#endif
    

    static bool otherAudioPlaying_;
    static bool interruptionActive_;

    static bool isInterruption() { return interruptionActive_; }


#if APEAL_OS_MAC || APEAL_OS_ANDROID
    static bool detectOtherAudioPlaying();
#else
    static bool detectOtherAudioPlaying() noexcept { return false; }
#endif

//    static void setOtherAudioPlaying(bool f);
    static bool otherAudioPlaying() noexcept { return otherAudioPlaying_; }

    static void onResignActive();
    static void onBecomeActive();


    static void scheduleCommandItem(AudioCommandQueueItem item);



private:
    //
    // AudioSystem thread
    //
    static void threadFunc();
    static void threadCommandProcess(const AudioCommandQueueItem& item, Timestamp timestamp);

#if APEAL_OS_MAC
    static void threadPlatformInit();
    static void threadPlatformDestroy();
    static void threadPlatformTimer(Timestamp);
#else
    static void threadPlatformInit(){}
    static void threadPlatformDestroy(){}
    static void threadPlatformTimer(Timestamp){}
#endif

};

} // namespace mangl
