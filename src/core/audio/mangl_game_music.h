/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_music.h"
#include "mangl_game_music_params.h"
#include "mangl_game_music_dummy.h"



namespace mangl {
#if MANGL_MUSIC

class GameMusic {

public:
    // Default parameters
    static Timestamp defaultRamp;
    static Timestamp defaultDelay;

    static Timestamp defaultCrossfadeDelay;
    static Timestamp defaultCrossfadeRamp;


private: 
    static GameMusicConfig config_;
    static void setupContinue();

    //
    // Processing system events
    //

    // Called when the app comes to foreground
    static void processAppActivate();
    // Called when the app is going to the background
    static void processAppDeactivate();
    

    static void processTrackFinished(MusicTrackId  trackId);
    
public:
    // Processing full screen ads
    static void processAdWillPresentFullScreen();
    static void processAdDidDismissFullScreen();



public:
    //
    // Initialization
    //
    template <class TrackT>
    static void setup(TrackT firstTrack, TrackT lastTrack, const GameMusicConfigParams<TrackT>& config = {}) {
        config_.assign(firstTrack, lastTrack, config);
        setupContinue();
    }

    template <class MusicResT>
    static void setup(const GameMusicConfigParams<MusicResT>& config = {}) {
        setup(MusicResT::MFirst, MusicResT::MLast, config);
    }

    template <class MusicResT>
    static void setupAuto() {
        setup(MusicResT::MFirst, MusicResT::MLast, {});
    }


    //
    // Settings adjustments
    //
    static void updateUserEnabledSetting(bool enabled);
    static void updateEnabledSetting(bool enabled) { updateUserEnabledSetting(enabled); }
    static void updateUserVolume(Real volume);
    static void updateUserVolumeEnd(Real volume);

    static void updateUserVolumePosition(Real volume) {  // Obsolete
        updateUserVolume(volume);
    }


    //
    // Autoplaying
    // TODO: implement
    //
    static void pauseAutoplay(){}
    static void resumeAutoplay(){}


    //
    // Playing
    //

    // Starts with welcome and continues
    static void play();
    static void playNext();
    static void stop();
    static void fadeOut(Timestamp ramp = 0);  // Stop the music with default ramp

    static void playWelcome();
    static void playGameBegin();
    static void playGameEnd();


    //
    // Callbacks
    //
    static Music::Callback onTrackStart;
    static Music::Callback onTrackFinish;

    static void clearCallbacks() {
        onTrackStart = {};
        onTrackFinish = {};
    }

  
};

#else

class GameMusic: public GameMusicDummy{};

#endif

}  // namespace mangl


