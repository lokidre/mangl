/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_common.h"
#include <concepts>
#include <variant>

namespace mangl {

constexpr std::uint8_t PacketPackerVersion = 1;
constexpr std::uint8_t PacketPackerHeader = 0xC;

using PacketFieldId = int;


enum class PacketFieldType {
    Invalid = -1,
    Int = 1,
    UInt,
    Float,
    Double,
    Bool,
    String,
    Binary,
    Map,
    Array,
    ObjectBegin,
    ObjectEnd,
    ElementBegin,
    ElementEnd,
};

enum class PacketExtType {
    Invalid,
    ObjectBegin,
    ObjectEnd,
    ElementBegin,
    ElementEnd,
};


struct PacketField {
    PacketFieldType type;
    PacketFieldId id;

    bool isInvalid() const noexcept { return PacketFieldType::Invalid == type; }

    operator bool() const noexcept { return PacketFieldType::Invalid != type; }
};

class PacketPacker {
public: // Options
    uint8_t version = PacketPackerVersion;
    uint8_t header = PacketPackerHeader;

    Index defaultBufferSize = 256;
    bool encodeLength{false};
    bool encodeCrc{false};


private:
    Index currentOffset_{};
    apl::Buffer<uint8_t> buffer_;

public:
    void begin();

    void end();

    void beginObject(int id);

    void beginElement();

    void endElement();

    void endObject(int id);

    void pack(PacketFieldId id, std::integral auto T) {
        checkBuffer(50);
        encodeInt64(id);
        encodeInt64(T);
    }

    /* void pack(int id, int value) {
         checkBuffer(10);
         encodeInt64(id);
         encodeInt64(value);
     }
     void pack(int id, int64_t value) {
         checkBuffer(10);
         encodeInt64(id);
         encodeInt64(value);
     }*/

    void pack(PacketFieldId id, bool value) {
        checkBuffer(10);
        encodeInt64(id);
        encodeBool(value);
    }

    void pack(int id, float value) {
        checkBuffer(50);
        encodeInt64(id);
        encodeFloat(value);
    }

    void pack(int id, double value) {
        checkBuffer(50);
        encodeInt64(id);
        encodeDouble(value);
    }

    void pack(PacketFieldId fieldId, StrView value) {
        checkBuffer(std::ssize(value) + 10);
        encodeInt64(fieldId);
        encodeString(value);
    }

    void packKey(std::integral auto T) {
        checkBuffer(50);
        encodeInt64(T);
    }

    void packKey(float value) {
        checkBuffer(50);
        encodeFloat(value);
    }

    void packKey(double value) {
        checkBuffer(50);
        encodeDouble(value);
    }

    void packKey(StrView value) {
        checkBuffer(std::ssize(value) + 10);
        encodeString(value);
    }



    //
    // Packing binary
    //
    void packBinary(PacketFieldId id, const void* value, Index size) {
        checkBuffer(size + 50);
        encodeInt64(id);
        encodeBinary(value, size);
    }

    template <class ContainerT>
    void packBinary(PacketFieldId id, const ContainerT& data) {
        packBinary(id, data.data(), data.size());
    }


    template <typename Size>
    void packArrayBegin(PacketFieldId id, Size size) {
        encodeInt64(id);
        encodeArrayBegin((Index)size);
    }

    template <class Container>
    void packArray(PacketFieldId id, const Container& container) {
        encodeInt64(id);
        encodeArray(container);
    }

    template <class ContainerT>
    void packMap(PacketFieldId id, const ContainerT& container) {
        encodeInt64(id);
        encodeMap(container);
    }

    template <class SizeT>
    void packMapBegin(PacketFieldId id, SizeT size) {
        encodeInt64(id);
        encodeMapBegin((Index)size);
    }



    //
    // Accessing data
    //
public:
    Index getSize() const noexcept { return currentOffset_; }

    const uint8_t* getData() const noexcept { return buffer_.get(); }

    const std::span<uint8_t> makeStdSpan() {
        return {(uint8_t*)getData(), (std::size_t)getSize()};
    }


    const std::string makeStdString() const noexcept {
        return {(const char*)getData(), (std::size_t)getSize()};
    }

    const std::string_view makeStdStringView() const noexcept {
        return {(const char*)getData(), (std::size_t)getSize()};
    }

private:
    void checkBuffer(Index length) {
        if (currentOffset_ + length >= std::ssize(buffer_)) {
            buffer_.resize(currentOffset_ + length + 200);
        }
    }


    void encode(std::integral auto value) {
        checkBuffer(50);
        encodeInt64(value);
    }


    // void encode(int64_t value) {
    //     checkBuffer(10);
    //     encodeInt64(value);
    // }

    // void encode(int value) {
    //     checkBuffer(10);
    //     encodeInt64(value);
    // }

    void encode(bool value) {
        checkBuffer(10);
        encodeBool(value);
    }

    void encode(std::floating_point auto value) {
        checkBuffer(10);
        if constexpr (std::same_as<decltype(value), float>) {
            encodeFloat(value);
        } else {
            encodeDouble(value);
        }
    }


    // void encode(float value) {
    //     checkBuffer(10);
    //     encodeFloat(value);
    // }

    // void encode(double value) {
    //     checkBuffer(10);
    //     encodeDouble(value);
    // }

    void encode(StrView value) {
        checkBuffer(std::ssize(value) + 10);
        encodeString(value);
    }

    void encode(const void* value, Index size) {
        checkBuffer(size + 10);
        encodeBinary(value, size);
    }

    void encodeInt64(int64_t value);

    void encodeBool(bool value);

    void encodeFloat(float value);

    void encodeDouble(double value);

    void encodeString(StrView value);

    void encodeBinary(const void* value, Index size);


    template <typename Container>
    void encodeMap(const Container& container) {
        Index size = std::ssize(container);
        
        encodeMapBegin(size);

        for (auto& [key, val]: container) {
            encode(key);
            encode(val);
        }
    }

    void encodeMapBegin(Index size) {

        auto buffer = buffer_.get();

        if (size <= 0x0f) {
            buffer[currentOffset_++] = 0x80 | (uint8_t)size;
        } else if (size <= 0xffff) {
            buffer[currentOffset_++] = 0xde;
            buffer[currentOffset_++] = (uint8_t)(size >> 8);
            buffer[currentOffset_++] = (uint8_t)size;
        } else {
            buffer[currentOffset_++] = 0xdf;
            buffer[currentOffset_++] = (uint8_t)(size >> 24);
            buffer[currentOffset_++] = (uint8_t)(size >> 16);
            buffer[currentOffset_++] = (uint8_t)(size >> 8);
            buffer[currentOffset_++] = (uint8_t)size;
        }
    }

    void encodeArrayBegin(Index size) {

        auto buffer = buffer_.get();

        if (size <= 0x0f) {
            buffer[currentOffset_++] = 0x90 | (uint8_t)size;
        } else if (size <= 0xffff) {
            buffer[currentOffset_++] = 0xdc;
            buffer[currentOffset_++] = (uint8_t)(size >> 8);
            buffer[currentOffset_++] = (uint8_t)size;
        } else {
            buffer[currentOffset_++] = 0xdd;
            buffer[currentOffset_++] = (uint8_t)(size >> 24);
            buffer[currentOffset_++] = (uint8_t)(size >> 16);
            buffer[currentOffset_++] = (uint8_t)(size >> 8);
            buffer[currentOffset_++] = (uint8_t)size;
        }
    }

    template <typename Container>
    void encodeArray(const Container& container) {

        auto buffer = buffer_.get();

        Index size = std::ssize(container);
        encodeArrayBegin(size);

        for (auto& val: container) {
            encode(val);
        }
    }
};

class PacketLoader {
private:
    const uint8_t* packetData_;
    Index packetSize_;

    const uint8_t* packetPtr_{};
    const uint8_t* packetEnd_{};

    Index currentOffset_{};

public:
    void load(const void* data, Index size);
    
    template <class ContainerT>
    void load(const ContainerT& data) {
        load(std::data(data), std::ssize(data));
    }


    template <typename T>
    void unpack(T* value) {
        decode(value);
    }

    template <typename T>
    void unpackVector(T* value) {
        value->resize(binarySize());

        decode(value->data(), value->size());
    }

    template <typename T>
    void unpackSize(T* value) {
        value->resize(arraySize());
    }


    auto unpackArrayBegin() {


        auto size = arraySize();
        //        currentOffset_ = 0;
        //        currentOffset_ += 2;
        auto type = getType();
        //        currentOffset_++;

        return size;
    }

    auto unpackMapBegin() {

        auto size = mapSize();
        auto type = getType();

        return size;
    }


    template <typename ContainerT>
    void unpackArray(ContainerT* container) {
        container->resize(arraySize());

        for (auto& value: *container) {
            decode(&value);
        }
    }

    template <typename Container>
    void unpackStdArray(Container* container) {
        auto arrSize = arraySize();
        Index idx = 0;

        auto curr = container->begin();
        auto end = container->end();

        for (; idx < arrSize; ++idx) {
            if (curr == end) {
                break;
            }
            decode(&(*curr));
            ++curr;
        }

        typename Container::value_type tempVal{};

        for (; idx < arrSize; ++idx) {
            decode(&tempVal);
        }
    }

    template <class ContainerT>
    void unpackSet(ContainerT* container) {

        auto size = arraySize();

        for (Index i = 0; i < size; i++) {

            typename ContainerT::value_type value{};

            decode(&value);

            container->emplace(std::move(value));
        }
    }


    template <class ContainerT>
    void unpackMap(ContainerT* container) {

        auto size = mapSize();

        for (Index i = 0; i < size; i++) {

            typename ContainerT::key_type key{};
            typename ContainerT::mapped_type value{};

            decode(&key);
            decode(&value);

            container->emplace(std::move(key), std::move(value));
        }
    }

    template <class StringT> 
    void unpackString(StringT* str) {
        decodeString(str);
    }


    void unpackObject() { currentOffset_ += 2; }

    void skip(PacketField field) {
        switch(field.type) { 
        case PacketFieldType::Array: {
            auto count = arraySize();
            auto type = getType();
            String dummy;
            for (Index i = 0; i < count; ++i) {
                switch (type) {
                case PacketFieldType::String: decode(&dummy); break;
                default:
                    MDebugTrap();
                    break;
                }
            }
            break;
        }
        default:
            MDebugTrap();
            break;
        }
    }

    void skipObject() { currentOffset_ += 2; }

    //    impl::PacketIterator begin() const noexcept;
    //    impl::PacketIterator end() const noexcept { return {-1, this};  }

    PacketField getId();


    auto getCurrentOffset() { return currentOffset_; }

    //        auto getInt64(int *val) {return *val = (int) decodeInt64();}
    template <class T>
    T unpackValue() {
        T val{};
        unpack(&val);
        return val;
    }

    auto getInt() { return decodeUint64(); };
    auto getBool() { return decodeBool(); };
    auto getFloat() { return decodeFloat(); };
    auto getDouble() { return decodeDouble(); };
    auto getUnpackObj() { return unpackObject(); };
    auto getSkipObj() { return skipObject(); };
    // auto getArr() { return unpackArrayBegin(); };
    auto getUnpackElem() { return unpackObject(); };
    auto getUnpackMap() { return unpackMapBegin(); }
    auto getString() {
        String s;
        decodeString(&s);
        return s;
    };

    Index binarySize();

    Index mapSize();

    Index arraySize();


    [[nodiscard]] PacketFieldType getType() const noexcept;

private:
    // int decodeInt();
    // PacketFieldType getType() const noexcept;
    void decode(std::signed_integral auto* val) {
        using T = std::remove_pointer<decltype(val)>::type;
        *val = static_cast<T>(decodeInt64());
    }

    void decode(std::unsigned_integral auto *val) {
        using T = std::remove_pointer<decltype(val)>::type;
        *val = static_cast<T>(decodeInt64());
    }


    //void decode(unsigned char* val) { *val = (unsigned char)decodeUint64(); }
    //void decode(char* val) { *val = (char)decodeInt64(); }

    //void decode(unsigned int* val) { *val = (unsigned int)decodeUint64(); }
    //void decode(int* val) { *val = (int)decodeInt64(); }

    //void decode(unsigned long* val) { *val = (unsigned long)decodeUint64(); }
    //void decode(long* val) { *val = (long)decodeInt64(); }

    //void decode(int64_t* val) { *val = (int64_t)decodeInt64(); }

    void decode(bool* val) { *val = (bool)decodeBool(); }

    void decode(float* val) { *val = (float)decodeFloat(); }

    void decode(double* val) { *val = (double)decodeDouble(); }

    void decode(String* val) { decodeString(val); }

    void decode(void* value, Index size) { decodeBinary(value, size); }

//    void decode(SizeI* val) {
//        val->w = (int)decodeInt64();
//        val->h = (int)decodeInt64();
//    }
//
//    void decode(PointI* val) {
//        val->x = (int)decodeInt64();
//        val->y = (int)decodeInt64();
//    }

    uint64_t decodeUint64();
    int64_t decodeInt64();

    bool decodeBool();
    float decodeFloat();
    double decodeDouble();

    void decodeString(String* value);
    void decodeBinary(void* value, Index size);
};


//
// Packing unpacking helper functions
//

namespace Packet {

bool isPropEmpty(std::integral auto val) { return val == 0; }
bool isPropEmpty(std::floating_point auto val) { return val == 0; }
bool isPropEmpty(const std::ranges::forward_range auto& val) { return val.empty(); }


template <class DefT>
bool isPropDefault(std::integral auto val, DefT def) noexcept { return val == static_cast<decltype(val)>(def); }

template <class DefT>
bool isPropDefault(std::floating_point auto val, DefT def) noexcept { return val == static_cast<decltype(val)>(def); }

template <class Container>
bool isPropDefault(const std::ranges::forward_range auto& val, const Container& def) { return val == def; }


void clearProp(std::integral auto* val) { *val = 0; }
void clearProp(std::floating_point auto* val) { *val = 0; }
void clearProp(std::ranges::forward_range auto* val) { val->clear(); }


template <class DefT>
void clearProp(std::integral auto* val, DefT def) { *val = def; }
template <class DefT>
void clearProp(std::floating_point auto* val, DefT def) { *val = def; }

template <class DefT>
void clearProp(std::ranges::forward_range auto* val, const DefT& def) { *val = def; }

}

} // namespace mangl
