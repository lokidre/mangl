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
#include "mangl_mp_client_status.h"
#include "mangl_mp_params_game.h"

namespace mangl::mmp {

class GameClientBase: public MpGameBase {
    friend class GameServerBase;

    using Self = GameClientBase;

protected:
    MpClientStatus status_{};


public:
    ~GameClientBase() override;

    void setup(MpGameParams params) {
        onSetup(&params);
        setupGameClient(params);
    }

    void start() {
        onStart();
        startGameClient();
    }

    //
    // setup()
    // start()
    // timer()
    // shotdown()



    MpPlayerKey getPlayerKey() const noexcept { return status_.playerKey; }
    MpPlayerKey getMatchKey() const noexcept { return status_.matchKey; }



private:
    void setupGameClient(const MpGameParams& params);
    void startGameClient();
    void shutdownGameClient();


private:  
    //
    // Connection
    // 
    net::NetworkBaseClient* network_{};
    MpPeer clientPeer_{};
    MpPeer hostPeer_{};
    String hostUrl_;

    void setupNetwork();
    void networkConnect();
    void transmitData(DataParamA data);
    void shutdownNetwork();


    MpPeerP processPeerError(PeerKey pk, ErrorInfo err);
    MpPeerP processPeerFound(PeerKey pk, DiscoveryInfoA di);
    MpPeerP processPeerState(PeerKey pk, PeerState ps);
    void processDataRecv(PeerKey pk, DataParamA data);



    //
    // Timers
    //
protected:
    void onBaseTimer(Timestamp timestamp) override;

private:
    void processSystemTimerBody(Timestamp timestamp);




protected:
    MpPacketP packetPrepare(MsgTyp type, MsgCat cat, MsgOrd ord);

    MpPacketP packetCommand(MsgCat cat, MsgOrd ord) { return packetPrepare(MsgTyp::Command, cat, ord); }
    MpPacketP packetCommandSystem(MsgOrd ord) { return packetCommand(MsgCat::System, ord);   }
    MpPacketP packetCommandMatch(MsgOrd ord) { return packetCommand(MsgCat::Match, ord);   }


    void packetSend(MpPacketP packet);


private:
    void processPacket(MpPacketCP packet);
    void processPacketAction(MpPacketCP packet);
    void processActionSystem(MpPacketCP packet);
    void processActionMatch(MpPacketCP packet);



protected:
    virtual void onPacketProcess(MpPacketP packet) {};

    virtual void onSetup(MpGameParams* params) {};
    virtual void onStart() {}

};




}  // namespace mangl::mmp
