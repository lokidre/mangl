/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_server_base.h"
#include "mangl_mp_game_instance_base.h"
#include "mangl_mp_client_base.h"

namespace mangl::mmp {

void GameServerBase::processMatchTimer(Timestamp timestamp) {

    using std::ranges::all_of;

    if (params_.matchCreate != MatchCreateStrategy::AutoCreate) {
        MpMatch* match{};
        for (auto player: players_) {
            if (player->match) {
                match = player->match;
            }
        }

        if (match && !match->isLaunched()) {
            bool playerAdded = false;
            for (auto player: players_) {
                if (!player->match) {
                    playerAdded = true;

                    player->match = match;
                    player->matching = false;

                    match->players.push_back(player);



                    auto packet = packetActionMatch(MsgOrd::MatchFound);

                    packet->matchKey = match->matchKey;
                    packet->matchId = match->matchId;

                    packetPush(player, packet);
                }
            }

            if (playerAdded) {
                // Now send match status to everybody
                broadcastMatchStatus(match);
            }


            // Check if this is autolaunch match
            if (match->players.size() >= params_.maxPlayers && params_.isMatchAutoLaunch()) {
                launchMatch(match);
            }

        }

    }


    if (params_.matchCreate == MatchCreateStrategy::AutoCreate) {
        bool wasWaitingPlayer{};

        // First go through all players
        for (auto player: players_) {
            if (!player->match) {
                if (player->matching) {
                    if (!wasWaitingPlayer) {
                        waitingPlayers_.clear();
                        wasWaitingPlayer = true;
                    }

                    waitingPlayers_.push_back(player);
                }
            }
        }

        if (!wasWaitingPlayer && !waitingPlayers_.empty()) {
            waitingPlayers_.clear();
        }

        if (wasWaitingPlayer && params_.minNpcCount > 0 && std::ssize(waitingPlayers_) < params_.minPlayers) {
            auto player = createNpcPlayer();
            if (player)
                waitingPlayers_.push_back(player);
        }


        // Check if we found matching waiting players
        if (wasWaitingPlayer && std::ssize(waitingPlayers_) >= params_.minPlayers) {

            MpMatch* match{};

            // check if we can create a match for all players
            auto matchKey = generateMatchKey();
            auto matchId = generateMatchId();

            match = matches_.create(matchKey, matchId);

            // Create game instance
            if (onGameInstanceCreate) {
                match->gameInstance = onGameInstanceCreate();
                match->gameInstance->onServerInstance(this);
            }

            match->stage = MatchStage::Created;



            auto packet = packetActionMatch(MsgOrd::MatchFound);
            packet->matchKey = match->matchKey;
            packet->matchId = match->matchId;

            for (auto player: waitingPlayers_) {
                match->players.push_back(player);
                player->match = match;
                player->matching = false;

                packetPush(player, packet);

                if (std::ssize(match->players) >= params_.maxPlayers)
                    break;
            }


            // Now send match status to everybody
            broadcastMatchStatus(match);



            // Check if this is autolaunch match
            if (params_.isMatchAutoLaunch()) {
                launchMatch(match);
            }

        }
    }


    // Now cycle through all the matches
    for (auto match: matches_) {
        if (match->isLaunched()) {
            // Check if we received all the ready packets from the players
            bool playersReady = all_of(match->players, [](auto p) { return p->matchReadyReceived; });

            if (!playersReady) continue;

            if (params_.countdown > 0) {
                match->stage = MatchStage::Countdown;
                match->countdownCurrent = params_.countdown;
                match->countdownTimestamp = timestamp;
            }
        }

        if (MatchStage::Countdown == match->stage) {

            if (timestamp >= match->countdownTimestamp) {
                if (match->countdownCurrent == 0) {
                    beginMatch(match);
                    continue;
                }

                // Send the countdown
                auto packet = packetActionMatch(MsgOrd::MatchCountdown);
                packet->value = match->countdownCurrent;

                match->countdownTimestamp += 1;  // 1 second
                --match->countdownCurrent;

                broadcastMatchPacket(match, packet);
            }

        }
    }
}

}  // namespace mangl::mmp



