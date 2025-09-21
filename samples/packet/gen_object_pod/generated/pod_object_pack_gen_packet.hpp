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

#include "pod_object.h"

#include <apeal/index.hpp>
#include <mangl/packet.h>
#include <apeal/file.hpp>

inline void pack(const PodObject& obj, mangl::PacketPacker* packer);
inline void unpack(mangl::PacketLoader& loader, PodObject* obj);


inline auto packStringPacket(const PodObject& obj) {
    mangl::PacketPacker packer;
    pack(obj, &packer);
    std::string packet{packer.makeStdStringView()};
    return packet;
}

inline auto unpackStringPacket(std::string_view packet, PodObject* obj) {
    mangl::PacketLoader loader;
    loader.load(packet);
    unpack(loader, obj);
}

inline auto packFilePacket(std::string_view filename, const PodObject& obj) {
    auto packet = packStringPacket(obj);
    apl::saveFile(filename, packet);
    return packet;
}

inline auto unpackFilePacket(std::string_view filename, PodObject* obj) {
    auto str = apl::loadFile(filename);
    unpackStringPacket(str, obj);
}



namespace impl_PodObject_packet {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(PodObject* obj);
inline bool isEmptyObject(const PodObject& obj);


inline void clearObject(PodObject* obj) {
    obj->intVal = {};
    obj->uintVal = {};
    obj->longVal = {};
    obj->ulongVal = {};
    obj->shortVal = {};
    obj->ushortVal = {};
    obj->charVal = 0;
    obj->ucharVal = 0;
    obj->doubleVal = {};
    obj->floatVal = {};
}
inline bool isEmptyObject(const PodObject& obj) {
    if (obj.intVal != 0) return false;
    if (obj.uintVal != 0) return false;
    if (obj.longVal != 0) return false;
    if (obj.ulongVal != 0) return false;
    if (obj.shortVal != 0) return false;
    if (obj.ushortVal != 0) return false;
    if (obj.charVal != 0) return false;
    if (obj.ucharVal != 0) return false;
    if (obj.doubleVal != 0) return false;
    if (obj.floatVal != 0) return false;
    return true;
}


inline void packObject(const PodObject& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, PodObject* obj);

inline void packObject(const PodObject& obj, mangl::PacketPacker* packer) {
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
    if (obj.charVal != 0)
        packer->pack(7, obj.charVal);
    if (obj.ucharVal != 0)
        packer->pack(8, obj.ucharVal);
    if (obj.doubleVal != 0)
        packer->pack(9, obj.doubleVal);
    if (obj.floatVal != 0)
        packer->pack(10, obj.floatVal);
}


inline void unpackObject(mangl::PacketLoader& loader, PodObject* obj) {
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
            loader.unpack(&(obj->charVal));
            break;
        case 8:
            loader.unpack(&(obj->ucharVal));
            break;
        case 9:
            loader.unpack(&(obj->doubleVal));
            break;
        case 10:
            loader.unpack(&(obj->floatVal));
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}


}  // namespace impl_PodObject_packet


inline void unpack(mangl::PacketLoader& loader, PodObject* obj) {
    impl_PodObject_packet::clearObject(obj);
    impl_PodObject_packet::unpackObject(loader, obj);
}
inline void pack(const PodObject& obj, mangl::PacketPacker* packet) {
    packet->begin();
    impl_PodObject_packet::packObject(obj, packet);
    packet->end();
}
