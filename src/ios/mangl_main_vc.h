/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#import "mangl_main_vc_decl.h"

#import "mangl_main_vc+ads.h"
#import "mangl_main_vc+timer.h"
#import "mangl_main_vc+touches.h"
#import "mangl_main_vc+except.h"

namespace mangl {

bool manglPushInit(std::function<void()> f);

}  // namespace mangl

