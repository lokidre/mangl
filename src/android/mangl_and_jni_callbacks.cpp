/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_common.h"

#include "mangl_and_assets.h"
#include "mangl_and_keyboard.h"

#include "../core/mangl_framework.h"

namespace mangl {

namespace android {

Cord layoutToScreenK() {
    return Env::physScreenSize.w / Env::layoutSize.w;
}

Cord screenToLayoutK() {
    return Env::layoutSize.w / Env::physScreenSize.w;
}

}  // namespace android

}  // namespace mangl

static bool mainLaunched_ = false;

using namespace mangl;
//using namespace mangl::android;

extern "C" {


//JNIEXPORT void JNICALL Java_mangl_MMainActivityJNI_nOnLoad(JNIEnv *env, jclass cls) {
//    logi("nOnLoad()");
//    //android_main() ;
//}


}


