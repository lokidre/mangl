/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "sample_game_state.hpp"

#include <mangl/multiplayer.h>

namespace sample::mp {

struct PacketChessPiece {
    ChessPieceType type{};
    ChessPieceColor color{};
    PointI pos{};

    auto operator<=>(const PacketChessPiece&) const noexcept = default;
};

struct PacketBoardCell {
    PointI pos{};
    Rect rect{};

    auto operator<=>(const PacketBoardCell&) const noexcept = default;
};

struct PacketGameBoard {
    SizeI dims{8, 8};
    
    auto operator<=>(const PacketGameBoard&) const noexcept = default;
};

struct PacketGameState {
    PacketGameBoard board;
    std::vector<PacketChessPiece> pieces;

    auto operator<=>(const PacketGameState&) const noexcept = default;
};


//struct PacketGamePlayer {
//
//    // Player identification
//    int playerKey;
//    String playerId;
//
//    int score;  // Player score
//
//    auto operator<=>(const PacketGamePlayer&) const noexcept = default;
//};


struct SampleGamePacket: public mmp::GamePacketBase {


    //
    // General data
    //
    //std::vector<PacketGamePlayer> players;
    PacketGameState state;


    auto operator<=>(const SampleGamePacket&) const noexcept = default;
};

using SampleGamePacketP = SampleGamePacket*;
using SampleGamePacketCP = const SampleGamePacket*;

}  // namespace
