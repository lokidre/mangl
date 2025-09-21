/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "screen.h"
#include "app.h"

void Screen::onStart()
{
    buildEmptyBackground();
//    buildModel();
}

void Screen::buildEmptyBackground() {
    //using namespace mangl;

    auto scene = App::getScene();

    auto screenLayout = Env::fullLayout();

    //scene->newRect(0xcd853f, {0, 0, Env::applicationRect.w, 10});
    scene->newBox(0xcd853f, screenLayout);

    double chsize = 32; // state->checkboardSize;



    int rows = (int)screenLayout.h / chsize + 1;
    int cols = (int)screenLayout.w / chsize + 1;

    for (int j = 0; j < rows; ++j) {
        for (int i = 0; i < cols; ++i) {
            if ((i & 1) == (j & 1)) {
                scene->newBox(0xe0e0e0, {i * chsize, j * chsize, chsize, chsize});
            }
        }
    }
}


void Screen::onTouchesBegin(TouchesA touches)
{
    lastTouches_ = touches;
}

void Screen::onTouchesMove(TouchesA touches)
{
    if (modelPrim_) {

        auto delta = touches.touches[0].p - lastTouches_.touches[0].p;

        if (!delta)
            return;

        auto d = delta / (modelSphereRadius_ * 2);
        auto k = d * pi_;  // to radians

        MatrixGL xM, yM, zM;

        xM.rotationX(-k.y);
        yM.rotationY(-k.x);

        cognit::mtx_mul(yM, modelMatrix_, zM);
        cognit::mtx_mul(xM, zM, modelMatrix_);

        modelMatrix_.updateHash();
    }

    lastTouches_ = touches;
}

void Screen::onTouchesEnd(TouchesA touches)
{
}

