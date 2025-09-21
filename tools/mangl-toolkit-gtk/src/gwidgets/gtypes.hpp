/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <gtk/gtk.h>

#include <apeal/geo.hpp>
#include <apeal/string.hpp>
#include <apeal/signal.hpp>


namespace gw {

using CordI = int;
using RectI = apl::Rect<CordI>;
using SizeI = apl::Size<CordI>;
using PointI = apl::Point<CordI>;

using String = apl::String;
using StrView = apl::StrView;

enum class Orientation: int {
    Horizontal = GTK_ORIENTATION_HORIZONTAL,
    Vertical = GTK_ORIENTATION_VERTICAL,
};

enum class Position: int {
    Top, Bottom, Left, Right,
};

enum class SelectionMode {
    NoSelection,
    SingleSelection,
    MultiSelection,
};


} // namespace gw
