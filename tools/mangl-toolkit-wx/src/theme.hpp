/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <apeal/singleton.hpp>

class ToolkitTheme: public apl::Singleton<ToolkitTheme> {
public:
    int layoutHorzPadding{10}, layoutVertPadding{10};
    int layoutColSpacing{10};
    int layoutRowSpacing{10};
};

