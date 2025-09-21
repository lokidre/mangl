/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_keyboard.h"

#include <unordered_map>
#include <map>

namespace mangl {

const KeyCodeContainer andScanCodes_ {
    {29, KeyCode::A},
    {30, KeyCode::A},


    {66, KeyCode::Return},
    {111, KeyCode::Escape},
    {67, KeyCode::BackSpace},
    {61, KeyCode::Tab},
    {62, KeyCode::Space},


    {21, KeyCode::Left},
    {19, KeyCode::Up},
    {22, KeyCode::Right},
    {20, KeyCode::Down},
};


}  // namespace mangl
