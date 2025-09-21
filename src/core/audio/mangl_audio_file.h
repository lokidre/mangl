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

namespace mangl {

class AudioFileLoader {
public:
    Index numChannels{};
    Index sampleRate{};
    Index bitsPerSample{};
    Index totalSamples{};
    
    apl::ByteBuffer buffer{};

    void load(StrView filename);


private:
    void loadWav(StrView filename);
    void loadFlac(StrView filename);

};

}  // namespace mangl

