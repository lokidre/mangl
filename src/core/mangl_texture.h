/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_types.h"

namespace mangl {

// Initialization
// {id, coords, count, density}

struct Texture {
    using Id = TextureId;

    Id tid;  // User supplied texture id of the corresponding atlas
    const RealGL* coords;  // pointer to texture coordinates
    int count;  // number of images
    RealGL density;

    [[nodiscard]] auto contentsFrame(Index index, int cnt = 0) const noexcept {
        Texture t{};
        t.tid = tid;
        t.coords = coords + index*4;
        t.count = cnt == 0 ? 1 : cnt;
        t.density = density;
        return t;
    }

    [[nodiscard]] auto frame(Index index, int cnt = 0) const noexcept { 
        return contentsFrame(index, cnt); 
    }

    [[nodiscard]] auto getFrame(Index index, int cnt = 0) const noexcept { 
        return contentsFrame(index, cnt); 
    }

    [[nodiscard]] auto getFramesCount() const noexcept { 
        if (!valid())
            return 0;
        return count;
    }


    [[nodiscard]] auto operator[](Index index) const noexcept { return contentsFrame(index); }

    [[nodiscard]] bool valid() const noexcept { return tid != 0; }
    explicit operator bool() const noexcept { return valid(); }
    static constexpr bool validId(Id tid) noexcept { return tid > 0; }
    static constexpr Id invalidId() noexcept { return Id{0}; }

    void clear() noexcept { tid = 0; }

    [[nodiscard]] auto w() const noexcept { return coords[2] / density; }
    [[nodiscard]] auto h() const noexcept { return coords[3] / density; }
    [[nodiscard]] SizeGL sizeGL() const noexcept { return {coords[2] / density, coords[3] / density}; }
    [[nodiscard]] Size size() const noexcept { return {coords[2] / density, coords[3] / density}; }


    // Getting dimensions based on ratio
    [[nodiscard]] Cord ratio() const noexcept { return coords[2] / coords[3]; }

    [[nodiscard]] Cord ratioWidth(Cord height) const noexcept { 
        return height * coords[2] / coords[3]; 
    }

    [[nodiscard]] Cord ratioHeight(Cord width) const noexcept { 
        return width * coords[3] / coords[2]; 
    }

    [[nodiscard]] Size sizeByWidth(Cord width) const noexcept {
        return {width, ratioHeight(width)};
    }

    [[nodiscard]] Size sizeByHeight(Cord height) const noexcept {
        return {ratioWidth(height), height};
    }



    [[nodiscard]] RectGL trect() const noexcept { return {coords[0], coords[1], coords[2], coords[3]}; }
    [[nodiscard]] RectGL origin() const noexcept { return {0, 0, coords[2], coords[3]}; }
    [[nodiscard]] PointGL tpos() const noexcept { return {coords[0], coords[1]}; }
};


using TextureA = const Texture&;

}  // namespace mangl

