/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../../common/framework.h"
using  namespace mangl;
class Screen: public Screen {
private:
	Prim *rectPrim_ ;

	Coord theta_ = 0 ;
	Matrix modelMatrix_ ;

protected:
	void onStart();
	void onTimer(Timestamp timestamp);
} ;

void Screen::onStart ()
{
	auto scene = Framework::instance()->scene() ;

	scene->newRect ( 0x00FF00, { 100, 300, 80, 60 } )->setWidth(4) ;

	rectPrim_ = scene->newRect ( 0xFF0000, { -40, -30, 80, 60 } )->setWidth(4) ;

	rectPrim_->modelMtx = &modelMatrix_ ;


	modelMatrix_.rotationZ(3.14/16) ;

	modelMatrix_.translate({ 100, 100 }) ;

	//rectPrim_->setRotation(3.14/16) ;
}

void Screen::onTimer(Timestamp timestamp)
{
	static auto lastTimestamp = timestamp ;
	auto delta = timestamp - lastTimestamp ;
	auto k = 0.3 ;

	theta_ += delta * k ;
	if ( theta_ > 3.149 * 2 )
		theta_ -= 3.149 * 2 ;

	modelMatrix_.rotationZ(theta_) ;
	modelMatrix_.translate({140,130}) ;

	lastTimestamp = timestamp ;
}

#include "../../common/framework.cpp"
