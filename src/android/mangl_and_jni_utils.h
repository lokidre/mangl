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

namespace mangl {

struct JniString {
    jstring jstr{};
    JNIEnv* jenv{};

    JniString() = delete;

    explicit JniString(StringA string);

    ~JniString() noexcept { release(); }

    void release() noexcept {
        //MDebugFunc();
        if (jstr) {
            jenv->DeleteLocalRef(jstr);
            jstr = nullptr;
        }
    }

    //
    // Move and copy operators and constructors
    //
    JniString(const JniString&) = delete;
    JniString& operator=(const JniString& other) = delete;

    JniString(JniString&& other) noexcept {
        swap(other);
    }
    JniString& operator=(JniString&& other) noexcept {
        swap(other);
        return *this;
    }

    void swap(JniString& other) {
        std::swap(jstr, other.jstr);
        std::swap(jenv, other.jenv);
    }

};

struct JniByteArray {
    jbyteArray jdata{};
    JNIEnv* jenv{};

    JniByteArray() = delete;

    explicit JniByteArray(DataParamA data): JniByteArray(data.data(), data.size()){}

    explicit JniByteArray(const void* ptr, std::size_t len);

    ~JniByteArray() noexcept { release(); }

    void release() noexcept {
        if (jdata) {
            jenv->DeleteLocalRef(jdata);
            jdata = nullptr;
        }
    }

    //
    // Move and copy operators and constructors
    //
    JniByteArray(const JniByteArray&) = delete;
    JniByteArray& operator=(const JniByteArray& other) = delete;

    JniByteArray(JniByteArray&& other) noexcept {
        swap(other);
    }
    JniByteArray& operator=(JniByteArray&& other) noexcept {
        swap(other);
        return *this;
    }

    void swap(JniByteArray& other) {
        std::swap(jdata, other.jdata);
        std::swap(jenv, other.jenv);
    }


};




String j2string(JNIEnv *env, jstring js);

//inline String j2string(JNIEnv *env, jobject js) {
//    return j2string(env, js);
//}

//jclass jFindClass(JNIEnv *env, const char *name);
//jclass jGetObjectClass(JNIEnv *env, jobject object);
//jmethodID jGetMethodID(JNIEnv *env, jclass clazz, const char *name, const char *type, const char *args = 0);
//jobject jCallObjectMethod(JNIEnv *env, jobject clazz, jmethodID method);

}  // namespace mangl
