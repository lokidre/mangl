/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_game_screen.h"

namespace mangl {


void GameScreenBase::initGameScreenLayout() {
    if (!haveScreenLayoutParams_)
        return;

    auto full = fullArea_; //Env::getFullArea();
    auto safe = safeArea_; //Env::getSafeArea();

    auto& sl = screenLayout_;
    auto& lp = screenLayoutParams_;


    if (lp.cellWidth > 0) {
        sl.gridSize = {lp.cellWidth, lp.cellWidth};
    }


    sl.gridCenter.setSize(sl.gridSize);
    sl.gridCenter.setPosition(safe.center());

    if (!lp.even) {
        sl.gridCenter.x -= sl.gridSize.w/2;
        sl.gridCenter.y -= sl.gridSize.h/2;
    }

    sl.gridContent = sl.gridCenter;

    while (sl.gridContent.x >= contentArea_.x + sl.gridSize.w)
        sl.gridContent.x -= sl.gridSize.w;
    while (sl.gridContent.right() < contentArea_.right() - sl.gridSize.w)
        sl.gridContent.w += sl.gridSize.w;

    while (sl.gridContent.y >= contentArea_.y + sl.gridSize.h)
        sl.gridContent.y -= sl.gridSize.h;
    while (sl.gridContent.top() < contentArea_.top() - sl.gridSize.h)
        sl.gridContent.h += sl.gridSize.h;


    sl.gridSafe = sl.gridContent;

    while (sl.gridSafe.x >= safe.x + sl.gridSize.w)
        sl.gridSafe.x -= sl.gridSize.w;
    while (sl.gridSafe.right() < safe.right() - sl.gridSize.w)
        sl.gridSafe.w += sl.gridSize.w;

    while (sl.gridSafe.y >= safe.y + sl.gridSize.h)
        sl.gridSafe.y -= sl.gridSize.h;
    while (sl.gridSafe.top() < safe.top() - sl.gridSize.h)
        sl.gridSafe.h += sl.gridSize.h;


    sl.gridFull = sl.gridSafe;

    while (sl.gridFull.x > full.x)
        sl.gridFull.x -= sl.gridSize.w;
    while (sl.gridFull.right() < full.right())
        sl.gridFull.w += sl.gridSize.w;

    while (sl.gridFull.y > full.y)
        sl.gridFull.y -= sl.gridSize.h;
    while (sl.gridFull.top() < full.top())
        sl.gridFull.h += sl.gridSize.h;

}



}  // namespace mangl
