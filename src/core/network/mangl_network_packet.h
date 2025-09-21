/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_common.h"

//#include "mangl_multiplayer.h"

//#include <apeal/hash.hpp>
//#include <apeal/buffer/ring_buffer.hpp>

//#include <concepts>

namespace mangl::net {

struct Packet {
    std::vector<std::byte> buffer;
    Index length{};
    int code{};
    bool valid{}, started{};


    auto feed(DataParamA data) {
        buffer.resize(data.size());
        std::memcpy(buffer.data(), data.data(), data.size());
        valid = true;
        length = data.size();
        return data.size();
    }

    auto alloc(std::size_t len) {
        buffer.resize(len);
        valid = true;
        return buffer.data();
    }

    [[nodiscard]] auto data() const noexcept { return DataParam{buffer.data(), buffer.size()}; }

    [[nodiscard]] auto span() const noexcept { return std::span{buffer.data(), buffer.size()}; }

    [[nodiscard]] auto strView() const noexcept {
        auto ptr = reinterpret_cast<const char *>(buffer.data());
        return StrView{ptr, buffer.size()};
    }
};

}  // namespace mangl::net
