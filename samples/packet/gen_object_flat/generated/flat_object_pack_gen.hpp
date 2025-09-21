/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
 /*** Automatically Generated *** DO NOT EDIT ***
 * Generated Date: 2024-06-20
 */
#pragma once

#include "flat_object.h"

#include <apeal/index.hpp>
#include <mangl/packet.h>
#include <apeal/file.hpp>

inline void pack(const FlatObject& obj, mangl::PacketPacker* packer);
inline void unpack(mangl::PacketLoader& loader, FlatObject* obj);


inline auto packStringPacket(const FlatObject& obj) {
    mangl::PacketPacker packer;
    pack(obj, &packer);
    std::string packet{packer.makeStdStringView()};
    return packet;
}

inline auto unpackStringPacket(std::string_view packet, FlatObject* obj) {
    mangl::PacketLoader loader;
    loader.load(packet);
    unpack(loader, obj);
}

inline auto packFilePacket(std::string_view filename, const FlatObject& obj) {
    auto packet = packStringPacket(obj);
    apl::saveFile(filename, packet);
    return packet;
}

inline auto unpackFilePacket(std::string_view filename, FlatObject* obj) {
    auto str = apl::loadFile(filename);
    unpackStringPacket(str, obj);
}



namespace impl_FlatObject_packet {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(FlatObject* obj);
inline bool isEmptyObject(const FlatObject& obj);


inline void clearObject(FlatObject* obj) {
    obj->intVal = {};
    obj->uintVal = {};
    obj->longVal = {};
    obj->ulongVal = {};
    obj->shortVal = {};
    obj->ushortVal = {};
    obj->doubleVal = {};
    obj->floatVal = {};
    obj->charVal = 0;
    obj->ucharVal = 0;
    obj->stringVal.clear();
    obj->charEnum = {};
    obj->intEnum = {};
    obj->vectorVal.clear();
    obj->mapVal.clear();
    obj->unorderedMapVal.clear();
    obj->setVal.clear();
}
inline bool isEmptyObject(const FlatObject& obj) {
    if (obj.intVal != 0) return false;
    if (obj.uintVal != 0) return false;
    if (obj.longVal != 0) return false;
    if (obj.ulongVal != 0) return false;
    if (obj.shortVal != 0) return false;
    if (obj.ushortVal != 0) return false;
    if (obj.doubleVal != 0) return false;
    if (obj.floatVal != 0) return false;
    if (obj.charVal != 0) return false;
    if (obj.ucharVal != 0) return false;
    if (!obj.stringVal.empty()) return false;
    if (int(obj.charEnum) != int(0)) return false;
    if (int(obj.intEnum) != int(0)) return false;
    if (!obj.vectorVal.empty()) return false;
    if (!obj.mapVal.empty()) return false;
    if (!obj.unorderedMapVal.empty()) return false;
    if (!obj.setVal.empty()) return false;
    return true;
}


inline void packObject(const FlatObject& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, FlatObject* obj);

inline void packObject(const FlatObject& obj, mangl::PacketPacker* packer) {
    if (obj.intVal != 0)
        packer->pack(1, obj.intVal);
    if (obj.uintVal != 0)
        packer->pack(2, obj.uintVal);
    if (obj.longVal != 0)
        packer->pack(3, obj.longVal);
    if (obj.ulongVal != 0)
        packer->pack(4, obj.ulongVal);
    if (obj.shortVal != 0)
        packer->pack(5, obj.shortVal);
    if (obj.ushortVal != 0)
        packer->pack(6, obj.ushortVal);
    if (obj.doubleVal != 0)
        packer->pack(7, obj.doubleVal);
    if (obj.floatVal != 0)
        packer->pack(8, obj.floatVal);
    if (obj.charVal != 0)
        packer->pack(9, obj.charVal);
    if (obj.ucharVal != 0)
        packer->pack(10, obj.ucharVal);
    if (!obj.stringVal.empty())
        packer->pack(11, obj.stringVal);
    if (static_cast<char>(obj.charEnum) != 0)
        packer->pack(12, static_cast<char>(obj.charEnum));
    if (static_cast<int>(obj.intEnum) != 0)
        packer->pack(13, static_cast<int>(obj.intEnum));
    if (!obj.vectorVal.empty()) {
        packer->packArray(14, obj.vectorVal);
    }
    if (!obj.mapVal.empty()) {
        packer->packMap(15, obj.mapVal);
    }
    if (!obj.unorderedMapVal.empty()) {
        packer->packMap(16, obj.unorderedMapVal);
    }
    if (!obj.setVal.empty()) {
        packer->packArray(17, obj.setVal);
    }
}


inline void unpackObject(mangl::PacketLoader& loader, FlatObject* obj) {
    while (auto field = loader.getId()) {
        if (PacketFieldType::ElementBegin == field.type) {
            loader.unpackObject();
            field = loader.getId();
        }
        if (PacketFieldType::ObjectEnd == field.type || PacketFieldType::ElementEnd == field.type) {
            loader.unpackObject();
            return;
        }
        switch(field.id) {
        case 1:
            loader.unpack(&(obj->intVal));
            break;
        case 2:
            loader.unpack(&(obj->uintVal));
            break;
        case 3:
            loader.unpack(&(obj->longVal));
            break;
        case 4:
            loader.unpack(&(obj->ulongVal));
            break;
        case 5:
            loader.unpack(&(obj->shortVal));
            break;
        case 6:
            loader.unpack(&(obj->ushortVal));
            break;
        case 7:
            loader.unpack(&(obj->doubleVal));
            break;
        case 8:
            loader.unpack(&(obj->floatVal));
            break;
        case 9:
            loader.unpack(&(obj->charVal));
            break;
        case 10:
            loader.unpack(&(obj->ucharVal));
            break;
        case 11:
            loader.unpack(&(obj->stringVal));
            break;
        case 12:
            {
                char val;
                loader.unpack(&val);
                obj->charEnum = static_cast<SampleCharEnum>(val);
            }
            break;
        case 13:
            {
                int val;
                loader.unpack(&val);
                obj->intEnum = static_cast<SampleIntEnum>(val);
            }
            break;
        case 14:
            loader.unpackArray(&obj->vectorVal);
            break;
        case 15:
            loader.unpackMap(&(obj->mapVal));
            break;
        case 16:
            loader.unpackMap(&(obj->unorderedMapVal));
            break;
        case 17:
            loader.unpackSet(&obj->setVal);
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}


}  // namespace impl_FlatObject_packet


inline void unpack(mangl::PacketLoader& loader, FlatObject* obj) {
    impl_FlatObject_packet::clearObject(obj);
    impl_FlatObject_packet::unpackObject(loader, obj);
}
inline void pack(const FlatObject& obj, mangl::PacketPacker* packet) {
    packet->begin();
    impl_FlatObject_packet::packObject(obj, packet);
    packet->end();
}
