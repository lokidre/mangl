/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
 /*** Automatically Generated *** DO NOT EDIT ***
 * Generated Date: 2024-09-28
 */
#pragma once

#include "../container_object.h"

#include <apeal/data_param.hpp>
#include <apeal/index.hpp>
#include <mangl/packet.h>

namespace container_test {

inline void pack(const ContainerObject& obj, mangl::PacketPacker* packer);
inline void unpack(mangl::PacketLoader& loader, ContainerObject* obj);


inline auto packStringPacket(const ContainerObject& obj) {
    mangl::PacketPacker packer;
    pack(obj, &packer);
    std::string packet{packer.makeStdStringView()};
    return packet;
}

inline auto unpackStringPacket(std::string_view packet, ContainerObject* obj) {
    mangl::PacketLoader loader;
    loader.load(packet);
    unpack(loader, obj);
}



namespace impl_ContainerObject_packet {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(ContainerObject* obj);
inline bool isEmptyObject(const ContainerObject& obj);
inline void clearObject(ContainerPodValue* obj);
inline bool isEmptyObject(const ContainerPodValue& obj);
inline void clearObject(ContainerVectorValue* obj);
inline bool isEmptyObject(const ContainerVectorValue& obj);
inline void clearObject(ContainerComplexValue* obj);
inline bool isEmptyObject(const ContainerComplexValue& obj);
inline void clearObject(ContainerVectorObjectValue* obj);
inline bool isEmptyObject(const ContainerVectorObjectValue& obj);

inline void clearArray(ContainerPodValue* begin, const ContainerPodValue* end);
inline bool isEmptyArray(const ContainerPodValue* begin, const ContainerPodValue* end);

inline void clearArray(std::integral auto* begin, const std::integral auto* end) {
    for (auto it = begin; it != end; ++it) *it = 0;
}
inline void clearArray(std::floating_point auto* begin, const std::floating_point auto* end) {
    for (auto it = begin; it != end; ++it) *it = 0;
}
inline bool isEmptyArray(const std::integral auto* begin, const std::integral auto* end) {
    for (auto it = begin; it != end; ++it) if (*it != 0) return false;
    return true;
}
inline bool isEmptyArray(const std::floating_point auto* begin, const std::floating_point auto* end) {
    for (auto it = begin; it != end; ++it) if (*it != 0) return false;
    return true;
}

inline void clearArray(ContainerPodValue* begin, const ContainerPodValue* end) {
    for (auto it = begin; it != end; ++it) {
        auto& el = *it;
        clearObject(&el);
    }
}

inline bool isEmptyArray(const ContainerPodValue* begin, const ContainerPodValue* end) {
    for (auto it = begin; it != end; ++it) {
        auto& el = *it;
        if (!isEmptyObject(el)) return false;
    }
    return true;
}


inline void clearObject(ContainerObject* obj) {
    Packet::clearProp(&(obj->charVal));
    Packet::clearProp(&(obj->ucharVal));
    Packet::clearProp(&(obj->intVal));
    Packet::clearProp(&(obj->uintVal));
    Packet::clearProp(&(obj->doubleVal));
    obj->podVector.clear();
    obj->vecVector.clear();
    clearArray(obj->intArray.data(), obj->intArray.data() + obj->intArray.size());
    clearArray(obj->podArray.data(), obj->podArray.data() + obj->podArray.size());
    obj->mapIntInt.clear();
    obj->mapIntStruct.clear();
    obj->mapStringStruct.clear();
    obj->complexVector.clear();
}
inline bool isEmptyObject(const ContainerObject& obj) {
    if (!Packet::isPropEmpty(obj.charVal)) return false;
    if (!Packet::isPropEmpty(obj.ucharVal)) return false;
    if (!Packet::isPropEmpty(obj.intVal)) return false;
    if (!Packet::isPropEmpty(obj.uintVal)) return false;
    if (!Packet::isPropEmpty(obj.doubleVal)) return false;
    if (!obj.podVector.empty()) return false;
    if (!obj.vecVector.empty()) return false;
    if (!isEmptyArray(obj.intArray.data(), obj.intArray.data() + obj.intArray.size())) return false;
    return true;
}

inline void clearObject(ContainerPodValue* obj) {
    Packet::clearProp(&(obj->intVal));
    Packet::clearProp(&(obj->doubleVal));
}
inline bool isEmptyObject(const ContainerPodValue& obj) {
    if (!Packet::isPropEmpty(obj.intVal)) return false;
    if (!Packet::isPropEmpty(obj.doubleVal)) return false;
    return true;
}

inline void clearObject(ContainerVectorValue* obj) {
    obj->podVecVal.clear();
}
inline bool isEmptyObject(const ContainerVectorValue& obj) {
    if (!obj.podVecVal.empty()) return false;
    return true;
}

inline void clearObject(ContainerComplexValue* obj) {
    Packet::clearProp(&(obj->intVal));
    Packet::clearProp(&(obj->doubleVal));
    Packet::clearProp(&(obj->stringVal));
}
inline bool isEmptyObject(const ContainerComplexValue& obj) {
    if (!Packet::isPropEmpty(obj.intVal)) return false;
    if (!Packet::isPropEmpty(obj.doubleVal)) return false;
    if (!Packet::isPropEmpty(obj.stringVal)) return false;
    return true;
}

inline void clearObject(ContainerVectorObjectValue* obj) {
    obj->complexVecVal.clear();
}
inline bool isEmptyObject(const ContainerVectorObjectValue& obj) {
    if (!obj.complexVecVal.empty()) return false;
    return true;
}


inline void packObject(const ContainerObject& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, ContainerObject* obj);
inline void packObject(const ContainerPodValue& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, ContainerPodValue* obj);
inline void packObject(const ContainerVectorValue& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, ContainerVectorValue* obj);
inline void packObject(const ContainerComplexValue& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, ContainerComplexValue* obj);
inline void packObject(const ContainerVectorObjectValue& obj, mangl::PacketPacker* packer);
inline void unpackObject(mangl::PacketLoader& loader, ContainerVectorObjectValue* obj);

inline void packObject(const ContainerObject& obj, mangl::PacketPacker* packer) {
    if (!Packet::isPropEmpty(obj.charVal))
        packer->pack(1, obj.charVal);
    if (!Packet::isPropEmpty(obj.ucharVal))
        packer->pack(2, obj.ucharVal);
    if (!Packet::isPropEmpty(obj.intVal))
        packer->pack(3, obj.intVal);
    if (!Packet::isPropEmpty(obj.uintVal))
        packer->pack(4, obj.uintVal);
    if (!Packet::isPropEmpty(obj.doubleVal))
        packer->pack(5, obj.doubleVal);
    if (!obj.podVector.empty()) {
        Index size = std::ssize(obj.podVector);
        packer->packArrayBegin<Index>(6, size);
        for (auto& val: obj.podVector) {
            packer->beginElement();
            packObject(val, packer);
            packer->endElement();
        }
    }
    if (!obj.vecVector.empty()) {
        Index size = std::ssize(obj.vecVector);
        packer->packArrayBegin<Index>(7, size);
        for (auto& val: obj.vecVector) {
            packer->beginElement();
            packObject(val, packer);
            packer->endElement();
        }
    }
    if (!obj.intArray.empty()) {
        packer->packArray(8, obj.intArray);
    }
    if (!obj.podArray.empty()) {
        Index size = std::ssize(obj.podArray);
        packer->packArrayBegin<Index>(9, size);
        for (auto& val: obj.podArray) {
            packer->beginElement();
            packObject(val, packer);
            packer->endElement();
        }
    }
    if (!obj.mapIntInt.empty()) {
        packer->packMap(10, obj.mapIntInt);
    }
    if (!obj.mapIntStruct.empty()) {
        Index size = std::ssize(obj.mapIntStruct);
        packer->packMapBegin(11, size);
        for (auto& [key, val]: obj.mapIntStruct) {
            packer->packKey(key);
            packer->beginElement();
            packObject(val, packer);
            packer->endElement();
        }
    }
    if (!obj.mapStringStruct.empty()) {
        Index size = std::ssize(obj.mapStringStruct);
        packer->packMapBegin(12, size);
        for (auto& [key, val]: obj.mapStringStruct) {
            packer->packKey(key);
            packer->beginElement();
            packObject(val, packer);
            packer->endElement();
        }
    }
    if (!obj.complexVector.empty()) {
        Index size = std::ssize(obj.complexVector);
        packer->packArrayBegin<Index>(13, size);
        for (auto& val: obj.complexVector) {
            packer->beginElement();
            packObject(val, packer);
            packer->endElement();
        }
    }
}

inline void packObject(const ContainerPodValue& obj, mangl::PacketPacker* packer) {
    if (!Packet::isPropEmpty(obj.intVal))
        packer->pack(1, obj.intVal);
    if (!Packet::isPropEmpty(obj.doubleVal))
        packer->pack(2, obj.doubleVal);
}

inline void packObject(const ContainerVectorValue& obj, mangl::PacketPacker* packer) {
    if (!obj.podVecVal.empty()) {
        packer->packArray(1, obj.podVecVal);
    }
}

inline void packObject(const ContainerComplexValue& obj, mangl::PacketPacker* packer) {
    if (!Packet::isPropEmpty(obj.intVal))
        packer->pack(1, obj.intVal);
    if (!Packet::isPropEmpty(obj.doubleVal))
        packer->pack(2, obj.doubleVal);
    if (!Packet::isPropEmpty(obj.stringVal))
        packer->pack(3, obj.stringVal);
}

inline void packObject(const ContainerVectorObjectValue& obj, mangl::PacketPacker* packer) {
    if (!obj.complexVecVal.empty()) {
        Index size = std::ssize(obj.complexVecVal);
        packer->packArrayBegin<Index>(1, size);
        for (auto& val: obj.complexVecVal) {
            packer->beginElement();
            packObject(val, packer);
            packer->endElement();
        }
    }
}


inline void unpackObject(mangl::PacketLoader& loader, ContainerObject* obj) {
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
            loader.unpack(&obj->charVal);
            break;
        case 2:
            loader.unpack(&obj->ucharVal);
            break;
        case 3:
            loader.unpack(&obj->intVal);
            break;
        case 4:
            loader.unpack(&obj->uintVal);
            break;
        case 5:
            loader.unpack(&obj->doubleVal);
            break;
        case 6:
            {
                auto size = loader.arraySize();
                obj->podVector.resize(size);
                for (auto& val: obj->podVector) {
                    unpackObject(loader, &val);
                }
            }
            break;
        case 7:
            {
                auto size = loader.arraySize();
                obj->vecVector.resize(size);
                for (auto& val: obj->vecVector) {
                    unpackObject(loader, &val);
                }
            }
            break;
        case 8:
            loader.unpackStdArray(&obj->intArray);
            break;
        case 9:
            {
                auto size = loader.arraySize();
                auto containerSize = ssize(obj->podArray);
                Index i = 0;
                for (; i < containerSize; ++i) {
                    unpackObject(loader, &obj->podArray[i]);
                }
                if (i < size) {
                    ContainerPodValue temp;
                    for (; i < size; ++i) {
                        unpackObject(loader, &temp);
                    }
                }
            }
            break;
        case 10:
            loader.unpackMap(&obj->mapIntInt);
            break;
        case 11:
            {
                auto size = loader.mapSize();

                decltype(obj->mapIntStruct)::key_type key;
                decltype(obj->mapIntStruct)::mapped_type value;

                for (int i = 0; i < size; i++) {
                    loader.unpack(&key);
                    unpackObject(loader, &value);
                    obj->mapIntStruct.emplace(key, value);
                }
            }
            break;
        case 12:
            {
                auto size = loader.mapSize();

                decltype(obj->mapStringStruct)::key_type key;
                decltype(obj->mapStringStruct)::mapped_type value;

                for (int i = 0; i < size; i++) {
                    loader.unpack(&key);
                    unpackObject(loader, &value);
                    obj->mapStringStruct.emplace(key, value);
                }
            }
            break;
        case 13:
            {
                auto size = loader.arraySize();
                obj->complexVector.resize(size);
                for (auto& val: obj->complexVector) {
                    unpackObject(loader, &val);
                }
            }
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, ContainerPodValue* obj) {
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
            loader.unpack(&obj->intVal);
            break;
        case 2:
            loader.unpack(&obj->doubleVal);
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, ContainerVectorValue* obj) {
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
            loader.unpackArray(&obj->podVecVal);
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, ContainerComplexValue* obj) {
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
            loader.unpack(&obj->intVal);
            break;
        case 2:
            loader.unpack(&obj->doubleVal);
            break;
        case 3:
            loader.unpack(&obj->stringVal);
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}

inline void unpackObject(mangl::PacketLoader& loader, ContainerVectorObjectValue* obj) {
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
            {
                auto size = loader.arraySize();
                obj->complexVecVal.resize(size);
                for (auto& val: obj->complexVecVal) {
                    unpackObject(loader, &val);
                }
            }
            break;
        default:
            loader.skip(field);
            break;
        }  // switch (property)
    }  // for all fields
}


}  // namespace impl_ContainerObject_packet


inline void unpack(mangl::PacketLoader& loader, ContainerObject* obj) {
    impl_ContainerObject_packet::clearObject(obj);
    impl_ContainerObject_packet::unpackObject(loader, obj);
}
inline void pack(const ContainerObject& obj, mangl::PacketPacker* packer) {
    packer->begin();
    impl_ContainerObject_packet::packObject(obj, packer);
    packer->end();
}
}  // namespace container_test

