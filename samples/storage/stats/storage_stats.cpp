/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../../common/console_main.hpp"

#include <mangl/storage.h>

using namespace apl ;
using namespace mangl;

void body()
{
	auto stats = StatsStorage::instance() ;

	stats->open("test.rdb") ;

	auto key = "sample_key" ;

	auto d = stats->get<double>(key,0.3) ; //stas->
	d += 1./3 ;
	stats->set(key,d) ;

	println("%s: %z", key, d ) ;

	stats->set("another", 3) ;

}


#include "../../common/console_main.cpp"
