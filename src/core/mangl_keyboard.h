/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

namespace mangl {

enum class KeyCode {
    None = 0,

    Escape,
    
    Return,
    Enter = Return,

    Space, Tab,

    A, B, C, D, E, F, G, H,
    I, J, K, L, M, N, O, P, Q,
    R, S, T, U, V, W, X, Y, Z,

    BackSpace,

    Left, Right, Up, Down,
    Home, End, PageUp, PageDown,

    Control, Alt, Shift,

    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,



    Keypad0,
    Keypad1, Keypad2, Keypad3,
    Keypad4, Keypad5, Keypad6,
    Keypad7, Keypad8, Keypad9,
};

}  // namespace mangl

