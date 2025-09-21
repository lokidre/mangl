/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_packet_proto.h"
#include "mangl_mp_player.h"
#include "mangl_mp_match.h"

#include "mangl_mp_packet_element.h"
#include "mangl_mp_packet_field.h"

namespace mangl::mmp {

struct GamePacketBase;

using MpPacket = GamePacketBase;
using MpPacketP = MpPacket*;
using MpPacketCP = const MpPacket*;

using PacketKey = unsigned;

struct GamePacketBase {

    //
    // Message type and category
    //
    MessageType type;
    MessageCategory cat;
    MessageOrder ord;

    bool isValid() const noexcept {
        return MessageType::Invalid != type &&
            MessageCategory::Invalid != cat &&
            MessageOrder::Invalid != ord;
    }

    [[nodiscard]] bool isAction() const noexcept { return MessageType::Action == type; }
    bool isCommand() const noexcept { return MessageType::Command == type; }
    bool isEvent() const noexcept { return MessageType::Event == type; }

    bool isAction(MessageCategory c) const noexcept { return isAction() && cat == c; }
    bool isCommand(MessageCategory c) const noexcept { return isCommand() && cat == c; }
    bool isEvent(MessageCategory c) const noexcept { return isEvent() && cat == c; }

    bool isSystem() const noexcept { return MessageCategory::System == cat; }
    bool isMatch() const noexcept { return MessageCategory::Match == cat; }
    bool isGame() const noexcept { return MessageCategory::Game == cat; }

    bool isActionSystem() const noexcept { return isAction() && isSystem(); }
    bool isActionMatch() const noexcept { return isAction() && isMatch(); }
    bool isActionGame() const noexcept { return isAction() && isGame(); }

    bool isActionSystem(MessageOrder o) const noexcept { return isActionSystem() && ord == o; }
    bool isActionMatch(MessageOrder o) const noexcept { return isActionMatch() && ord == o; }
    bool isActionGame(MessageOrder o) const noexcept { return isActionGame() && ord == o; }


    bool isCommandSystem() const noexcept { return isCommand() && isSystem(); }
    bool isCommandMatch() const noexcept { return isCommand() && isMatch(); }
    bool isCommandGame() const noexcept { return isCommand() && isGame(); }

    bool isCommandSystem(MessageOrder o) const noexcept { return isCommandSystem() && ord == o; }
    bool isCommandMatch(MessageOrder o) const noexcept { return isCommandMatch() && ord == o; }
    bool isCommandGame(MessageOrder o) const noexcept { return isCommandGame() && ord == o; }

    bool isEventGame() const noexcept { return isEvent() && isGame(); }


    [[nodiscard]] MessageOrder ordActionSystem() const noexcept { return isActionSystem() ? ord : MessageOrder::Invalid; }
    MessageOrder ordActionMatch() const noexcept { return isActionMatch() ? ord : MessageOrder::Invalid; }
    MessageOrder ordActionGame() const noexcept { return isActionGame() ? ord : MessageOrder::Invalid; }

    MessageOrder ordCommandSystem() const noexcept { return isCommandSystem() ? ord : MessageOrder::Invalid; }
    MessageOrder ordCommandMatch() const noexcept { return isCommandMatch() ? ord : MessageOrder::Invalid; }
    MessageOrder ordCommandGame() const noexcept { return isCommandGame() ? ord : MessageOrder::Invalid; }

    // Common system packets
    [[nodiscard]] bool isSystemWelcome() const noexcept { return isSystem() && MessageOrder::Welcome == ord; }

    // Common match functions
    [[nodiscard]] bool isMatchLaunch() const noexcept { return isMatch() && MessageOrder::MatchLaunch == ord; }
    bool isMatchCountdown() const noexcept { return isMatch() && MessageOrder::MatchCountdown == ord; }
    bool isMatchBegin() const noexcept { return isMatch() && MessageOrder::MatchBegin == ord; }
    bool isMatchPaused() const noexcept { return isMatch() && MessageOrder::MatchPaused == ord; }
    bool isMatchResumed() const noexcept { return isMatch() && MessageOrder::MatchResumed == ord; }

    // Common game functions
    bool isGameState() const noexcept { return isGame() && MessageOrder::GameState == ord; }
    bool isGameField() const noexcept { return isGame() && MessageOrder::GameField == ord; }
    bool isGameElement() const noexcept { return isGame() && MessageOrder::GameElement == ord; }
    bool isGameCharacter() const noexcept { return isGame() && MessageOrder::GameCharacter == ord; }


    uint64_t messageId;
    uint64_t messageTime;
    uint64_t messageHash;

    unsigned to;  // Message destination
    MpPlayerP rtToPlayer;  // Only runtime, does not transmit

    unsigned from;  // Message originator
    unsigned who;  // Who this message refers to

    //
    // Versioning
    //
    unsigned serverVersion;  // Server version
    unsigned hostVersion;  // Host app version
    unsigned clientVersion;  // Joining app version


    //
    // Error handling and generic parameters
    //
    MessageErrorCat error;


    //
    // Various generic parameters
    //

    int code;  // Generic int code
    String message;  // Generic string message

    int value;  // Generic int value
    String text;  // Generic text

    uint64_t time;  // generic timestamp

    PointI positionI;
    PointI secondPositionI;
    Point position;
    SizeI sizeI;
    Size size;
    RectI rectI;
    Rect rect;


    //
    // Identification
    //
    unsigned serverKey;
    String serverId;

    unsigned clientKey;
    String clientId;

    unsigned playerKey;
    String playerId;


    //
    // Match status
    //
    MatchStage matchStage;
    unsigned matchKey;
    String matchId;
    MpPacketPlayerContainer players;
  

    //
    // Game state
    //
    uint64_t gameStateId;  // Incremental id
    uint64_t gameStateTimestamp;  // When the game state was generated
    uint64_t gameStateHash;


    MpPacketElement element;
    MpPacketElementsContainer elements;
    MpPacketElementGroupContainer groups;

};

using GamePacketBaseP = GamePacketBase*;


}  // namespace mmp


