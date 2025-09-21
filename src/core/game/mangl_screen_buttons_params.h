/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_texture.h"
#include "../widgets/mangl_button.h"
#include "../widgets/mangl_label.h"

namespace mangl {

struct ScreenButtonParams {
    String title;

    Texture decorTex{};
    String suffixText;

    std::vector<String> expl;
    Size size{};
    Margins margins{};
    SlotVoid onPush{};


    //
    // Runtime information
    //
    struct ExplContext {
        String text;

        // Runtime information
        Point pos{};
        LabelP label{};
    };

    std::vector<ExplContext> rtExplanation;
    Size extent{};

    Cord explTextSize{};

    Texture buttonTexture{};

    Rect contentsFrame{}, totalFrame{};

    ButtonP button{};
};

struct LayoutScreenButtonsParams {
    Margins margins;
};
using LayoutScreenButtonsParamsA = const LayoutScreenButtonsParams&;


struct ScreenButtonsParams: public std::vector<ScreenButtonParams> {
    void add(ScreenButtonParams params) {

        for (auto& e: params.expl) {
            auto& expl = params.rtExplanation.emplace_back();
            expl.text = e;
        }

        //this->emplace_back(std::move(params));
        this->push_back(params);
    }
};


struct GameScreenToolButtonParams {
    bool checkbox;
    Point offset;
    
    Texture texture;
    bool checked;  // For checkbox

    std::function<void()> onPush;  // For button
    std::function<void(bool)> onCheck;  // For checkboxes
};


struct GameScreenToolButtonsParams {
    Cord buttonWidth;
    Cord buttonHeight;
    Size buttonSize;

    Point locationCoeff;

    Cord buttonAdvanceX;
    Cord buttonAdvanceY;
    Point buttonAdvance;

    std::vector<GameScreenToolButtonParams> buttons;
};




}  // namespace buttons

