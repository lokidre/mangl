/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_common.h"
#include "mangl_and_main_engine.h"

#include <sys/types.h>
#include <unistd.h>

#include <apeal/util.hpp>

#include "mangl_and_jni.h"

#include "../core/mangl_environment.h"

#include "mangl_and_jni_bluetooth.h"

namespace mangl {

//
// Static members
//
android_app* Jni::app{};
Jni::JniEnvContextContainer Jni::jenvContextContainer_{};
bool Jni::jniInitialized_{false};
JNIEnv* Jni::mainJenv{};  // Main thread jenv
pid_t Jni::mainTid{};
jobject Jni::jMainActivity{};

JniCallMethodsContainer Jni::staticMethods_;
JniCallMethodsContainer Jni::activityMethods_;






bool dead_{false};
AAssetManager* assetManager_{};

// Lifecycle
jmethodID jmFinishApp{};
jmethodID jmFinishAppStatic{};

// Display information
jmethodID jmGetDisplaySize{}, jmGetPixelDensity{}, jmGetDisplayInsets{}, jmIsTablet{};


// Setup
jmethodID jmDie{};

// Settings
jmethodID jm_settingsSetString{};
jmethodID jm_settingsStringValue{};


JNIEnv* Jni::getJniEnv() {
    auto tid = gettid();

    //MDebugFunc("tid: %d", tid);

    for (auto& ctx: jenvContextContainer_) {
        if (ctx.tid == tid)
            return ctx.jenv;
    }

    JNIEnv* jenv = nullptr;

    auto res = Jni::getActivity()->vm->AttachCurrentThread(&jenv, nullptr);

    if (res) {
        *((volatile char*)nullptr) = 'a'; // Trigger crash
    }

    jenvContextContainer_.push_back({
        .jenv = jenv,
        .tid = tid,
    });

    if (!mainJenv) {
        mainJenv = jenv;
        mainTid = tid;
    }

    return jenv;
}

void Jni::init() {
    //MDebugFunc("tid: %d", gettid());

    if (jniInitialized_)
        return;

    jniInitialized_ = true;

    staticMethods_.clear();
    activityMethods_.clear();

    staticMethods_.clear();
    activityMethods_.clear();

    staticMethods_.insert(staticMethods_.end(), {
        {&jmFinishAppStatic, "jmFinishAppStatic", "()V"},
    });

    activityMethods_.insert(activityMethods_.end(), {
        {&jmFinishApp, "jmFinishApp", "()V"},
        {&jmDie, "jmDie", "(Ljava/lang/String;)V"},

        {&jmGetDisplaySize, "jniGetDisplaySize", "()[I"},
        {&jmGetPixelDensity, "jniGetPixelDensity", "()F"},
        {&jmGetDisplayInsets, "jmGetDisplayInsets", "()[I"},
        {&jmIsTablet, "jniIsTablet", "()Z"},


        {&jm_settingsSetString, "settingsSetString", "(Ljava/lang/String;Ljava/lang/String;)V"},
        {&jm_settingsStringValue, "settingsGetString", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"},
    });


    registerDeviceMethods();
    registerGuiMethods();
    registerInvitesMethods();

#if MANGL_ADS
    registerAdsMethods();
#endif

#if MANGL_BLUETOOTH_JNI
    registerBluetoothMethods();
#endif

#if MANGL_MUSIC_JNI
    registerMusicMethods();
#endif

    auto jclass = getJavaActivityClass();
    jMainActivity = getJavaActivity();

    //auto jclass = jenv->FindClass("org/manglsample/mangl_sample_nearby_pong/MainApplication");
    apl::verify(jclass, "Cannot find MainActivityJNI class");


    for (auto& m: staticMethods_) {
        auto methodId = mainJenv->GetStaticMethodID(jclass, m.name, m.params);
        apl::verify(methodId, "JNI:Cannot load static method: %s", m.name);
        *(m.method) = methodId;
    }

    for (auto& m: activityMethods_) {
        auto methodId = mainJenv->GetMethodID(jclass, m.name, m.params);
        apl::verify(methodId, "JNI:Cannot load Main Activity method: %s", m.name);
        *(m.method) = methodId;
    }
}

void Jni::destroy() {
    jniInitialized_ = false;
    jenvContextContainer_.clear();
    mainJenv = nullptr;
    mainTid = -1;
    jMainActivity = {};
}

//
// Threading
//
void Jni::debugAssertMainThread() {
    if (gettid() != mainTid) {
        auto msg = "JNI called from another thread";
        loge("%s", msg);
        throw std::runtime_error(msg);
    }
}


//
// Java class access
//
jclass Jni::findClass(apl::StringA name) {
    debugAssertMainThread();

    jclass clazz = mainJenv->FindClass(name.c_str());

    apl::verify(clazz != nullptr, "JNI:Cannot Find Class: %s", name);

    return clazz;
}

String Jni::j2string(jobject jobj) {
    auto jstr = reinterpret_cast<jstring>(jobj);
    const char *ptr = mainJenv->GetStringUTFChars(jstr, nullptr);

    String s{ptr};

    mainJenv->ReleaseStringUTFChars(jstr, ptr);

    return s;
}



//
// Lifecycle
//
void Jni::finishApp() {
    callStaticVoidMethod(jmFinishAppStatic);
    //callActivityVoidMethod(jmFinishApp);
}

//
// Display information
//
Size Jni::getDisplaySize() {
    auto jsize = callActivityIntArrayMethod<2>(jmGetDisplaySize);
    return {
        .w = static_cast<Cord>(jsize[0]),
        .h = static_cast<Cord>(jsize[1]),
    };
}

Cord Jni::getPixelDensity() {
    auto density = callActivityFloatMethod(jmGetPixelDensity);
    return static_cast<Cord>(density);
}

Margins Jni::getDisplayInsets() {
    auto insets = callActivityIntArrayMethod<4>(jmGetDisplayInsets);
    return {
        .t = static_cast<Cord>(insets[0]),
        .r = static_cast<Cord>(insets[1]),
        .b = static_cast<Cord>(insets[2]),
        .l = static_cast<Cord>(insets[3]),
    };
}

bool Jni::isTablet() {
    return callActivityBoolMethod(jmIsTablet);
}


//
// Messages and alerts
//
void Jni::die(StringA msg) {
    loge(" +++ dying +++ %s", msg.c_str());

    AndroidMainEngine::dead_ = true;

    JniString jmessage{msg};
    Jni::callActivityVoidMethod(jmDie, jmessage.jstr);
}




} // namespace mangl



#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
    logd("JNI_OnLoad");
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void*) {
    logd("JNI_OnUnload");
    //android::javaVm_ = nullptr;
}

void on_quit_application() {
    // D_QuitNetGame ();
    //I_ShutdownSound();
    //I_ShutdownMusic();
    //M_SaveDefaults ();
    //I_ShutdownGraphics();
    exit(0);
}

#ifdef __cplusplus
}
#endif

