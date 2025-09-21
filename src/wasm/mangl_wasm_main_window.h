/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_types.h"
#include "../core/mangl_framework.h"
#include "../core/mangl_storage.h"

#include <apeal/singleton.hpp>

namespace mangl {

class ManglMainWindow: public apl::Singleton<ManglMainWindow, false> {
    using Self = ManglMainWindow;
    using SelfP = Self*;

public:
    Timestamp fps_{};

//private:
    bool gameLoopNeedExit_{};
    bool timerInitialized_{};
    Timestamp timestamp_{};


public:

    ManglMainWindow();
    ~ManglMainWindow();

    void onInit();
    void onStart();
    void onStop();
    void onDestroy();

    void initOpenGL();
    void releaseOpenGL();

    void onTimer();

private:
    void mmCallbackError(const char* err);

    static void main_loop_cb(void* arg) {
        //std::cout << "main_loop_cb()" << std::endl;
        static_cast<SelfP>(arg)->onTimer();
    }
};

}  // namespace mangl

