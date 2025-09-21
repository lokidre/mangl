/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_jni_bluetooth.h"
#include "mangl_and_main_engine.h"

#if MANGL_BLUETOOTH_JNI

namespace mangl {

static jmethodID jmBluetoothIsAvailable;
//static jmethodID jmBluetoothIsEnabled;

static jmethodID jmBleStartAdvertising, jmBleStopAdvertising;
static jmethodID jmBlePeripheralSendData;
static jmethodID jmBlePeripheralShutdown, jmBlePeripheralDisconnect;

static jmethodID jmBleStartScanning, jmBleStopScanning;
static jmethodID jmBleCentralSendData;


void Jni::registerBluetoothMethods() {
    activityMethods_.insert(activityMethods_.end(), {
        {&jmBluetoothIsAvailable, "jmBluetoothIsAvailable", "()Z"},
        //{&jmBluetoothIsEnabled, "jmBluetoothIsEnabled", "()Z"},

        // Peripheral side
        {&jmBleStartAdvertising, "jmBleStartAdvertising", "()V"},
        {&jmBleStopAdvertising, "jmBleStopAdvertising", "()V"},
        {&jmBlePeripheralSendData, "jmBlePeripheralSendData", "(I[B)V"},
        {&jmBlePeripheralDisconnect, "jmBlePeripheralDisconnect", "(I)V"},
        {&jmBlePeripheralShutdown, "jmBlePeripheralShutdown", "()V"},

        // Client side
        {&jmBleStartScanning, "jmBleStartScan", "()V"},
        {&jmBleStopScanning, "jmBleStopScan", "()V"},
        {&jmBleCentralSendData, "jmBleCentralSendData", "([B)V"},

    });
}

}  // namespace mangl



namespace mangl::net {

constexpr auto BleCallbackActionError = 1;
constexpr auto BleCallbackActionStatus = 2;

constexpr auto BleCallbackCodeError = 1;
constexpr auto BleCallbackCodeConnected = 2;
constexpr auto BleCallbackCodeDisconnected = 3;
constexpr auto BleCallbackCodeConnecting = 4;





static BluetoothServerJni* bluetoothServerJni_{};
static BluetoothClientJni* bluetoothClientJni_{};


//
// Server side
//

bool BluetoothServerJni::isBluetoothAvailable() {
    auto btAvail = Jni::callActivityBoolMethod(jmBluetoothIsAvailable);
    MDebugFunc("BT isAvailable: %d", btAvail);
    return btAvail;
}

void BluetoothServerJni::doStart() {
    if (!isBluetoothAvailable()) {
        onError(0, ErrorInfo{"Bluetooth is not available"});
        return;
    }

    if (bluetoothServerJni_ != nullptr && bluetoothServerJni_ != this)
        mthrowError("Duplicate Bluetooth Server instantiated. Only one supported");

    bluetoothServerJni_ = this;


    //auto isEnabled = Jni::callActivityBoolMethod(jmBluetoothIsEnabled);
    //MDebugFunc("BT isEnabled: %d", isEnabled);

//    if (!isEnabled)
//        Jni::callActivityVoidMethod(jmBlePeripheralEnable);
}

void BluetoothServerJni::doSend(NetworkDataSendParamsA params) {
    JniByteArray data{params.data};
    Jni::callActivityVoidMethod(jmBlePeripheralSendData, params.pk, data.jdata);
}


void BluetoothServerJni::doDisconnect(PeerKey pk) {
    Jni::callActivityVoidMethod(jmBlePeripheralDisconnect, pk);
}


void BluetoothServerJni::doShutdown() {
    Jni::callActivityVoidMethod(jmBlePeripheralShutdown);
    bluetoothServerJni_ = nullptr;
}


void BluetoothServerJni::doStartAdvertising() {
    Jni::callActivityVoidMethod(jmBleStartAdvertising);
}

void BluetoothServerJni::doStopAdvertising() {
    Jni::callActivityVoidMethod(jmBleStopAdvertising);
}


//
// Client side
//
bool BluetoothClientJni::isBluetoothAvailable() {
    return Jni::callActivityBoolMethod(jmBluetoothIsAvailable);
}

void BluetoothClientJni::doStartDiscovery(DiscoveryInfoA di) {
    Base::doStartDiscovery(di);

    if (!isBluetoothAvailable()) {
        onError(0, ErrorInfo{"Bluetooth is not available"});
        return;
    }

    if (bluetoothClientJni_ != nullptr && bluetoothClientJni_ != this)
        mthrowError("Duplicate Bluetooth Central instantiated. Only one supported");

    bluetoothClientJni_ = this;


//    auto isEnabled = Jni::callActivityBoolMethod(jmBluetoothIsEnabled);
//
//    MDebugFunc("BT isEnabled: %d", isEnabled);

//    if (!isEnabled)
//        Jni::callActivityVoidMethod(jmBluetoothEnable);

    Jni::callActivityVoidMethod(jmBleStartScanning);
}

void BluetoothClientJni::doStopDiscovery() {
    Base::doStopDiscovery();
    Jni::callActivityVoidMethod(jmBleStopScanning);
}

void BluetoothClientJni::doSend(NetworkDataSendParamsA params) {
    JniByteArray data{params.data};
    Jni::callActivityVoidMethod(jmBleCentralSendData, data.jdata);
}




}  // namespace mangl::net

#endif  // If bluetooth enabled

using namespace mangl;
using namespace mangl::net;


//enum class BluetoothCallbackAction {
//};


extern "C"
JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityBlePeripheral_jniBlePeripheralStatusCallback__IILjava_lang_String_2(JNIEnv* env,
                                                                                          jclass clazz,
                                                                                          jint action,
                                                                                          jint code,
                                                                                          jstring jmessage) {
#if MANGL_BLUETOOTH_JNI
    if (!bluetoothServerJni_) {
        if (BleCallbackActionError == action) {
            Jni::die(j2string(env, jmessage));
        }
        return;
    }

    switch (action) {
    case BleCallbackActionError: {
        auto text = j2string(env, jmessage);
        auto& eq = bluetoothServerJni_->getEventsQueue();
        eq.pushError(0, code, text);
        break;
    }
    default:
        break;
    }


#endif
}

extern "C"
JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityBleCentral_jniBleCentralStatusCallback__IILjava_lang_String_2(JNIEnv* env,
                                                                                       jclass clazz,
                                                                                       jint action,
                                                                                       jint code,
                                                                                       jstring message) {
#if MANGL_BLUETOOTH_JNI
    if (!bluetoothClientJni_) {
        if (BleCallbackActionError == action) {
            Jni::die(j2string(env, message));
        }
        return;
    }

    switch (action) {
    case BleCallbackActionError: {
        auto text = j2string(env, message);
        auto& eq = bluetoothClientJni_->getEventsQueue();
        eq.pushError(0, code, text);
        break;
    }
    case BleCallbackActionStatus: {
        auto& eq = bluetoothClientJni_->getEventsQueue();
        switch (code) {
        case BleCallbackCodeConnected: eq.pushState(0, PeerState::Connected); break;
        case BleCallbackCodeDisconnected: eq.pushState(0, PeerState::Disconnected); break;
        case BleCallbackCodeConnecting: eq.pushState(0, PeerState::Connecting); break;
        default:
            break;
        }
        break;
    }

    default:
        break;
    }
#endif
}

extern "C"
JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityBleCentral_jniBleCentralReadDataCallback___3B(JNIEnv* env,
                                                                          jclass clazz,
                                                                          jbyteArray jdata) {
#if MANGL_BLUETOOTH_JNI
    if (!bluetoothClientJni_) return;

    int jlen = env->GetArrayLength(jdata);

    auto& eq = bluetoothClientJni_->getEventsQueue();

    eq.pushData(0, jlen, [env, jdata](auto buf, auto len) {
        env->GetByteArrayRegion(jdata, 0, len, reinterpret_cast<jbyte*>(buf));
    });

#endif
}
extern "C"
JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityBlePeripheral_jniBlePeripheralSubscribed__ILjava_lang_String_2(
    JNIEnv* jenv,
    jclass,
    jint pk,
    jstring jdi) {

#if MANGL_BLUETOOTH_JNI
    if (!bluetoothServerJni_) return;


    auto& eq = bluetoothServerJni_->getEventsQueue();
    auto sjdi = j2string(jenv, jdi);

    MDebugFunc("di: %s", sjdi);

    eq.pushFound(pk, sjdi);
    eq.pushState(pk, PeerState::Connected);
#endif
}

extern "C"
JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityBlePeripheral_jniBlePeripheralRecv__I_3B(
    JNIEnv* env,
    jclass,
    jint pk,
    jbyteArray jdata) {
#if MANGL_BLUETOOTH_JNI
    if (!bluetoothServerJni_) return;
    auto& eq = bluetoothServerJni_->getEventsQueue();

    int jlen = env->GetArrayLength(jdata);

    eq.pushData(pk, jlen, [env, jdata](auto buf, auto len) {
        env->GetByteArrayRegion(jdata, 0, len, reinterpret_cast<jbyte*>(buf));
    });
#endif
}