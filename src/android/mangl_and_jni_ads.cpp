/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_jni.h"

#if MANGL_ADS

#include "../core/mangl_ads.h"
#include "../core/mangl_framework.h"
#include "../core/mangl_dispatch.h"
#include "../core/audio/mangl_game_music.h"

namespace mangl {

constexpr static int AdNetworkAdMob = 1;

static jmethodID jniAdsBannerShow{}, jniAdsBannerHide{};
static jmethodID jniAdsInterstitialLoad{}, jniAdsInterstitialShow{}, jniAdsInterstitialIsReady{};
static jmethodID jniAdsAppOpenLoad{}, jniAdsAppOpenShow{}, jniAdsAppOpenIsReady{};
static jmethodID jniAdsRewardedLoad{}, jniAdsRewardedShow{}, jniAdsRewardedIsReady{};

void Jni::registerAdsMethods() {
    activityMethods_.insert(activityMethods_.end(), {
        {&jniAdsBannerShow, "jniAdsBannerShow", "(ILjava/lang/String;FFFF)V"},
        {&jniAdsBannerHide, "jniAdsBannerHide", "(I)V"},

        {&jniAdsInterstitialLoad, "jniAdsInterstitialLoad", "(ILjava/lang/String;)V"},
        {&jniAdsInterstitialShow, "jniAdsInterstitialShow", "(ILjava/lang/String;F)V"},
        {&jniAdsInterstitialIsReady, "jniAdsInterstitialIsReady", "(ILjava/lang/String;)Z"},

        {&jniAdsAppOpenLoad, "jniAdsAppOpenLoad", "(ILjava/lang/String;)V"},
        {&jniAdsAppOpenShow, "jniAdsAppOpenShow", "(ILjava/lang/String;)V"},
        {&jniAdsAppOpenIsReady, "jniAdsAppOpenIsReady", "(ILjava/lang/String;)Z"},

        {&jniAdsRewardedLoad, "jniAdsRewardedLoad", "(ILjava/lang/String;)V"},
        {&jniAdsRewardedShow, "jniAdsRewardedShow", "(ILjava/lang/String;)V"},
        {&jniAdsRewardedIsReady, "jniAdsRewardedIsReady", "(ILjava/lang/String;)Z"},
    });
}


void Ads::showBanner() {
    if (!Ads::enabled())
        return;

    bannerHidden = false;
    auto layout = getBannerRect();
    auto rect = Env::toScreen(layout);

    MDebugFunc("Rect: {%z}", rect.convert<RectI>());

    int network = AdNetworkAdMob;

    auto x = static_cast<jfloat>(rect.x);
    auto y = static_cast<jfloat>(rect.y);
    auto w = static_cast<jfloat>(rect.w);
    auto h = static_cast<jfloat>(rect.h);

    JniString unitId{MANGL_ADS_BANNER_ADMOB_UNIT_ID};
    Jni::callActivityVoidMethod(jniAdsBannerShow, network, unitId.jstr, x, y, w, h);
}

void Ads::hideBanner() {
    bannerHidden = true;
//    Dispatch::asyncMain([] {
        int network = AdNetworkAdMob;
        Jni::callActivityVoidMethod(jniAdsBannerHide, network);
//    });
}

AdStatus Ads::getInterstitialStatus() noexcept {
    AdStatus status{};

#if MANGL_ADS_INTERSTITIAL_ADMOB
    JniString unitId{MANGL_ADS_INTERSTITIAL_ADMOB_UNIT_ID};
    status.ready = Jni::callActivityBoolMethod(jniAdsInterstitialIsReady, AdNetworkAdMob, unitId.jstr);
#endif

//    if (status.ready) {
//        status.bid = 0;
//    }

    return status;
}


void Ads::showInterstitial() {
    if (!Ads::enabled())
        return;

    Dispatch::asyncMain([] {
        int network = AdNetworkAdMob;
        JniString unitId{MANGL_ADS_INTERSTITIAL_ADMOB_UNIT_ID};
        Jni::callActivityVoidMethod(jniAdsInterstitialShow, network, unitId.jstr, (jfloat) 0);
    });
}

void Ads::showInterstitialDelayed(Timestamp delay) {
    if (!Ads::enabled())
        return;

    Dispatch::asyncMain([delay] {
        int network = AdNetworkAdMob;
        JniString unitId{MANGL_ADS_INTERSTITIAL_ADMOB_UNIT_ID};
        Jni::callActivityVoidMethod(jniAdsInterstitialShow, network, unitId.jstr, (jfloat) delay);
    });
}

//
// App Open ads
//

bool Ads::isAppOpenReady() noexcept {
    bool ready = false;

#ifdef MANGL_ADS_APPOPEN_ADMOB_UNIT_ID
    JniString unitId{MANGL_ADS_APPOPEN_ADMOB_UNIT_ID};
    ready = Jni::callActivityBoolMethod(jniAdsAppOpenIsReady, AdNetworkAdMob, unitId.jstr);
#endif

    return ready;
}

void Ads::showAppOpen() {
    if (!Ads::enabled())
        return;

#ifdef MANGL_ADS_APPOPEN_ADMOB_UNIT_ID
    Dispatch::asyncMain([] {
        int network = AdNetworkAdMob;
        JniString unitId{MANGL_ADS_APPOPEN_ADMOB_UNIT_ID};
        Jni::callActivityVoidMethod(jniAdsAppOpenShow, network, unitId.jstr);
    });
#endif
}


static bool appOpenShowScheduled_ = false;

void Ads::scheduleAppOpen() {
    if (isAppOpenReady()) {
        showAppOpen();
        return;
    }

    appOpenShowScheduled_ = true;
}

void Ads::dismissAppOpen() {
    appOpenShowScheduled_ = false;
}




//
// Rewarded ads
//
bool Ads::isRewardedReady() noexcept {
#if MANGL_ADS_REWARDED_ADMOB
    JniString unitId{MANGL_ADS_REWARDED_ADMOB_UNIT_ID};
    return Jni::callActivityBoolMethod(jniAdsRewardedIsReady, AdNetworkAdMob, unitId.jstr);
#else
    return false;
#endif
}

void Ads::showRewarded() {
#if MANGL_ADS_REWARDED_ADMOB
    JniString unitId{MANGL_ADS_REWARDED_ADMOB_UNIT_ID};
    Jni::callActivityVoidMethod(jniAdsRewardedShow, AdNetworkAdMob, unitId.jstr);
#endif
}


}  // namespace mangl

using namespace mangl;

extern "C" {

JNIEXPORT void JNICALL Java_mangl_ManglMainActivityAds_jniOnAdsInitialized(JNIEnv *env, jclass cls, jint network) {
    // Ads initilized. Request to load interstitial
#if MANGL_ADS_INTERSTITIAL_ADMOB
    Dispatch::asyncMain([network] {
        JniString unitId{MANGL_ADS_INTERSTITIAL_ADMOB_UNIT_ID};
        Jni::callActivityVoidMethod(jniAdsInterstitialLoad, network, unitId.jstr);
    });
#endif
}


JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityAds_jniOnAdsBannerWillPresentFullScreen__(JNIEnv *env, jclass cls) {
    Dispatch::asyncMain([] {
        auto fw = Framework::instance();
        fw->onInterstitialWillPresentFullScreen();
        fw->onPause();

        GameMusic::processAdWillPresentFullScreen();
    });
}

JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityAds_jniOnAdsBannerDidDismissedFullScreen__(JNIEnv *env, jclass cls) {
    Dispatch::asyncMain([] {
        Framework::instance()->onAdsDidDismissFullScreen();
        GameMusic::processAdDidDismissFullScreen();
    });
}


JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityAds_jniOnAdsInterstitialWillPresentFullScreen__Ljava_lang_String_2(
    JNIEnv* env,
    jclass clazz,
    jstring jUnitId) {

    auto unitId = j2string(env, jUnitId);

    Dispatch::asyncMain([unitId] {
        auto fw = Framework::instance();
        fw->onInterstitialWillPresentFullScreen();
        fw->onPause();

        GameMusic::processAdWillPresentFullScreen();

        if (Ads::onInterstitialShown) Ads::onInterstitialShown(unitId);
    });
}


JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityAds_jniOnAdsInterstitialDidDismissFullScreen__Ljava_lang_String_2(JNIEnv* env,
                                                                                              jclass clazz,
                                                                                              jstring jUnitId) {
    auto unitId = j2string(env, jUnitId);

    Dispatch::asyncMain([unitId] {
        Framework::instance()->onAdsDidDismissFullScreen();
        GameMusic::processAdDidDismissFullScreen();
        if (Ads::onInterstitialDismissed) Ads::onInterstitialDismissed(unitId);
    });
}

//
// App Open
//
JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityAds_jniOnAdsAppOpenWillPresentFullScreen__Ljava_lang_String_2(
    JNIEnv* env,
    jclass clazz,
    jstring jUnitId) {

    auto unitId = j2string(env, jUnitId);

    Dispatch::asyncMain([unitId] {
        auto fw = Framework::instance();
        fw->onInterstitialWillPresentFullScreen();
        fw->onPause();

        GameMusic::processAdWillPresentFullScreen();

        if (Ads::onAppOpenShown) Ads::onAppOpenShown(unitId);
    });
}


JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityAds_jniOnAdsAppOpenDidDismissFullScreen__Ljava_lang_String_2(JNIEnv* env,
                                                                                              jclass clazz,
                                                                                              jstring jUnitId) {
    auto unitId = j2string(env, jUnitId);

    Dispatch::asyncMain([unitId] {
        Framework::instance()->onAdsDidDismissFullScreen();
        GameMusic::processAdDidDismissFullScreen();
        if (Ads::onAppOpenDismissed) Ads::onAppOpenDismissed(unitId);
    });
}


//
// Rewarded
//

JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityAds_jniOnAdsReward__Ljava_lang_String_2I(JNIEnv* env,
                                                                     jclass clazz,
                                                                     jstring jreward,
                                                                     jint jamount) {
    auto reward = j2string(env, jreward);
    auto amount = static_cast<double>(jamount);

    Dispatch::asyncMain([reward, amount] {
        if (Ads::onRewardEarned)
            Ads::onRewardEarned(reward, amount);
    });
}

JNIEXPORT void JNICALL
Java_mangl_ManglMainActivityAds_jniOnAdsRewardError__Ljava_lang_String_2(JNIEnv* env,
                                                                         jclass clazz,
                                                                         jstring jmessage) {
    auto message = j2string(env, jmessage);

    Dispatch::asyncMain([message] {
        if (Ads::onRewardedError)
            Ads::onRewardedError(message);
    });
}

}  // Extern C



#endif

