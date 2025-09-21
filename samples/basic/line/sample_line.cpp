/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <test_app.h>

class TestScreen: public Screen {
protected:
    void onStart() override;
};

void TestScreen::onStart()
{
    buildSampleBackground();
    
    auto scene = App::getScene();


    auto testLine = [&](Point a, Point b, Color c, Real w) {
        scene->newLine(a, b)->setColors(c, c)->setWidth(w);

        scene->newPoint(a, M::White, 2);
        scene->newPoint(b, M::White, 1);
    };


    //scene->newLine ( {10,10}, {20,100} )->setColor(0xFF0000)->setWidth(1) ;
    //scene->newLine ( {10,10}, {20,100} )->setColor(0xFF0000) ;

    //scene->newLine ( {30,10}, {30,100} )->setColor(0xFFFF00)->setWidth(3) ;

    //scene->newLine ( {50,10}, {50,100} )->setColor(0xFFFF00)->setWidth(5) ;


    //scene->newLine ( {10,110}, {50,210} )->setColor(0x00FF00)->setWidth(5) ;

    //scene->newLine ( {10,110}, {50,110} )->setColor(0x00FF00)->setWidth(5) ;

    //scene->newLine ( {50,110}, {10,210} )->setColor(0x00FF00)->setWidth(5) ;


    //scene->newLine ( {50,110}, {10,210} )->setColors(0xFF0000,0x00FF00)->setWidth(5) ;

    //scene->newLine ( {50,210}, {10,110} )->setColors(0xFF0000,0x00FF00)->setWidth(20) ;


    //scene->newLine ( {10,310}, {200,330} )->setColors(0xFF0000,0x00FF00)->setWidth(20) ;
    //scene->newLine ( {200,310}, {10,330} )->setColors(0xFF0000,0x00FF00)->setWidth(20) ;


    testLine({200, 330}, {215, 350}, M::Lime, 4);

    testLine({2, 0}, {320 - 4, 350}, M::Yellow, 4);

    testLine({10, 310}, {200, 330}, M::Red, 6);

}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
