/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_widget.h"
#include "../mangl_texture.h"
#include "../mangl_storage.h"
#include "mangl_slider_params.h"

namespace mangl {


class Slider: public WidgetTemplate<Slider> {
    friend class Scene;


    using Base = WidgetTemplate<Slider>;
    using Self = Slider;


    SliderStyles* styles_{};


public:
    using Position = Real;

public:
    auto setBackgroundTex(TextureA tex) {
        backgroundTex_ = tex;
        return this;
    }

    auto setSliderTex(TextureA t, Size s = {}) {
        sliderTex_ = t;
        sliderTexSize_ = s;
        return this;
    }
    auto setEmptyTex(TextureA t, Size s = {}) {
        emptyTex_ = t;
        emptyTexSize_ = s;
        return this;
    }
    auto setFullTex(TextureA t, Size s = {}) {
        fullTex_ = t;
        fullTexSize_ = s;
        return this;
    }
    auto setLeftTex(TextureA t, Size s = {}) {
        leftTex_ = t;
        leftTexSize_ = s;
        return this;
    }
    auto setRightTex(TextureA t, Size s={}) {
        rightTex_ = t;
        rightTexSize_ = s;
        return this;
    }

    SliderP setPosition(Position pos);
    Position position() const { return currentPosition_; }

    SlotReal onPosition{}, onPositionBegin{}, onPositionEnd{};
    SlotIdReal onPositionId{}, onPositionBeginId{}, onPositionEndId{};

    auto setStateTag(StrView tag, Position defPosition = {}) {
        if (tag.empty())
            return this;

        statePosition_.setKey("mangl_widget_slider_position.").setSuffix(tag);
        stateDefaultPosition_ = defPosition;
        loadSavedState();
        return this;
    }

    Setting<Position> statePosition_;


public:
    //virtual Slider *setEnabled(bool flag = true);

    auto setGranularity(Cord g) noexcept {
        granularity_ = g;
        return this;
    }


private:
    Texture backgroundTex_{}, emptyTex_{}, fullTex_{}, leftTex_{}, rightTex_{}, sliderTex_{};
    PrimP sliderPrim_{}, emptyPrim_{}, fullPrim_{}, beginPrim_{}, endPrim_{};
    std::vector<PrimP> componentPrims_;
    Size emptyTexSize_{}, fullTexSize_{}, leftTexSize_{}, rightTexSize_{}, sliderTexSize_{};

    Cord currentPosition_{0.5};
    Cord sliderLeftMargin_{}, sliderRightMargin_{};
    Cord lastSentPosition_{};
    Cord granularity_{};

    bool tracking_{};  // tracking started

    String stateTag_;
    Position stateDefaultPosition_{0.5};
    bool stateChanged_{};
    bool savedStateLoaded_{};
    void loadSavedState();
    void saveCurrentState();


    SliderParams params_{};


    SliderP onNew(SliderParamsA params);
    void setParams(SliderParamsA params) {
        params_ = params;

        setStateTag(params.stateTag);
    }




    void updateVisible(bool visible);
    void updateLayout();
    void updateSlider();
    void updatePosition();
    void processSelection(Position x);
    void emitPositionSignals();

    bool positionEqual(Cord p1, Cord p2) {
        if (granularity_ == 0)
            return p1 == p2;
        return std::abs(p1 - p2) < granularity_;
    }

protected:
    void onCreate(SceneP scene) override;
    void onDestroy() override;

    void onVisible(bool visible) override { updateVisible(visible); }
    void onAlpha() override { updateLayout(); }
    void onScissor(bool scissorFlag, RectA scissorRect) override {
        updateLayout();
    }
    void onMove(RectA rect) override { updateLayout(); }
    void onEnable(bool enabled) override;

    bool onTouchDown(TouchA) override;
    bool onTouchMove(TouchA) override;
    bool onTouchUp(TouchA) override;

};


}  // namespace mangl

