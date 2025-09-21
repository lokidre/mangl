/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_effect.h"
#include "mangl_easing.h"

#include <cognit/const.hpp>

namespace mangl {

void GuiEffect::attach(Prim *prim)
{
    attachedPrim_ = prim;
    attachedWidget_ = nullptr;

    //attachedPrims_.push_back(prim) ;
    //onAttachSprite(prim) ;
}

void GuiEffect::attach(Widget *widget)
{
    attachedPrim_ = nullptr;
    attachedWidget_ = widget;

    //attachedWidgets_.push_back(gwidget) ;
}

void GuiEffect::detach()
{
    attachedWidget_ = nullptr;
    attachedPrim_ = nullptr;

    //attachedWidgets_.clear() ;
    //attachedPrims_.clear() ;

    if (scene_) {
        scene_ = nullptr;
    }
}

void GuiEffect::start(Timestamp when) {
    started_ = false;
    stopped_ = false;
    justStarted_ = false;
    firstRun_ = true;
    active_ = true;
    startTime_ = when;
    currentTimestamp_ = 0;
}

void GuiEffect::stop()
{
    if (stopped_ || !active_)
        return;
    if (started_)
        onStop();
    stopped_ = true;
    active_ = false;
    started_ = false;
}

void GuiEffect::pause()
{
    if (paused_)
        return;
    paused_ = true;
    pauseStartTimestamp_ = currentTimestamp_;
}

void GuiEffect::resume()
{
    if (!paused_)
        return;
    paused_ = false;
    pauseAdjustmentTime_ += currentTimestamp_ - pauseStartTimestamp_;
}




void GuiEffect::processTimer(Timestamp timestamp)
{
    if (!active_)
        return;

    currentTimestamp_ = timestamp - pauseAdjustmentTime_;

    // handle the first 0 timestamp condition
    if (startTimestamp_ == 0)
        startTimestamp_ = currentTimestamp_;

    // handle the first request
    if (firstRun_) {
        lastTimestamp_ = timestamp;
        firstTimestamp_ = timestamp;
        firstRun_ = false;
    }

    // first handle the starting delay
    if (!started_ && currentTimestamp_ >= firstTimestamp_ + startTime_) {
        started_ = true;
        justStarted_ = true;
        stopped_ = false;
        startTimestamp_ = currentTimestamp_;
        onStart();
        onRestart();
        if (startSignal)
            startSignal();
    }

    if (!paused_) {

        // check if we need to finish the effect
        if (started_ && duration_ != 0 && !stopped_) {
            if ((!repeat_ || repeatPeriod_ != 0) && currentTimestamp_ >= startTimestamp_ + duration_) {
                onTimer(1);  // call once at the end
                stopped_ = true;
                stopTimestamp_ = currentTimestamp_;
                if (disappear_) {
                    if (attachedPrim_) attachedPrim_->hide();
                    if (attachedWidget_) attachedWidget_->hide();
                }
            }
        }

        if (stopped_) {
            if (repeat_) {
                if (currentTimestamp_ >= startTimestamp_ + repeatPeriod_) {
                    stopped_ = false;
                    justStarted_ = true;
                    startTimestamp_ = currentTimestamp_;

                    // if it was due to repeat period, then restart
                    if (repeatPeriod_ != 0) {
                        if (bounceDirection_)
                            direction_ = !direction_;
                        onRestart();
                    }

                }
            } else {
                onStop();
                if (stopSignal)
                    stopSignal();
                active_ = false;
            }
        }


        if (started_ && active_ && !stopped_) {
            // TODO: easing
            auto lambda = (currentTimestamp_ - startTimestamp_)/duration_;

            Timestamp cycles;
            lambda = std::modf(lambda, &cycles);

            if (lambda > 0 && lambda < 1 && easing_ != Easing::None)
                lambda = M::ease(easing_, lambda);

            bool direction = !reverse_ && direction_;
            if (!direction)
                lambda = 1 - lambda;

            onTimer(lambda);
            if (progressSignal)
                progressSignal(lambda);
        }
    }

    justStarted_ = false;
    lastTimestamp_ = currentTimestamp_;
}



void GuiEffect::clear()
{
    startSignal = nullptr;
    progressSignal = nullptr;
    stopSignal = nullptr;

    onClear();
}

}
