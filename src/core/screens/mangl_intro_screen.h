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

namespace mangl {


enum class IntroScreenProgressType {
    None,
    Reveal,  // Each progress item unhides
    Cover,  // Two sets of items. 
};

struct IntroScreenProgressItemParams {
    Texture pendingTexture;
    Texture completeTexture;
    Texture finalTexture;

    std::function<void()> onProgress;

    // Runtime vars
    Rect primRect;
    PrimP pendingPrim, completePrim, finalPrim;
};

struct IntroScreenParams {
    StrView titleText;
    Texture titleTexture;

    StrView loadingText;
    Texture loadingTexture;
    Cord loadingWidth;
    Cord loadingHeight;
    Point loadingOffset;

    // Loading progress
    IntroScreenProgressType progressType;
    Cord progressStepWidth;
    std::vector<IntroScreenProgressItemParams> progressSteps;
};



template <class BaseScreen>
class IntroScreenBase: public BaseScreen {
    using Base = BaseScreen;
    //using Self = IntroScreenBase;

    int progressCurrent_{}, progressTotal_{};
    Timestamp progressTime_{}, progressPeriod_{}, progressTimestamp_{};
    Timestamp progressStart_{};

protected:
    void onTimerPrefix(Timestamp timestamp) override {
        if (0 == timestamp)
            return;

        if (progressTotal_ == 0) {
            progressStart_ = timestamp;
            progressTime_ = onProgressTime();
            progressTotal_ = onProgressTotal();

            MDebugTrapAssert(progressTotal_ != 0);
            apl::verify(progressTotal_ != 0, "onProgressTotal is not overriden");

            progressCurrent_ = 0;
            progressPeriod_ = progressTime_ / Timestamp(progressTotal_);

            progressTimestamp_ = timestamp;
        }

        if (timestamp >= progressTimestamp_ && progressCurrent_ < progressTotal_) {
            //MDebugFunc("total: %f, progT: %f, prog#: %d", progressTime_, timestamp - progressStart_, progressCurrent_);

            onProgress(progressCurrent_);
            progressCurrent_++;
            if (progressCurrent_ >= progressTotal_)
                onLaunch();
            progressTimestamp_ = timestamp + progressPeriod_;
        }
    }



    //
    // Required virtual functions
    //
protected:
    virtual void onLaunch() = 0;
    virtual void onProgress(int current) = 0;
    virtual int onProgressTotal() = 0;
    virtual Timestamp onProgressTime() = 0;

    [[nodiscard]] Timestamp getProgressStart() const noexcept { 
        return progressStart_; 
    }
};


template <class BaseScreen>
class GameIntroScreenBase: public IntroScreenBase<BaseScreen> {
    using Base = IntroScreenBase<BaseScreen>;

protected:
    IntroScreenParams introParams_;

    void setup(const IntroScreenParams& params) {
        introParams_ = params;

        BaseScreen::setup({
            .type = GameScreenType::Intro,
            .titleText = params.titleText,
            .titleTexture = params.titleTexture,
        });
    }

    void onStartPrefix() override {
        Base::onStartPrefix();

        const auto& sl = this->screenLayout_;
        auto scene = this->mainScene_;


        auto& ip = introParams_;
        if (ip.progressSteps.empty())
            return;

        if (ip.loadingTexture.valid()) {
            Size labelSize{};
            if (ip.loadingWidth > 0)
                labelSize = ip.loadingTexture.sizeByWidth(ip.loadingWidth);
            if (ip.loadingHeight > 0)
                labelSize = ip.loadingTexture.sizeByHeight(ip.loadingHeight);
            MDebugTrapAssert(labelSize);
            apl::verify(labelSize, "loadingSize is not defined");

            Rect labelRect{};
            auto labelPos = sl.gridCenter.center() + Point{0, sl.gridSize.h * 2};

            if (this->screenLayoutParams_.even) {
                labelRect = sl.adjustGrid(labelPos, labelSize);
            } else {
                labelRect = sl.adjustGridCenter(labelPos, labelSize);
            }
            scene->newImage(labelRect, ip.loadingTexture);
        }

        Size progressStepSize{ip.progressStepWidth, ip.progressStepWidth};

        MDebugTrapAssert(progressStepSize);
        apl::verify(progressStepSize, "progressStepSize is not defined");

        Cord progressAdvance = sl.gridSize.w;

        Rect current{};

        if (this->screenLayoutParams_.even) {
            current = sl.adjustGrid(sl.gridCenter.center(), progressStepSize);
        } else {
            current = sl.adjustGridCenter(sl.gridCenter.center(), progressStepSize);
        }

        current.x -= progressAdvance * std::floor(std::ssize(ip.progressSteps) / 2);


        for (auto& step: ip.progressSteps) {
            if (step.pendingTexture)
                step.pendingPrim = scene->newImage(current, step.pendingTexture);

            if (step.completeTexture)
                step.completePrim = scene->newImage(current, step.completeTexture, M::Hidden);

            if (step.finalTexture)
                step.finalPrim = scene->newImage(current, step.finalTexture, M::Hidden);

            current.x += progressAdvance;
        }
    }



    void onProgress(int current) override {
        auto& step = introParams_.progressSteps[current];

        if (step.completePrim) {
            if (step.pendingPrim) step.pendingPrim->hide();
            step.completePrim->unhide();
        }


        if (step.onProgress)
            step.onProgress();
    }

    int onProgressTotal() override {
        return (int)std::ssize(introParams_.progressSteps);
    }

    Timestamp onProgressTime() override {
        // TODO: debug
        //return 8;

        if (Env::isDesktop())
            return 0.4;
        return 0.5;
    }


};


}  // namespace mangl
