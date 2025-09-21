/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_base.h"
#include "mangl_mp_match_container.h"

#include "../network/mangl_network_peer_container.h"

namespace mangl::mmp {

class GameServerBase: public MpGameBase {
    friend class GameClientBase;
    friend class GameInstanceBase;

    using Self = GameServerBase;


public:
    ~GameServerBase() override;

	//
	// Setup and launching
	//
public:
    // setup()
    // start()
    // timer(timestamp)
    // shotdown();

    void setupGameServer(MpGameParamsA params) {
        MpGameBase::setupMpGameBase(params);
    }
    void startGameServer();
    void shutdownGameServer();
    //void onTimerGameServer(Timestamp timestamp);



public:
    String getHostUrl() { return params_.hostUrl; }

    auto& peers() { return peers_; }
    auto getPeer(PeerKey pk) { return peers_.get(pk); }





    //
    // Handling events
    //




	//
	// Peers: Connections, disconnection, reconnection, etc.
	//
private:
    net::NetworkBaseServer* network_{};
    net::PeerContainer<MpPeer> peers_;
    void setupNetwork();
    void shutdownNetwork();

    // Processing connection events
    MpPeerP processPeerError(PeerKey pk, ErrorInfo err);
    MpPeerP processPeerFound(PeerKey pk, DiscoveryInfoA di);
    MpPeerP processPeerState(PeerKey pk, PeerState ps);
    MpPeerP processDataRecv(PeerKey pk, DataParamA data);

    void transmitData(MpPeerP peer, DataParamA data);

    void disconnect(MpPeerP player);
    void disconnect(MpPlayerP player);


    // Local connection options
    //using LocalConnectionContainer = std::vector<
    //GameClientBaseP local
//public:
    MpPeerP localClientConnect(GameClientBaseP client, DiscoveryInfoA di);
    //void localDataReceive(PeerKey pk, DataParamA data);




//private:
    // Id management
    static MpPlayerKey currentPlayerIndex_;
    static MpPlayerKey generatePlayerKey() noexcept { return currentPlayerIndex_++; }
    String generatePlayerId();

    static MpMatchKey currentMatchIndex_;
    static MpMatchKey generateMatchKey() noexcept { return currentMatchIndex_++; }
    String generateMatchId();



    // NPC management
    MpPlayerP createNpcPlayer();


    //
    // Processing events
    //
private:  


    //
    // Everything related to timer processing
    //
protected:
    void onBaseTimer(Timestamp timestamp) override;

private:
    void processTimerBody(Timestamp timestamp);


private:

	//
	// Everything related to packet processing
	//
    void processPacket(MpPeerP peer, MpPacketCP packet);
    void processCommandSystem(MpPlayerP player, MpPacketCP packet);
    void processCommandMatch(MpPlayerP player, MpPacketCP packet);
    void processCommandGame(MpPlayerP player, MpPacketCP packet);

public:
    //MpPacketP packetPrepare(MpPlayerP player, MsgTyp type, MsgCat cat, MsgOrd ord);
    MpPacketP packetPrepare(MsgTyp type, MsgCat cat, MsgOrd ord);
    //MpPacketP packetPrepare(MsgTyp type, MsgCat cat, MsgOrd ord) {
    //    return packetPrepare((MpPeerP)nullptr, type, cat, ord);
    //}

    MpPacketP packetAction(MsgCat cat, MsgOrd ord) { return packetPrepare(MsgTyp::Action, cat, ord); }
    MpPacketP packetActionSystem(MsgOrd ord) { return packetAction(MsgCat::System, ord); }
    MpPacketP packetActionMatch(MsgOrd ord) { return packetAction(MsgCat::Match, ord); }
    MpPacketP packetActionGame(MsgOrd ord) { return packetAction(MsgCat::Game, ord); }


    void packetPush(MpPlayerP player, MpPacketP packet) { packetPush(player->peer, packet); }
    void packetPush(MpPeerP peer, MpPacketP packet);

    //void packetSend(MpPeerP peer, DataParamA data);
    //void packetSend(PlayerP player, BPacketP packet);

    void packetError(MpPeerP peer, StrView text);
    void packetError(MpPlayerP player, StrView text) { return packetError(player->peer, text);  }


    void packetBroadcast(MpMatchP match, MpPacketP packet);


//public:
    // 
    // Packet creation for game insance
    //
    //MpPacketP beginGameAction(MpPlayerP player, MsgOrd ord) {
    //    return packetPrepareAction(
    //}

    //MpPacketP beginAction(MsgOrd ord) { return beginAction(nullptr, ord);  }


private:
    //
    // Matchmaking
    // 
    // TODO: move to matchmaker class
    std::vector<MpPlayerP> waitingPlayers_;
    bool waitingNpcLaunched_{};

    MpMatchContainer matches_;
    MpPlayerContainer players_;


    void processMatchTimer(Timestamp timestamp);
    void broadcastMatchStatus(MpMatchP match);
    void broadcastMatchPacket(MpMatchP match, MpPacketP packet);
    void launchMatch(MpMatchP match);
    void beginMatch(MpMatchP match);
    void processPlayerDisconnect(MpMatchP match, MpPlayerP player);
    void discardMatch(MpMatchP match);

public:
    using OnGameInstanceCreate = std::function<GameInstanceBase*(void)>;
    using OnGameInstanceDestroy = std::function<void (GameInstanceBase*)>;

    OnGameInstanceCreate onGameInstanceCreate{};
    OnGameInstanceDestroy onGameInstanceDestroy{};

    using OnGameNpcCreate = std::function<GameClientBase*(void)>;
    using OnGameNpcDestroy = std::function<void (GameClientBase*)>;

    OnGameNpcCreate onGameNpcCreate{};
    OnGameNpcDestroy onGameNpcDestroy{};



private: // debugging
    std::size_t debugLastPeersCount_{};
    std::size_t debugLastPlayersCount_{};
    std::size_t debugLastMatchesCount_{};

    void debugPrintStats();
};




}  // namespace mangl::mmp


