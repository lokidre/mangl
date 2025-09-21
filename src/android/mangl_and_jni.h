/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <jni.h>
#include <android/asset_manager.h>

#include <game-activity/native_app_glue/android_native_app_glue.h>

#include <unistd.h>

#include "mangl_and_log.h"


#include "../core/mangl_setup.h"
#include "../core/mangl_types.h"

#include "mangl_and_debug.h"
#include "mangl_and_jni_utils.h"

namespace mangl {

//
// JNI Initialisation
//
struct JavaCallMethod {
    jmethodID *method;
    const char *name;
    const char *params;
};
using JniCallMethodsContainer = std::vector<JavaCallMethod>;


class Jni {
    struct JniEnvContext {
        JNIEnv* jenv;
        pid_t tid;
    };

    using JniEnvContextContainer = std::vector<JniEnvContext>;

    static JniCallMethodsContainer staticMethods_;
    static JniCallMethodsContainer activityMethods_;

    static JniEnvContextContainer jenvContextContainer_;
    static bool jniInitialized_;

//#if _DEBUG
    static void debugAssertMainThread();



public:
    // Initialization
    static void init();
    static void destroy();

    static void registerDeviceMethods();
    static void registerGuiMethods();
    static void registerInvitesMethods();
    static void registerMusicMethods();

    static void registerAdsMethods();
    static void registerBluetoothMethods();

    // Access to global variables
    static android_app* app;

    static auto getActivity() noexcept { return app->activity; }
    static auto getJavaActivity() noexcept { return app->activity->javaGameActivity; }
    static jclass getJavaActivityClass() {
        return getJniEnv()->GetObjectClass(app->activity->javaGameActivity);
    }


    static JNIEnv* getJniEnv();
    static JNIEnv* mainJenv;  // Main thread jenv
    static pid_t mainTid;
    static jobject jMainActivity;

    //
    // Accessing Java classes
    //
    static jclass findClass(StringA name);
    static String j2string(jobject jstr);
    static jmethodID getMethodId(jclass clazz, const char* name, const char* sig) {
        debugAssertMainThread();
        auto methodId = mainJenv->GetMethodID(clazz, name, sig);
        apl::verify(methodId != nullptr, "JNI:Cannot Find Method: %s - %s", name, sig);
        return methodId;
    }
    static jmethodID getActivityMethodId(const char* name, const char* sig) {
        return getMethodId(getJavaActivityClass(), name, sig);
    }

    //
    // Calling Java functions
    //
    static void callActivityVoidMethod(jmethodID methodID, ...) {
        debugAssertMainThread();

        va_list args;
        va_start(args, methodID);
        mainJenv->CallVoidMethodV(jMainActivity, methodID, args);
        va_end(args);
    }

    static bool callActivityBoolMethod(jmethodID methodID, ...) {
        debugAssertMainThread();

        va_list args;
        va_start(args, methodID);
        bool result = mainJenv->CallBooleanMethodV(jMainActivity, methodID, args);
        va_end(args);

        return result;
    }

    static float callActivityFloatMethod(jmethodID methodID, ...) {
        debugAssertMainThread();

        va_list args;
        va_start(args, methodID);
        float result = mainJenv->CallFloatMethodV(jMainActivity, methodID, args);
        va_end(args);

        return result;
    }


    static String callActivityStringMethod(jmethodID methodID, ...) {
        debugAssertMainThread();

        va_list args;
        va_start(args, methodID);
        auto result = mainJenv->CallObjectMethodV(jMainActivity, methodID, args);
        va_end(args);

        auto jvalue = reinterpret_cast<jstring>(result);

        const char *valueStr = mainJenv->GetStringUTFChars(jvalue, nullptr);

        String str{valueStr};

        mainJenv->ReleaseStringUTFChars(jvalue, valueStr);

        return str;
    }

    template <int N>
    static std::array<int, N> callActivityIntArrayMethod(jmethodID methodID, ...) {
        debugAssertMainThread();

        va_list args;
        va_start(args, methodID);
        auto result = mainJenv->CallObjectMethodV(jMainActivity, methodID, args);
        va_end(args);

        jboolean isCopy{};
        auto jvalue = reinterpret_cast<jintArray>(result);
        auto intsArray = mainJenv->GetIntArrayElements(jvalue, &isCopy);

        std::array<int, N> values;

        for (int i = 0; i < N; i++)
            values[i] = intsArray[i];

        mainJenv->ReleaseIntArrayElements(jvalue, intsArray, 0);

        return values;
    }



    static void callStaticVoidMethod(jmethodID methodID, ...) {
        debugAssertMainThread();

        auto jclass = getJavaActivityClass();

        va_list args;
        va_start(args, methodID);
        mainJenv->CallStaticVoidMethodV(jclass, methodID, args);
        va_end(args);
    }

    static bool callStaticBoolMethod(jmethodID methodID, ...) {
        debugAssertMainThread();

        auto jclass = getJavaActivityClass();

        va_list args;
        va_start(args, methodID);
        auto result = mainJenv->CallStaticBooleanMethod(jclass, methodID, args);
        va_end(args);

        return result;
    }

    static String callStaticStringMethod(jmethodID methodID, ...) {
        debugAssertMainThread();

        auto jclass = getJavaActivityClass();

        va_list args;
        va_start(args, methodID);
        auto result = mainJenv->CallStaticObjectMethod(jclass, methodID, args);
        va_end(args);

        return j2string(result);
    }



//    static void callVoidMethodThread(jmethodID methodID, ...) {
//        auto jclass = getJavaActivityClass();
//
//        va_list args;
//        va_start(args, methodID);
//        getJniEnv()->CallStaticVoidMethodV(jclass, methodID, args);
//        va_end(args);
//    }
//
//    static bool callBoolMethodThread(jmethodID methodID, ...) {
//        auto jclass = getJavaActivityClass();
//
//        va_list args;
//        va_start(args, methodID);
//        auto result = getJniEnv()->CallStaticBooleanMethod(jclass, methodID, args);
//        va_end(args);
//
//        return result;
//    }



    //
    // Display information
    //
    static Size getDisplaySize();
    static Cord getPixelDensity();
    static Margins getDisplayInsets();
    static bool isTablet();

    //
    // Lifecycle
    static void finishApp();

    //
    // Alerts and messages
    //
    static void die(StringA msg);
};



// TODO: layouts are used in Native GUI
namespace android {
Cord layoutToScreenK();
Cord screenToLayoutK();
}  // namespace android

inline Cord layoutToScreen(Cord layout) { return layout * android::layoutToScreenK(); }
inline Cord screenToLayout(Cord screen) { return screen * android::screenToLayoutK(); }

inline Rect layoutToScreen(RectA layout) {
    return {layoutToScreen(layout.x), layoutToScreen(layout.y), layoutToScreen(layout.w),
            layoutToScreen(layout.h)};
}

inline Rect screenToLayout(RectA layout) {
    return {screenToLayout(layout.x), screenToLayout(layout.y), screenToLayout(layout.w),
            screenToLayout(layout.h)};
}


//
// Settings
//
void jni_settingsSetString(const char *name, const char *value);
String jni_settingsStringValue(const char *name, const char *defaultValue);

}  // namespace mangl

