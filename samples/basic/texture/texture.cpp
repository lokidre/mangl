/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <test_app.h>

#include <mangl/texture.h>
#include <mangl/widgets.h>
#include <mangl/resources.h>


class TestScreen: public ScreenBase {
protected:
    void onStart() override;

private:
    TextureAtlas gameTex;
};


void TestScreen::onStart() {
    auto scene = App::getScene();

    gameTex.load(Resources::bundleDirJoin("texture"));

    Rect imageRect{10, 10, 300, 300};

    scene->newImage(imageRect, gameTex["lenna"]);
}


void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
