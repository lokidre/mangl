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

#include "nested_object.h"

#include <apeal/index.hpp>
#include <mangl/packet.h>
#include <apeal/file.hpp>

inline void pack(const NestedObject& obj, mangl::PacketPacker* packer);
inline void unpack(mangl::PacketLoader& loader, NestedObject* obj);


inline auto packStringPacket(const NestedObject& obj) {
    mangl::PacketPacker packer;
    pack(obj, &packer);
    std::string packet{packer.makeStdStringView()};
    return packet;
}

inline auto unpackStringPacket(std::string_view packet, NestedObject* obj) {
    mangl::PacketLoader loader;
    loader.load(packet);
    unpack(loader, obj);
}

inline auto packFilePacket(std::string_view filename, const NestedObject& obj) {
    auto packet = packStringPacket(obj);
    apl::saveFile(filename, packet);
    return packet;
}

inline auto unpackFilePacket(std::string_view filename, NestedObject* obj) {
    auto str = apl::loadFile(filename);
    unpackStringPacket(str, obj);
}



namespace impl_NestedObject_packet {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(NestedObject* obj);
inline bool isEmptyObject(const NestedObject& obj);
inline void clearObject(SubObject* obj);
inline bool isEmptyObject(const SubObject& obj);
inline void clearObject(SubSubObject* obj);
inline bool isEmptyObject(const SubSubObject& obj);


inline void clearObject(NestedObject* obj) {
    obj->charVal = 0;
    obj->ucharVal = 0;
    obj->intVal = 0;
    obj->uintVal = 0;
    obj->floatVal = 0;
    obj->doubleVal = 0;
    clearObject(&(obj->subVal));
}
inline bool isEmptyObject(const NestedObject& obj) {
    if (obj.charVal != 0) return false;
    if (obj.ucharVal != 0) return false;
    if (obj.intVal != 0) return false;
    if (obj.uintVal != 0) return false;
    if (obj.floatVal != 0) return false;
    if (obj.doubleVal != 0) return false;
    if (!isEmptyObject(obj.subVal)) return false;
    return true;
}

inline void clearObject(SubObject* obj) {
    obj->vectorVal.clear();
    clearObject(&(obj->subSubVal));
}
inline bool isEmptyObject(const SubObject& obj) {
    if (!obj.vectorVal.empty()) return false;
    if (!isEmptyObject(obj.subSubVal)) return false;
    return true;
}

inline void clearObject(SubSubObject* obj) {
    obj->mapVal.clear();
}
inline bool isEmptyObject(const SubSubObject& obj) {
    if (!obj.mapVal.empty()) return false;
    return true;
}


inline void packObject(const NestedObject& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, NestedObject* obj);
inline void packObject(const SubObject& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, SubObject* obj);
inline void packObject(const SubSubObject& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, SubSubObject* obj);

inline void packObject(const NestedObject& obj, mangl::PacketPacker* packer) {
    if (obj.charVal != 0)
        packer->pack(1, obj.charVal);
    if (obj.ucharVal != 0)
        packer->pack(2, obj.ucharVal);
    if (obj.intVal != 0)
        packer->pack(3, obj.intVal);
    if (obj.uintVal != 0)
        packer->pack(4, obj.uintVal);
    if (obj.floatVal != 0)
        packer->pack(5, obj.floatVal);
    if (obj.doubleVal != 0)
        packer->pack(6, obj.doubleVal);
    if (!isEmptyObject(obj.subVal)) {
        packer->beginObject(7);
        packObject(obj.subVal, packer);
        packer->endObject(7);
    }

}

inline void packObject(const SubObject& obj, mangl::PacketPacker* packer) {
    if (!obj.vectorVal.empty()) {
        packer->packArray(1, obj.vectorVal);
    }
    if (!isEmptyObject(obj.subSubVal)) {
        packer->beginObject(2);
        packObject(obj.subSubVal, packer);
        packer->endObject(2);
    }

}

inline void packObject(const SubSubObject& obj, mangl::PacketPacker* packer) {
    if (!obj.mapVal.empty()) {
        packer->packMap(1, obj.mapVal);
    }
}


inline void unpackObject(mangl::PacketLoader& loader, NestedObject* obj) {
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
            loader.unpack(&(obj->charVal));
            break;
        case 2:
            loader.unpack(&(obj->ucharVal));
            break;
        case 3:
            loader.unpack(&(obj->intVal));
            break;
        case 4:
            loader.unpack(&(obj->uintVal));
            break;
        case 5:
            loader.unpack(&(obj->floatVal));
            break;
        case 6:
            loader.unpack(&(obj->doubleVal));
            break;
        case 7:
            loader.unpackObject();
            if (PacketFieldType::ObjectBegin == field.type) {
                unpackObject(loader, &(obj->subVal));
            }
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, SubObject* obj) {
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
            loader.unpackArray(&obj->vectorVal);
            break;
        case 2:
            loader.unpackObject();
            if (PacketFieldType::ObjectBegin == field.type) {
                unpackObject(loader, &(obj->subSubVal));
            }
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, SubSubObject* obj) {
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
            loader.unpackMap(&(obj->mapVal));
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}


}  // namespace impl_NestedObject_packet


inline void unpack(mangl::PacketLoader& loader, NestedObject* obj) {
    impl_NestedObject_packet::clearObject(obj);
    impl_NestedObject_packet::unpackObject(loader, obj);
}
inline void pack(const NestedObject& obj, mangl::PacketPacker* packet) {
    packet->begin();
    impl_NestedObject_packet::packObject(obj, packet);
    packet->end();
}
