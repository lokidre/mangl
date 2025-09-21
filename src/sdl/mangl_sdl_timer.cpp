/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_sdl_main_window.h"

#include "../core/mangl_dispatch.h"

#if MANGL_SDL

#include "mangl_sdl.h"

#include <chrono>


namespace mangl {

static Timestamp currentTimestamp_{}, timestampDelta_{}, timestampPeriod_{};
static uint64_t lastTimerCounter_{}, currentTimerCounter{};
static Framework *fw_{};
static SceneRenderer *renderer{};

static constexpr uint64_t timerFrequency = 1000;

#if MANGL_DEBUG_STATS_FPS
static uint64_t debugStatsTimerBegin_{}, debugStatsTimerRuntime_{};
static uint64_t debugStatsTimerScene_{}, debugStatsTimerRender_{}, debugStatsTimerEnd_{};
#endif

static inline uint64_t getCurrentTimer() {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::steady_clock;

    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}


static uint64_t lastIdleTimer_{};

bool ManglMainWindow::processLoopIdle() {
    auto currentTimer = getCurrentTimer();

    if (lastIdleTimer_ == 0) {
        lastIdleTimer_ = currentTimer;
        return onTimer();
    }


    uint64_t elapsedInt = currentTimer - lastIdleTimer_;

    if (elapsedInt >= fpsRecipTimer_) {
        lastIdleTimer_ = currentTimer;
        return onTimer();
    }

    uint64_t delayAmount = fpsRecipTimer_ - elapsedInt;
    std::this_thread::sleep_for(std::chrono::milliseconds(delayAmount));

    lastIdleTimer_ = currentTimer;
    return onTimer();
}


bool ManglMainWindow::onTimer() {

#if MANGL_DEBUG_STATS_FPS
    debugStatsTimerBegin_ = getCurrentTimer();
#endif

    // try {
        if (gameLoopNeedExit_) {
            releaseOpenGL();
            return false;
        }

        if (!timerInitialized_) {
            initOpenGL();


            //
            // Main Message cycle
            //
            fw_ = Framework::instance();
            fw_->onStart();
            renderer = Framework::getRenderer();
            fw_->onViewport();

            lastTimerCounter_ = getCurrentTimer();

            currentTimestamp_ = 0;
            timestampDelta_ = 0;

            timerInitialized_ = true;


            fw_->onInitialized();
        }

        Dispatch::process();

        currentTimerCounter = getCurrentTimer();

        uint64_t elapsedInt = currentTimerCounter - lastTimerCounter_;
        Timestamp elapsed = static_cast<Timestamp>(elapsedInt) / timerFrequency;


#if MANGL_DEBUG_INCREMENTAL_TIMER
        currentTimestamp_ += elapsed < timestampPeriod_ ? elapsed : timestampPeriod_;
#else
        currentTimestamp_ += elapsed;
#endif

        timestamp_ = currentTimestamp_;
        renderer->prepareScene();

#if MANGL_DEBUG_STATS_FPS
        debugStatsTimerBegin_ = getCurrentTimer();
#endif
        fw_->processTimer(timestamp_);

#if MANGL_DEBUG_STATS_FPS
        debugStatsTimerRuntime_ = getCurrentTimer();
#endif

        renderer->onTimer(timestamp_);

#if MANGL_DEBUG_STATS_FPS
            debugStatsTimerScene_ = getCurrentTimer();
#endif

        renderer->onRender();

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
                auto loadValue = static_cast<Timestamp>(totalTimeInt) * 100 / timerFrequency / elapsed;

                st.load_ = st.addData(st.loadBuffer_, loadValue);

                st.runtimeLoad_ = st.addData(st.runtimeLoadBuffer_, static_cast<Timestamp>(runtimeTimeInt) / timerFrequency);
                st.sceneLoad_ = st.addData(st.sceneLoadBuffer_, static_cast<Timestamp>(sceneTimeInt) / timerFrequency);
                st.renderLoad_ = st.addData(st.renderLoadBuffer_, static_cast<Timestamp>(renderTimeInt) / timerFrequency);
            }
#endif
        lastTimerCounter_ = currentTimerCounter;
    // }
    // catch(const std::exception& e) {
    //     mmCallbackError(e.what());
    //     return false;
    // }
    // catch(...) {
    //     mmCallbackError("MM Callback: Unhandled Exception!");
    //     return false;
    // }

    return true;
}

void ManglMainWindow::mmCallbackError(const char *err)
{
    SDL_MessageBoxFlags flags = SDL_MESSAGEBOX_ERROR;
    SDL_ShowSimpleMessageBox(flags, "Error", err, sdlWindow_);

    exit(1);
}


}  // namespace mangl

#endif
