/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <mangl/packet.h>

#include <apeal/parse/yaml.hpp>
#include <apeal/parse/json.hpp>

#include <apeal/file.hpp>
#include <apeal/console.hpp>

#include <limits>


template <class T>
void setLowestValue1(T* val) {
    *val = std::numeric_limits<T>::lowest() + 1;
}

template <class T>
void setLowestValue(T* val) {
    *val = std::numeric_limits<T>::lowest();
}


template <class T>
void setMaxValue1(T* val) {
    *val = std::numeric_limits<T>::max() - 1;
}


template <class T>
void setMaxValue(T* val) {
    *val = std::numeric_limits<T>::max();
}


template <class PacketObject>
void packetTest(const PacketObject& obj, apl::String filePrefix) {

    using namespace mangl;

    String packetFile;

    PacketObject unpacked{};


    auto verifyPacket = [&] {
        apl::println("Checking: %s", packetFile);

        if (obj != unpacked) {
            mthrowError("%s: %s", packetFile, "Unpacked data doesn't match");
        }
    };

    auto makePacketFile = [&](StrView ext) {
        return apl::fmt("packet-%s.%s", filePrefix, ext);
    };


#define PACKET_TEST_SHORT 0

#if PACKET_TEST_SHORT
    packetFile = makePacketFile("bin");
    packFilePacket(packetFile, obj);
    unpackFilePacket(packetFile, &unpacked);
    verifyPacket();

    packetFile = makePacketFile("yaml");
    packFileYaml(packetFile, obj);
    unpackFileYaml(packetFile, &unpacked);
    verifyPacket();

    packetFile = makePacketFile("json");
    packFileJson(packetFile, obj);
    unpackFileJson(packetFile, &unpacked);
    verifyPacket();

#else

    PacketPacker packer;
    pack(obj, &packer);
    packetFile = makePacketFile("bin");

    apl::saveFile(packetFile, packer.makeStdStringView());


    PacketLoader loader;
    auto packetData = apl::loadFile(packetFile);

    loader.load(packetData);
    unpack(loader, &unpacked);

    verifyPacket();


    packetFile = makePacketFile("yaml");

    apl::YamlBuilder yamlBuilder;
    pack(obj, &yamlBuilder);
    apl::saveFile(packetFile, yamlBuilder.str());

    auto yaml = apl::loadYaml(packetFile);
    unpack(yaml, &unpacked);

    verifyPacket();

    packetFile = makePacketFile("json");


    apl::JsonBuilder jsonBuilder;
    pack(obj, &jsonBuilder);
    apl::saveFile(packetFile, jsonBuilder.str());

    auto json = apl::loadJson(packetFile);
    unpack(json, &unpacked);

    verifyPacket();

#endif
}
