/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_widget.h"

#include "../mangl_scene.h"

namespace mangl {

void Widget::onSetZIndexFront() {
	for (auto& item: childItems_) {
		scene_->moveToFront(item.prim);
		scene_->setZIndexFront(item.widget);
	}
}


}  // namespace mangl

//#include <cognit/mat/func.hpp>

/*
//
// SceneControl class
//
auto Widget::move ( Cord x, Cord y, Cord w, Cord h, MFlags flags ) -> SelfRet
{
	r_.make ( x, y, w, h ) ;

	onMove(flags) ;

	return this ;
}

auto Widget::move ( Cord x, Cord y, MFlags flags ) -> SelfRet
{
	if ( flags & M::AlignCenter )
		x -= r_.w/2 ;
	else if ( flags & M::AlignRight )
		x -= r_.w ;

	if ( flags & M::AlignVCenter )
		y -= r_.h/2 ;
	else if ( flags & M::AlignTop )
		y -= r_.h ;

	r_.x = x ;
	r_.y = y ;

	onMove(flags) ;

	return this ;
}

*/

