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
#include "../core/main/mangl_main.h"

#include <swappy/swappyGL.h>
#include <dlfcn.h>


#undef MDebugPrint
#define MDebugPrint MDebugNoop


namespace mangl {

void Framework::finish() {
    GameActivity_finish(Jni::getActivity());
    Jni::finishApp();
}


//
// Joystick support
//
static bool joystickInitDone_ = false;
static const int SOURCE_CLASS_JOYSTICK = 16;
typedef float (*_getAxisValue_sig)(const AInputEvent*, int32_t, size_t);
static _getAxisValue_sig _getAxisValue = nullptr;

static void joystickInit() {
    if (joystickInitDone_) {
        return;
    }

    joystickInitDone_ = true;

    // look up the AMotionEvent_getAxisValue function
//    MDebugFunc("Trying to dlopen libandroid.so");
    void *libandroid = dlopen("libandroid.so", 0);
    if (libandroid) {
//        MDebugFunc("Opened libandroid.so, looking for AMotionEvent_getAxisValue.");
        _getAxisValue = (_getAxisValue_sig)dlsym(libandroid, "AMotionEvent_getAxisValue");
//        MDebugFunc("AMotionEvent_getAxisValue() address is %p", _getAxisValue);
    } else {
        MDebugFunc("Failed to open libandroid.so.");
    }
}



static const std::unordered_map<uint32_t, const char*> glueCmdStrings_ = {
//static const std::map<uint32_t, const char*> glueCmdStrings_ = {
    {UNUSED_APP_CMD_INPUT_CHANGED, "APP_CMD_INPUT_CHANGED"},
    {APP_CMD_INIT_WINDOW, "APP_CMD_INIT_WINDOW"},
    {APP_CMD_TERM_WINDOW, "APP_CMD_TERM_WINDOW"},
    {APP_CMD_WINDOW_RESIZED, "APP_CMD_WINDOW_RESIZED"},
    {APP_CMD_WINDOW_REDRAW_NEEDED, "APP_CMD_WINDOW_REDRAW_NEEDED"},
    {APP_CMD_CONTENT_RECT_CHANGED, "APP_CMD_CONTENT_RECT_CHANGED"},
    {APP_CMD_GAINED_FOCUS, "APP_CMD_GAINED_FOCUS"},
    {APP_CMD_LOST_FOCUS, "APP_CMD_LOST_FOCUS"},
    {APP_CMD_CONFIG_CHANGED, "APP_CMD_CONFIG_CHANGED"},
    {APP_CMD_LOW_MEMORY, "APP_CMD_LOW_MEMORY"},

    {APP_CMD_START, "APP_CMD_START"},
    {APP_CMD_RESUME, "APP_CMD_RESUME"},
    {APP_CMD_SAVE_STATE, "APP_CMD_SAVE_STATE"},
    {APP_CMD_PAUSE, "APP_CMD_PAUSE"},
    {APP_CMD_STOP, "APP_CMD_STOP"},
    {APP_CMD_DESTROY, "APP_CMD_DESTROY"},

    {APP_CMD_WINDOW_INSETS_CHANGED, "APP_CMD_WINDOW_INSETS_CHANGED"},
    {APP_CMD_EDITOR_ACTION, "APP_CMD_EDITOR_ACTION"},
    {APP_CMD_TOUCH_EVENT, "APP_CMD_TOUCH_EVENT"},
};


void AndroidMainEngine::handleCommand(uint32_t cmd)
{
    // TODO: proper exception handling
    try {
        handleCommandImpl(cmd);
    }
    catch(const std::exception& e) {
        loge("Exception: %s", e.what());
        lifeCycleEnd_ = true;
        //exit(1);
    }
    catch(...) {
        loge("Unhandled Exception");
        lifeCycleEnd_ = true;
        //exit(1);
    }

}


void AndroidMainEngine::handleCommandImpl(uint32_t cmd) {

    const char *cmdString = "INVALID COMMAND";
    auto it = glueCmdStrings_.find(cmd);
    if (it != glueCmdStrings_.end())
        cmdString = it->second;

    MDebugPrint("td: %d; cmd: (%d) %s", gettid(), cmd, cmdString);


    switch (cmd) {
    case APP_CMD_START:
        if (!lifeCycleStarted_) {
            MDebugPrint("APP_CMD_START:Thread Id: %d", M::debugGetCurrentThreadId());
            joystickInit();
        }

        Jni::init();
        Env::init();

//        if (!envInitialized_) {
//            envInitialized_ = true;
//            // TODO: check what needs to be initialized after what (Env::init() or Jni::init())
//        }

        MDebugPrint("APP_CMD_START Activity: %ull", Jni::getJavaActivityClass());

        if (!fwInitialized_) {
            fwInitialized_ = true;

            mangl_main();

            framework_ = Framework::instance();
            renderer_ = Framework::getRenderer();
            framework_->init();
            framework_->onInitialized();

//            if (useFramePacing_) {
//                SwappyGL_init(getJniEnv(), app_->activity->javaGameActivity);
//                SwappyGL_setSwapIntervalNS(SWAPPY_SWAP_60FPS);
//                useFramePacing_ = SwappyGL_isEnabled();
//            }
        }


//        if (lifeCycleStopped_) {
//            lifeCycleStopped_ = false;
//            eglNeedResumeResources_ = true;
//        }
        break;

    case APP_CMD_RESUME:
        if (fwStarted_) {
            framework_->onResume();
            AudioSystem::onBecomeActive();
            lifeCyclePaused_ = false;
            touchesResumed_ = true;
        }
        break;

    case APP_CMD_WINDOW_INSETS_CHANGED: {
//        ARect insets;
//
//        GameActivity_getWindowInsets(app_->activity, GAMECOMMON_INSETS_TYPE_SYSTEM_BARS, &insets);
//        MDebugPrint("Cmd System bars: %g, %g, %g, %g", insets.top, insets.right, insets.bottom,insets.left);
//
//        GameActivity_getWindowInsets(app_->activity, GAMECOMMON_INSETS_TYPE_DISPLAY_CUTOUT, &insets);
//        MDebugPrint("Cmd Activity insets: %g, %g, %g, %g", insets.top, insets.right, insets.bottom, insets.left);
//
//        GameActivity_getWindowInsets(app_->activity, GAMECOMMON_INSETS_TYPE_WATERFALL, &insets);
//        MDebugPrint("Cmd Waterfall: %g, %g, %g, %g", insets.top, insets.right, insets.bottom, insets.left);
        break;
    }

    case APP_CMD_INIT_WINDOW:
        MDebugPrint("APP_CMD_INIT_WINDOW:Thread id: %d", M::debugGetCurrentThreadId());

        if (!fwStarted_) {
            fwStarted_ = true;
            framework_->onStart();
            framework_->onViewport();

            framework_->onResume();
            AudioSystem::onBecomeActive();
            lifeCyclePaused_ = false;
            touchesResumed_ = true;
        }

        MDebugPrint("APP_CMD_INIT_WINDOW Activity: %ull", Jni::getJavaActivityClass());


        nativeWindow_ = Jni::app->window;
        haveRenderWindow_ = true;

        lifeCycleStarted_ = true;

        if (useFramePacing_) {
            SwappyGL_init(Jni::getJniEnv(), Jni::getJavaActivity());
            SwappyGL_setSwapIntervalNS(SWAPPY_SWAP_60FPS);
            useFramePacing_ = SwappyGL_isEnabled();
        }


        if (useFramePacing_) {
            SwappyGL_setWindow(Jni::app->window);
        }

        //logd("native window: %p", app_->window);
        //lifeCycleSurface_ = true;

        processLoopIdle();

        break;


    case APP_CMD_STOP:
//        SceneRenderer::pauseTextures();
//        Sfx::pauseSounds();
//        eglNeedResumeResources_ = true;
//        lifeCycleStopped_ = true;
        break;

    case APP_CMD_PAUSE:
        AudioSystem::onResignActive();
        framework_->onPause();
        lifeCyclePaused_ = true;
        break;

    case APP_CMD_CONTENT_RECT_CHANGED:
        break;

    case APP_CMD_TERM_WINDOW:
        SceneRenderer::pauseTextures();
        Sfx::pauseSounds();
        eglNeedResumeResources_ = true;
        lifeCycleStopped_ = true;

        Framework::getRenderer()->destroyScene();
        renderDestroyContext();
        renderDestroySurface();

        haveRenderWindow_ = false;
        nativeWindow_ = nullptr;

        Jni::destroy();
        break;

    case APP_CMD_WINDOW_RESIZED:
        break;

    case APP_CMD_WINDOW_REDRAW_NEEDED:
        processLoopIdle();
        break;

    case APP_CMD_TOUCH_EVENT:
        break;

    case APP_CMD_KEY_EVENT:
        break;

    case APP_CMD_DESTROY:
        SceneRenderer::destroyTexturesAll();
        break;

    default:
        break;
    }


}


}  // namespace mangl

