/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <mangl/game.h>

#include <apeal/singleton.hpp>


using namespace mangl;

class Leaderboards: public apl::Singleton<Leaderboards> {
public:
    Leaderboard marathon{"marathon", "Marathon"};
};

