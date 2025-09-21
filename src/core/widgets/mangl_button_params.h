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
#include "../mangl_texture.h"
#include "../mangl_styles.h"

namespace mangl {

struct ButtonParams {
    WidgetId id;

    ButtonStyle* style;
    ButtonContentWidget* widget;

    String text;
    StrView textView;

    Rect rect;
    Margins margins;

    Texture texture;
    Texture pressedTexture;
    Texture disabledTexture;

    bool checkbox;
    bool checked;

    KeyCode keyCode;

    bool disabled;
    bool hidden;
    
    Flags flags;

    SlotBool onPress;
    SlotIdBool onPressId;

    SlotVoid onPush;
    SlotIdVoid onPushId;

    SlotBool onCheck;
    SlotIdBool onCheckId;
};

using ButtonParamsA = const ButtonParams&;
using ButtonParamsCP = const ButtonParams*;

}  // namespace mangl

