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

    std::vector<string> vect;
    std::map<int, std::string> map;
    std::array<string, 12> arr{};
};

int checkParams(const Player &playerU, const Player &player) {

    if (player.vect.size() != playerU.vect.size()) {
        std::cout << "Error 'vector': " << std::endl;
        return 1;
    }

    if (player.map.size() != playerU.map.size()) {
        std::cout << "Error 'map': " << std::endl;
        return 1;
    }

    if (player.vect != playerU.vect) {
        throw std::invalid_argument("wrong vector element");
    }

    if (player.arr != playerU.arr) {
        throw std::invalid_argument("wrong array element");

    }

    if (player.map != playerU.map) {
        throw std::invalid_argument("wrong map element");

    }


//
//    for(int i = 0; i < player.map.size(); i++) {
//
//    for (auto& pi: player.map) {
//
//        auto piu = playerU.map.find(pi.first);
//
//        if (piu == playerU.map.end()) {
//            // throw sljdlskjf
//        }
//
//        if (piu->second != pi.second) {
//
//        }


    return 0;
}

int testPodObject(const Player &player) {
    using namespace mangl;

    const auto packetFile = "packet.temp";

    PacketPacker packet;

    packet.begin();


//    packet.packArray(7, player.vect);

//    packet.packMap(8, player.map);
    packet.packArray(9, player.arr);


    packet.end();

    apl::saveFile(packetFile, packet.getData(), packet.getSize());

    Player playerU;

    PacketLoader loader;
    auto packetData = apl::loadFile(packetFile);
    loader.load(packetData.data(), packetData.size());

    while (auto field = loader.getId()) {
        switch (field.id) {
            case 7:
                loader.unpackArray(&playerU.vect);
                break;
            case 8:
                loader.unpackMap(&playerU.map);
                break;
            case 9:
                loader.unpackStdArray(&playerU.arr);
                break;
            default:
                break;
        }
    }


    auto retCode = checkParams(playerU, player);

    if (retCode != 0)
        exit(retCode);

    return retCode;

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
    for (int n = 0; n < player.arr.size(); ++n) {
        player.arr[n] = text;

    }

//    for (int n = 0; n < player.arr.size(); ++n) {
//        player.arr[n]=distrib(gen);
//
//    }


    std::string a = "a";
    std::string ab = "ab";
    std::string abc = "abc";

//    player.map[1] = text;
//    player.map[2] = text;
//    player.map[3] = text;
//    player.map[4] = text;
//    player.map[5] = text;
//    player.map[6] = text;
//    player.map[7] = text;
//    player.map[8] = text;
//    player.map[9] = text;
//    player.map[10] = text;
//    player.map[11] = text;
//    player.map[12] = text;

//        for (int i = 0; i < 11; ++i) {
//        player.vect.push_back(text);
//    }

    auto retCode = testPodObject(player);


    return 0;
}
