/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../../common/console_main.hpp"

#include <apeal/geo.hpp>


#include <mangl/storage.h>

using namespace apl ;
using namespace mangl;

void body()
{
	SettingsStorage::instance()->setLocation("settings.rocksdb") ;

	Setting<int> test("test",25) ;

	//auto v = test.get() ;

	println("%s: %z", test.key(), test.get() ) ;

	test = test() + 35 ;

	println("%s: %z", test.key(), test.get() ) ;

	//MSettingsStorage::instance()->shutdown() ;



	Setting<RectI> rect("rect") ;

	println("%s: %z", rect.key(), rect() ) ;

	rect = rect().zoomedAdd(1) ;

	println("%s: %z", rect.key(), rect() ) ;



	apl::SingletonManager::instance()->shutdown() ;

}


#include "../../common/console_main.cpp"
