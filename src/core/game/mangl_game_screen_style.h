/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../styles/mangl_styles_button.h"

#include "mangl_screen_background_style.h"

namespace mangl {

struct GameScreenTitleStyles {
    TextStyle text;
    Cord width{};
    Cord height{};
    Margins margins{};
};

struct GameScreenToolButtonStyles {
    ButtonStyle button;
    Point location;
    Point titleOffset;
    SlotVoid onPush;
};

struct GameScreenStyles {
    GameScreenBackgroundStyles background{};

    GameScreenTitleStyles title{
        .text = {.color = ThemeColors::TextPrimary, .size = 30},
        .height = 40,
        .margins = {.t = 4},
    };

    GameScreenTitleStyles homeTitle{
        .text = {.color = ThemeColors::TextPrimary, .size = 40},
        .height = 90,
        .margins = {.t = 20},
    };


    // Screen big select button
    ButtonStyle screenSelectButton{.size = {280, 40}};
    ButtonStyle screenChoiceButton{.size = {120, 40}};
    ButtonStyle screenToolButton{.size = {30, 30}};

    TextStyle screenButtonHint{.color = ThemeColors::PrimaryLight, .size = 12};



    GameScreenToolButtonStyles closeButton{
        .button = {
            .size = {20, 20},
        },
        .location = {1, 1},  // Top right
        .titleOffset = {-6, 0},
    };
    GameScreenToolButtonStyles backButton{
        .button = {
            .size = {20, 20},
        },
        .location = {0, 1},  // Top left
        .titleOffset = {6, 0},
    };

    //Color screenButtonExplTextColor{M::Black};

};

}

