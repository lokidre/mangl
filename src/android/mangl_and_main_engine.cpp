/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_main_engine.h"

//#include <swappy/swappyGL.h>


namespace mangl {

bool AndroidMainEngine::dead_ = true;

namespace mangl_impl_ {
static AndroidMainEngine* mainEngineInstance_{nullptr};
} // namespace mangl_impl_

using namespace mangl_impl_;

AndroidMainEngine* AndroidMainEngine::instance() {
    return mainEngineInstance_;
}


AndroidMainEngine::AndroidMainEngine(android_app *app)
{
    Jni::app = app;

    mainEngineInstance_ = this;
    //LOGD("NativeEngine: initializing.");

    //app_ = app;

    //android::mainActivity_ = app->activity->javaGameActivity;


    if (Jni::app->savedState) {
        // we are starting with previously saved state -- restore it
        state_ = *(struct NativeEngineSavedState *)Jni::app->savedState;
    }

    fps_ = 60;

#ifdef MANGL_FPS
    fps_ = MANGL_FPS;
#endif

    fpsRecipTimer_ = static_cast<uint64_t>(Real(1000) / fps_);


    android_app_set_key_event_filter(Jni::app, nullptr);
    android_app_set_motion_event_filter(Jni::app, nullptr);
}


AndroidMainEngine::~AndroidMainEngine() {
    mainEngineInstance_ = nullptr;
}


}  // namespace mangl


