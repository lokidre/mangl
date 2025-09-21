/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "sample_echo_packet.h"

#include "../shared/sample_game_packet.hpp"
#include "../shared/sample_game_packet_gen.hpp"
#include "../shared/sample_game_packet_gen_yaml.hpp"
#include "../shared/sample_game_packet_gen_json.hpp"

#include <apeal/console.hpp>

#include <limits>


namespace sample::mp {

String processPacket(DataParamA packet) {
    SampleGamePacket samplePacket;

    PacketLoader packetLoader;

    packetLoader.load(packet.data(),packet.size());

    unpack(packetLoader, &samplePacket);


    PacketPacker packetPacker;

    packetPacker.begin();
    pack(samplePacket, &packetPacker);
    packetPacker.end();

    std::string str = packetPacker.makeStdString();

    return str;
}

}  // namespace sample::mp

