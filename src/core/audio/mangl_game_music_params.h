/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"

namespace mangl {


enum class GameMusicCycleMode {
    Cycle = 0,
    Stop,
};

template <typename TrackT>
struct GameMusicConfigParams {
    // Providing all tracks
    TrackT firstTrack;
    TrackT lastTrack;
    std::initializer_list<TrackT> tracks;


    // Key even tracks
    TrackT welcomeTrack;  // If zero the first track is played
    std::initializer_list<TrackT> welcomeTracks;

    TrackT gameBeginTrack;  // if zero no track is played
    std::initializer_list<TrackT> gameBeginTracks;

    TrackT gameEndTrack;  // if zero no track is played, continues
    std::initializer_list<TrackT> gameEndTracks;


    // Cycleing mode
    GameMusicCycleMode cycleMode;


    // Timing events
    Timestamp delay;
    Timestamp ramp;
    Timestamp playRamp;
    Timestamp stopRamp;

    // Crossfade timing
    Timestamp crossfadeDelay;  // Delay of the next track 
    Timestamp crossfadeRamp;  // 

    // Welcome timing
    Timestamp welcomeDelay;
    Timestamp welcomeRamp;

    // Game timing
    Timestamp gameStartDelay;
    Timestamp gameStartRamp;

    Timestamp gameEndDelay;
    Timestamp gameEndRamp;

    // Callbacks (in case)
};


struct GameMusicConfig: public GameMusicConfigParams<MusicTrackId> {
    std::vector<MusicTrackId> tracksParam;
    std::vector<MusicTrackId> welcomeTracksParam;
    std::vector<MusicTrackId> gameBeginTracksParam;
    std::vector<MusicTrackId> gameEndTracksParam;


    template <typename TrackT>
    void assign(TrackT firstTrackArg, TrackT lastTrackArg, const GameMusicConfigParams<TrackT>& config) {

        // Clean up
        GameMusicConfig t{};
        std::swap(*this, t);

        using Id = MusicTrackId;


        firstTrack = static_cast<Id>(firstTrackArg);
        lastTrack = static_cast<Id>(lastTrackArg);

        auto copyParamsList = [](auto& source, auto* target) {
            for (auto& t: source) {
                target->push_back(static_cast<Id>(t));
            }
        };

        copyParamsList(config.tracks, &tracksParam);
        copyParamsList(config.welcomeTracks, &welcomeTracksParam);
        copyParamsList(config.gameBeginTracks, &gameBeginTracksParam);
        copyParamsList(config.gameEndTracks, &gameEndTracksParam);

        cycleMode = config.cycleMode;

        ramp = config.ramp;
        playRamp = config.playRamp;
        stopRamp = config.stopRamp;
        
        crossfadeDelay = config.crossfadeDelay;
        crossfadeRamp = config.crossfadeRamp;

        welcomeDelay = config.welcomeDelay;
        welcomeRamp = config.welcomeRamp;


        gameStartDelay = config.gameStartDelay;
        gameStartRamp = config.gameStartRamp;

        gameEndDelay = config.gameEndDelay;
        gameEndRamp = config.gameEndRamp;


    }
};




}  // namespace params
