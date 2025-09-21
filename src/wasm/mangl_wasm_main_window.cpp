/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_wasm_main_window.h"

#include <emscripten/html5.h>
#include <emscripten/bind.h>

#include "../core/mangl_environment.h"
#include "../core/mangl_common.h"
#include "../core/opengl/mangl_render_opengl.h"



namespace mangl {

ManglMainWindow::ManglMainWindow() {
    fps_ = 30;
    // fps_ = 60 ;

#ifdef MANGL_FPS
    fps_ = MANGL_FPS;
#endif
}

ManglMainWindow::~ManglMainWindow() {}

void ManglMainWindow::initOpenGL(){}
void ManglMainWindow::releaseOpenGL(){}


void ManglMainWindow::onInit() {
    try {
        std::cout << "Mangl::initialize()" << std::endl;

        glewInit();

        EmscriptenWebGLContextAttributes attrs;
        emscripten_webgl_init_context_attributes(&attrs);
        attrs.enableExtensionsByDefault = 1;
        attrs.majorVersion = 2;
        attrs.minorVersion = 0;
        attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_DEFAULT;

        auto context = emscripten_webgl_create_context("#wasm_canvas", &attrs);

        massert(context, "Error creating canvas.context");

        memset(&attrs, -1, sizeof(attrs));
        auto res = emscripten_webgl_get_context_attributes(context, &attrs);

        massert(res == EMSCRIPTEN_RESULT_SUCCESS, "Error getting context atts");

        res = emscripten_webgl_make_context_current(context);
        massert(res == EMSCRIPTEN_RESULT_SUCCESS, "Error setting context");

        massert(emscripten_webgl_get_current_context() == context, "Invalid context");

        //throw std::runtime_error("Test error");

        //        int numExtensions = 0;
        //        glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
        //        bool hasGLExtension = false;
        //        for(int i = 0; i < numExtensions; ++i) {
        //            const char *ext = (const char *)glGetStringi(GL_EXTENSIONS, i);
        //            printf("extension %d: %s\n", i, ext);
        //            if (strstr(ext, "GL_") == 0)
        //                hasGLExtension = true;
        //        }
        //        massert(hasGLExtension, "GLExtention not found");


        auto env = Env::instance();

        mangl_main();
        MFrameworkNative::instance()->init();


    } catch (const std::exception& e) {
        mmCallbackError(e.what());
    } catch (...) {
        mmCallbackError("onWasmInit::Unhandled Exception!");
    }
}



void ManglMainWindow::onStart() {
    //emscripten_set_main_loop_arg(main_loop_cb, this, (int)fps_, 1);
    emscripten_set_main_loop_arg(main_loop_cb, this, 0, 0);
}

void ManglMainWindow::onStop() {}

void ManglMainWindow::onDestroy() {}


static Timestamp currentTimestamp{}, timestampDelta{}, timestampPeriod{};
static double lastTimerCounter{}, currentTimerCounter{};
static MFrameworkNative *fw_{};
static SceneRenderer *renderer{};


void ManglMainWindow::onTimer() {
    try {
        //double currentTime = emscripten_get_now();

        if (gameLoopNeedExit_) {
            releaseOpenGL();
            return;
        }

        if (!timerInitialized_) {
            //initOpenGL();
            //timerInitialized_ = true;

            //
            // Main Message cycle
            //
            fw_ = MFrameworkNative::instance();
            fw_->onStart();

            renderer = fw_->getRenderer();
            fw_->onViewport();


            lastTimerCounter = emscripten_get_now();

            currentTimestamp = 0;
            timestampDelta = 0;

            timerInitialized_ = true;


            fw_->onInitialized();
        }

        currentTimerCounter = emscripten_get_now();

        Timestamp elapsed = (currentTimerCounter - lastTimerCounter) / 1000;

#if MANGL_DEBUG_INCREMENTAL_TIMER
        currentTimestamp_ += elapsed < timestampPeriod_ ? elapsed : timestampPeriod_;
#else
        currentTimestamp += elapsed;
#endif

        timestamp_ = currentTimestamp;

        fw_->processTimer(timestamp_);

        renderer->onTimer(timestamp_);

        renderer->prepareScene();
        renderer->onRender();


    } catch (const std::exception& e) {
        mmCallbackError(e.what());
    } catch (...) {
        mmCallbackError("onWasmInit::Unhandled Exception!");
    }
}


EM_JS(void, showWasmError, (const char *err), {
    onWasmError(UTF8ToString(err));
})

void ManglMainWindow::mmCallbackError(const char* err) {
    std::cout << err << std::endl;
    showWasmError(err);
    emscripten_cancel_main_loop();
}

} // namespace mangl
