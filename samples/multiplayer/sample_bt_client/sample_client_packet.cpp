/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "sample_client_packet.h"

#include "../shared/sample_game_packet.hpp"
#include "../shared/sample_game_packet_gen.hpp"
#include "../shared/sample_game_packet_gen_yaml.hpp"
#include "../shared/sample_game_packet_gen_json.hpp"

#include <apeal/console.hpp>
#include <mangl/framework.h>

#include <limits>


namespace sample::mp {

String buildPacket() {
    SampleGamePacket samplePacket;

    M::random.range(-1000, 1000, &samplePacket.code);

//    zchess.board.dims = {8,8};
//    Piece first, second;
//    BoardCell cell1,cell2,cell3;
//    cell1.pos = {1,1};
//    cell1.rect = {1,1,1,1};
//    cell2.pos = {10,10};
//    cell2.rect = {10,10,10,10};
//    cell3.pos = {100,100};
//    cell3.rect = {100,100,100,100};
//    first.color = PieceColor::White;
//    first.type = PieceType::Queen;
//    first.pos = {20, 20};
//    second.color = PieceColor::Black;
//    second.type = PieceType::King;
//    second.pos = {30, 30};
//    zchess.figures.push_back(first);
//    zchess.figures.push_back(second);
//    zchess.boardCell.insert({1,cell1});
//    zchess.boardCell.insert({10,cell2});
//    zchess.boardCell.insert({100,cell3});
//    samplePacket.state = zchess;
    //zchess.board.dims = {(int)M::random.range((Index)std::numeric_limits<int>::min,(Index)std::numeric_limits<int>::max),8};
    auto& board = samplePacket.state.board;

    board.dims = {M::random.indexInt(10000), M::random.indexInt(10000)};

    /*
    Piece first, second;
    BoardCell cell1,cell2,cell3;
//    cell1.pos = {M::random.indexInt((0,(Index)std::numeric_limits<int>::max)),M::random.indexInt((0,(Index)std::numeric_limits< int>::max))};
//    cell1.rect = {M::random.ranged(0,(Index)std::numeric_limits<double>::max),M::random.ranged(0,(Index)std::numeric_limits<double>::max),
//                  M::random.ranged(0,(Index)std::numeric_limits<double>::max),M::random.ranged(0,(Index)std::numeric_limits<double>::max)};
//    cell2.pos = {M::random.indexInt((0,(Index)std::numeric_limits<unsigned int>::max)),M::random.indexInt((0,(Index)std::numeric_limits<unsigned int>::max))};
//    cell2.rect = {M::random.ranged(0,(Index)std::numeric_limits<double>::max),M::random.ranged(0,(Index)std::numeric_limits<double>::max),
//                  M::random.ranged(0,(Index)std::numeric_limits<double>::max),M::random.ranged(0,(Index)std::numeric_limits<double>::max)};
//    cell3.pos = {M::random.indexInt((0,(Index)std::numeric_limits<unsigned int>::max)),M::random.indexInt((0,(Index)std::numeric_limits<unsigned int>::max))};
//    cell3.rect = {M::random.ranged(0,(Index)std::numeric_limits<double>::max),M::random.ranged(0,(Index)std::numeric_limits<double>::max),
//                  M::random.ranged(0,(Index)std::numeric_limits<double>::max),M::random.ranged(0,(Index)std::numeric_limits<double>::max)};
//    cell1.pos = {M::random.indexInt((0,(Index)(std::numeric_limits<int>::max)/2)),M::random.indexInt((0,(Index)(std::numeric_limits<int>::max)/2))};
//    cell1.rect = {10000000.356,10000000.789,10000000.74586,10000000.5463};
//    cell2.pos = {M::random.indexInt((0,(Index)(std::numeric_limits<int>::max)/2)),M::random.indexInt((0,(Index)(std::numeric_limits<int>::max)/2))};
//    cell2.rect = {10000000.213,10000000.45163,10000000.453,10000000.345};
//    cell3.pos = {M::random.indexInt((0,(Index)(std::numeric_limits<int>::max)/2)),M::random.indexInt((0,(Index)(std::numeric_limits<int>::max)/2))};
//    cell3.rect = {10000000.23,10000000.25,10000000.56,10000000.57};
    //cell1.pos = {M::random.indexInt((0,std::numeric_limits<int>::max())),M::random.indexInt((0,std::numeric_limits<int>::max()))};
    //cell1.rect = {10000000.356,10000000.789,10000000.74586,10000000.5463};
    M::random.index((0,std::numeric_limits<int>::max()),&cell1.pos.x);
    M::random.index((0,std::numeric_limits<int>::max()),&cell1.pos.y);
    //M::random.index((0,std::numeric_limits<double>::max()),&cell1.rect.x); //TODO generate() max double
    M::random.index((0,std::numeric_limits<int>::max()),&cell1.rect.x);
    M::random.index((0,std::numeric_limits<int>::max()),&cell1.rect.y);
    M::random.index((0,std::numeric_limits<int>::max()),&cell1.rect.w);
    M::random.index((0,std::numeric_limits<int>::max()),&cell1.rect.h);
    M::random.index((0,std::numeric_limits<int>::max()),&cell2.pos.x);
    M::random.index((0,std::numeric_limits<int>::max()),&cell2.pos.y);
    M::random.index((0,std::numeric_limits<int>::max()),&cell2.rect.x);
    M::random.index((0,std::numeric_limits<int>::max()),&cell2.rect.y);
    M::random.index((0,std::numeric_limits<int>::max()),&cell2.rect.w);
    M::random.index((0,std::numeric_limits<int>::max()),&cell2.rect.h);
    M::random.index((0,std::numeric_limits<int>::max()),&cell3.pos.x);
    M::random.index((0,std::numeric_limits<int>::max()),&cell3.pos.y);
    M::random.index((0,std::numeric_limits<int>::max()),&cell3.rect.x);
    M::random.index((0,std::numeric_limits<int>::max()),&cell3.rect.y);
    M::random.index((0,std::numeric_limits<int>::max()),&cell3.rect.w);
    M::random.index((0,std::numeric_limits<int>::max()),&cell3.rect.h);
//    cell2.pos = {M::random.indexInt((0,std::numeric_limits<int>::max())),M::random.indexInt((0,std::numeric_limits<int>::max()))};
//    cell2.rect = {10000000.213,10000000.45163,10000000.453,10000000.345};
//    cell3.pos = {M::random.indexInt((0,std::numeric_limits<int>::max())),M::random.indexInt((0,std::numeric_limits<int>::max()))};
//    cell3.rect = {10000000.23,10000000.25,100000000.56,10000000.57};
    first.color = PieceColor::White;
    first.type = PieceType::Queen;
    //first.pos = {M::random.indexInt((0,std::numeric_limits<int>::max())),M::random.indexInt((0,std::numeric_limits<int>::max()))};
    M::random.index((0,std::numeric_limits<int>::max()),&first.pos.x);
    M::random.index((0,std::numeric_limits<int>::max()),&first.pos.y);
    second.color = PieceColor::Black;
    second.type = PieceType::King;
    //second.pos = {M::random.indexInt((0,std::numeric_limits<int>::max())),M::random.indexInt((0,std::numeric_limits<int>::max()))};
    M::random.index((0,std::numeric_limits<int>::max()),&second.pos.x);
    M::random.index((0,std::numeric_limits<int>::max()),&second.pos.y);
    zchess.figures.push_back(first);
    zchess.figures.push_back(second);
    zchess.boardCell.insert({1,cell1});
    zchess.boardCell.insert({10,cell2});
    zchess.boardCell.insert({100,cell3});
    samplePacket.state = zchess;
    */


    PacketPacker packet;
    packet.begin();
    pack(samplePacket, &packet);
    packet.end();





    std::string str;
    str.assign((const char*)packet.getData(),(std::size_t)packet.getSize());


    apl::YamlBuilder yamlBuilder;
    pack(samplePacket, &yamlBuilder);


    apl::JsonBuilder jsonBuilder;
    pack(samplePacket, &jsonBuilder);


    //std::cout<<str<<std::endl;

    //const auto packetFile = "packet.temp";
    //apl::saveFile(packetFile, packet.getData(),packet.getSize());
    return str;
}

bool checkPacket(StrView currentPacket, DataParamA recvPacket) {

    if(currentPacket.size()!= recvPacket.size()){
        apl::println("Packet Error: Packets sizes don't match");
        return false;
    }

    auto retcode = std::memcmp(&currentPacket,&recvPacket,recvPacket.size());

    if(retcode){
        return true;
    }
    else{
        return false;
    }
    //return true;
}

}  // namespace multiplayer
