/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_tutorial_screen.h"

#include "../effects/mangl_crossfade_effect.h"
#include "../audio/mangl_sound.h"


namespace mangl {

void TutorialScreenWidget::setup(GameTutorialSetupA params) {
    params_ = params;

    StrView titleText = params.titleText;

    if (titleText.empty() && !params.titleTexture)
        titleText = "TUTORIAL";


    Index pageIndex{};

    auto setupImagePage = [&](TextureA image) {
        auto& pi = pages_.emplace_back();

        pi.index = pageIndex;
        pi.imageTex = image;

        ++pageIndex;
    };


    for (auto& pp: params.pages) {
        if (pp.image) {
            setupImagePage(pp.image);
        }
    }

    if (params.pageImage) {
        setupImagePage(params_.pageImage);
    }

    for (auto& image: params_.pageImages) {
        setupImagePage(image);
    }




    GameScreenParams sp{
        .type = GameScreenType::Tutorial,
        .titleText = titleText,
        .titleTexture = params.titleTexture,
        .closeButton = true, //params.closeButton,
        .backButton = pages_.size() > 1, //params.backButton,
    };

    if (popup_) {
        sp.closeButton = true,
        popup_->setup(sp);
    } else {
        screen_->setup(sp);
    }
}


void TutorialScreenWidget::onStart() {
    auto scene = screen_->mainScene_;
    auto safe = screen_->contentArea_;

    auto& ctx = popup_ ? popup_->contentContext_ : screen_->contentContext_;

    Cord msgTextSize = 12;
    Point msgPosition = safe.position({0.5, 0.2});

    msgPosition.y = screen_->contentArea_.y + msgTextSize + 20;

    Cord availHeight = screen_->contentArea_.top() - msgPosition.y;
    if (ctx.screenTitle.rect.y > 0)
        availHeight = ctx.screenTitle.rect.y - msgPosition.y;

    //Index pageIndex{};

    //auto setupImagePage = [&](Texture image) {
    //    auto& pi = pages_.emplace_back();

    //    pi.index = pageIndex;

    //    const auto& imageTex = image;

    //    auto imageSize = imageTex.sizeByWidth(safe.w);

    //    if (availHeight < imageSize.h) {
    //        imageSize = imageTex.sizeByHeight(availHeight);
    //    }

    //    auto imageArea = Rect{safe.x, msgPosition.y, safe.w, availHeight};
    //    pi.imageRect = imageArea.positioned(imageSize, {0.5, 0.5});


    //    pi.imagePrim = scene->newImage(pi.imageRect, imageTex);
    //    pi.imagePrim->hide();

    //    ++pageIndex;
    //};

    for (auto& pi: pages_) {
        //const auto& imageTex = image;

        if (pi.imageTex) {
            auto imageSize = pi.imageTex.sizeByWidth(safe.w);

            if (availHeight < imageSize.h) {
                imageSize = pi.imageTex.sizeByHeight(availHeight);
            }

            auto imageArea = Rect{safe.x, msgPosition.y, safe.w, availHeight};
            pi.imageRect = imageArea.positioned(imageSize, {0.5, 0.5});

            pi.imagePrim = scene->newImage(pi.imageRect, pi.imageTex);
            pi.imagePrim->hide();
        }
    }



    auto label = scene->newLabel({
        .text = "TAP ON SCREEN TO CONTINUE",
        .pos = msgPosition,
        .color = params_.tapMessageColor1,
        .textSize = msgTextSize,
        .align = M::AlignCenterTop,
    });

    scene->newCrossfadeEffect({
        .widget = label,
        .color1 = params_.tapMessageColor2,
        .color2 = params_.tapMessageColor1,
        .rate = 1,
    })->start();


    if (popup_) {
        if (ctx.closeButton) {
            ctx.closeButton->onPush = [this] {
                isFinished_ = true;
            };
        }
    }

    if (ctx.backButton) {
        ctx.backButton->setVisible(false);
        ctx.backButton->onPush = [this] {
            setCurrentPage(currentPageIndex_ > 0 ? currentPageIndex_ - 1 : 0);
        };
    }

    setCurrentPage(0);
}

void TutorialScreenWidget::setCurrentPage(Index index) {
    if (pages_.empty())
        return;

    MDebugAssert(index < std::ssize(pages_));

    // Hide the current page
    if (currentPageIndex_ != index) {
        auto& pi = pages_[currentPageIndex_];
        if (pi.imagePrim) pi.imagePrim->hide();
    }


    currentPageIndex_ = index;

    auto& pi = pages_[currentPageIndex_];
    if (pi.imagePrim) pi.imagePrim->unhide();

    auto& ctx = popup_ ? popup_->contentContext_ : screen_->contentContext_;

    if (ctx.backButton) {
        ctx.backButton->setVisible(currentPageIndex_ > 0);
    }
}



void TutorialScreenWidget::processTouch() 
{
    bool finished = currentPageIndex_ + 1 >= std::ssize(pages_);

    if (finished) {
        isFinished_ = true;

        if (mangl_internal::defaultButtonSound_ != mangl_internal::InvalidSoundButton) {
            Sfx::play(mangl_internal::defaultButtonSound_);
        }

        return;
    }


    setCurrentPage(currentPageIndex_ + 1);
}

}  // namespace mangl

