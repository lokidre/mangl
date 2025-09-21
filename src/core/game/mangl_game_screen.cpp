/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_game_screen.h"

#include "../mangl_framework.h"
#include "../mangl_scene.h"

#include "../mangl_ads.h"

#ifndef MANGL_CONSOLE

namespace mangl {

namespace impl_ {
static SceneP screenBackgroundScene_{};
static SceneP screenMainScene_{};
static SceneP screenPopupScene_{};
static SceneP screenOverlayScene_{};

static SceneP screenDebugScene_{};
static bool debugBannerShown_{};
}  // namespace impl_

GameScreenBaseP GameScreenBase::currentScreen_{};


using namespace impl_;


GameScreenPopup::GameScreenPopup() {
    screen_ = GameScreenBase::currentScreen_;
}


void GameScreenBase::init() {

    if (!screenBackgroundScene_) {

        auto renderer = Framework::getRenderer();

        screenBackgroundScene_ = Framework::getScene();
        screenBackgroundScene_->setInteractionEnabled(false);

        screenMainScene_ = renderer->newScene();
        
        screenPopupScene_ = renderer->newScene();
        screenPopupScene_->setVisible(false);

        screenOverlayScene_ = renderer->newScene();
        screenOverlayScene_->setVisible(false);

        screenDebugScene_ = renderer->newScene();
        screenDebugScene_->setVisible(false);

        Framework::setScene(screenMainScene_);
    } 

    backgroundScene_ = screenBackgroundScene_;
    mainScene_ = screenMainScene_;
    contentScene_ = mainScene_;

    popupScene_ = screenPopupScene_;
    overlayScene_ = screenOverlayScene_;
    debugScene_ = screenDebugScene_;


    fullArea_ = Env::getFullArea();
    safeArea_ = Env::getSafeArea();
    contentArea_ = safeArea_;

    if (Ads::bannerEnabled()) {
        auto bannerTop = Ads::bannerLayout().top();
        auto diff = bannerTop - contentArea_.y;
        contentArea_.y += diff;
        contentArea_.h -= diff;
    }

    //initLayout();
}

void GameScreenBase::buildContent(GameScreenContentContext& ctx) {
    if (ctx.haveScreenParams) {
        auto& sp = ctx.screenParams;

        if (sp.onInit) sp.onInit();

        buildBackground(sp.type);

        if (sp.titleTexture.valid()) sp.title.texture = sp.titleTexture;

        if (!sp.titleText.empty()) sp.title.text = sp.titleText;

        buildTitle(sp.title, sp.type);

        if (sp.closeButton) {
            ctx.closeButton = buildNavButton(M::Styles::screen.closeButton, {1, 1}, sp.type);
        }

        if (sp.backButton) {
            ctx.backButton = buildNavButton(M::Styles::screen.backButton, {0, 1}, sp.type);
        }


        for (auto& buttonParams: sp.screenButtons) {
            addScreenButton(buttonParams);
        }
    }
}


void GameScreenBase::onStartPrefix() {
    currentScreen_ = this;
    buildContent(contentContext_);
}


void GameScreenBase::onStartPostfix() {
    if (!screenButtons_.empty()) {
        layoutScreenButtons(screenButtons_, screenButtonsParams_);
        buildScreenButtons(screenButtons_);
    }


    if (!contentContext_.screenParams.toolButtons.buttons.empty()) {
        buildScreenToolButtons();
    }



#if defined(MANGL_ADS_IMITATION) || defined(MANGL_ADS_EMBED)
    // Imitate ads banner
    if (Ads::bannerEnabled() && !debugBannerShown_) {
        debugBannerShown_ = true;
        Ads::debugSetBannerPrim({
            .prim = debugScene_->newBox(Color{M::LightYellow, .6}, {}),
        });

        debugScene_->setVisible(true);
        debugScene_->setInteractionEnabled(false);
    }
#endif

}



void GameScreenBase::onReleasePostfix() {
    NativeGui::instance()->clearCallbacks();
    NativeGui::instance()->clearWidgets();

    mainScene_->release();


    for (auto& loadedTex: contentContext_.loadedTempTextures) {
        loadedTex->release();
    }
    contentContext_.loadedTempTextures.clear();
}


void GameScreenBase::showPopup(GameScreenPopupP popup) {
    MDebugTrapAssert(popup_ == nullptr);

    popup_ = popup;

    mainScene_->hide();
    mainScene_ = popupScene_;
    mainScene_->unhide();

    Framework::setScene(mainScene_);


    buildContent(popup_->contentContext_);
    popup_->onStart();

}

void GameScreenBase::dismissPopup(GameScreenPopupP popup) {
    MDebugTrapAssert(popup_ == popup);

    popup_->onRelease();

    // Deal with the temporary loaded textures
    for (auto& atlas: popup_->contentContext_.loadedTempTextures) {
        //std::erase_if(loadedTempTextures_, [&atlas](auto v) { return v == atlas; });
        std::erase(contentContext_.loadedTempTextures, atlas);
        atlas->release();
    }


    delete popup_;
    popup_ = nullptr;

    mainScene_->clear();

    mainScene_->hide();
    mainScene_ = contentScene_;
    mainScene_->unhide();

    Framework::setScene(mainScene_);

}


void GameScreenBase::onResume()
{
    // Prepare the scenes
    mainScene_->hide();
    mainScene_ = contentScene_;
    mainScene_->unhide();

    Framework::setScene(mainScene_);
}


}  // namespace sample

#endif

