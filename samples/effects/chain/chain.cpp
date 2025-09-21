/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "test_app.h"

#include <mangl/effects.h>

class TestScreen: public ScreenBase {
protected:
    void onStart() override;
};


void TestScreen::onStart()
{
	auto scene = App::instance()->scene() ;

	auto distance = 130. ;
	auto radius = 20. ;
	auto period = 1. ;
	auto gap = radius * 2 + 20 ;

	auto current = MPoint{ 80, 80 } ;

	auto prim = scene->newCircle ( current, radius )->setColor(0xFF0000)->setColor(0xFF) ;
	auto effect = scene->newEffect<MoveEffect>()->attach(prim)->setPeriod(period)->setOffset({0,distance})->setRepeat()->setBounce()->start() ;

	current.x += gap ;
	prim = scene->newCircle ( current, radius )->setColor(0xFF0000)->setColor(0xFF00) ;
	effect = scene->newEffect<MoveEffect>()->attach(prim)->setPeriod(period)->setOffset({0,distance})->setRepeat()->setBounce()->start()
		->setEasing(Easing::QuartInOut) ;

	current.x += gap ;
	prim = scene->newCircle ( current, radius )->setColor(0xFF0000)->setColor(0xFFFF00) ;
	effect = scene->newEffect<MoveEffect>()->attach(prim)->setPeriod(period)->setOffset({0,distance})->setRepeat()->setBounce()->start()
		->setEasing(Easing::CubicInOut) ;
}

void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}