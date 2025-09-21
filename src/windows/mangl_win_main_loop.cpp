/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_win_main_window.h"

#include "../core/mangl_dispatch.h"

#if MANGL_WINMAIN

#if MANGL_WIN_GAME_LOOP_TIMER_MM
#include <Mmsystem.h>
#endif



#if MANGL_AUDIO_OPENAL
#include <al/alut.h>
#endif

#if MANGL_CAPTURE_VIDEO
#include "../core/game/game_capture_video.h"
#endif


#if MANGL_CAPTURE_VIDEO
MACaptureVideo *__capture_video;
#endif

namespace mangl {

#if MANGL_WIN_GAME_LOOP_TIMER_MM
static 
void CALLBACK timerCallback(UINT, UINT, DWORD_PTR ptr, DWORD_PTR, DWORD_PTR) {
    auto mainWindow = (ManglMainWindow *)ptr;
    mainWindow->multimediaTimerCallback();
}
#endif

void ManglMainWindow::gameLoopInit(){
}

void ManglMainWindow::gameLoopStart(){
    mainLoopPendingExit_ = false;

    timeBeginPeriod(1);


#if MANGL_WIN_GAME_LOOP_TIMER_MM
    timerExitEvent_.create();
    UINT uDelay = 1000 / UINT(fps_);
    auto dwUser = (DWORD_PTR)this;
    UINT fuEvent = TIME_PERIODIC | TIME_KILL_SYNCHRONOUS;
    timerHandle_ = timeSetEvent(uDelay, 0, timerCallback, dwUser, fuEvent);
#endif
}

void ManglMainWindow::gameLoopStop()
{
    mainLoopPendingExit_ = true;

#if MANGL_WIN_GAME_LOOP_TIMER_MM
    timerExitEvent_.reset();
    timerExitEvent_.wait();
    timerExitEvent_.destroy();

    timeKillEvent(timerHandle_);
#endif

    mainLoopPendingInit_ = true;

    timeEndPeriod(1);
}


static Framework* fw_{};
static SceneRenderer* renderer{};

static __int64 timerFrequency_{}, currentTimerCounter_{}, lastTimerCounter_{}, endTimerCounter_{};
static Timestamp currentTimestamp_{}, timestampDelta_{}, timestampPeriod_{};

static uint64_t debugStatsTimerBegin_{}, debugStatsTimerRuntime_{};
static uint64_t debugStatsTimerScene_{}, debugStatsTimerRender_{}, debugStatsTimerEnd_{};

void ManglMainWindow::multimediaTimerCallback()
{
#if MANGL_DEBUG_STATS_FPS
    QueryPerformanceCounter((LARGE_INTEGER *)&debugStatsTimerBegin_);
#endif

    try {
        if (mainLoopPendingExit_) {

#if MANGL_OPENGL
            releaseOpenGL();
#endif

#if MANGL_AUDIO_OPENAL
            alutExit();  // destroy alut
#endif

#if MANGL_CAPTURE_VIDEO
            __capture_video->stop();
#endif

            //if (hdc_) {
            //    ReleaseDC(hwnd_, hdc_);
            //    hdc_ = 0;
            //}

#if MANGL_WIN_GAME_LOOP_TIMER_MM
            timeKillEvent(timerHandle_);
            timerExitEvent_.set();
#endif
            return;
        }


        if (mainLoopPendingInit_) {
            mainLoopPendingInit_ = false;

#if MANGL_AUDIO_OPENAL
            // Initialize ALU
            // Must be initialized in the same thread
            static int argc_ = 1;
            static char* argv_ = "TEST";
            alutInit(&argc_, &argv_);
#endif

            // Get the associated device context
            //hdc_ = GetDC(hwnd_);

#if MANGL_OPENGL
            initOpenGL();
#endif

#if MANGL_CAPTURE_VIDEO
            __capture_video = MACaptureVideo::instance();
            __capture_video->setup(window_width, window_height, fps_);
            __capture_video->start();
#endif
            timestampPeriod_ = 1 / fps_;

            //
            // Main Message cycle
            //
            fw_ = Framework::instance();
            fw_->onStart();
            renderer = fw_->getRenderer();
            fw_->onViewport();
            //renderer->setViewport(Env::applicationFrame.subframe());

            //
            // Precise timing
            //
            auto res = QueryPerformanceFrequency((LARGE_INTEGER *)&timerFrequency_);
            apl::verify(res, "QueryPerformanceFrequency is not supported");

            QueryPerformanceCounter((LARGE_INTEGER *)&lastTimerCounter_);
            currentTimestamp_ = 0;
            timestampDelta_ = 0;


            fw_->onInitialized();
        }

        if (mainLoopPendingLayout_) {
            mainLoopPendingLayout_ = false;

            //auto hdc = GetDC(hwnd_);
            //wglMakeCurrent(hdc, hglrc_);
            //ReleaseDC(hwnd_, hdc);

            fw_->onViewport();
            //renderer->prepareScene();

            if (loopJustLaunched_) {
                loopJustLaunched_ = false;

                if (stateWindowFullScreen.get(false)) {
                    setFullScreen(true);
                }
            }
        }

        QueryPerformanceCounter((LARGE_INTEGER *)&currentTimerCounter_);

        __int64 elapsedInt = (currentTimerCounter_ - lastTimerCounter_) * 1000000 / timerFrequency_;
        Timestamp elapsed = Timestamp(elapsedInt)/1000000;

#if MANGL_DEBUG_INCREMENTAL_TIMER
        currentTimestamp_ += elapsed < timestampPeriod_ ? elapsed : timestampPeriod_;
#else
        currentTimestamp_ += elapsed;
#endif

        timestamp_ = currentTimestamp_;


        // Processing all the tasks in main thread
        Dispatch::process();

#if MANGL_DEBUG_STATS_FPS
        QueryPerformanceCounter((LARGE_INTEGER *)&debugStatsTimerBegin_);
#endif

        fw_->processTimer(timestamp_);

#if MANGL_DEBUG_STATS_FPS
        QueryPerformanceCounter((LARGE_INTEGER *)&debugStatsTimerRuntime_);
#endif

        renderer->onTimer(timestamp_);

#if MANGL_DEBUG_STATS_FPS
        QueryPerformanceCounter((LARGE_INTEGER *)&debugStatsTimerScene_);
#endif

        renderer->prepareScene();
        renderer->onRender();

#if MANGL_DEBUG_STATS_FPS
        QueryPerformanceCounter((LARGE_INTEGER *)&debugStatsTimerRender_);
        debugStatsTimerEnd_ = debugStatsTimerRender_;
#endif

        lastTimerCounter_ = currentTimerCounter_;


#if MANGL_OPENGL
        SwapBuffers(renderWindowDC_);
#endif

#if MANGL_RENDER_DIRECTX
        __dx_swap_chain->Present(0, 0);
#endif

#if MANGL_CAPTURE_VIDEO
        __capture_video->capture();
#endif

        if (fw_->frameworkActionQueueIndex_) {
            PostMessageW(hwnd_, MANGL_WM_PROCESS_FRAMEWORK_ACTIONS, 0, 0);
        }

#if MANGL_DEBUG_STATS_FPS
        if (elapsed > 0.0000001) {
            auto& st = fw_->debugStats;
            st.fps_ = st.addData(st.fpsBuffer_, 1./elapsed);

            uint64_t totalTimeInt = (debugStatsTimerEnd_ - debugStatsTimerBegin_);
            uint64_t runtimeTimeInt = (debugStatsTimerRuntime_ - debugStatsTimerBegin_);
            uint64_t sceneTimeInt = (debugStatsTimerScene_ - debugStatsTimerRuntime_);
            uint64_t renderTimeInt = (debugStatsTimerRender_ - debugStatsTimerScene_);

            // * 100 
            auto loadValue = Timestamp(totalTimeInt) * 100 / timerFrequency_ / elapsed;

            st.load_ = st.addData(st.loadBuffer_, loadValue);

            st.runtimeLoad_ = st.addData(st.runtimeLoadBuffer_, Timestamp(runtimeTimeInt) / timerFrequency_);
            st.sceneLoad_ = st.addData(st.sceneLoadBuffer_, Timestamp(sceneTimeInt) / timerFrequency_);
            st.renderLoad_ = st.addData(st.renderLoadBuffer_, Timestamp(renderTimeInt) / timerFrequency_);
        }
#endif

        QueryPerformanceCounter((LARGE_INTEGER *)&endTimerCounter_);

#if MANGL_WIN_GAME_LOOP_TIMER_SLEEP
        // CreateWaitableTimerExW 
        // Decide how long we have to sleep
        __int64 timerElapsed = (endTimerCounter_ - currentTimerCounter_) * 1000000 / timerFrequency_;
        
        DWORD waitTime = DWORD(timerElapsed / 1000);
        DWORD timerFps = DWORD(1000 / fps_);

        if (waitTime < timerFps) {
            //waitTime = 1;
            SleepEx(timerFps - waitTime, FALSE);
        }
#endif

    }
    catch(const std::exception& e) {
        mmCallbackError(e.what());
    }
    catch(...) {
        mmCallbackError("MM Callback: Unhandled Exception!");
    }

}

void ManglMainWindow::mmCallbackError(const char *err)
{
    UINT flags = MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND;
    MessageBoxW(hwnd_, apl::ApiString(err).win(), L"Error!", flags);
    exit(1);
}

}  // namespace mangl


#endif
