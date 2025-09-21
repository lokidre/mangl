/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "sample_mp_types.hpp"

namespace sample::mp {

enum class ChessPieceType: int {
    Unknown = -1,
    Pawn = 5,
    King = 0,
    Queen = 1,
    Rook = 2,
    Bishop = 3,
    Knight = 4,
};

enum class ChessPieceColor: int {
    Unknown,
    White,
    Black,
};



}  // namespace multiplayer
