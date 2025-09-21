/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_setup.h"

#include "mangl_easing.h"
#include "../widgets/mangl_label.h"

namespace mangl {

struct GuiEffect {
    GuiEffect() = default;
    GuiEffect(const GuiEffect &) = delete;
    GuiEffect &operator=(const GuiEffect &) = delete;

    virtual ~GuiEffect() = default;


    // Flags controlling effect execution
    //
    bool active_{};
    bool firstRun_{true};  // the effect is running for the first time
    bool started_{}, stopped_{};  // the effect started or stopped after starting
    bool justStarted_{};  // flag indicating the first call for effect

    bool paused_{};
    Timestamp pauseStartTimestamp_{}, pauseAdjustmentTime_{};

    Timestamp startTime_{};
    Timestamp currentTimestamp_{}, lastTimestamp_{};
    Timestamp firstTimestamp_{}, startTimestamp_{}, stopTimestamp_{};

    WidgetP attachedWidget_{};
    PrimP attachedPrim_{};

    Timestamp repeatPeriod_{0};  // seconds

    Easing easing_{Easing::None};


protected:  // Setup
    bool sceneAttached_{};
    bool repeat_{};  // repeat the animation
    bool reverse_{};  // reverse animation
    bool permanent_{true};  // the effect stays with the object
    bool disappear_{};  // after finishing the effect the scene object becomes invisible
    Timestamp duration_{1};  // effect duration in seconds

    bool direction_{true};  // effect direction
    bool bounceDirection_{};


private:
    SceneP scene_{};

public:

    void attach(PrimP prim);
    void attach(WidgetP widget);
    void detach();

    void start(Timestamp delay = 0);
    void stop();

    void pause();
    void resume();

    // status request
    [[nodiscard]] bool active() const noexcept { return active_; }

    void clear();


    [[nodiscard]] Timestamp duration() const noexcept { return duration_; }
    GuiEffect *setDuration(Timestamp d) noexcept { duration_ = d; return this; }

    GuiEffect *setPeriod(Timestamp p) noexcept { repeatPeriod_ = p;  return this; }
    GuiEffect *setRepeat(bool f = true) noexcept { repeat_ = f;  return this; }
    void setDisappear(bool f = true) noexcept { disappear_ = f; }

    GuiEffect *setBounce(bool f = true) noexcept { bounceDirection_ = f;  return this; }
    GuiEffect *setPermanent(bool f = true) noexcept { permanent_ = f;  return this; }
    GuiEffect *setReverse(bool f = true) noexcept { reverse_ = f;  return this; }

    GuiEffect* setRate(Timestamp p) noexcept {
        this->setDuration(1 / p);
        this->setPeriod(1 / p);
        this->setRepeat(true);
        //this->setBounce(true);
        return this; 
    }

    GuiEffect *setEasing(Easing easing) noexcept { easing_ = easing;  return this; }


    // Actions
    SignalVoid startSignal, stopSignal;
    SignalReal progressSignal;

    void processTimer(Timestamp timestamp);

protected:
    template <class Params>
    void setupParams(const Params& params) noexcept {
        if (params.prim)
            this->attach(params.prim);
        if (params.widget)
            this->attach(params.widget);

        this->setRepeat(params.repeat);
        this->setBounce(params.bounce);

        if (params.period != 0)
            setPeriod(params.period);

        if (params.duration != 0)
            setDuration(params.duration);

        if (params.rate != 0) {
            this->setRate(params.rate);
            //this->setDuration(1 / params.rate);
            //this->setPeriod(1 / params.rate);
            //this->setRepeat(true);
            //this->setBounce(true);
        }

        if (params.easing != Easing::None)
            this->setEasing(params.easing);

        this->setPermanent(params.permanent);
        this->setReverse(params.reverse);
        this->setDisappear(params.disappear);
    }

protected:
    virtual void onStart() {}
    virtual void onRestart() {}
    virtual void onTimer(Timestamp lambda) {}
    virtual void onClear() {}
    virtual void onStop() {}
    virtual void onPause() {}
    virtual void onResume() {}
};


template <class T>
struct GuiEffectTemplate: public GuiEffect {
    using Base = GuiEffect;
    using TP = T *;
    using ref_type = T &;
    using pointer_type = T *;

    T *clear() { Base::clear();  return static_cast<TP>(this); }


    T *setRate(Timestamp r) { return (TP)Base::setRate(r); }
    T *setPeriod(Timestamp r) { return (TP)Base::setPeriod(r); }
    T *setDuration(Timestamp d) { Base::setDuration(d);  return static_cast<TP>(this); }
    T *setBounce(bool f = true) { return (TP)Base::setBounce(f); }
    T *setRepeat(bool f = true) { return (TP)Base::setRepeat(f); }
    T *setDisappear(bool f = true) { Base::setDisappear(f); return static_cast<TP>(this); }
    T *setReverse(bool f = true) { return static_cast<TP>(Base::setReverse(f)); }
    T *setPermanent(bool f = true) { return static_cast<TP>(Base::setPermanent(f)); }
    T *setEasing(Easing easing) { return (TP)Base::setEasing(easing); }

    T *attach(Prim *prim) { Base::attach(prim);  return static_cast<TP>(this); }
    T *attach(Widget *widget) { Base::attach(widget);  return static_cast<TP>(this); }
    T *detach() { Base::detach();  return static_cast<TP>(this); }

    T *start(Timestamp startTime = 0) { Base::start(startTime); return static_cast<TP>(this); }

};

}  // namespace mangl
