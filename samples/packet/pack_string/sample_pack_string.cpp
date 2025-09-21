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

#include <iostream>
#include <stdexcept>


void testString(const mangl::StrView str) {
    using namespace mangl;

    const auto packetFile = "packet.temp";

    constexpr PacketFieldId fieldId = 10000;


    PacketPacker packet;

    packet.begin();
    packet.pack(fieldId, str);
    packet.end();


    apl::saveFile(packetFile, packet.makeStdStringView());



    PacketLoader loader;
    auto packetData = apl::loadFile(packetFile);

    loader.load(packetData);

    String unpacked;

    while (auto field = loader.getId()) {
        switch(field.id) {
            case fieldId: loader.unpackString(&unpacked); break;
            default: break;
        }
    }

    if (str != unpacked) {
        mthrowError("Unpacked data doesn't match");
    }
}


int main(int argc, char* argv[])
{
    int retCode = 1;
    

    try {
        testString("");
        testString("abcd");
        testString(
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
            "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris " 
            "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor "
            "in reprehenderit in voluptate "
            "velit esse cillum dolore eu fugiat nulla pariatur. "
            "Excepteur sint occaecat cupidatat non proident, "
            "sunt in culpa qui officia deserunt mollit anim id est laborum."
        );


        retCode = 0;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Exception!" << std::endl;
    }

    return retCode;
}
