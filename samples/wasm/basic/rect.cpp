/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <common/framework.h>

class AppScreen: public ScreenBase {
protected:
    void onStart() override;
};


void AppScreen::onStart()
{
    auto scene = Framework::instance()->scene();

    auto debugRect = [&](RectA r) {
        auto c = M::White;// c{0xFFFFFF} ;
        auto w = 1.;
        scene->newPoint(r.position(), c, w);
        scene->newPoint(r.position().offset({0, r.h}), c, 1);
        scene->newPoint(r.extent(), c, w);
        scene->newPoint(r.position().offset({r.w, 0}), c, w);
    };

    auto testRect = [&](RectA r, ColorA color, Cord width = 0) {
        auto prim = scene->newRect(color, r);

        if (width)
            prim->setWidth(width);

        debugRect(r);
        return prim;
    };

    auto testRoundedRect = [&](RectA frame, ColorA color, Cord radius, Cord width = 0) {
        testRect(frame, color, width)->setRadius(radius);
    };


    auto testBox = [&](RectA r, ColorA color) {
        auto prim = scene->newBox(color, r);

        debugRect(r);
        return prim;
    };

    auto testRoundedBox = [&](RectA frame, ColorA color, Cord radius) {
        testBox(frame, color)->setRadius(radius);
    };



    testRect({10,10,100,80}, 0xFF0000, 4);

    testBox({160, 20, 80, 70}, 0x00FF00);


    testRoundedRect({10, 110, 90, 80}, 0xFFFF00, 10);

    testRoundedRect({140, 110, 130, 80}, 0xFF00FF, 20, 5);


    testRoundedBox({20, 200, 90, 80}, 0x00FFFF, 10);


    // Colors
    scene->newBox({20, 300, 100, 70})->setRadius(12)->setColors(0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00);

    scene->newRect({140, 300, 100, 70})->setRadius(12)->setWidth(5)->setColors(0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00);


    scene->newBox({20, 400, 100, 70})->setColors(0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00);

    scene->newRect({140, 400, 100, 70})->setWidth(5)->setColors(0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00);



    //scene->newRect ( 0xFF0000, {10,10,100,80} )->setWidth(4) ;

    //scene->newTri ( {10,10}, {160,180}, {300,20} )->setColor(0xFF0000) ;

    /*
    scene->newCircle ( {80,80}, 40 )->setColor(0xFF) ;

    scene->newCircle ( {240,80}, 40 )->setColor(0xFFFF)->setWidth(3) ;

    scene->newCircle ( {80,200}, 40 )->setColor(0xFF0000)->setFillColor(0xFFFF00)->setWidth(2) ;
    */
}

#include <common/framework.cpp>
