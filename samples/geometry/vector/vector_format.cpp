/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../../common/console_main.hpp"

#include <test_app.h>

int main(){
    auto v = Point { 0.45, -5422 } ;

    println ( "Point: %.2t", v ) ;

    println ( "Scaled: %.2t", v.scaled(-2) ) ;
    println ( "Floored: %.0t", v.floor() ) ;

    auto v3 = Point3D { -3, 123, 0.01 } ;

    println ( "Point3D: %.2t", v3 ) ;

    //cout << "Point: " << v << endl ;

    println ( "Size: %.0t", Size{100,20} ) ;

    println ( "Rect: %.0t", Rect{10,20,200,50} ) ;
}


/*using namespace mangl;
class TestScreen: public ScreenBase {
protected: void onStart() override;
};

void TestScreen::onStart()
{
    auto scene = App::instance()->scene();
	auto v = Point { 0.45, -5422 } ;

	println ( "Point: %.2t", v ) ;

	println ( "Scaled: %.2t", v.scaled(-2) ) ;
	println ( "Floored: %.0t", v.floor() ) ;

	auto v3 = Point3D { -3, 123, 0.01 } ;

	println ( "Point3D: %.2t", v3 ) ;

	//cout << "Point: " << v << endl ;

	println ( "Size: %.0t", Size{100,20} ) ;

	println ( "Rect: %.0t", Rect{10,20,200,50} ) ;

}
void  App::onStart() {
    if(sampleApplicationLayout_.h==0) {
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    } else {Env::setLayout(sampleApplicationLayout_);}
    switchScreen(new TestScreen);
}
*/
// #include "../../common/console_main.cpp"
