/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../framework/mangl_screen_base.h"
#include "../native/mangl_native_gui.h"
#include "../mangl_texture_atlas.h"

#include "mangl_game_screen_params.h"

#include "mangl_screen_grid.h"
#include "mangl_screen_buttons_params.h"
#include "mangl_screen_title.h"
#include "mangl_screen_background.h"


namespace mangl {

class GameScreenPopup;
using GameScreenPopupP = GameScreenPopup*;


class GameScreenBase;
using GameScreenBaseP = GameScreenBase*;


struct GameScreenContentContext {
    bool haveScreenParams{};
    GameScreenParams screenParams{};

    ScreenTitle screenTitle{};

    std::vector<TextureAtlasP> loadedTempTextures;
    ButtonP closeButton{};
    ButtonP backButton{};
};


class GameScreenPopup {
    friend class GameScreenBase;

public:
    GameScreenBaseP screen_{};
    GameScreenContentContext contentContext_;

    GameScreenPopup();

public:
    void setup(GameScreenParamsA params) {
        contentContext_.screenParams = params;
        contentContext_.haveScreenParams = true;
    }

    void loadTempTexture(StrView file, TextureAtlasP atlas);

protected:
    virtual void onStart() {}

    virtual void onRelease() {}

    virtual void onTouchUp(TouchA) {}
    virtual void onKeyUp(KeyCode) {}
    virtual void onTimerPostfix(Timestamp) {}
};



struct GameScreenLayoutParams {
    Cord cellWidth;  // Cell width
    Size cellSize;  // Size. If width given then {width, width}
    bool even;  // Even or odd number of cells

    Index columns;
    Index rows;  
    SizeI gridDims;  // Number of cells. {columns, rows}
};

using GameScreenLayoutParamsA = const GameScreenLayoutParams&;



class GameScreenBase: public ScreenBase {
    friend class TutorialScreenWidget;
    friend class GameScreenPopup;

//
// Initialization
//
public:
    GameScreenBase() {
        init();
    }

    explicit GameScreenBase(GameScreenParamsA params) {
        init();
        setup(params);
    }

    static GameScreenBaseP currentScreen_;

    void loadTempTexture(StrView file, TextureAtlasP atlas) {
        atlas->load(file);
        contentContext_.loadedTempTextures.push_back(atlas);
    }


    void setup(GameScreenParamsA params) {
        contentContext_.screenParams = params;
        contentContext_.haveScreenParams = true;
    }


private:
    void init();


    //
    // Scenes
    //
public:
    SceneP backgroundScene_{};
    SceneP mainScene_{}, contentScene_{};
    SceneP popupScene_{}, overlayScene_{};
    SceneP debugScene_{};

    Rect fullArea_{}, safeArea_{}, contentArea_{};


    GameScreenPopupP popup_{};

    void showPopup(GameScreenPopupP popup);
    void dismissPopup(GameScreenPopupP popup);

    template <class ScreenT>
    void showPopup() {
        showPopup(new ScreenT);
    }


public:
    GameScreenLayout screenLayout_{};
    bool haveScreenLayoutParams_{};
    GameScreenLayoutParams screenLayoutParams_{};

    void setupGameScreenLayout(GameScreenLayoutParamsA params) {
        screenLayoutParams_ = params;
        haveScreenLayoutParams_ = true;

        initGameScreenLayout();
    }

    void initGameScreenLayout();


    GameScreenContentContext contentContext_;

    void buildContent(GameScreenContentContext& ctx);




    //
    // Screen elements
    //
    using ScreenType = GameScreenType;

    // Background
    GameScreenBackground background_;
    void buildBackground(ScreenType style = {});

    // Title
    ScreenTitle& screenTitle_{contentContext_.screenTitle};
    void buildTitle(GameScreenContentContext& ctx, ScreenTitleBuildParamsA params, ScreenType type);
    void buildTitle(ScreenTitleBuildParamsA params, ScreenType type = {}) {
        buildTitle(contentContext_, params, type);
    }

    void buildTitle(StringA text, ScreenType type = {}) {
        buildTitle({.text = text}, type);
    }
    void buildTitle(TextureA tex, ScreenType type = {}) {
        buildTitle({.texture = tex}, type);
    }


    //
    // Navigation buttons
    //
    ButtonP buildNavButton(const GameScreenToolButtonStyles& styles, Point position, ScreenType type = {});


    // 
    // Main selection buttons
    //
    ScreenButtonsParams screenButtons_{};
    LayoutScreenButtonsParams screenButtonsParams_{};

    void addScreenButton(ScreenButtonParams params) {
        screenButtons_.add(params);
    }

    void layoutScreenButtons(ScreenButtonsParams& buttons, LayoutScreenButtonsParamsA params);
    void buildScreenButtons(ScreenButtonsParams& buttons);

    void buildScreenToolButtons();




    //
    // Event handling
    //
protected:
    void onStartPrefix() override;
    void onStartPostfix() override;

    void onReleasePostfix() override;
    void onResume() override;

    void onTouchUpPostfix(TouchA touch) override {
        ScreenBase::onTouchUpPostfix(touch);

        if (popup_) {
            popup_->onTouchUp(touch);
        }
    }

    void onTimerPostfix(Timestamp timestamp) override {

        ScreenBase::onTimerPostfix(timestamp);

        if (popup_) {
            popup_->onTimerPostfix(timestamp);
        }
    }


    virtual void onBuildBackground(ScreenType type) {}

};


}  // namespace mangl

