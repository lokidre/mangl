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

#include <limits>

using namespace std;

struct Player {
    int x{};
    float y{};
    double score{};
    bool alive{};
    mangl::SizeI dims{};
    mangl::PointI pos{};
};

int checkParams(Player playerU, Player player) {

    if (playerU.x != player.x) {
        cout << "Error 'x':" << player.x << " != " << playerU.x << endl;
        return 1;
    }

    if (playerU.y != player.y) {
        cout << "Error 'y':" << player.y << " != " << playerU.y << endl;
        return 1;
    }

//    if (playerU.score != player.score) {
//        cout << "Error 'score':" << player.score << " != " << playerU.score << endl;
//        return 1;
//    }
//
//    if (playerU.alive != player.alive) {
//        cout << "Error 'alive':" << player.alive << " != " << playerU.alive << endl;
//        return 1;
//    }
//    if(playerU.dims != player.dims){
//        cout << "Error 'dims':" " != "  << endl;
//        cout << playerU.dims.w << "   " << player.dims.w << endl;
//        cout << playerU.dims.h << "   " << player.dims.h << endl;
//        return 1;
//    }
//    if(playerU.pos != player.pos){
//        cout << "Error 'pos':" " != "  << endl;
//        cout << playerU.pos.x << "   " << player.pos.x << endl;
//        cout << playerU.pos.y << "   " << player.pos.y << endl;
//        return 1;
//    }

    return 0;
}

int testPodObject(const Player& player) {

    using namespace mangl;

    PacketPacker packet;

    packet.begin();
    packet.pack(1, player.x);
    packet.pack(2, player.y);
//    packet.pack(3, player.score);
//    packet.pack(4, player.alive);
    //packet.pack(5,player.dims);
    //packet.pack(6,player.pos);

    // packet.pack(5,0.12358);

    packet.end();

    const auto packetFile = "packet.temp";


    apl::saveFile(packetFile, packet.getData(), packet.getSize());


    Player playerU;

    PacketLoader loader;
    auto packetData = apl::loadFile(packetFile);
    loader.load(packetData.data(), packetData.size());

    while (auto field = loader.getId()) {
        switch (field.id) {
        case 1: loader.unpack(&playerU.x); break;
        case 2: loader.unpack(&playerU.y); break;
        case 3: loader.unpack(&playerU.score); break;
        case 4: loader.unpack(&playerU.alive); break;
        case 5: loader.unpack(&playerU.dims);break;
        case 6: loader.unpack(&playerU.pos);break;
        default: break;
        }
    }


    auto retCode = checkParams(playerU, player);

    if (retCode != 0)
        exit(retCode);

    return retCode;
}

int main() {
    using namespace mangl;

    Player player;

    player.x = 1;
    player.y = 5;
    player.score = 9;
    player.alive = true;
    player.dims = {8,8};
    player.pos = {10,10000000};

    auto retCode = testPodObject(player);


    player.x = 1330;
    player.y = 5.5;
    player.score = 9.223372036854775808;
    player.alive = false;
    player.dims = {9,9};

    retCode = testPodObject(player);


    player.x = std::numeric_limits<int>::max();
    player.y = 8.5;
    player.score = -2.147483648;
    player.alive = true;

    retCode = testPodObject(player);

    player.x = 2147483647;
    player.y = 1.175494351f;
    player.score = -2.147483648;
    player.alive = true;

    retCode = testPodObject(player);


    // auto cmpRes = std::memcmp(&player, &playerU, sizeof(player));


    return retCode;
}
