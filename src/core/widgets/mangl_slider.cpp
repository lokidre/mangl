/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_slider.h"

#include "../mangl_scene.h"

namespace mangl {

SliderP Scene::newSlider(SliderParamsA params) {
    return newWidget<Slider>(params.flags)->onNew(params)->create();
}


SliderP Slider::onNew(SliderParamsA params) {
    type = WidgetType::Slider;

    setParams(params);

    styles_ = &M::Styles::slider;

    auto sliderHeight = params.rect.h;

    setId(params.id);

    auto sliderTex = &params.textures;

    if (!sliderTex->valid() && styles_ && styles_->textures.valid()) {
        sliderTex = &styles_->textures;
    }


    if (sliderTex && sliderTex->valid()) {
        auto& tex = *sliderTex;
        setSliderTex(tex.frame(0), {0, sliderHeight});
        setFullTex(tex.frame(2), {0, sliderHeight});
        setEmptyTex(tex.frame(3), {0, sliderHeight});
        setLeftTex(tex.frame(4), {0, sliderHeight});
        setRightTex(tex.frame(5), {0, sliderHeight});
    }

    move(params.rect);

    // setEnabled(!params.disabled);

    onPosition = params.onPosition ? params.onPosition : params.positionSignal;
    onPositionBegin = params.onPositionBegin ? params.onPositionBegin : params.positionBeginSignal;
    onPositionEnd = params.onPositionEnd ? params.onPositionEnd : params.positionEndSignal;

    onPositionId = params.onPositionId ? params.onPositionId : params.positionSignalId;
    onPositionBeginId = params.onPositionBeginId ? params.onPositionBeginId : params.positionBeginSignalId;
    onPositionEndId = params.onPositionEndId ? params.onPositionEndId : params.positionEndSignalId;

    setStateTag(params.stateTag);

    setPosition(params.position != 0 ? params.position : stateDefaultPosition_);
    setGranularity(params.granularity);

    return this;
}


void Slider::onCreate(SceneP scene) {
    lastSentPosition_ = -1;

    if (emptyTex_) {
        emptyTexSize_ = emptyTex_.size().scaled(emptyTexSize_);
        emptyPrim_ = scene->newPrim(M::TiledHorz)->setTexture(emptyTex_);
        componentPrims_.push_back(emptyPrim_);
    }

    if (fullTex_.tid) {
        fullTexSize_ = fullTex_.size().scaled(fullTexSize_);
        fullPrim_ = scene->newPrim(M::TiledHorz)->setTexture(fullTex_);
        componentPrims_.push_back(fullPrim_);
    }

    if (leftTex_.tid) {
        leftTexSize_ = leftTex_.size().scaled(leftTexSize_);
        beginPrim_ = scene->newPrim()->setTexture(leftTex_);
        componentPrims_.push_back(beginPrim_);
    }

    if (rightTex_.tid) {
        rightTexSize_ = rightTex_.size().scaled(rightTexSize_);
        endPrim_ = scene->newPrim()->setTexture(rightTex_);
        componentPrims_.push_back(endPrim_);
    }

    if (sliderTex_.tid) {
        sliderTexSize_ = sliderTex_.size().scaled(sliderTexSize_);
        sliderPrim_ = scene_->newPrim()->setTexture(sliderTex_);
        componentPrims_.push_back(sliderPrim_);
    }

    sliderLeftMargin_ = (leftTexSize_.w - sliderTexSize_.w) / 2;
    sliderRightMargin_ = (rightTexSize_.w - sliderTexSize_.w) / 2;
    if (sliderLeftMargin_ < 0)
        sliderLeftMargin_ = 0;
    if (sliderRightMargin_ < 0)
        sliderRightMargin_ = 0;

    updateLayout();
    updateVisible(!params_.hidden);
    setEnabled(!disabled_);

    loadSavedState();
}

void Slider::saveCurrentState() {
    if (statePosition_.hasKey() && stateChanged_) {
        statePosition_ = currentPosition_;
    }
}

void Slider::onDestroy()
{
    saveCurrentState();
}



Slider *Slider::setPosition(Position pos)
{
    currentPosition_ = apl::aclamp(pos, 0, 1);

    updateSlider();
    return this;
}


void Slider::onEnable(bool enabled) {
    for (auto &prim: componentPrims_) {
        if (enabled)
            prim->setColor(M::LightGrey);
        else
            prim->clearColor();
    }
}

void Slider::updateVisible(bool visible) {
    if (!created_)
        return;

    for (auto& prim: componentPrims_)
        prim->setVisible(visible);
}

void Slider::updateLayout()
{
    if (!created_)
        return;

    Cord leftMargin = beginPrim_ ? leftTexSize_.w : 0;
    Cord rightMargin = endPrim_ ? rightTexSize_.w : 0;

    if (emptyPrim_)
        emptyPrim_->move(r_.x + leftMargin, r_.y + (r_.h-emptyTexSize_.h)/2, r_.w-(leftMargin+rightMargin), emptyTexSize_.h)
            ->setElSize(emptyTexSize_)
            ->setScissor(scissorFlag_, scissorRect_);

    if (beginPrim_)
        beginPrim_->move({r_.x, r_.y + (r_.h-leftTexSize_.h)/2}, leftTexSize_)
            ->setScissor(scissorFlag_, scissorRect_);

    if (endPrim_)
        endPrim_->move({r_.x + r_.w - rightTexSize_.w, r_.y + (r_.h-rightTexSize_.h)/2}, rightTexSize_)
            ->setScissor(scissorFlag_, scissorRect_);

    updateSlider();
}

void Slider::updateSlider()
{
    Position sliderPosition = sliderLeftMargin_ + (r_.w - sliderLeftMargin_ - sliderRightMargin_ - sliderTexSize_.w) * currentPosition_;

    if (sliderPrim_) {
        sliderPrim_->move({r_.x + sliderPosition, r_.y + (r_.h-sliderTexSize_.h)/2}, sliderTexSize_)
            ->setScissor(scissorFlag_, scissorRect_)
            ->setAlpha(alphaFlag_, alpha_);
    }


    if (beginPrim_)
        beginPrim_->setAlpha(alphaFlag_, alpha_)->setScissor(scissorFlag_, scissorRect_);
    if (endPrim_)
        endPrim_->setAlpha(alphaFlag_, alpha_)->setScissor(scissorFlag_, scissorRect_);
    if (fullPrim_)
        fullPrim_->setAlpha(alphaFlag_, alpha_)->setScissor(scissorFlag_, scissorRect_);
    if (emptyPrim_)
        emptyPrim_->setAlpha(alphaFlag_, alpha_)->setScissor(scissorFlag_, scissorRect_);


    if (fullPrim_ && sliderPrim_) {
        Position leftMargin = beginPrim_ ? beginPrim_->getRect().w : 0;
        fullPrim_->move(r_.x + leftMargin, r_.y + (r_.h-fullTexSize_.h)/2, sliderPosition - leftMargin + sliderPrim_->getRect().w/2, fullTexSize_.h)
            ->setElSize(fullTexSize_)
            ->setScissor(scissorFlag_, scissorRect_);
    }
}

void Slider::updatePosition()
{
}

void Slider::processSelection(Position x)
{
    auto srect = sliderPrim_->getRect();

    if (x - srect.w/2 <= r_.x + sliderLeftMargin_)
        currentPosition_ = 0;
    else if (x + srect.w/2 > r_.x + r_.w - sliderRightMargin_)
        currentPosition_ = 1;
    else
        currentPosition_ = (x - srect.w/2 - (r_.x+sliderLeftMargin_))/((r_.w-sliderLeftMargin_ - sliderRightMargin_) - srect.w);

    // calculate new slider position
    updateSlider();

    stateChanged_ = true;
}

void Slider::emitPositionSignals()
{
    if (onPosition) onPosition(currentPosition_);
    if (onPositionId) onPositionId(controlId_, currentPosition_);
    lastSentPosition_ = currentPosition_;
}



void Slider::loadSavedState()
{
    if (!statePosition_.hasKey() || !created_ || savedStateLoaded_)
        return;

    setPosition(statePosition_(stateDefaultPosition_));

    savedStateLoaded_ = true;
}


bool Slider::onTouchDown(TouchA touch)
{
    if (disabled_)
        return false;

    auto& p = touch.p;

    bool contains = r_.contains(p);

    // if slider is horizontal, allow tracking from anywhere on the screen along x
    // withing some distance
    if (!contains) {
        contains = r_.contains({p.x - 30, p.y});
        if (contains)
            contains = r_.contains({p.x + 30, p.y});
    }

    if (contains) {
        processSelection(p.x);

        if (onPositionBegin) onPositionBegin(currentPosition_);
        if (onPositionBeginId) onPositionBeginId(controlId_, currentPosition_);

        if (onPosition) onPosition(currentPosition_);
        if (onPositionId) onPositionId(controlId_, currentPosition_);

        tracking_ = true;
    }

    return tracking_;
}

bool Slider::onTouchMove(TouchA touch)
{
    if (disabled_)
        return false;

    if (tracking_ && r_.contains(touch.p)) {
        processSelection(touch.p.x);
        if (!positionEqual(currentPosition_, lastSentPosition_))
            emitPositionSignals();
    }

    return tracking_;
}

bool Slider::onTouchUp(TouchA touch)
{
    if (disabled_)
        return false;

    bool moved = false;

    if (tracking_) {
        processSelection(touch.p.x);

        if (onPosition) onPosition(currentPosition_);
        if (onPositionId) onPositionId(controlId_, currentPosition_);

        if (onPositionEnd) onPositionEnd(currentPosition_);
        if (onPositionEndId)  onPositionEndId(controlId_, currentPosition_);

        stateChanged_ = true;
        moved = false;

        saveCurrentState();

        tracking_ = false;
    }

    return moved;
}

}  // namespace mangl
