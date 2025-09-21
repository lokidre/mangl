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

#include <map>
#include <unordered_map>

#include <iostream>
#include <stdexcept>


template <class ContainerT>
void testBinary(const ContainerT& data) {
    using namespace mangl;

    const auto packetFile = "packet.temp";

    constexpr PacketFieldId fieldId = 10000;


    PacketPacker packet;

    packet.begin();
    packet.packMap(fieldId, data);
    packet.end();


    apl::saveFile(packetFile, packet.makeStdStringView());



    PacketLoader loader;
    auto packetData = apl::loadFile(packetFile);

    loader.load(packetData);

    ContainerT unpackedData{};

    while (auto field = loader.getId()) {
        switch(field.id) {
            case fieldId: loader.unpackMap(&unpackedData); break;
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
            std::map<int, int> data;
            data.insert({1, 2});
            data.insert({3, 4});
            testBinary(data);
        }


        {
            std::unordered_map<int, double> data;
            data.insert({1, 2.4});
            data.insert({3, 4.8});
            testBinary(data);
        }


        {
            std::unordered_map<int, std::string> data;
            data.insert({1, "2.4"});
            data.insert({3, "4.8"});
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
