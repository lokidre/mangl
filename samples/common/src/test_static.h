/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "test_types.h"

#include "tex_common.hpp"

#include <mangl/resources.h>

namespace sample {

inline auto getCommonTexBundlePath() { return Res::bundleDirJoin("tex_common"); }
inline auto getTutorialTexBundlePath() { return Res::bundleDirJoin("tex_tutorial"); }



#ifndef MANGL_CONSOLE
extern CommonTexture texCommon;
extern Size sampleApplicationSize_;
extern Padding sampleScreenPadding_;
extern Size sampleApplicationLayout_;
extern InterfaceIdiom sampleInterfaceIdiom_;
#endif

}  // namespace sample
