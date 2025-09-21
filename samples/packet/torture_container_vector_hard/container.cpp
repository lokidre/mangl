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

//struct Foo {
//    int a;
//    double b;
//
//    auto operator<=>(const Foo&) const = default;
//};

struct Secondary {
    //std::array<int, 10> arr{};
    string str{};
    double dbl{};
    float flt{};
    int i{};
    int64_t i64{};
    bool b{};

    //Foo foo{};

    auto operator<=>(const Secondary&) const = default;
};

struct Player {

    vector<Secondary> vect;

//    std::map<int, std::string> map;
    array<array<int, 12>, 2> arr{};



};


int checkParams(const Player &playerU, const Player &player, const Secondary &secondaryU) {

    if (player.vect.size() != playerU.vect.size()) {
        std::cout << "Error 'vector': " << std::endl;
        return 1;
    }

//    if (player.map.size() != playerU.map.size()) {
//        std::cout << "Error 'map': " << std::endl;
//        return 1;
//    }
//

    if (player.vect != playerU.vect) {
        throw std::invalid_argument("wrong vector element");
    }

//    for (auto item: player.vect.size()) {
//        for(auto &iu: secondaryU)
//    }

//
//    if (player.arr != playerU.arr) {
//        throw std::invalid_argument("wrong array element");
//
//    }
//
//    if (player.map != playerU.map) {
//        throw std::invalid_argument("wrong map element");
//
//    }


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
    int retCode = 0;

    const auto packetFile = "packet.temp";

    PacketPacker packet;

    packet.begin();


    packet.packArrayBegin(7, player.vect.size());

    int idx = 18;
    for (auto &item: player.vect) {
        packet.beginObject(idx);
        packet.pack(8, item.str);
        packet.pack(9, item.dbl);
        packet.pack(10, item.flt);
        packet.pack(11, item.i);
        packet.pack(12, item.i64);
        packet.pack(13, item.b);
        packet.endObject(idx);
        idx += 1;
    }


//    packet.packMap(8, player.map);
//    packet.packArray(9, player.arr);
//    packet.packArray(9, player.secondary.arr);

    packet.end();

    apl::saveFile(packetFile, packet.getData(), packet.getSize());


    Player playerU;
    Secondary secondaryU;

    PacketLoader loader;
    auto packetData = apl::loadFile(packetFile);
    loader.load(packetData.data(), packetData.size());

    while (auto field = loader.getId()) {
        switch (field.id) {
            case 7: {
                auto size = loader.unpackArrayBegin();
//                playerU.vect.resize(size);
                for (Index i = 0; i < size; ++i) {
                    while (auto field = loader.getId()) {
                        if (field.type == PacketFieldType::ObjectEnd){
                            loader.skipObject();
                            break;
                        }

                        switch (field.id) {
                            case 8:
                                loader.unpack(&secondaryU.str);
                                break;
                            case 9:
                                loader.unpack(&secondaryU.dbl);
                                break;
                            case 10:
                                loader.unpack(&secondaryU.flt);
                                break;
                            case 11:
                                loader.unpack(&secondaryU.i);
                                break;
                            case 12:
                                loader.unpack(&secondaryU.i64);
                                break;
                            case 13:
                                loader.unpack(&secondaryU.b);
                                break;
                            case 18:
                                loader.unpackObject();
                                break;
                            case 19:
                                loader.unpackObject();
                                break;
                            default:
                                break;

                        }
                    }
                    playerU.vect.push_back(secondaryU);

                }
                break;
            }
//            case 8:
//                loader.unpackMap(&playerU.map);
//                break;
//            case 9:
//                loader.unpackStdArray(&playerU.arr);
//                break;
            default:
                break;
        }
    }


    retCode = checkParams(playerU, player, secondaryU);

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

    array<int, 5> playerArrOne {};
    array<int, 5> playerArrSecond {};

    for (int n = 0; n < playerArrOne.size(); ++n) {
//        player.arr[n]=distrib(gen);
    playerArrOne[n] = distrib(gen);
    }

//    for(int n = 0; n < playerArrSecond.size())


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

//    Secondary secondary;
//    player.vect.push_back(secondary);

    player.vect.assign({
                               {
                                       .str="sample string",
                                       .dbl=3.4,
                                       .flt = 5.5,
                                       .i = 15,
                                       .i64 = 9223372036,
                                       .b = true,
                               },
                               {
                                       .str="sample string 2",
                                       .dbl=6.4,
                                       .flt = 8.5,
                                       .i = 65,
                                       .i64 = 8223372036,
                                       .b = false,
                               },
                       });
/*
    player.vect.push_back({
                                  .str="sample string",
                                  .dbl=3.4,
                                  .flt = 5.5,
                                  .i = 15,
                                  .i64 = 922337203685477,
                                  .b = true,
                          });
*/

    auto retCode = testPodObject(player);


    return 0;
}
