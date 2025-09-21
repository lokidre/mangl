/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <mangl/font.h>

#include <apeal/geo.hpp>
#include <apeal/types.hpp>

#include <apeal/image.hpp>


using Index = apl::Index;
using String = apl::String;
using StringA = const String&;
using StrView = apl::StrView;
using FontChar = mangl::FontCharCode;
using RectI = apl::Rect<int>;
using PointI = apl::Point<int>;
using SizeI = apl::Size<int>;
using StringList = std::vector<String>;

using Image = apl::Image;
using ImageCR = const Image&;

