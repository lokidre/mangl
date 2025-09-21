/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_jni.h"

#include "../core/native/mangl_native_gui.h"

namespace mangl {

static jmethodID jmAlert;
static jmethodID jm_guiCreateLabel, jm_guiCreateTextInput;
static jmethodID jmGuiClearWidgets;
static jmethodID jm_guiSetTextColor, jm_guiSetBackgroundColor, jm_guiSetTextSize;
static jmethodID jm_guiGetText, jm_guiSetText;

void Jni::registerGuiMethods() {
    activityMethods_.insert(activityMethods_.end(), {
        {&jmAlert, "jmAlert", "(Ljava/lang/String;Ljava/lang/String;)V"},

        {&jm_guiCreateLabel, "jm_guiCreateLabel", "(Ljava/lang/String;IIII)I"},
        {&jm_guiCreateTextInput, "jm_guiCreateTextInput", "(Ljava/lang/String;IIII)I"},
        {&jmGuiClearWidgets, "jmGuiClearWidgets", "()V"},
        {&jm_guiSetTextColor, "jm_guiSetTextColor", "(IFFFF)V"},
        {&jm_guiSetBackgroundColor, "jm_guiSetBackgroundColor", "(IFFFF)V"},
        {&jm_guiSetTextSize, "jm_guiSetTextSize", "(IF)V"},
        {&jm_guiGetText, "jm_guiGetText", "(I)Ljava/lang/String;"},
        {&jm_guiSetText, "jm_guiSetText", "(ILjava/lang/String;)V"},
    });
}


void NativeGui::alert(AlertParamCR parm) {
    apl::ApiString message{parm.message};
    apl::ApiString title{parm.title};

    auto jenv = Jni::getJniEnv();
    auto jmessage = jenv->NewStringUTF(message.c_str());
    auto jtitle = jenv->NewStringUTF(title.c_str());
    jenv->CallStaticVoidMethod(Jni::getJavaActivityClass(), jmAlert, jmessage, jtitle);
    jenv->DeleteLocalRef(jmessage);
    jenv->DeleteLocalRef(jtitle);
}

NativeGui *NativeGui::clearWidgets() {
    Jni::callActivityVoidMethod(jmGuiClearWidgets);
    return this;
}

NativeGui *NativeGui::setVisible(WidgetId widgetId, bool visible) {
    return this;
}

NativeGui *NativeGui::move(WidgetId widgetId, RectA frame) {
    return this;
}

WidgetId NativeGui::createInput(StrView text, RectA frame, Flags flags) {
    auto screen = layoutToScreen(frame);
    auto y = layoutToScreen(Env::layoutSize.h - frame.y - frame.h);

    apl::ApiString apiText{text};

    auto jenv = Jni::getJniEnv();
    auto jclass = Jni::getJavaActivityClass();
    auto jname = jenv->NewStringUTF(apiText.c_str());
    int widgetId = jenv->CallStaticIntMethod(jclass,
                                             jm_guiCreateTextInput,
                                             jname,
                                             (jint)screen.x,
                                             (jint)y,
                                             (jint)screen.w,
                                             (jint)screen.h);
    jenv->DeleteLocalRef(jname);

    return widgetId;
}

void NativeGui::setInputPlaceholder(WidgetId, StrView text) {
    //return this;
}

WidgetId NativeGui::createLabel(StrView text, RectA rect, Flags flags) {
    auto screen = layoutToScreen(rect);

    //auto env = mangl_environment_;
    auto y = layoutToScreen(Env::layoutSize.h - rect.y - rect.h);

    apl::ApiString apiText{text};

    auto jenv = Jni::getJniEnv();
    auto jname = jenv->NewStringUTF(apiText.c_str());
    int widgetId = jenv->CallStaticIntMethod(Jni::getJavaActivityClass(),
                                             jm_guiCreateLabel,
                                             jname,
                                             (jint) screen.x,
                                             (jint) y,
                                             (jint) screen.w,
                                             (jint) screen.h);
    jenv->DeleteLocalRef(jname);

    return widgetId;
}

NativeGui *NativeGui::setText(WidgetId widgetId, StrView text) {

    apl::ApiString apiText{text};

    auto jenv = Jni::getJniEnv();
    auto jtext = jenv->NewStringUTF(apiText.c_str());
    jenv->CallStaticVoidMethod(Jni::getJavaActivityClass(), jm_guiSetText, widgetId, jtext);
    jenv->DeleteLocalRef(jtext);

    return this;
}

NativeGui *NativeGui::setFont(WidgetId widgetId, Cord size, StrView fontName) {
    auto jenv = Jni::getJniEnv();

    //if (fontName.empty()) {
    jenv->CallStaticVoidMethod(Jni::getJavaActivityClass(),
                               jm_guiSetTextSize,
                               widgetId,
                               (jfloat) layoutToScreen(size));
    //}

    return this;
}

NativeGui *NativeGui::setTextColor(WidgetId widgetId, ColorA color) {
    auto jenv = Jni::getJniEnv();
    jenv->CallStaticVoidMethod(Jni::getJavaActivityClass(),
                               jm_guiSetTextColor,
                               widgetId,
                               color.r,
                               color.g,
                               color.b,
                               color.a);
    return this;
}

NativeGui *NativeGui::setBackgroundColor(WidgetId widgetId, ColorA color) {
    auto jenv = Jni::getJniEnv();
    jenv->CallStaticVoidMethod(Jni::getJavaActivityClass(),
                               jm_guiSetBackgroundColor,
                               widgetId,
                               color.r,
                               color.g,
                               color.b,
                               color.a);
    return this;
}

String NativeGui::getText(WidgetId widgetId) {
    auto jenv = Jni::getJniEnv();
    auto jvalue = (jstring)jenv->CallStaticObjectMethod(Jni::getJavaActivityClass(),
                                                        jm_guiGetText,
                                                        widgetId);

    if (jvalue)
        return "NULL";

    const char *valueStr = jenv->GetStringUTFChars(jvalue, nullptr);
    if (!valueStr)
        return "NULL";

    String value = valueStr;

    jenv->ReleaseStringUTFChars(jvalue, valueStr);

    return value;
}

//extern "C" {
//
//JNIEXPORT void JNICALL Java_mangl_ManglMainActivityGUI_00024Companion_nOnAlertConfirmed__(JNIEnv *env, jclass cls) {
//    NativeGui::instance()->onAlertConfirmed(0);
//}
//
//}  // extern C


}  // namespace mangl

using namespace mangl;

extern "C"
JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityGui_nOnAlertConfirmed__(JNIEnv* env, jclass clazz) {
    NativeGui::instance()->onAlertConfirmed(0);
}

