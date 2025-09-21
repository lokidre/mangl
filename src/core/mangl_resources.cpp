/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_resources.h"

namespace mangl {

StrView Resources::soundExt{"flac"};
StrView Resources::musicExt{"mp3"};
String Resources::textureSuffix;

StrView Resources::textureImageExt{"png"};
StrView Resources::textureAtlasExt{"yaml"};


KeyValCont<Resources::Key, String> Resources::resList_, Resources::bundlePathList_;
Resources::SuffixList Resources::suffixes_, Resources::backupSuffixes_, Resources::extSubstList_;

 
}  // namespace mangl

