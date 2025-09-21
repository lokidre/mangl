/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
// *** Automatically Generated *** DO NOT EDIT ***
// * Generated Date: 2024-10-14
#pragma once

#include <mangl/resources.h>
#include <mangl/audio.h>

namespace mangl {

enum class MusicRes: int {
    MInvalid = 0,
    MBegin = 1,
    MFirst = MBegin,

    NightInKyoto = MFirst,
    Welcome = NightInKyoto,
    NoTimeToDie,
    NostalgicMoment,
    BoundAndDetermined,
    DistantHorizon,
    EyesAhead,
    MountainsOfMars,
    UnderCover,
    UnravelingTheMystery,

    MLast = UnravelingTheMystery,
    MEnd,
};

constexpr bool operator !(MusicRes e) noexcept { return e == static_cast<MusicRes>(0); }

enum class SoundsRes: int {
    MInvalid = 0,
    MBegin = 1,
    MFirst = MBegin,

    Button = MFirst,
    Level,
    Drop,
    Rotate,
    LineClear,
    LineClear2,
    LineClear3,
    Countdown1,
    Countdown2,
    Countdown3,
    Countdown4,
    Win,
    Move,
    HighScore,
    MissionComplete,
    ConcreteCrush,
    GemCollect,
    Fireworks,
    GameOver,
    GameOver2,

    MLast = GameOver2,
    MEnd,
};

constexpr bool operator !(SoundsRes e) noexcept { return e == static_cast<SoundsRes>(0); }

inline void loadMusic() {
    using namespace mangl;
    using enum MusicRes;

    Music::load(NightInKyoto, Res::makeBundleMusicPath("night-in-kyoto-avbe-main-version-21302-01-57"));
    Music::load(NoTimeToDie, Res::makeBundleMusicPath("no-time-to-die-hartzmann-main-version-9251-02-01"));
    Music::load(NostalgicMoment, Res::makeBundleMusicPath("nostalgic-moment-monument-music-main-version-21355-03-05"));
    Music::load(BoundAndDetermined, Res::makeBundleMusicPath("music_bound_and_determined"));
    Music::load(DistantHorizon, Res::makeBundleMusicPath("music_distant_horizon"));
    Music::load(EyesAhead, Res::makeBundleMusicPath("music_eyes_ahead"));
    Music::load(MountainsOfMars, Res::makeBundleMusicPath("music_the_mountains_of_mars"));
    Music::load(UnderCover, Res::makeBundleMusicPath("music_under_cover"));
    Music::load(UnravelingTheMystery, Res::makeBundleMusicPath("music_unraveling_the_mystery"));
}

inline void loadSfx() {
    using namespace mangl;
    using enum SoundsRes;

    Sfx::load(Button, Res::makeBundleSoundPath("computerkeyboardtypingsingle_1_51195"));
    Sfx::load(Level, Res::makeBundleSoundPath("science_fiction_magical_wand_wave_pixie_dust_sparkle_shimmer_delay_01_43998"));
    Sfx::load(Drop, Res::makeBundleSoundPath("movie_things_falling_down_crumble_56719"));
    Sfx::load(Rotate, Res::makeBundleSoundPath("sound_rotate"));
    Sfx::load(LineClear, Res::makeBundleSoundPath("crashcupboard_bw.17164"));
    Sfx::load(LineClear2, Res::makeBundleSoundPath("crashglass_bw.17239"));
    Sfx::load(LineClear3, Res::makeBundleSoundPath("crashcrtmonitor_bw.60466"));
    Sfx::load(Countdown1, Res::makeBundleSoundPath("countdown_4"));
    Sfx::load(Countdown2, Res::makeBundleSoundPath("countdown_3"));
    Sfx::load(Countdown3, Res::makeBundleSoundPath("countdown_2"));
    Sfx::load(Countdown4, Res::makeBundleSoundPath("countdown_1"));
    Sfx::load(Win, Res::makeBundleSoundPath("success02"));
    Sfx::load(Move, Res::makeBundleSoundPath("foley_wood_stick_whip_whoosh_01_26769"));
    Sfx::load(HighScore, Res::makeBundleSoundPath("sound_high_score"));
    Sfx::load(MissionComplete, Res::makeBundleSoundPath("success02"));
    Sfx::load(ConcreteCrush, Res::makeBundleSoundPath("bone_crush_01"));
    Sfx::load(GemCollect, Res::makeBundleSoundPath("nicechime"));
    Sfx::load(Fireworks, Res::makeBundleSoundPath("fireworks_1"));
    Sfx::load(GameOver, Res::makeBundleSoundPath("outofluck"));
    Sfx::load(GameOver2, Res::makeBundleSoundPath("game_over"));
}

}  // namespace mangl
