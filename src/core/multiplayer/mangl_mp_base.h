/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_peer.h"
#include "mangl_mp_player.h"
#include "mangl_mp_params_game.h"
#include "mangl_mp_packet_queue.h"


#include "mangl_mp_packet.h"
#include "../mangl_packet.h"

#include <apeal/parse/yaml.hpp>
#include <apeal/parse/json.hpp>



namespace mangl::mmp {

class GameClientBase;
using GameClientBaseP = GameClientBase*;

class GameServerBase;
using GameServerBaseP = GameServerBase*;


//
//  Common for client and server
//
class MpGameBase {
protected:
    using MsgTyp = MessageType;
    using MsgCat = MessageCategory;
    using MsgOrd = MessageOrder;


    MpGameParams params_{};

    static uint64_t currentPacketIndex_;

    // The engine status
    bool started_{};
    bool finished_{};

    //
    // Initialization
    //
    virtual ~MpGameBase();
    

    //
    // Setup
    //
    void setupMpGameBase(const MpGameParams& params);

    //
    // Callbacks
    //
public:
    // Connection
    using OnPeerFound = std::function<void (PeerKey, DiscoveryInfoA)>;
    using OnPeerError = std::function<void (MpPeerCP, ErrorInfo)>;
    using OnPeerState = std::function<void (MpPeerCP, PeerState)>;
    using OnPeerData = std::function<void (MpPeerCP, DataParamA)>;

    // Packet handling. Generic and specific
    using OnPacket = std::function<void (MpPeerCP, MpPacketCP)>;

    // Timing
    using OnTimer = std::function<void (Timestamp)>;

    // Debugging
    using OnDebugMessage = std::function<void (StrView)>;



    OnPeerFound onPeerFound{};
    OnPeerError onPeerError{};
    OnPeerState onPeerState{};
    OnPeerData onPeerData{};

    OnTimer onTimer;


    OnDebugMessage onDebugMessage{};

    OnDebugMessage onDebugMessageConnection{};
    OnDebugMessage onDebugMessageData{};

    OnDebugMessage onDebugMessageSystem{};
    OnDebugMessage onDebugMessagePacket;
    OnDebugMessage onDebugMessageMatch{};

    void clearCallbacks() noexcept {
        onPeerFound = nullptr;
        onPeerError = nullptr;
        onPeerState = nullptr;
        onPeerData = nullptr;

        onTimer = nullptr;

        onDebugMessage = nullptr;
        onDebugMessageConnection = nullptr;
        onDebugMessageData = nullptr;
        onDebugMessageSystem = nullptr;
        onDebugMessageMatch = nullptr;

        onProcessClearCallbacks();
    }

protected:
    virtual void onProcessClearCallbacks() = 0;


protected:
    // Launching / shutdown
    void startMpGameBase();
    void shutdownMpGameBase();


    //
    // Connection/Communication
    //
    //net::NetworkBaseP network_{};



    //
    // Packet managements, parsing building
    //
    PacketPacker packetPacker_;
    PacketLoader packetLoader_;

    apl::JsonBuilder jsonBuilder_;
    apl::YamlBuilder yamlBuilder_;

    apl::Json jsonPacket_;
    apl::JsonParser jsonParser_;
    apl::Yaml yamlPacket_;
    apl::YamlParser yamlParser_;

    MpPacketQueue packetQueue_{};


    virtual MpPacketP onPacketUnpack(MpPeerP peer, DataParamA data) = 0;
    virtual MpPacketP onPacketPrepare() = 0;
    virtual DataParam onPacketBuild(MpPeerP peer, MpPacketP packet) = 0;


    virtual void onProcessPacket(MpPeerCP peer, MpPacketCP packet) = 0;

    //
    // Time functions
    //
private:
    // Timer handling
    TimerHandler timerHandler_;

    uint64_t currentEpochTime_{};
    bool currentEpochTimeUpdated_{};

    void baseProcessTimer(Timestamp timestamp);

protected:
    Timestamp currentTimestamp_{}, lastTimestamp_{};
    virtual void onBaseTimer(Timestamp timestamp) {}

    void processTimerMpGameBaseBegin(Timestamp timestamp) noexcept {
        currentTimestamp_ = timestamp;
        currentEpochTimeUpdated_ = false;
    }

    void processTimerMpGameBaseEnd(Timestamp timestamp) noexcept {
        lastTimestamp_ = currentTimestamp_;
    }


    uint64_t updateCurrentEpochTime() noexcept;
    uint64_t getCurrentEpochTime() noexcept {
    	    return currentEpochTimeUpdated_ ? currentEpochTime_ :  updateCurrentEpochTime();
    }




    //
    // Debugging
    //

    Timestamp debugPrintStatusTimestamp_{};

    String debugMakePeerInfo(PeerKey pk, MpPeerCP peer) const noexcept;
    auto debugMakePeerInfo(MpPeerCP peer) const noexcept { return debugMakePeerInfo(0, peer); }

    void debugConStatus(StrView prefix, PeerKey pk, MpPeerCP peer, StrView suffix);
    void debugConStatus(StrView prefix, PeerKey pk, MpPeerCP peer, PeerState ps);


    void debugSystemStatus(StrView text, MpPeerCP peer, bool send) {
        if (!params_.debugVerbosePacket) return;
        auto str = apl::fmt("%c %s: %s", send ? '>' : '<', text, debugMakePeerInfo(0, peer));
        if (onDebugMessage) onDebugMessage(str);
        if (onDebugMessageMatch) onDebugMessageSystem(str);
    }

    void debugSystemRecvStatus(StrView text, MpPeerCP peer) { debugSystemStatus(text, peer, false); }
    void debugSystemRecvStatus(StrView text, MpPlayerCP player) { debugSystemRecvStatus(text, player->peer); }

    void debugSystemSendStatus(StrView text, MpPeerCP peer) {debugSystemStatus(text, peer, true); }
    void debugSystemSendStatus(StrView text, MpPlayerCP player) { debugSystemSendStatus(text, player->peer); }

    String debugMakeMatchInfo(MpPacketCP packet) const noexcept;

    void debugMatchRecvStatus(StrView text, MpPeerCP peer) noexcept {
        if (!params_.debugVerboseMatch) return;

        auto str = apl::fmt("< Match %s: %s", text, debugMakePeerInfo(0, peer));
        if (onDebugMessage) onDebugMessage(str);
        if (onDebugMessageMatch) onDebugMessageMatch(str);
    }

    void debugMatchRecvStatus(StrView text, MpPlayerCP player) { 
        debugMatchRecvStatus(text, player->peer); 
    }

    void debugMatchRecvStatus(MpPacketCP packet) noexcept {
        if (!params_.debugVerboseMatch) return;
        auto str = apl::fmt("< Match %s: %s", (char)packet->ord, debugMakeMatchInfo(packet));
        if (onDebugMessage) onDebugMessage(str);
        if (onDebugMessageMatch) onDebugMessageMatch(str);
    }



    void debugMatchSendStatus(StrView text, MpPeerP peer) {
        if (!params_.debugVerboseMatch) return;

        auto str = apl::fmt("> Match %s: %s", text, debugMakePeerInfo(0, peer));
        if (onDebugMessage) onDebugMessage(str);
        if (onDebugMessageMatch) onDebugMessageMatch(str);
    }

    void debugMatchSendStatus(StrView text, MpPlayerP player) { 
        if (!params_.debugVerboseMatch) return;
        debugMatchSendStatus(text, player->peer);
    }





    void debugDataSendStatus(MpPlayerCP player, StrView text) {
        if (!params_.debugVerboseConData) return;
        String str = apl::fmt("> %s ==> %s", text, debugMakePeerInfo(0, player->peer));
        if (onDebugMessage) onDebugMessage(str);
        if (onDebugMessageData) onDebugMessageData(str);
    }

    void debugDataRecvStatus(MpPlayerP player, StrView text) {
        if (!params_.debugVerboseConData) return;
        String str = apl::fmt("< %s <== %s", text, debugMakePeerInfo(0, player->peer));
        if (onDebugMessage) onDebugMessage(str);
        if (onDebugMessageData) onDebugMessageData(str);
    }


    void debugDataRecvStatus(PeerKey pk, MpPeerP peer, DataParamA data) {
        if (!params_.debugVerboseConData) return;

        String str = apl::fmt("< Data: %s, size: %d", debugMakePeerInfo(pk, peer), data.size());
        if (onDebugMessage) onDebugMessage(str);
        if (onDebugMessageData) onDebugMessageData(str);
    }


    //virtual void onDebugPrint(StrView text) {}



    // Logging and dumping
    void debugDumpPacket(bool send, MpPeerCP peer, DataParamA data);
    void debugVerbosePacket(bool send, MpPeerCP peer, MpPacketCP packet);

};

}  // namespace mangl::mmp
