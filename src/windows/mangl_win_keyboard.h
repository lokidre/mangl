/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once


#include "../core/mangl_setup.h"
#include "../core/mangl_keyboard.h"

#include <apeal/windows/windows.hpp>

#include <unordered_map>

namespace mangl {

const extern std::unordered_map<WPARAM, KeyCode> winScanCodes_;

}  // namespace mangl



