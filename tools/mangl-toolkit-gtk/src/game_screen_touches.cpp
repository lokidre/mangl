/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "game_screen.h"

namespace toolkit {

void GameScreen::onTouchDown(TouchA ti)
{
    lastTouches_ = ti;
}

void GameScreen::onTouchMove(TouchA ti)
{
    if (modelPrim_) {

        auto delta = ti.p - lastTouches_.p;

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

    lastTouches_ = ti;
}

void GameScreen::onTouchUp(TouchA ti)
{
}

}  // namespace toolkit
