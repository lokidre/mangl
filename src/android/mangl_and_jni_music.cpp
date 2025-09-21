/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_jni_music.h"

#if MANGL_MUSIC_JNI

#include "mangl_and_main_engine.h"

#include "../core/mangl_environment.h"


namespace mangl {

static jmethodID jm_musicPlay{}, jm_musicPlayCrossfade{}, jm_musicStop{};
static jmethodID jm_musicIsPlaying{}, jm_musicIsOtherAudioPlaying;
static jmethodID jm_musicPause{}, jm_musicResume{};
static jmethodID jm_musicSetVolume{};


void Jni::registerMusicMethods() {
    activityMethods_.insert(activityMethods_.end(), {
            {&jm_musicPlay, "musicPlay", "(Ljava/lang/String;IFFF)V"},
            {&jm_musicPlayCrossfade, "musicPlayCrossfade", "(Ljava/lang/String;IFFFF)V"},
            {&jm_musicStop, "musicStop", "(FF)V"},
            {&jm_musicIsPlaying, "musicIsPlaying", "()Z"},
            {&jm_musicIsOtherAudioPlaying, "musicIsOtherAudioPlaying", "()Z"},
            {&jm_musicPause, "musicPause", "()V"},
            {&jm_musicResume, "musicResume", "()V"},
            {&jm_musicSetVolume, "musicSetVolume", "(FF)V"},
    });
}


bool AudioSystem::detectOtherAudioPlaying() {
    // TODO: implement Dispatch::syncMain
    return false;
    //return Jni::callBoolMethodThread(jm_musicIsOtherAudioPlaying);
}


void MusicEngineJni::play(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Real volume) {
    Dispatch::asyncMain([track, delay, ramp, volume] {
        JniString jtrack{track->filename};
        Jni::callActivityVoidMethod(jm_musicPlay,
                                  jtrack.jstr,
                                  (jint) track->trackId,
                                  (jfloat) delay,
                                  (jfloat) ramp,
                                  (jfloat) volume);
    });
}

void MusicEngineJni::playCrossfade(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Timestamp gap, Real volume) {
    Dispatch::asyncMain([track, delay, ramp, gap, volume] {
        JniString jtrack{track->filename};
        Jni::callActivityVoidMethod(jm_musicPlayCrossfade,
                                  jtrack.jstr,
                                  (jint) track->trackId,
                                  (jfloat) delay,
                                  (jfloat) ramp,
                                  (jfloat) gap,
                                  (jfloat) volume);
    });
}

void MusicEngineJni::setVolume(Real volume, Real ramp) {
    Dispatch::asyncMain([volume, ramp] {
        Jni::callActivityVoidMethod(jm_musicSetVolume, (jfloat) volume, (jfloat) ramp);
    });
}

void MusicEngineJni::stop(Real volume, Real ramp) {
    Dispatch::asyncMain([volume, ramp] {
        Jni::callActivityVoidMethod(jm_musicStop, (jfloat) volume, (jfloat) ramp);
    });
}

void MusicEngineJni::pause() {
    Dispatch::asyncMain([] {
        Jni::callActivityVoidMethod(jm_musicPause);
    });
}

void MusicEngineJni::resume() {
    Dispatch::asyncMain([] {
        Jni::callActivityVoidMethod(jm_musicResume);
    });
}

void MusicEngineJni::shutdownContext() {
    MDebugFunc();
//    Jni::callActivityVoidMethod(jm_musicStop, 0, 0);

    Dispatch::asyncMain([] {
        Jni::callActivityVoidMethod(jm_musicStop, 0, 0);
    });
}

void MusicEngineJni::restoreContext() {
}

void MusicEngineJni::shutdown() {
    Dispatch::asyncMain([] {
        Jni::callActivityVoidMethod(jm_musicStop, 0, 0);
    });
}



}  // namesapce mangl

#endif

extern "C"
JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityMusic_nOnMusicState__II(JNIEnv* env,
                                                    jclass clazz,
                                                    jint trackId,
                                                    jint state) {
    using namespace mangl;

    switch (state) {
    case 1:  Music::processTrackStart(trackId); break;
    case 2:  Music::processTrackStop(trackId); break;
    case 3:  Music::processTrackFinish(trackId); break;
    default:
        break;
    }
    // TODO: implement nOnMusicState()
}
