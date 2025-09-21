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
#include "../mangl_texture.h"

namespace mangl {

struct BackgroundWidgetParams {

    Texture texture;
    // Size backgroundBlockSize;
    Color color;

    Texture beginTexture; // End beginning texture
    Texture endTexture;  // Ending texture


    Texture cornerTexture;  // Left Top, Right Top, Right Bottom, Left Bottom
    Texture cornerLeftTopTexture;
    Texture cornerRightTopTexture;
    Texture cornerRightBottomTexture;
    Texture cornerLeftBottomTexture;
    Size cornerBlockSize;
    Cord cornerRadius;

    Texture borderTexture;  // Left, Top, Right, Bottom
    Size borderBlockSize;
    Cord borderRadius;
    Cord borderWidth;

    Size blockSize;

    Rect rect;

    Flags flags;

    [[nodiscard]] bool valid() const noexcept {
        return texture
            || color.valid()
            || blockSize.valid() 
            || cornerBlockSize.valid() 
            || borderBlockSize.valid()
            || beginTexture || endTexture;
    }

    void clear() {
        BackgroundWidgetParams self{};
        std::swap(*this, self);
    }
};


using BackgroundWidgetParamsA = const BackgroundWidgetParams&;

} // namespace mangl
