/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/

#include <test_app.h>

using namespace mangl;
class TestScreen: public ScreenBase {
protected: void onStart() override;
};
void TestScreen::onStart()
{
    auto scene = App::instance()->scene();

    scene->newTri({ 30,30 }, { 200,40 }, { 250,250 })->setColor({ 1, 0, 0 });

}

void  App::onStart() {
    if(sampleApplicationLayout_.h==0) {
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    } else {Env::setLayout(sampleApplicationLayout_);}
    switchScreen(new TestScreen);
}
