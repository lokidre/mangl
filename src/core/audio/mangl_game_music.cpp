/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_game_music.h"

#include "../mangl_storage.h"
#include "../mangl_random.h"


namespace mangl {

#if MANGL_MUSIC

Timestamp GameMusic::defaultRamp = 3;
Timestamp GameMusic::defaultDelay = 2;

Timestamp GameMusic::defaultCrossfadeDelay = 3;
Timestamp GameMusic::defaultCrossfadeRamp = 4;


GameMusicConfig GameMusic::config_{};

Music::Callback GameMusic::onTrackStart{};
Music::Callback GameMusic::onTrackFinish{};



namespace mangl_impl_ {

using TrackId = MusicTrackId;
using TrackList = std::vector<MusicTrackId>;
using TrackPersist = Stat<TrackId>;


static TrackPersist lastWelcomeTrack_{"mangl_game_music_last_welcome_track", 0};
static TrackPersist lastGameBeginTrack_{"mangl_game_music_last_game_begin_track", 0};
static TrackPersist lastGameEndTrack_{"mangl_game_music_last_game_end_track", 0};


enum class MusicStage {
    Cycle,
    Welcome,
    GameBegin,
    GameEnd,
};


struct MusicState {

    //
    // Parameters
    //

    TrackList tracks{};  // All tracks
    TrackList welcomeTracks{};
    TrackList gameBeginTracks{};
    TrackList gameEndTracks{};

    Timestamp playDelay{};
    Timestamp playRamp{};
    Timestamp stopRamp{};

    Timestamp welcomeDelay{};
    Timestamp welcomeRamp{};

    Timestamp gameBeginDelay{};
    Timestamp gameBeginRamp{};

    Timestamp gameEndDelay{};
    Timestamp gameEndRamp{};

    Timestamp crossfadeDelay{};
    Timestamp crossfadeRamp{};


    MusicStage lastStage{MusicStage::Cycle};
    MusicStage currentStage{MusicStage::Cycle};

    Index currentTrackIndex{};
    Index currentListTrackIndex{};

    bool startedPlaying{false};



};


static MusicState st_{};

template <typename T>
static T assignPriority(T a, T b, T c = 0) {
    if (a != 0) return a;
    if (b != 0) return b;
    if (c != 0) return c;

    return 0;
}


static Index getTrackIndex(const TrackList& list, TrackId trackId) {
    Index idx{0};
    for (auto& t: list) {
        if (t == trackId)
            return idx;
        ++idx;
    }
    return -1;
}

static Index advanceListIndex(const TrackList& list, Index* index) {
    if (list.empty()) {
        MDebugTrap();
        return 0;
    }
    if (list.size() == 1)
        return 0;
    *index = *index + 1;
    if (*index >= std::ssize(list))
        *index = 0;
    return *index;
}



//
// Playing state
//
static bool otherAudioPlaying_{};



static TrackId pickNextTrackList(const TrackList& list, Index& listIndex) {
    if (list.empty()) {
        st_.currentStage = MusicStage::Cycle;
        advanceListIndex(st_.tracks, &st_.currentTrackIndex);
        return st_.tracks[st_.currentListTrackIndex];
    }

    if (list.size() == 1) {
        st_.currentListTrackIndex = getTrackIndex(list, list[0]);
        st_.currentStage = MusicStage::Cycle;
        advanceListIndex(st_.tracks, &st_.currentTrackIndex);
        return st_.tracks[st_.currentListTrackIndex];
    }

    advanceListIndex(list, &listIndex);
    return list[listIndex];
}


static TrackId pickPlayTrack(MusicStage stage) 
{
    TrackList* list{};
    
    switch (stage) {
    using enum MusicStage;
            
    case Welcome: list = &st_.welcomeTracks; break;
    case GameBegin: list = &st_.gameBeginTracks; break;
    case GameEnd: list = &st_.gameEndTracks; break;
    case Cycle: list = &st_.tracks; break;
    default:
        MDebugTrap();
        break;
    }

    st_.lastStage = st_.currentStage;
    st_.currentStage = stage;
    st_.startedPlaying = true;

    st_.currentListTrackIndex = 0;
    if (list->size() > 1)
        st_.currentListTrackIndex = M::random.index(list->size());

    auto trackId = list->at(st_.currentListTrackIndex);
    st_.currentTrackIndex = getTrackIndex(st_.tracks, trackId);
    return trackId;
}

static TrackId pickNextTrack() {
    if (st_.tracks.empty())
        return 0;
    
    TrackList* list{};
    bool switchList = false;
    
    switch (st_.currentStage) {
    using enum MusicStage;
            
    case Welcome: 
        list = &st_.welcomeTracks;
        switchList = true;
        break;
    case GameBegin: 
        list = &st_.gameBeginTracks;
        switchList = true;
        break;
    case GameEnd: 
        list = &st_.gameEndTracks;
        switchList = true;
        break;
    case Cycle: 
        list = &st_.tracks;
        break;
    default:
        MDebugTrap();
        break;
    }
    
    if (switchList) {
        st_.lastStage = st_.currentStage;
        st_.currentStage = MusicStage::Cycle;
//        list = &st_.tracks;
        st_.currentTrackIndex = M::random.index(list->size());
        st_.currentListTrackIndex = st_.currentTrackIndex;
        return st_.tracks[st_.currentTrackIndex];
    }
    

    // Here we are just picking next track
    st_.currentListTrackIndex++;
    if (st_.currentListTrackIndex >= std::ssize(*list))
        st_.currentListTrackIndex = 0;
    auto trackId = list->at(st_.currentListTrackIndex);
    st_.currentTrackIndex = getTrackIndex(*list, trackId);
    return trackId;
}


static void scheduleStop(Timestamp delay, Timestamp ramp) {
    if (delay == 0) {
        Music::stop(ramp);
    } else {
        Music::stopDelayed(delay, ramp);
    }

    st_.startedPlaying = true;
}


static void schedulePlay(TrackId track, Timestamp delay, Timestamp ramp) {

    if (!track) {
        MDebugTrap();
        return;
    }

    if (delay == 0) {
        Music::play(track, ramp);
    } else {
        Music::playDelayed(track, delay, ramp);
    }

    st_.startedPlaying = true;
}

static void schedulePlayStage(TrackId track) {
    Timestamp delay{}, ramp{};
 
    switch (st_.currentStage) {
    using enum MusicStage;
            
    case Welcome: delay = st_.welcomeDelay, ramp = st_.welcomeRamp; break;
    case GameBegin: delay = st_.gameBeginDelay, ramp = st_.gameBeginRamp; break;
    case GameEnd:  delay = st_.gameBeginDelay, ramp = st_.gameEndRamp; break;
    case Cycle:  delay = st_.crossfadeDelay, ramp = st_.crossfadeRamp; break;
    default:
        MDebugTrap();
        break;
    }
    
    // Imitate crossfade
    if (Music::playing()) {
        scheduleStop(0, ramp);
        delay += ramp;
    }

    schedulePlay(track, delay, ramp);
}




}  // namespace mangl_impl_



using namespace mangl_impl_;



void GameMusic::setupContinue() 
{
    // Clean up
    MusicState t{};
    std::swap(st_, t);


    Music::onGameMusicFinish = [](auto id){ 
        processTrackFinished(id); 
    };

    for (auto& ct: config_.tracksParam) {
        st_.tracks.push_back(ct);
    }

    if (config_.firstTrack && config_.lastTrack) {
        for (auto id = config_.firstTrack; id <= config_.lastTrack; ++id)
            st_.tracks.push_back(id);
    }

    for (auto& ct: config_.welcomeTracksParam) {
        st_.welcomeTracks.push_back(ct);
        if (getTrackIndex(st_.welcomeTracks, ct) < 0)
            st_.tracks.insert(st_.tracks.begin(), ct);
    }
    if (st_.welcomeTracks.empty())
        st_.welcomeTracks = st_.tracks;


    for (auto& ct: config_.gameBeginTracksParam) {
        st_.gameBeginTracks.push_back(ct);
        if (getTrackIndex(st_.gameBeginTracks, ct) < 0)
            st_.tracks.push_back(ct);
    }
    if (st_.gameBeginTracks.empty())
        st_.gameBeginTracks = st_.tracks;

    
    for (auto& ct: config_.gameEndTracksParam) {
        st_.gameEndTracks.push_back(ct);
        if (getTrackIndex(st_.gameEndTracks, ct) < 0)
            st_.tracks.push_back(ct);
    }
    if (st_.gameEndTracks.empty())
        st_.gameEndTracks = st_.tracks;


    // Timing
    //config_.startRamp = 
    st_.playDelay = config_.delay;

    st_.playRamp = assignPriority(config_.playRamp, config_.ramp, defaultRamp);
    st_.stopRamp = assignPriority(config_.stopRamp, config_.ramp, defaultRamp);

    st_.welcomeDelay = assignPriority(config_.welcomeDelay, st_.playDelay);
    st_.welcomeRamp = assignPriority(config_.welcomeRamp, st_.playRamp, defaultRamp);

    
    st_.gameBeginDelay = assignPriority(config_.gameStartDelay, st_.playDelay);
    st_.gameBeginRamp = assignPriority(config_.gameStartRamp, st_.playRamp, defaultRamp);

    st_.gameEndDelay = assignPriority(config_.gameStartDelay, st_.playDelay);
    st_.gameEndRamp = assignPriority(config_.gameStartRamp, st_.playRamp, defaultRamp);

    st_.crossfadeDelay = assignPriority(config_.crossfadeDelay, defaultCrossfadeDelay);
    st_.crossfadeRamp = assignPriority(config_.crossfadeRamp, defaultCrossfadeRamp);


}


void GameMusic::updateUserEnabledSetting(bool enabled) {
    Music::updateUserEnabledSetting(enabled, st_.tracks[0]);
}


void GameMusic::updateUserVolume(Real volume) {
    // Select track
    // TODO: current track
    Music::updateUserVolumePosition(volume, st_.tracks[0]);
}

void GameMusic::updateUserVolumeEnd(Real volume) {
    Music::updateUserVolumeEnd(volume, st_.tracks[0]);
}




void GameMusic::playWelcome() {
    auto trackId = pickPlayTrack(MusicStage::Welcome);
    schedulePlayStage(trackId);
}


void GameMusic::playGameBegin() {
    auto trackId = pickPlayTrack(MusicStage::GameBegin);
    schedulePlayStage(trackId);
}

void GameMusic::playGameEnd() {
    auto trackId = pickPlayTrack(MusicStage::GameEnd);
    schedulePlayStage(trackId);
}


void GameMusic::play() {
    if (!st_.startedPlaying) {
        playWelcome();
        return;
    }

    auto trackId = pickPlayTrack(MusicStage::Cycle);
    schedulePlayStage(trackId);
}


void GameMusic::playNext() {
    auto track = pickNextTrack();
    schedulePlayStage(track);
}

void GameMusic::stop() {
    scheduleStop(0, 0);
}

void GameMusic::fadeOut(Timestamp ramp) {
    scheduleStop(0, ramp == 0 ? st_.stopRamp : ramp);
}



//
// Handling music events
//


//
// Processing events
//

static bool adIsPresentingFullScreen_ = false;
static MusicTrackId adCurrentTrackId_ = {};

// Called when the app comes to foreground
void GameMusic::processAppActivate() {
    otherAudioPlaying_ = AudioSystem::detectOtherAudioPlaying();
}

// Called when the app is going to the background
void GameMusic::processAppDeactivate() {
}



void GameMusic::processTrackFinished(MusicTrackId trackId) {
    //if (GameMusicCycleMode::Cycle != config_.cycleMode)
    //    return;

    otherAudioPlaying_ = AudioSystem::detectOtherAudioPlaying();
    if (otherAudioPlaying_)
        return;

    if (Music::playing()) { // This shouldn't be happening
        MDebugTrap();
        return;
    }

    // If the result was user's stopping it
    if (!Music::settingEnabled() || !Music::enabled() || Music::stopped())
        return;
    
    if (adIsPresentingFullScreen_)
        return;


    auto nextTrack = pickNextTrack();
    schedulePlay(nextTrack, st_.crossfadeDelay, st_.crossfadeRamp);
}



// Ads processing
void GameMusic::processAdWillPresentFullScreen() {
    adIsPresentingFullScreen_ = Music::playing();
    if (adIsPresentingFullScreen_) {
        Music::stop(1);
        adCurrentTrackId_ = Music::currentId();
    }
    
}

void GameMusic::processAdDidDismissFullScreen() {
    if (adIsPresentingFullScreen_) {
        Music::play(adCurrentTrackId_, 2);
        adIsPresentingFullScreen_ = false;
    }
}



//void onBackgroundMusicFinished() {
//    if (!BackgroundMusic::autoContinue)
//        return;
//
//    if (AudioSystem::otherAudioPlaying())
//        return;
//
//    if (Music::playing()) // This shouldn't be happening
//        return;
//
//    // if our intention was to stop the music
//    if (!Music::settingEnabled || !Music::enabled() || Music::stopped())
//        return;
//
//    auto track = PentixMode::instance()->pickGameSoundtrack();
//
//    Music::playDelayed(track, musicCrossfadeDelay, musicCrossfadeRamp);
//}

#endif

}  // namespace mangl
