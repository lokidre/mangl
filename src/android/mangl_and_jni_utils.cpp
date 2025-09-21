/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_jni_utils.h"
#include "mangl_and_jni.h"

namespace mangl {

//
// JNI calling helper classes
//
JniString::JniString(StringA string) {
    //MDebugFunc("%s", string.c_str());
    jenv = Jni::getJniEnv();
    jstr = jenv->NewStringUTF(string.c_str());
}

JniByteArray::JniByteArray(const void* data, std::size_t len) {
    //MDebugFunc("%s", string.c_str());
    jenv = Jni::getJniEnv();
    jdata = jenv->NewByteArray((jsize)len);
    if (jdata == nullptr)
        mthrowError("JNI data allocation error");

    jenv->SetByteArrayRegion(jdata, 0, (jsize)len, (jbyte*)data);
}



String j2string(JNIEnv *jenv, jstring js) {
    if (!js) return {};

    const char *ptr = jenv->GetStringUTFChars(js, nullptr);
    if (!ptr) return {};

    String s{ptr};
    jenv->ReleaseStringUTFChars(js, ptr);
    return s;
}

//jclass jFindClass(JNIEnv *env, const char *name) {
//    jclass clazz = env->FindClass(name);
//
//    apl::verify(clazz != nullptr, "JNI:Cannot Find Class: %s", name);
//
//    return clazz;
//}

//jclass jGetObjectClass(JNIEnv *env, jobject object) {
//    jclass clazz = env->GetObjectClass(object);
//
//    if (clazz == nullptr)
//        throw std::runtime_error("JNI:Cannot Get Object Class");
//
//    return clazz;
//}
//
//jmethodID jGetMethodID(JNIEnv *jenv, jclass clazz, const char *name, const char *type, const char *args)
//{
//    auto method = jenv->GetMethodID(clazz, name, type);
//
//    apl::verify(method != nullptr, "JNI:Cannot Find Method: %s", name);
//
//    return method;
//}
//
//jobject jCallObjectMethod(JNIEnv *env, jobject clazz, jmethodID method) {
//    return env->CallObjectMethod(clazz, method);
//}

} // namespace mangl
