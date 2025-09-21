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


template <class ContainerT>
void testBinary(const ContainerT& data) {
    using namespace mangl;

    const auto packetFile = "packet.temp";

    constexpr PacketFieldId fieldId = 10000;


    PacketPacker packet;

    packet.begin();
    packet.packBinary(fieldId, data);
    packet.end();


    apl::saveFile(packetFile, packet.makeStdStringView());



    PacketLoader loader;
    auto packetData = apl::loadFile(packetFile);

    loader.load(packetData);

    ContainerT unpackedData{};

    while (auto field = loader.getId()) {
        switch(field.id) {
            case fieldId: loader.unpackVector(&unpackedData); break;
            default: break;
        }
    }

    if (data != unpackedData) {
        mthrowError("Unpacked data doesn't match");
    }
}


int main(int argc, char* argv[])
{
    int retCode = 1;
    

    try {
        {
            std::string data;
            data.append(1, '1');
            testBinary(data);
        }


        {
            std::vector<std::uint8_t> data;
            data.push_back(0);
            data.push_back(10);
            data.push_back(30);
            testBinary(data);
        }


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
