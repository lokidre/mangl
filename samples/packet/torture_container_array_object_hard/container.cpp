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

struct Secondary {
   string str{};
   int i{};

   auto operator<=>(const Secondary &) const = default;

};


struct Player {

   array<array<Secondary, 3>, 2> arr{};


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

//    int idx = 30;
   int packObjIdx = 2147483644;
//   int packObjIdx = 50;
   int arrIdx = 0;
   int arrBeginIdx = 18;

   for (auto &item: player.arr) {
       packet.packArrayBegin(arrBeginIdx, item.size());
       for (auto &item: player.arr[arrIdx]) {
           packet.beginObject(packObjIdx);
           packet.pack(1000, item.str);
           packet.pack(1001, item.i);
           packet.endObject(packObjIdx);
           packObjIdx += 1;
       }
       arrIdx++;
       arrBeginIdx++;
       packObjIdx = 2147483644;
   }


   packet.end();

   apl::saveFile(packetFile, packet.getData(), packet.getSize());


   Player playerU;
   Secondary secondaryU;

   PacketLoader loader;
   auto packetData = apl::loadFile(packetFile);
   loader.load(packetData.data(), packetData.size());

   int objIdx = 0;

   while (auto field = loader.getId()) {
       switch (field.id) {
           case 9: {
               auto size = loader.unpackArrayBegin();
               for (Index i = 0; i < size; ++i) {
                   while (auto fd = loader.getId()) {
                       switch (fd.id) {
                           case 18: {
                               auto s = loader.unpackArrayBegin();
                               for (Index n = 0; n < s; ++n) {
                                   while (auto f = loader.getId()) {
                                       if (f.type == PacketFieldType::ObjectEnd) {
                                           loader.skipObject();
                                           break;
                                       }
                                       switch (f.id) {
                                           case 1000:
                                               loader.unpack(&secondaryU.str);
                                               break;
                                           case 1001:
                                               loader.unpack(&secondaryU.i);
                                               break;
                                           case 2147483644:
                                               loader.unpackObject();
                                               break;
                                           case 2147483645:
                                               loader.unpackObject();
                                               break;
                                           case 2147483646:
                                               loader.unpackObject();
                                               break;
                                           default:
                                               break;
                                       }
                                   }
                                   playerU.arr[0][n] = secondaryU;
                               }
                               break;
                           }
                           case 19: {
                               auto s = loader.unpackArrayBegin();
                               for (Index n = 0; n < s; ++n) {
                                   while (auto f = loader.getId()) {
                                       if (f.type == PacketFieldType::ObjectEnd) {
                                           loader.skipObject();
                                           break;
                                       }
                                       switch (f.id) {
                                           case 1000:
                                               loader.unpack(&secondaryU.str);
                                               break;
                                           case 1001:
                                               loader.unpack(&secondaryU.i);
                                               break;
                                           case 2147483644:
                                               loader.unpackObject();
                                               break;
                                           case 2147483645:
                                               loader.unpackObject();
                                               break;
                                           case 2147483646:
                                               loader.unpackObject();
                                               break;
                                           default:
                                               break;
                                       }
                                   }
                                   playerU.arr[1][n] = secondaryU;
                               }
                               break;
                           }
                           default:
                               break;
                       }
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

//    random_device rd;  // Will be used to obtain a seed for the random number engine
//    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
//    uniform_int_distribution<long long> distrib(1, 0x7FFFFFFFFFFFFFFFLL);


   String text = "";
   String addText = "cccccccccc";

   for (int i = 0; i < 6500; i++) {
       text += addText;
   }

   array<Secondary, 3> playerArrOne{};
   array<Secondary, 3> playerArrSecond{};

   for (int n = 0; n < playerArrOne.size(); ++n) {
//        player.arr[n]=distrib(gen);
//       playerArrOne[n].str = "aaaaaaaaaaaaaaaaaaaaaaaaaa";
       playerArrOne[n].str = text;
   }

   for (int n = 0; n < playerArrSecond.size(); n++) {
//       playerArrSecond[n].str = "aaaaaaaaaaaaaaaaaaaaaaaaaa";
       playerArrSecond[n].str = text;
//        playerArrOne[n] = n;
   }

   for (int n = 0; n < playerArrOne.size(); n++) {
       playerArrOne[n].i = n;
       playerArrSecond[n].i = n;
   }


   player.arr[0] = playerArrOne;
   player.arr[1] = playerArrSecond;


   auto retCode = testPodObject(player);


   return 0;
}
