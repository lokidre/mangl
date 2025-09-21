/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_sdl_keyboard.h"

#if MANGL_SDL

namespace mangl {

const std::unordered_map<SDL_Scancode, KeyCode> sdlScanCodes_{
    {SDL_SCANCODE_A, KeyCode::A},
    {SDL_SCANCODE_B, KeyCode::B},
    {SDL_SCANCODE_C, KeyCode::C},
    {SDL_SCANCODE_D, KeyCode::D},
    {SDL_SCANCODE_E, KeyCode::E},
    {SDL_SCANCODE_F, KeyCode::F},
    {SDL_SCANCODE_G, KeyCode::G},
    {SDL_SCANCODE_H, KeyCode::H},
    {SDL_SCANCODE_J, KeyCode::J},
    {SDL_SCANCODE_K, KeyCode::K},
    {SDL_SCANCODE_L, KeyCode::L},
    {SDL_SCANCODE_M, KeyCode::M},
    {SDL_SCANCODE_N, KeyCode::N},
    {SDL_SCANCODE_O, KeyCode::O},
    {SDL_SCANCODE_P, KeyCode::P},
    {SDL_SCANCODE_Q, KeyCode::Q},
    {SDL_SCANCODE_R, KeyCode::R},
    {SDL_SCANCODE_S, KeyCode::S},
    {SDL_SCANCODE_T, KeyCode::T},
    {SDL_SCANCODE_U, KeyCode::U},
    {SDL_SCANCODE_V, KeyCode::V},
    {SDL_SCANCODE_W, KeyCode::W},
    {SDL_SCANCODE_X, KeyCode::X},
    {SDL_SCANCODE_Y, KeyCode::Y},
    {SDL_SCANCODE_Z, KeyCode::Z},

    {SDL_SCANCODE_F1, KeyCode::F1},
    {SDL_SCANCODE_F2, KeyCode::F2},
    {SDL_SCANCODE_F3, KeyCode::F3},
    {SDL_SCANCODE_F4, KeyCode::F4},
    {SDL_SCANCODE_F5, KeyCode::F5},
    {SDL_SCANCODE_F6, KeyCode::F6},
    {SDL_SCANCODE_F7, KeyCode::F7},
    {SDL_SCANCODE_F8, KeyCode::F8},
    {SDL_SCANCODE_F9, KeyCode::F9},
    {SDL_SCANCODE_F10, KeyCode::F10},
    {SDL_SCANCODE_F11, KeyCode::F11},
    {SDL_SCANCODE_F12, KeyCode::F12},


    {SDL_SCANCODE_LCTRL, KeyCode::Control},
    {SDL_SCANCODE_RCTRL, KeyCode::Control},
    {SDL_SCANCODE_LGUI, KeyCode::Control},
    {SDL_SCANCODE_RGUI, KeyCode::Control},

    {SDL_SCANCODE_LALT, KeyCode::Alt},
    {SDL_SCANCODE_RALT, KeyCode::Alt},

    {SDL_SCANCODE_RETURN, KeyCode::Return},
    {SDL_SCANCODE_ESCAPE, KeyCode::Escape},
    {SDL_SCANCODE_BACKSPACE, KeyCode::BackSpace},
    {SDL_SCANCODE_TAB, KeyCode::Tab},
    {SDL_SCANCODE_SPACE, KeyCode::Space},


    {SDL_SCANCODE_LEFT, KeyCode::Left},
    {SDL_SCANCODE_UP, KeyCode::Up},
    {SDL_SCANCODE_RIGHT, KeyCode::Right},
    {SDL_SCANCODE_DOWN, KeyCode::Down},

};

} // namespace mangl

#endif  // SDL
