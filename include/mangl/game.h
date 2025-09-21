/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../../src/core/mangl_types.h"


//#ifndef MANGL_CONSOLE

//
// Game Screens
//

// Main abstract app screen
#include "../../src/core/screens/mangl_app_screen.h"

#include "../../src/core/game/mangl_game_screen.h"
#include "../../src/core/game/mangl_screen_buttons_params.h"


// Common game screens
#include "../../src/core/screens/mangl_high_scores_screen.h"
#include "../../src/core/screens/mangl_intro_screen.h"
#include "../../src/core/screens/mangl_tutorial_screen.h"
#include "../../src/core/screens/mangl_game_play_screen.h"
#include "../../src/core/screens/mangl_settings_screen.h"


//
// Gui elements
//
#include "../../src/core/gui/mangl_screen_button.h"


//
// Leaderboards, scores
//
#include "../../src/core/game/mangl_leaderboard.h"

//
// Social components
//
#include "../../src/core/social/mangl_game_center.h"


//
// Game elements
//
//#include "../../src/core/world/mangl_game_character.h"
//#include "../../src/core/world/mangl_game_world.h"
//#include "../../src/core/world/mangl_game_object.h"

//#endif
