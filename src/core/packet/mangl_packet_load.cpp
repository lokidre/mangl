/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_packet.h"

namespace mangl {

void PacketLoader::load(const void* data, Index size) {

    packetData_ = (const uint8_t*)data;
    packetSize_ = size;

    packetPtr_ = packetData_;
    packetEnd_ = packetPtr_ + packetSize_;

    currentOffset_ = 0;

    currentOffset_ += 2;
}

PacketField PacketLoader::getId() {

    using enum PacketFieldType;

    if (packetData_ + currentOffset_ >= packetEnd_) {
        return {.type = Invalid};
    }

    PacketField field{.id = 0};

    auto type = getType();

    if (ObjectBegin == type || ObjectEnd == type || ElementBegin == type || ElementEnd == type) {
        field.type = getType();
    } else {
        massert(Int == type || UInt == type, "Expected integer field id" );

        field.id = static_cast<PacketFieldId>(decodeUint64());

        field.type = getType();
    }

    return field;
}

PacketFieldType PacketLoader::getType() const noexcept {
    auto data = packetData_;


    if ((data[currentOffset_] & 0x80) == 0x00)  return PacketFieldType::UInt;
    if ((data[currentOffset_] & 0xe0) == 0xe0)  return PacketFieldType::Int;
    if ((data[currentOffset_] & 0xa0) == 0xa0)  return PacketFieldType::String;
    if ((data[currentOffset_] & 0xf0) == 0x90)  return PacketFieldType::Array;
    if ((data[currentOffset_] & 0xf0) == 0x80)  return PacketFieldType::Map;


    if ((data[currentOffset_] == 0xd4) &&
        (data[currentOffset_ + 1] == (uint8_t)PacketExtType::ObjectBegin)) {
        return PacketFieldType::ObjectBegin;
    }

    if ((data[currentOffset_] == 0xd5) &&
        (data[currentOffset_ + 1] == (uint8_t)PacketExtType::ElementBegin)) {
        return PacketFieldType::ElementBegin;
    }

    if ((data[currentOffset_] == 0xd6) &&
        (data[currentOffset_ + 1] == (uint8_t)PacketExtType::ObjectBegin)) {
        return PacketFieldType::ObjectBegin;
    }

    if ((data[currentOffset_] == 0xd7) &&
        (data[currentOffset_ + 1] == (uint8_t)PacketExtType::ObjectBegin)) {
        return PacketFieldType::ObjectBegin;
    }

    if ((data[currentOffset_] == 0xd4) &&
        (data[currentOffset_ + 1] == (uint8_t)PacketExtType::ObjectEnd)) {
        return PacketFieldType::ObjectEnd;
    }

    if ((data[currentOffset_] == 0xd5) &&
        (data[currentOffset_ + 1] == (uint8_t)PacketExtType::ElementEnd)) {
        return PacketFieldType::ElementEnd;
    }

    if ((data[currentOffset_] == 0xd6) &&
        (data[currentOffset_ + 1] == (uint8_t)PacketExtType::ObjectEnd)) {
        return PacketFieldType::ObjectEnd;
    }

    if ((data[currentOffset_] == 0xd7) &&
        (data[currentOffset_ + 1] == (uint8_t)PacketExtType::ObjectEnd)) {
        return PacketFieldType::ObjectEnd;
    }

    switch (data[currentOffset_]) {
    case 0xcc:
    case 0xcd:
    case 0xce:
    case 0xcf: return PacketFieldType::UInt;
    case 0xd0:
    case 0xd1:
    case 0xd2:
    case 0xd3: return PacketFieldType::Int;
    case 0xc3:
    case 0xc2: return PacketFieldType::Bool;
    case 0xca: return PacketFieldType::Float;
    case 0xcb: return PacketFieldType::Double;
    case 0xd9:
    case 0xda:
    case 0xdb: return PacketFieldType::String;
    case 0xc4:
    case 0xc5:
    case 0xc6: return PacketFieldType::Binary;
    case 0xdc:
    case 0xdd: return PacketFieldType::Array;
    case 0xde:
    case 0xdf: return PacketFieldType::Map;
    default: return PacketFieldType::Invalid;
    }
}

uint64_t PacketLoader::decodeUint64() {

    auto data = packetData_;

    auto isHighBit = data[currentOffset_] & 0x80;

    if (!isHighBit) {
        return data[currentOffset_++];
    }

    switch (data[currentOffset_]) {
    case 0xcc: currentOffset_++; return data[currentOffset_++];
    case 0xcd: {
        currentOffset_++;
        uint64_t dUint = data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        return dUint;
    }
    case 0xce: {
        currentOffset_++;
        uint64_t dUint = data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        return dUint;
    }
    case 0xcf: {
        currentOffset_++;
        uint64_t dUint = data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        return dUint;
    }
    default: throw std::runtime_error("Invalid format data");
    }


    // return 0;
}


int64_t PacketLoader::decodeInt64() {

    auto data = packetData_;
    // auto data = (int8_t*)packetData_;

    auto isHighBit = data[currentOffset_] & 0x80;

    if (!isHighBit) {
        return data[currentOffset_++];
    }
    if ((data[currentOffset_] & 0xe0) == 0xe0) {
        return (int64_t)(int8_t)data[currentOffset_++];
        // return (int64_t)data[currentOffset_++];
    }

    switch (data[currentOffset_]) {
    case 0xd0: currentOffset_++; return (int64_t)(int8_t)data[currentOffset_++];
    case 0xd1: {
        currentOffset_++;
        uint16_t dint = data[currentOffset_++];
        dint <<= 8;
        dint += data[currentOffset_++];
        return (int64_t)(int16_t)dint;
    }
    case 0xd2: {
        currentOffset_++;
        uint32_t dint = data[currentOffset_++];
        dint <<= 8;
        dint += data[currentOffset_++];
        dint <<= 8;
        dint += data[currentOffset_++];
        dint <<= 8;
        dint += data[currentOffset_++];
        return (int64_t)(int32_t)dint;
    }
    case 0xd3: {
        currentOffset_++;
        uint64_t dint = data[currentOffset_++];
        dint <<= 8;
        dint += data[currentOffset_++];
        dint <<= 8;
        dint += data[currentOffset_++];
        dint <<= 8;
        dint += data[currentOffset_++];
        dint <<= 8;
        dint += data[currentOffset_++];
        dint <<= 8;
        dint += data[currentOffset_++];
        dint <<= 8;
        dint += data[currentOffset_++];
        dint <<= 8;
        dint += data[currentOffset_++];
        return (int64_t)dint;
    }
    case 0xcc: currentOffset_++; return data[currentOffset_++];
    case 0xcd: {
        currentOffset_++;
        int64_t dUint = data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        return dUint;
    }
    case 0xce: {
        currentOffset_++;
        int64_t dUint = data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        return dUint;
    }
    case 0xcf: {
        currentOffset_++;
        int64_t dUint = data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        dUint <<= 8;
        dUint += data[currentOffset_++];
        return dUint;
    }
    default:
        throw std::runtime_error("Invalid format data");
        // return 0;
    }

    // return 0;
}

bool PacketLoader::decodeBool() {

    auto data = packetData_;

    if (0xc3 == data[currentOffset_]) {
        currentOffset_++;
        return true;
    } else {
        currentOffset_++;
        return false;
    }
}

float PacketLoader::decodeFloat() {

    auto data = packetData_;
    float value{};

    if (0xca == data[currentOffset_++]) {
        uint32_t bits = data[currentOffset_++];
        bits <<= 8;
        bits += data[currentOffset_++];
        bits <<= 8;
        bits += data[currentOffset_++];
        bits <<= 8;
        bits += data[currentOffset_++];

        std::memcpy(&value, &bits, sizeof(bits));
        return value;
    }

    throw std::runtime_error("invalid float format");
}

double PacketLoader::decodeDouble() {

    auto data = packetData_;
    double value{};

    if (0xcb == data[currentOffset_++]) {
        uint64_t bits = data[currentOffset_++];
        bits <<= 8;
        bits += data[currentOffset_++];
        bits <<= 8;
        bits += data[currentOffset_++];
        bits <<= 8;
        bits += data[currentOffset_++];
        bits <<= 8;
        bits += data[currentOffset_++];
        bits <<= 8;
        bits += data[currentOffset_++];
        bits <<= 8;
        bits += data[currentOffset_++];
        bits <<= 8;
        bits += data[currentOffset_++];
        std::memcpy(&value, &bits, sizeof(bits));
        return value;
    }

    throw std::runtime_error("invalid double format");
}

void PacketLoader::decodeString(String* str) {

    auto data = packetData_;

    if ((data[currentOffset_] & 0xa0) == 0xa0) {
        auto size = data[currentOffset_++] & 0x1f;
        str->assign(reinterpret_cast<const char*>(data + currentOffset_), size);
        currentOffset_ += size;
    } else {
        switch (data[currentOffset_]) {
        case 0xd9: {
            currentOffset_++;
            auto slength = data[currentOffset_++];
            str->assign(reinterpret_cast<const char*>(data + currentOffset_), slength);
            currentOffset_ += slength;
            break;
        }
        case 0xda: {
            currentOffset_++;
            Index slength = data[currentOffset_++];
            slength <<= 8;
            slength += data[currentOffset_++];
            str->assign(reinterpret_cast<const char*>(data + currentOffset_), slength);
            currentOffset_ += slength;
            break;
        }
        case 0xdb: {
            currentOffset_++;
            Index slength = data[currentOffset_++];
            slength <<= 8;
            slength += data[currentOffset_++];
            slength <<= 8;
            slength += data[currentOffset_++];
            slength <<= 8;
            slength += data[currentOffset_++];
            str->assign(reinterpret_cast<const char*>(data + currentOffset_), slength);
            currentOffset_ += slength;
            break;
        }
        default: break;
        }
    }
}

Index PacketLoader::binarySize() {

    Index size = 0;

    auto data = packetData_;
    switch (data[currentOffset_++]) {
    case 0xc4: {
        size = data[currentOffset_++];
        break;
    }

    case 0xc5: {
        size = data[currentOffset_++];
        size <<= 8;
        size += data[currentOffset_++];
        break;
    }
    case 0xc6: {
        size = data[currentOffset_++];
        size <<= 8;
        size += data[currentOffset_++];
        size <<= 8;
        size += data[currentOffset_++];
        size <<= 8;
        size += data[currentOffset_++];
        break;
    }
    default: throw std::runtime_error("Wrong size of binary"); break;
    }
    return size;
}

Index PacketLoader::arraySize() {

    Index size = 0;
    auto data = packetData_;

    if ((data[currentOffset_] & 0x90) == 0x90) {
        size = data[currentOffset_++] & 0x0f;
    } else {
        switch (data[currentOffset_++]) {
        case 0xdc: {
            size = data[currentOffset_++];
            size <<= 8;
            size += data[currentOffset_++];
            break;
        }
        case 0xdd: {
            size = data[currentOffset_++];
            size <<= 8;
            size += data[currentOffset_++];
            size <<= 8;
            size += data[currentOffset_++];
            size <<= 8;
            size += data[currentOffset_++];
            break;
        }

        default:
            // throw std::runtime_error("Wrong size of array");
            break;
        }
    }
    return size;
}

Index PacketLoader::mapSize() {

    Index size = 0;
    auto data = packetData_;

    if ((data[currentOffset_] & 0x80) == 0x80) {
        size = data[currentOffset_++] & 0x0f;
    } else {
        switch (data[currentOffset_++]) {
        case 0xde: {
            size = data[currentOffset_++];
            size <<= 8;
            size += data[currentOffset_++];
            break;
        }
        case 0xdf: {
            size = data[currentOffset_++];
            size <<= 8;
            size += data[currentOffset_++];
            size <<= 8;
            size += data[currentOffset_++];
            size <<= 8;
            size += data[currentOffset_++];
            break;
        }

        default:
            // throw std::runtime_error("Wrong size of map");
            break;
        }
    }
    return size;
}

void PacketLoader::decodeBinary(void* value, Index size) {

    auto data = packetData_;

    std::memcpy(value, &data[currentOffset_], size);
    currentOffset_ += size;
}


} // namespace mangl
