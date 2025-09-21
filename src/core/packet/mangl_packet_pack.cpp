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

void PacketPacker::begin()
{
    buffer_.alloc(defaultBufferSize);
    currentOffset_ = 0;

    auto buffer = buffer_.get();

    // Encode header byte
    buffer[currentOffset_++] = (uint8_t)((header << 4) | version);
    buffer[currentOffset_++] = 0;  // options
}

void PacketPacker::end()
{
    // TODO: encode length and crc (optional)
    //buffer_.end();
}



void PacketPacker::encodeInt64(int64_t value)
{
    auto buffer = buffer_.get();

    if (value >= 0) {
        
        if (value <= 0x7f) {
            buffer[currentOffset_++] = (uint8_t)value;
            return;
        } 
        
        if (value <= 0xff) {
            buffer[currentOffset_++] = 0xcc;
            buffer[currentOffset_++] = (uint8_t)value;
            return;
        } 

        if (value <= 0xffff) {
            buffer[currentOffset_++] = 0xcd;
            buffer[currentOffset_++] = (uint8_t)(value >> 8);
            buffer[currentOffset_++] = (uint8_t)(value);
            return;
        }

        if (value <= 0xffffffffll) {
            buffer[currentOffset_++] = 0xce;
            buffer[currentOffset_++] = (uint8_t)(value >> 24);
            buffer[currentOffset_++] = (uint8_t)(value >> 16);
            buffer[currentOffset_++] = (uint8_t)(value >> 8);
            buffer[currentOffset_++] = (uint8_t)(value);
            return;

        }
        if (value <= 0xffffffffffffffffll) {
            buffer[currentOffset_++] = 0xcf;
            buffer[currentOffset_++] = (uint8_t)(value >> 56);
            buffer[currentOffset_++] = (uint8_t)(value >> 48);
            buffer[currentOffset_++] = (uint8_t)(value >> 40);
            buffer[currentOffset_++] = (uint8_t)(value >> 32);
            buffer[currentOffset_++] = (uint8_t)(value >> 24);
            buffer[currentOffset_++] = (uint8_t)(value >> 16);
            buffer[currentOffset_++] = (uint8_t)(value >> 8);
            buffer[currentOffset_++] = (uint8_t)(value);
            return;
        }
        else {
            throw std::runtime_error("Invalid format data");
            //return ;
        }
    
    } else {  // value is negative
        if (value >= -0x20) {
            buffer[currentOffset_++] = (int8_t)value;
            return;
        }

        if (value >= -0xff) {
            buffer[currentOffset_++] = 0xd0;
            buffer[currentOffset_++] = (int8_t)value;
            return;
        }

        if (value >= -0xffff) {
            buffer[currentOffset_++] = 0xd1;
            buffer[currentOffset_++] = (int8_t)(value >> 8);
            buffer[currentOffset_++] = (int8_t)value;
            return;
        }
        if (value >= -0xffffffffll) {
            buffer[currentOffset_++] = 0xd2;
            buffer[currentOffset_++] = (int8_t)(value >> 24);
            buffer[currentOffset_++] = (int8_t)(value >> 16);
            buffer[currentOffset_++] = (int8_t)(value >> 8);
            buffer[currentOffset_++] = (int8_t)value;
            return;
        }

        //if ( value >= -0xffffffffffffffffll) {
            buffer[currentOffset_++] = 0xd3;
            buffer[currentOffset_++] = (int8_t)(value >> 56);
            buffer[currentOffset_++] = (int8_t)(value >> 48);
            buffer[currentOffset_++] = (int8_t)(value >> 40);
            buffer[currentOffset_++] = (int8_t)(value >> 32);
            buffer[currentOffset_++] = (int8_t)(value >> 24);
            buffer[currentOffset_++] = (int8_t)(value >> 16);
            buffer[currentOffset_++] = (int8_t)(value >> 8);
            buffer[currentOffset_++] = (int8_t)(value);
            return;
        //}

        //else {
        //    throw std::runtime_error("Invalid format data");
        //    //return;
        //}
    }
}

void PacketPacker::encodeBool(bool value) {

    auto buffer = buffer_.get();

    if (value) {
        buffer[currentOffset_++] = 0xc3;
    }
    else {
        buffer[currentOffset_++] = 0xc2;
    }
}

void PacketPacker::encodeFloat(float value) {

    auto buffer = buffer_.get();

    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(value));
    buffer[currentOffset_++] = 0xca;
    buffer[currentOffset_++] = (uint8_t)(bits >> 24);
    buffer[currentOffset_++] = (uint8_t)(bits >> 16);
    buffer[currentOffset_++] = (uint8_t)(bits >> 8);
    buffer[currentOffset_++] = (uint8_t)(bits);
}

void PacketPacker::encodeDouble(double value) {
    auto buffer = buffer_.get();

    uint64_t bits;
    std::memcpy(&bits, &value, sizeof(value));
    buffer[currentOffset_++] = 0xcb;
    buffer[currentOffset_++] = (uint8_t)(bits >> 56);
    buffer[currentOffset_++] = (uint8_t)(bits >> 48);
    buffer[currentOffset_++] = (uint8_t)(bits >> 40);
    buffer[currentOffset_++] = (uint8_t)(bits >> 32);
    buffer[currentOffset_++] = (uint8_t)(bits >> 24);
    buffer[currentOffset_++] = (uint8_t)(bits >> 16);
    buffer[currentOffset_++] = (uint8_t)(bits >> 8);
    buffer[currentOffset_++] = (uint8_t)(bits);

}

void PacketPacker::encodeString(StrView value) {

    auto buffer = buffer_.get();
    Index slength = std::ssize(value);
    const uint8_t* sdata = (const uint8_t * )value.data();

    if (slength <= 0x1f) {
        buffer[currentOffset_++] = 0xA0 | (uint8_t)slength;

        std::memcpy(&buffer[currentOffset_], sdata, slength);
        currentOffset_ += value.length();

        //for (Index i = 0; i < length; ++i) {
        //    buffer[currentOffset_++] = sdata[i];
        //}
        return;
    }

    if (value.length() <= 0xff) {
        buffer[currentOffset_++] = 0xd9;

        buffer[currentOffset_++] = (uint8_t)slength;

        std::memcpy(&buffer[currentOffset_], sdata, slength);
        currentOffset_ += value.length();

        return;
    }
    if (value.length() <= 0xffff) {
        buffer[currentOffset_++] = 0xda;

        buffer[currentOffset_++] = (uint8_t)(slength >>8);
        buffer[currentOffset_++] = (uint8_t)slength;

        std::memcpy(&buffer[currentOffset_], sdata, slength);
        currentOffset_ += value.length();
  
        return;
    }
    else {
        buffer[currentOffset_++] = 0xdb;
        buffer[currentOffset_++] = (uint8_t)(slength >> 24);
        buffer[currentOffset_++] = (uint8_t)(slength >> 16);
        buffer[currentOffset_++] = (uint8_t)(slength >> 8);
        buffer[currentOffset_++] = (uint8_t)slength;

        std::memcpy(&buffer[currentOffset_], sdata, slength);
        currentOffset_ += value.length();

        return;
    }
    
}

void PacketPacker::encodeBinary(const void* value, Index size) {

    auto buffer = buffer_.get();
    const uint8_t* sdata = (const uint8_t*)value;

    if (size <= 0xff) {
        buffer[currentOffset_++] = 0xc4;
        buffer[currentOffset_++] = (uint8_t)size;

        std::memcpy(&buffer[currentOffset_], sdata, size);
        currentOffset_ += size;
        return;
    }
    if (size <= 0xffff) {
        buffer[currentOffset_++] = 0xc5;
        buffer[currentOffset_++] = (uint8_t)(size >> 8);
        buffer[currentOffset_++] = (uint8_t)size;

        std::memcpy(&buffer[currentOffset_], sdata, size);
        currentOffset_ += size;
        return;
    }
    else {
        buffer[currentOffset_++] = 0xc6;
        buffer[currentOffset_++] = (uint8_t)(size >> 24);
        buffer[currentOffset_++] = (uint8_t)(size >> 16);
        buffer[currentOffset_++] = (uint8_t)(size >> 8);
        buffer[currentOffset_++] = (uint8_t)size;

        std::memcpy(&buffer[currentOffset_], sdata, size);
        currentOffset_ += size;
        return;
    
    }

}

void PacketPacker::beginObject(int id) {

    auto buffer = buffer_.get();

    encodeInt64(id);

    buffer[currentOffset_++] = 0xd4;
    buffer[currentOffset_++] = (uint8_t)PacketExtType::ObjectBegin;

/*
    if (id < 0xff) {
        buffer[currentOffset_++] = (uint8_t)id;
        buffer[currentOffset_++] = 0xd4;
        buffer[currentOffset_++] = (uint8_t)PacketExtType::ObjectBegin;
        return;
    }
    if (id < 0xffff) {
        buffer[currentOffset_++] = (uint8_t)(id >> 8);
        buffer[currentOffset_++] = (uint8_t)id;
        buffer[currentOffset_++] = 0xd5;
        buffer[currentOffset_++] = (uint8_t)PacketExtType::ObjectBegin;
        return;
    }
    if (id < 0xffffffff) {
        buffer[currentOffset_++] = (uint8_t)(id >> 24);
        buffer[currentOffset_++] = (uint8_t)(id >> 16);
        buffer[currentOffset_++] = (uint8_t)(id >> 8);
        buffer[currentOffset_++] = (uint8_t)id;
        buffer[currentOffset_++] = 0xd6;
        buffer[currentOffset_++] = (uint8_t)PacketExtType::ObjectBegin;
        return;
    }
    else {
        buffer[currentOffset_++] = (uint8_t)(id >> 56);
        buffer[currentOffset_++] = (uint8_t)(id >> 48);
        buffer[currentOffset_++] = (uint8_t)(id >> 40);
        buffer[currentOffset_++] = (uint8_t)(id >> 32);
        buffer[currentOffset_++] = (uint8_t)(id >> 24);
        buffer[currentOffset_++] = (uint8_t)(id >> 16);
        buffer[currentOffset_++] = (uint8_t)(id >> 8);
        buffer[currentOffset_++] = (uint8_t)id;
        buffer[currentOffset_++] = 0xd7;
        buffer[currentOffset_++] = (uint8_t)PacketExtType::ObjectBegin;
        return;
    }
*/

}

void PacketPacker::beginElement() {

    auto buffer = buffer_.get();

    //encodeInt64(id);

    buffer[currentOffset_++] = 0xd5;
    buffer[currentOffset_++] = (uint8_t)PacketExtType::ElementBegin;
}


void PacketPacker::endObject(int id) {

    auto buffer = buffer_.get();

    encodeInt64(id);

    buffer[currentOffset_++] = 0xd4;
    buffer[currentOffset_++] = (uint8_t)PacketExtType::ObjectEnd;

    /*
    if (id < 0xff) {
        buffer[currentOffset_++] = (uint8_t)id;
        buffer[currentOffset_++] = 0xd4;
        buffer[currentOffset_++] = (uint8_t)PacketExtType::ObjectEnd;
        return;
    }
    if (id < 0xffff) {
        buffer[currentOffset_++] = (uint8_t)(id >> 8);
        buffer[currentOffset_++] = (uint8_t)id;
        buffer[currentOffset_++] = 0xd5;
        buffer[currentOffset_++] = (uint8_t)PacketExtType::ObjectEnd;
        return;
    }
    if (id < 0xffffffff) {
        buffer[currentOffset_++] = (uint8_t)(id >> 16);
        buffer[currentOffset_++] = (uint8_t)(id >> 8);
        buffer[currentOffset_++] = (uint8_t)id;
        buffer[currentOffset_++] = 0xd6;
        buffer[currentOffset_++] = (uint8_t)PacketExtType::ObjectEnd;
        return;
    } else {
        buffer[currentOffset_++] = (uint8_t)(id >> 24);
        buffer[currentOffset_++] = (uint8_t)(id >> 16);
        buffer[currentOffset_++] = (uint8_t)(id >> 8);
        buffer[currentOffset_++] = (uint8_t)id;
        buffer[currentOffset_++] = 0xd7;
        buffer[currentOffset_++] = (uint8_t)PacketExtType::ObjectEnd;
        return;
    }
*/
}

void PacketPacker::endElement() {

    auto buffer = buffer_.get();

    //encodeInt64(id);

    buffer[currentOffset_++] = 0xd5;
    buffer[currentOffset_++] = (uint8_t)PacketExtType::ElementEnd;
}



}  // namespace mangl
