/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

#include <mangl/ads.h>

namespace sample {

class TestScreen: public Screen {
protected:
    void onStart() override {
        sampleBuildBackground();
        sampleBuildStatusBar();

        sampleGridBuildScreenButton({
            .text = "Show Banner",
            .onPush = [this] {
                Ads::showBanner();
                sampleSetStatusMessage("Showing Banner");
            },
        });


        sampleGridBuildScreenButton({
            .text = "Hide Banner",
            .onPush = [this] {
                Ads::hideBanner();
                sampleSetStatusMessage("Hiding Banner");
            },
        });

        sampleGridBuildScreenButton({
            .text = "Show Interstitial",
            .onPush = [this] {
                Ads::showInterstitial();
                sampleSetStatusMessage("Showing Interstitial");
            },
        });

        sampleGridBuildScreenButton({
            .text = "Interstitial in 2s",
            .onPush = [this] {
                Ads::showInterstitialDelayed(2);
                sampleSetStatusMessage("Showing Interstitial in 2s");
            },
        });

        Ads::setBannerLayout({320, 50});
    }
};


void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

}  // namespace
