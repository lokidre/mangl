/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_main_engine.h"

#include "../core/audio/mangl_audio_system.h"
#include "../core/audio/mangl_sound.h"

#include "../core/mangl_dispatch.h"

#include <thread>
#include <chrono>

#include <swappy/swappyGL.h>


namespace mangl {


static Timestamp currentTimestamp_{}, timestampDelta_{}, timestampPeriod{};
static uint64_t lastTimerCounter_{}, currentTimerCounter{};
static uint64_t lastIdleTimer_{};


static constexpr uint64_t timerFrequency = 1000;

static uint64_t debugStatsTimerBegin_{}, debugStatsTimerRuntime_{};
static uint64_t debugStatsTimerScene_{}, debugStatsTimerRender_{}, debugStatsTimerEnd_{};

static inline uint64_t getCurrentTimer() {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::steady_clock;

    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}


static void handleCmdProxy(struct android_app *app, int32_t cmd) {
    static_cast<AndroidMainEngine *>(app->userData)->handleCommand(cmd);
}

//static int _handle_input_proxy(struct android_app *app, AInputEvent *event) {
//    return static_cast<AndroidMainEngine *>(app->userData)->handleInput(event);
//}

void AndroidMainEngine::mainLoop() {
    try {
        mainLoopImpl();
    }
    catch(const std::exception& e) {
        MDebugPrint("Exception: %s", e.what());
        lifeCycleEnd_ = true;
        lifeCycleFatalError_ = true;
        lifeCycleFatalErrorMessage_ = e.what();
    }
    catch(...) {
        MDebugPrint("Unhandled Exception");
        lifeCycleEnd_ = true;
        lifeCycleFatalError_ = true;
        lifeCycleFatalErrorMessage_ = "Unhandled exception!";
    }

    if (lifeCycleFatalError_) {
        Jni::die(lifeCycleFatalErrorMessage_);
        for(;;) {
            M::sleep(10);
        }
    }

}

void AndroidMainEngine::mainLoopImpl() {
    Jni::app->userData = this;
    Jni::app->onAppCmd = handleCmdProxy;
    Jni::app->textInputState = 0;

    lastTimerCounter_ = getCurrentTimer();

    currentTimestamp_ = 0;
    timestampDelta_ = 0;

    while (!lifeCycleEnd_) {

        int events{};
        struct android_poll_source* source{};

        while (!lifeCycleEnd_) {
            auto res = ALooper_pollOnce(0, nullptr, &events, (void **) &source);

            if (res < 0)
                break;

            if (source)
                source->process(Jni::app, source);

            if (Jni::app->destroyRequested)
                return;
        }

        processActivityInputs();
        processLoopIdle();
    }

}


bool AndroidMainEngine::processLoopIdle() {
    if (!processTimerInit()) {
        uint64_t delayAmount = 200;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayAmount));
        return true;
    }

    auto currentTimer = getCurrentTimer();

    if (lifeCyclePaused_) {
        lifeCycleWasPaused_ = true;
        lastTimerCounter_ = currentTimer;
        lastIdleTimer_ = currentTimer;
        uint64_t delayAmount = fpsRecipTimer_;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayAmount));
        return true;
    }

//    if (lifeCycleWasPaused_) {
//        lifeCycleWasPaused_ = false;
//        lastTimerCounter_ = currentTimer;
//        lastIdleTimer_ = currentTimer;
//        uint64_t delayAmount = fpsRecipTimer_;
//        std::this_thread::sleep_for(std::chrono::milliseconds(delayAmount));
//        return true;
//    }

    if (lastIdleTimer_ == 0) {
        lastIdleTimer_ = currentTimer;
        return processTimerTick();
    }


    uint64_t elapsedInt = currentTimer - lastIdleTimer_;

    if (elapsedInt >= fpsRecipTimer_) {
        lastIdleTimer_ = currentTimer;
        return processTimerTick();
    }

    if (!useFramePacing_) {
        uint64_t delayAmount = fpsRecipTimer_ - elapsedInt;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayAmount));
    }

    lastIdleTimer_ = currentTimer;
    return processTimerTick();
}

bool AndroidMainEngine::processTimerInit() {
    if (!haveRenderWindow_)
        return false;

    if (eglContext_ == EGL_NO_CONTEXT) {
        initOpenGL();
    }

    if (eglNeedResumeResources_) {
        eglNeedResumeResources_ = false;
        SceneRenderer::resumeTextures();
        Sfx::resumeSounds();
    }

    return true;
}



bool AndroidMainEngine::processTimerTick() {

#if MANGL_DEBUG_STATS_FPS
    debugStatsTimerBegin_ = getCurrentTimer();
#endif

    currentTimerCounter = getCurrentTimer();

    uint64_t elapsedInt = currentTimerCounter - lastTimerCounter_;
    Timestamp elapsed = Timestamp(elapsedInt) / timerFrequency;

    currentTimestamp_ += elapsed;

    timestamp_ = currentTimestamp_;

    renderer_->prepareScene();

    Dispatch::process();

#if MANGL_DEBUG_STATS_FPS
    debugStatsTimerBegin_ = getCurrentTimer();
#endif
    framework_->processTimer(timestamp_);

#if MANGL_DEBUG_STATS_FPS
    debugStatsTimerRuntime_ = getCurrentTimer();
#endif

    renderer_->onTimer(timestamp_);

#if MANGL_DEBUG_STATS_FPS
    debugStatsTimerScene_ = getCurrentTimer();
#endif

    renderer_->onRender();


    if (useFramePacing_) {
        SwappyGL_swap(eglDisplay_, eglSurface_);
    } else {
        eglSwapBuffers(eglDisplay_, eglSurface_);
    }


#if MANGL_DEBUG_STATS_FPS
    debugStatsTimerRender_ = getCurrentTimer();
    debugStatsTimerEnd_ = debugStatsTimerRender_;

    if (elapsed > 0.0000001) {
        auto& st = Framework::debugStats;
        st.fps_ = st.addData(st.fpsBuffer_, 1./elapsed);

        uint64_t totalTimeInt = (debugStatsTimerEnd_ - debugStatsTimerBegin_);
        uint64_t runtimeTimeInt = (debugStatsTimerRuntime_ - debugStatsTimerBegin_);
        uint64_t sceneTimeInt = (debugStatsTimerScene_ - debugStatsTimerRuntime_);
        uint64_t renderTimeInt = (debugStatsTimerRender_ - debugStatsTimerScene_);

        // * 100
        auto loadValue = Timestamp(totalTimeInt) * 100 / timerFrequency / elapsed;

        st.load_ = MFrameworkStats::addData(st.loadBuffer_, loadValue);

        st.runtimeLoad_ = st.addData(st.runtimeLoadBuffer_, Timestamp(runtimeTimeInt) / timerFrequency);
        st.sceneLoad_ = st.addData(st.sceneLoadBuffer_, Timestamp(sceneTimeInt) / timerFrequency);
        st.renderLoad_ = st.addData(st.renderLoadBuffer_, Timestamp(renderTimeInt) / timerFrequency);
    }
#endif


    lastTimerCounter_ = currentTimerCounter;

    return true;
}


void AndroidMainEngine::mmCallbackError(const char *err) {
    // TODO: show error to the user

    exit(1);
}


}  // namespace mangl


