/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_and_common.h"
#include "mangl_and_jni.h"

//#include <game-activity/native_app_glue/android_native_app_glue.h>

#include <EGL/egl.h>


namespace mangl {

struct NativeEngineSavedState {
    bool mHasFocus;
};

class AndroidMainEngine {
public:
    explicit AndroidMainEngine(android_app *app);
    ~AndroidMainEngine();

    static AndroidMainEngine* instance();

    void mainLoop();
    void mainLoopImpl();

    void handleCommand(uint32_t cmd);
    void handleCommandImpl(uint32_t cmd);

    //struct android_app* getApp() noexcept { return app_; }


public:
//    static auto getJavaActivity() {
//        return Jni::app->activity->javaGameActivity;
//    }
//    static jclass getActivityClass() {
//        return Jni::getJniEnv()->GetObjectClass(Jni::app->activity->javaGameActivity);
//    }
//
//    static auto getActivity() {
//        return Jni::app->activity;
//    }

    static bool dead_;


private:
    //
    // Initialization and environment
    //
    //struct android_app *app_{};
    struct NativeEngineSavedState state_{};


    // Android app lifecycle
    bool fwInitialized_{};
    bool fwStarted_{};
    Framework* framework_{};
    SceneRenderer* renderer_{};
    //bool envInitialized_{};
    bool haveRenderWindow_{false};

    bool hasFocus_{}, visible_{}, hasWindow_{};
    bool isFirstFrame_{true};

    // JNI initialization
    //void init();


    // EGL stuff
    //bool mHasGLObjects{};
    bool eglNeedContext_{false};
    bool eglNeedResumeResources_{false};
    EGLDisplay eglDisplay_{EGL_NO_DISPLAY};
    EGLSurface eglSurface_{EGL_NO_SURFACE};
    EGLContext eglContext_{EGL_NO_CONTEXT};
    EGLConfig eglConfig_{nullptr};

    Size surfaceSize_{};


    Timestamp fps_{};
    uint64_t fpsRecipTimer_{};  // 1 / FPS in milliseconds
    bool gameLoopNeedExit_{};
    Timestamp timestamp_{};


    bool processLoopIdle();
    void processActivityInputs();

    bool processTimerInit();
    bool processTimerTick();
    void mmCallbackError(const char *err);


    bool lifeCycleFatalError_{false};
    String lifeCycleFatalErrorMessage_{};

    //bool lifeCycleSurface_{false};  // Has surface to render
    bool lifeCycleStarted_{false};
    bool lifeCycleStopped_{false};
    bool lifeCyclePaused_{true};
    bool lifeCycleWasPaused_{true};
    bool lifeCycleEnd_{};

    ANativeWindow* nativeWindow_{};

//#if MANGL_OPENGL
    bool useFramePacing_ = true;
    //bool useFramePacing_ = false;
    //bool unitializeGraphicsAPI();

    void renderInitDisplay();
    void renderInitConfig();
    void renderInitSurface();
    void renderInitContext();
    void renderDestroyContext();
    void renderDestroySurface();
    void renderDestroyDisplay();

    void initOpenGL();
    void destroyOpenGL();

    bool touchesResumed_{true};

    //
    // Processing motion events
    //
    void processMotionEventDown(GameActivityMotionEvent *motionEvent, float x, float y);
    void processMotionEventMove(GameActivityMotionEvent *motionEvent, float x, float y);
    void processMotionEventUp(GameActivityMotionEvent *motionEvent, float x, float y);

};

}  // namespace mangl

