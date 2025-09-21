/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "test_static.h"

namespace sample {

#ifndef MANGL_CONSOLE
CommonTexture texCommon;
Size sampleApplicationSize_ = {414, 896};
Padding sampleScreenPadding_ = {.t = 44, .b = 34};
Size sampleApplicationLayout_ = {320, 0};
InterfaceIdiom sampleInterfaceIdiom_ = InterfaceIdiom::Phone;
#endif

}  // namespace sample
