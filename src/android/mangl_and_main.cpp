/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_main_engine.h"

extern "C" {

void android_main(struct android_app* app) {
    auto engine = new mangl::AndroidMainEngine(app);
    engine->mainLoop();
    delete engine;
}

}  // extern C
