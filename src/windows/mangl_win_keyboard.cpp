/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_win_keyboard.h"


namespace mangl {

const std::unordered_map<WPARAM, KeyCode> winScanCodes_{
    {VK_RETURN, KeyCode::Return},
    {VK_LEFT, KeyCode::Left },
    {VK_RIGHT,  KeyCode::Right},
    {VK_UP, KeyCode::Up},
    {VK_DOWN, KeyCode::Down},
    {VK_BACK, KeyCode::BackSpace},
    {VK_TAB, KeyCode::Tab},
    {VK_ESCAPE, KeyCode::Escape},
    {VK_SPACE, KeyCode::Space},


    {VK_CONTROL, KeyCode::Control},
    {VK_MENU, KeyCode::Alt},
    {VK_SHIFT, KeyCode::Shift},


    {VK_F1, KeyCode::F1},
    {VK_F2, KeyCode::F2},
    {VK_F3, KeyCode::F3},
    {VK_F4, KeyCode::F4},
    {VK_F5, KeyCode::F5},
    {VK_F6, KeyCode::F6},
    {VK_F7, KeyCode::F7},
    {VK_F8, KeyCode::F8},
    {VK_F9, KeyCode::F9},
    {VK_F10, KeyCode::F10},
    {VK_F11, KeyCode::F11},
    {VK_F12, KeyCode::F12},


    {'A', KeyCode::A},
    {'B', KeyCode::B},
    {'C', KeyCode::C},
    {'D', KeyCode::D},
    {'E', KeyCode::E},
    {'F', KeyCode::F},

    {'O', KeyCode::O},
    {'P', KeyCode::P},
    {'Q', KeyCode::Q},


};

}  // namespace mangl

