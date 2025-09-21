/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "test_app.h"

#include <mangl/timer.h>

class TestScreen: public ScreenBase {
protected:
    void onStart() override;
};

void TestScreen::onStart()
{
	auto scene = App::instance()->scene() ;
	auto r1 = scene->newRect ( 0xFF0000, {80,80, 40,40} ) ;
	auto r2 = scene->newRect ( 0x00FF00, {100,200, 50, 50} ) ;


    RunLoopTimer::instance()->schedule ( 1, [r1]() {
		auto color = r1->color_ ;
		color.g += 0.2 ;
		if ( color.g > 1 )
			color.g = 0 ;
		r1->setColor(color) ;
	}) ;

    RunLoopTimer::instance()->schedule ( 1.5, [r2]() {
		auto color = r2->color_ ;
		color.b += 0.4 ;
		if ( color.b > 1 )
			color.b = 0 ;
		r2->setColor(color) ;
	}) ;

    RunLoopTimer::instance()->schedule ( 10, []() {
        RunLoopTimer::instance()->clear() ;
	}) ;

}

void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

