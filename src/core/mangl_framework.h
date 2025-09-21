/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_setup.h"
#include "app/mangl_app_params.h"

#include "framework/mangl_framework_stats.h"
#include "framework/mangl_runloop_timer.h"
#include "framework/mangl_timer_handler.h"
#include "mangl_time.h"

#include "mangl_random.h"


#include "framework/mangl_screen_base.h"
#include "mangl_render.h"

#include "framework/mangl_screen_transition.h"

#include "mangl_environment.h"

#include <apeal/random.hpp>
#include <apeal/mutex.hpp>

#include <chrono>
#include <stack>

namespace mangl {

enum class MActionCode {
    IAP_REQUEST_PRODUCTS, IAP_PURCHASE, IAP_RESTORE,

    // Only for Windows platform, needs to be moved to somewhere private
    GUI_ALERT,
    GUI_CLEAR,
    GUI_PROMPT_TEXT_INPUT,
    GUI_CREATE_TEXT_INPUT, GUI_CREATE_LABEL,
    GUI_SET_TEXT, GUI_SET_FONT, //GUI_LABEL_MOVE,
    GUI_SET_VISIBLE, GUI_MOVE,
};


#if MANGL_ADS && MANGL_ADS_EMBED
class AdsNetworkClient;
#endif

struct MFrameworkAction {
    MActionCode code;
    uint_fast64_t data;
    String param, param2, param3;
    Rect contentsFrame;
    void *dataPtr;
    int dataSize;
};

namespace mangl_internal {
constexpr int InvalidSoundButton = -1;
extern int defaultButtonSound_;
}  // namespace mangl_internal


class Framework;
using FrameworkP = Framework*;

class Framework: public apl::NonCopyable {
    friend class ScreenBase;
    friend struct RunLoopTimer;

    using Self = Framework;



public:
    static FrameworkP instance_;
    static FrameworkP instance() noexcept { return instance_; }

    static SceneRenderer *render_;
    static SceneRenderer *renderer() { return render_; }  // obsolete
    static auto getRenderer() { return render_; }

    static SceneP scene_;
    static auto scene() noexcept { return scene_; } // obsolete
    static auto getScene() noexcept { return scene_; }
    static void setScene(SceneP scene) noexcept { 
        scene_ = scene; 
    }

    template <typename SfxT>
    static void setDefaultButtonSound(SfxT resId) {
        setDefaultButtonSoundBody(static_cast<SfxId>(resId));
    }
    static void setDefaultButtonSoundBody(SfxId resId);

    apl::Random &rand_{M::random}; // =*MRandom::instance();  // random number generator

public:
    constexpr static int FRAMEWORK_ACTION_QUEUE_SIZE = 100;
    MFrameworkAction frameworkActionQueue_[FRAMEWORK_ACTION_QUEUE_SIZE]{};
    int frameworkActionQueueBegin_{}, frameworkActionQueueIndex_{};

    MFrameworkAction &selectAvailFrameworkAction(const MActionCode &code) {
        if (frameworkActionQueueIndex_ >= FRAMEWORK_ACTION_QUEUE_SIZE)
            frameworkActionQueueIndex_--;  // overwrite the last item, what else todo?
        auto &q = frameworkActionQueue_[frameworkActionQueueIndex_];
        frameworkActionQueueIndex_++;
        q = {};
        q.code = code;
        return q;
    }


    void pushFrameworkAction(MActionCode code, StrView param, uint64_t data = 0, StrView param2 = {}, StrView param3 = {})
    {
        auto& q = selectAvailFrameworkAction(code);

        q.data = data;
        q.param = param;
        q.param2 = param2;
        q.param3 = param3;
    }

    void pushFrameworkAction(MActionCode code, uint64_t data = 0) {
        auto &q = selectAvailFrameworkAction(code);
        q.data = data;
    }

    void pushFrameworkAction(MActionCode code, RectA contentsFrame, StrView param = {}) {
        auto &q = selectAvailFrameworkAction(code);
        q.contentsFrame = contentsFrame;
        q.param = param;
    }

    void pushFrameworkAction(MActionCode code, RectA contentsFrame, uint64_t data) {
        auto &q = selectAvailFrameworkAction(code);
        q.contentsFrame = contentsFrame;
        q.data = data;
    }


    void pushFrameworkAction(MActionCode code, size_t size, void *ptr, uint64_t data = 0, StrView param = {})
    {
        auto &q = selectAvailFrameworkAction(code);
        q.data = data;
        q.param = param;
        q.dataPtr = ptr;
        q.dataSize = static_cast<int>(size);
    }


public:
    Framework();
    ~Framework() override;

    void init();

    static void finish();  // Exits the application

    void reset() {
        onRelease();
        init();

        if (currentScreen_)
            switchScreen(nullptr);
    }


    void setup(AppParamsA params) {
        appParams_ = params;
        haveAppParams_ = true;
    }

    static bool haveAppParams_;
    static AppParams appParams_;

    //
    // PentixScreen management
    //
protected:
    static ScreenBaseP currentScreen_;

public:
    static auto timestamp() noexcept {  return currentTimestamp_;  }
    static auto currentTimestamp() noexcept { return currentTimestamp_; }

    static auto timestampAtomic() noexcept {  return currentTimestampAtomic_.load();  }


    //
    // Screen transitions
    //
private:
    static ScreenBaseP transitionQueuedScreen_;
    static ScreenTransitionContext outScreenTransitionContext_, inScreenTransitionContext_;

public:
    static void setScreenTransition(ScreenTransitionParamsA params) {
        outScreenTransitionContext_.setParams(params);
        outScreenTransitionContext_.direction = false;
        inScreenTransitionContext_.setParams(params);
        inScreenTransitionContext_.direction = true;
    }



    // screen must be allocated with 'new', the previous screen will be freed with 'delete'
public:
    [[nodiscard]] static auto currentScreen() noexcept { return currentScreen_; }
    [[nodiscard]] static auto getCurrentScreen() noexcept { return currentScreen_; }

    static void switchScreen(ScreenBaseP screen) noexcept;

    template <class ScreenT>
    static void switchScreen() noexcept {
        using ScreenTR = std::remove_reference_t<ScreenT>;
        using ScreenTP = std::remove_pointer_t<ScreenTR>;
        switchScreen(new ScreenTP);
    }


private:
    static std::stack<std::function<void()>> screensBackStack_;
    void timerSwitchScreen(ScreenBaseP newScreen);
    static void timerReleaseScreen(ScreenBaseP screen);

public:
    //
    // push and pop screen functions unload the current screen
    // and load the new
    //
    template <class R>
    static void pushScreen() noexcept {
        screensBackStack_.push([] {  switchScreen<R>();  });
    };

    template <class R>
    static void pushScreen(ScreenBaseP screen) noexcept {
        screensBackStack_.push([] { switchScreen<R>();  });
        switchScreen(screen);
    };

    template <class ParentT, class ScreenT>
    static void pushScreenCreate() noexcept {
        pushScreen<ParentT>(new ScreenT);
    }

    static void popScreen() {
        if (screensBackStack_.empty())
            throw std::runtime_error("Screen stack is empty");
        auto f = screensBackStack_.top();
        screensBackStack_.pop();
        f();
    }
    
    static void popScreenTop() {
        if (screensBackStack_.empty())
            throw std::runtime_error("Screen stack is empty");
        
        while (screensBackStack_.size() > 1)
            screensBackStack_.pop();
        
        auto f = screensBackStack_.top();
        //screensBackStack_.pop();
        f();
    }


    // Show screen keeps the current screen in memory,
    // issues "pause" signal and loads the new screen
    static void showScreen(ScreenBaseP currentScreen, ScreenBaseP newScreen) noexcept {
        queuedShowScreen_ = newScreen;
        showScreenQueued_ = true;
    }




private:
    static ScreenBaseP queuedScreen_;
    static ScreenBaseP queuedTransitionScreen_;

    static ScheduleTrigger showScreenQueued_;
    static ScreenBaseP queuedShowScreen_;

    static ScreenBaseP queuedResumeScreen_;

    static void resumeScreen(ScreenBaseP screen);


public:
    virtual void onInit() {}
    virtual void onLayout();
    virtual void onViewport();
    virtual void onStart();
    void onInitialized();
    virtual void onResume();
    virtual void onPause();
    virtual void onRelease();
    virtual void onResignActive();
    virtual void onResumeActive();
    //void onReleased() ;


    // handling ads
    virtual void onInterstitialWillPresentFullScreen() {}
    virtual void onInterstitialDidDismissFullScreen() {}
    virtual void onAdsWillPresentFullScreen() {}
    virtual void onAdsDidDismissFullScreen() {}

    //void setBannerHeight(Cord h) ;

    //
    // Handling timer
    //

private:
    static bool timerFirstRun_;
    static bool timerServiceQueued_;
    static RunLoopTimer* timerService_;

protected:
    static Timestamp currentTimestamp_, lastTimestamp_;
    static std::atomic<Timestamp> currentTimestampAtomic_;

    void timerPrefix();
    void timerPostfix();


    virtual void onTimer(Timestamp){}

public:
    void processTimer(Timestamp timestamp) {
        currentTimestamp_ = timestamp;
        currentTimestampAtomic_.store(timestamp);
        timerPrefix();
        onTimer(timestamp);
        timerPostfix();
        lastTimestamp_ = timestamp;
    }


public:

    //
    // Synchronization/threads
    //
    static ThreadId mainThreadId;

    virtual void onScreenStart(Timestamp) {}
    virtual void onScreenTimer(Timestamp) {}

    // GUI feedback
    void onAlertAccepted() {}


    //
    // Handling user input
    //

    // Keyboard shortcuts
public:
    using KeyboardShortcut = std::vector<KeyCode>;
    KeyboardShortcut fullScreenShortcut_{KeyCode::Alt, KeyCode::F};
    KeyboardShortcut quitShortcut_{KeyCode::Control, KeyCode::Q};
    KeyboardShortcut screenshotShortcut_{KeyCode::Alt, KeyCode::F3};
    KeyboardShortcut backShortcut_{KeyCode::Escape};


    //
    // Touches and interaction
    //
    int touchesCount_{};
    static bool interactionEnabled_;

    static bool setInteractionEnabled(bool b) {
        auto wasEnabled = interactionEnabled_;
        interactionEnabled_ = b;
        return wasEnabled;
    }

public:
    virtual void onTouchDown(TouchA);
    virtual void onTouchMove(TouchA);
    virtual void onTouchUp(TouchA);
    virtual void onTouchCancel(TouchA);

    virtual void onKeyDown(KeyCode key);
    virtual void onKeyUp(KeyCode key);
    virtual void onMouseWheel(Cord delta, Flags keys, Point pos);

    virtual String onCall(StrView name, StrView data) { return {}; }
    String processCall(StrView name, StrView data);


    virtual void onTerminate();


    static MFrameworkStats debugStats;


    static void processBack();
};

//using Framework = MFrameworkNative;


}  // namespace mangl
