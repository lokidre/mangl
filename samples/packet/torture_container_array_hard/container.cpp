/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl/packet.h>

#include <apeal/file.hpp>
#include <random>


using namespace std;



struct Player {

    array<array<string, 2>, 2> arr{};


};


int checkParams(const Player &playerU, const Player &player) {


    if (player.arr != playerU.arr) {
        throw std::invalid_argument("wrong array element");

    }


    return 0;
}

int testPodObject(const Player &player) {
    using namespace mangl;
    int retCode = 0;

    const auto packetFile = "packet.temp";

    PacketPacker packet;

    packet.begin();


    packet.packArrayBegin(9, player.arr.size());

    int idx = 2147483640;
    for (int i= 0; i < player.arr.size(); i++) {

     packet.packArray(idx, player.arr[i]);

        idx ++;
    }


//    packet.packArray(9, player.arr);


    packet.end();

    apl::saveFile(packetFile, packet.getData(), packet.getSize());


    Player playerU;

    PacketLoader loader;
    auto packetData = apl::loadFile(packetFile);
    loader.load(packetData.data(), packetData.size());

    while (auto field = loader.getId()) {
        switch (field.id) {
            case 9: {
                auto size = loader.unpackArrayBegin();
                for (Index i = 0; i < size; ++i) {

                    while (auto field = loader.getId()) {
                        switch (field.id) {
                            case 2147483640:
                                loader.unpackStdArray(&playerU.arr[0]);
                                break;
                            case 2147483641:
                                loader.unpackStdArray(&playerU.arr[1]);
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                }
                break;
            }
            default:
                break;
        }
    }


    retCode = checkParams(playerU, player);

    if (retCode != 0)
        exit(retCode);


    return
            retCode;
}


int main() {
    using namespace mangl;

    const auto packetFile = "packet.temp";

    Player player;

    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<long long> distrib(1, 0x7FFFFFFFFFFFFFFFLL);


    String text = "";
    String addText = "cccccccccc";

    for (int i = 0; i < 21474836; i++) {
        text += addText;
    }

//    player.secondary.arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,};

//    for (int n = 0; n < player.arr.size(); ++n) {
//        player.arr[n] = text;
//
//    }

//    int playerArrOne [5]{};
//    int playerArrSecond [5]{};

    array<string, 2> playerArrOne{};
    array<string, 2> playerArrSecond{};

    for (int n = 0; n < playerArrOne.size(); ++n) {
//        player.arr[n]=distrib(gen);
        playerArrOne[n] = text;
    }

    for (int n = 0; n < playerArrSecond.size(); n++) {
        playerArrSecond[n] = text;
//        playerArrOne[n] = n;
    }

    player.arr[0] = playerArrOne;
    player.arr[1] = playerArrSecond;



    auto retCode = testPodObject(player);


    return 0;
}
