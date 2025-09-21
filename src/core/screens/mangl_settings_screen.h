/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
﻿#pragma once

#include "../game/mangl_game_screen.h"
#include "../mangl_texture_atlas.h"
#include "../mangl_framework.h"


namespace mangl {

enum class GameSettingsItemType {
    Invalid,
    Checkbox,
    Sounds,
    Music,
};

struct GameSettingsScreenItemParams {
    GameSettingsItemType type;
    Texture checkboxTexture;
    Texture sliderTexture;

    Cord rtHeight;
};

struct GameSettingsScreenParams {
    StrView titleText;
    Texture titleTexture;
    bool closeButton;

    std::vector<GameSettingsScreenItemParams> items;
};

using GameSettingsScreenParamsA = const GameSettingsScreenParams&;



struct SettingScreenBaseItem {
    GameSettingsScreenItemParams params;

    Rect rect{};

    LabelP titleLabel{};
    CheckboxP checkbox{};
    SliderP slider{};

    std::vector<WidgetP> widgets;

    void clearWidgets() {
        widgets.clear();

        titleLabel = nullptr;
        checkbox = nullptr;
        slider = nullptr;
    }
};


class SettingsScreenBaseWidget {

    GameScreenBaseP screen_{};
    GameScreenPopupP popup_{};

    //GameSettingsScreenParams params_{};
    bool isFinished_{};
    bool isClosed_{};


    SliderP soundsVolumeSlider_{};
    SliderP musicVolumeSlider_{};


    std::vector<SettingScreenBaseItem> items_;

public:
    SettingsScreenBaseWidget(GameScreenBaseP screen, GameScreenPopupP popup): 
        screen_{screen}, popup_{popup} {
    }

    void setup(GameSettingsScreenParamsA params);

    bool isFinished() const noexcept { return isFinished_; }

    void onStart();

    void onTimerPostfix(Timestamp) {
        if (isFinished_ && !isClosed_) {
            isClosed_ = true;

            if (popup_) {
                screen_->dismissPopup(popup_);
            } else {
                Framework::popScreen();
            }
        }
    }


//private:
    //CheckboxP createCheckbox(const GameSettingsScreenItemParams& item, Cord currentY, CheckboxParams cp);
};




class SettingsPopupBase: public GameScreenPopup {
    SettingsScreenBaseWidget widget_{GameScreenBase::currentScreen_, this};

public:
    void setup(GameSettingsScreenParamsA setup) {
        widget_.setup(setup);
    }

protected:
    void onStart() override { widget_.onStart(); }
    void onTimerPostfix(Timestamp timestamp) override {
        GameScreenPopup::onTimerPostfix(timestamp);
        widget_.onTimerPostfix(timestamp);
    }


};


template <class BaseScreenT>
class SettingsScreenBase: public BaseScreenT {

    SettingsScreenBaseWidget widget_{this, nullptr};

public:
    void setup(GameSettingsScreenParamsA setup) {
        widget_.setup(setup);
    }

protected:
    void onStart() override { widget_.onStart(); }
};




}  // namespace mangl
