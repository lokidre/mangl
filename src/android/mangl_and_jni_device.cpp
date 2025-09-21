/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_jni.h"

#include "../core/mangl_util.h"
#include "../core/mangl_environment.h"

namespace mangl {

//static jmethodID jm_getUuidString;
static jmethodID jmGetDeviceUserName{};

void Jni::registerDeviceMethods() {
    activityMethods_.insert(activityMethods_.end(), {
		//{&jm_getUuidString, "jm_getUuidString", "()Ljava/lang/String;"},
		{&jmGetDeviceUserName, "getDeviceUserName", "()Ljava/lang/String;"},
	});
}

//MString MUUID::generateString() {
//	MString value;
//
//	auto jenv = getJniEnv();
//	auto jvalue = (jstring) jenv->CallStaticObjectMethod(jniNativesClass_, jm_getUuidString);
//
//	const char *valueStr = jenv->GetStringUTFChars(jvalue, 0);
//	if (valueStr) {
//		value = valueStr;
//		jenv->ReleaseStringUTFChars(jvalue, valueStr);
//	}
//
//	return value;
//}

String Env::deviceUserName() {
    return Jni::callActivityStringMethod(jmGetDeviceUserName);
}

}  // namespace mangl

