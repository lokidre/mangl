/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_common.h"
#include "../mangl_texture.h"

#include "mangl_screen_background_params.h"
#include "mangl_screen_buttons_params.h"
#include "mangl_screen_title_params.h"


namespace mangl {

struct GameScreenMapsParams {
    int totalMaps{};
    SizeI pageDims{4, 4};  // Buttons per page (columns and rows)
    std::function<ButtonP (int)> onCreateMapButton{};
    std::function<ButtonP ()> onCreateNextButton{};
    std::function<ButtonP ()> onCreatePrevButton{};
};

struct GameScreenParams {
    GameScreenType type;

    ScreenTitleBuildParams title;
    StrView titleText;
    Texture titleTexture;

    bool closeButton;
    bool backButton;

    std::vector<ScreenButtonParams> screenButtons;
    LayoutScreenButtonsParams screenButtonsLayout;

    GameScreenMapsParams maps;


    GameScreenToolButtonsParams toolButtons;


    SlotVoid onInit;
};

using GameScreenParamsA = const GameScreenParams&;

}  // namespace mangl

