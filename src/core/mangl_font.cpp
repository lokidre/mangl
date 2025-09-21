/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_font.h"

namespace mangl {

namespace internal {
const Font* defaultFont_{};
}

void Font::setDefault() const { internal::defaultFont_ = this; }

const Font* Font::getDefaultFont() noexcept { return internal::defaultFont_; }

bool Font::isCharsetSupported(StrView text) const noexcept {
    for (auto&& ch: text) {
        if (apl::parse_is_space(ch))
            continue;
        if (charlist.find(ch) == charlist.end())
            return false;
    }

    return true; // If we are here then all the characters were found
}

} // namespace mangl
