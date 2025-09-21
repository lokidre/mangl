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

#include <iostream>
#include <stdexcept>

using namespace mangl;


constexpr PacketFieldId CharFieldId = 1000;
constexpr PacketFieldId UnsignedCharFieldId = 1001;
constexpr PacketFieldId IntFieldId = 2000;
constexpr PacketFieldId UnsignedIntFieldId = 2001;
constexpr PacketFieldId FloatFieldId = 3000;
constexpr PacketFieldId DoubleFieldId = 4000;
constexpr PacketFieldId LongDoubleFieldId = 5000;


struct PodObject {
    char charVal;
    unsigned char unsignedCharVal;
    int intVal;
    unsigned int unsignedIntVal;
};


void testObject(const PodObject& obj) {
    using namespace mangl;

    const auto packetFile = "packet.temp";


    PacketPacker packet;

    packet.begin();
    packet.pack(CharFieldId, obj.charVal);
    packet.pack(IntFieldId, obj.intVal);
    packet.end();


    apl::saveFile(packetFile, packet.makeStdStringView());



    PacketLoader loader;
    auto packetData = apl::loadFile(packetFile);

    loader.load(packetData);

    PodObject unpacked{};

    while (auto field = loader.getId()) {
        switch(field.id) {
        case CharFieldId: loader.unpack(&unpacked.charVal); break;
        case IntFieldId: loader.unpack(&unpacked.intVal); break;
        default: break;
        }
    }

    if (std::memcmp(&obj, &unpacked, sizeof(obj)) != 0) {
        mthrowError("Unpacked data doesn't match");
    }
}


int main(int argc, char* argv[])
{
    int retCode = 1;
    

    try {
        {
            PodObject obj{};
            obj.intVal = 1;
            testObject(obj);
        }

        {
            PodObject obj{};
            obj.charVal = 'A';
            obj.intVal = std::numeric_limits<int>::max();
            testObject(obj);
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
