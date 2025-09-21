/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "./mangl_button_params.h"

namespace mangl {

struct CheckboxParams {
    StrView stateTag;

    Rect rect;

    Color backgroundColor;
    Color pressedBackgroundColor;
    Color disabledBackgroundColor;

    Texture texture;
    Texture onTexture, offTexture;

    WidgetP onWidget{}, onPressedWidget{}, onDisabledWidget{};
    WidgetP offWidget{}, offPressedWidget{}, offDisabledWidget{};


    Cord radius{-1};

    Flags flags;

    bool checked;
    bool defaultChecked;

    SlotBool onCheck;
    SlotIdBool onCheckId;

    std::function<void(bool)> checkSignal;
    std::function<void(CommonId, bool)> checkSignalId;
};

using CheckboxParamsA = const CheckboxParams&;

}  // namespace mangl

