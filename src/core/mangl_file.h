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

#ifdef QT_CORE_LIB
APEAL_QT_HEADERS_BEGIN
#include <QFile>
APEAL_QT_HEADERS_END
#endif

#if APEAL_OS_ANDROID
#include "../android/mangl_asset_manager_android.h"
#endif

#include <apeal/file.hpp>

namespace mangl {

using MAssetFile = apl::File ;

}  // namespace mangl
