/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../game/mangl_game_screen.h"
#include "../mangl_texture_atlas.h"
#include "../mangl_framework.h"

//#include "../../../include/mangl/animation.h"


namespace mangl {

struct GameTutorialPage {
    Texture image;
};

struct GameTutorialSetup {
    StrView titleText;
    Texture titleTexture;

    bool closeButton;
    bool backButton;

    std::vector<GameTutorialPage> pages;

    // Shortcuts for page descriptions
    Texture pageImage;
    std::vector<Texture> pageImages;

    // Tap continue message
    Color tapMessageColor1{M::Red};
    Color tapMessageColor2{M::Yellow};

};

using GameTutorialSetupA = const GameTutorialSetup&;


class TutorialScreenWidget {

    GameScreenBaseP screen_{};
    GameScreenPopupP popup_{};
    SlotVoid closeButtonSlotBack_{};

    GameTutorialSetup params_{};
    bool isFinished_{};
    bool isClosed_{};


    struct PageContext {
        Index index{};

        Texture imageTex{};
        Rect imageRect{};
        PrimP imagePrim{};

    };

    std::vector<PageContext> pages_;
    Index currentPageIndex_{};


public:
    TutorialScreenWidget(GameScreenBaseP screen, GameScreenPopupP popup): 
        screen_{screen}, popup_{popup} {
    }

    void setup(GameTutorialSetupA params);
    void onStart();

    void processTouch();
    void setCurrentPage(Index index);

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

};


class TutorialPopupBase: public GameScreenPopup {
    TutorialScreenWidget widget_{GameScreenBase::currentScreen_, this};

public:
    void setup(GameTutorialSetupA setup) {
        widget_.setup(setup);
    }
    void setup(Texture image) {
        setup({.pageImage = image});
    }
    void setup(Texture titleTexture, Texture image) {
        setup({
            .titleTexture = titleTexture,
            .pageImage = image,
        });
    }


protected:
    void onStart() override { widget_.onStart(); }
    void onTouchUp(TouchA) override { widget_.processTouch(); }
    void onKeyUp(KeyCode) override { widget_.processTouch(); }
    void onTimerPostfix(Timestamp timestamp) override {
        GameScreenPopup::onTimerPostfix(timestamp);
        widget_.onTimerPostfix(timestamp);
    }

};


template <class BaseScreenT>
class TutorialScreenBase: public BaseScreenT {

    TutorialScreenWidget widget_{this, nullptr};

public:
    void setup(GameTutorialSetupA setup) {
        widget_.setup(setup);
    }

    void setup(Texture image) {
        setup({.pageImage = image});
    }

    void setup(Texture titleTexture, Texture image) {
        setup({
            .titleTexture = titleTexture,
            .pageImage = image,
        });
    }



protected:
    void onStart() override { widget_.onStart(); }
    void onTouchUp(TouchA) override { widget_.processTouch(); }
    void onKeyUp(KeyCode) override { widget_.processTouch(); }
    void onTimerPostfix(Timestamp timestamp) override {
        BaseScreenT::onTimerPostfix(timestamp);
        widget_.onTimerPostfix(timestamp);
    }
};

}  // namespace mangl
