/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

namespace sample {


class TestScreen: public Screen {
protected:
    void onStart() override;
};


class Screen2: public Screen {
protected:
    void onStart() override {
        buildSampleBackground();
        buildTitle("Screen 2");

        gridAddScreenButton({
            .text = "Screen 1",
            .onPush = App::popScreen,
        });

        auto scene = App::getScene();

        scene->newBox(M::Red, {10, 60, 200, 20});
    }
};

class Screen1: public Screen {
protected:
    void onStart() override {
        buildSampleBackground();
        buildTitle("Screen 1");

        gridAddScreenButton({
            .text = "Screen 2",
            .onPush = App::pushScreenCreate<decltype(this), Screen2>,
        });

        auto scene = App::getScene();

        scene->newBox(M::Blue, {20, 40, 230, 40});
    }
};




void TestScreen::onStart()
{
    buildSampleBackground();
    buildTitle("Transitions");

    App::setScreenTransition({
        .transition = Transition::Fade,
        //.duration = 4,
        .scene = App::getScene(),
    });


    gridAddScreenButton({
        .text = "Fade",
        .onPush = App::pushScreenCreate<decltype(*this), Screen1>,
    });
}

void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);


    switchScreen<TestScreen>();
}

}  // namespace
