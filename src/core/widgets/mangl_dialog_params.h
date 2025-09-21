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


namespace mangl {

enum class DialogType: int {
    Unknown = 0,
    Display,  // No buttons
    Confirm,
    YesNo,
    YesNoCancel,
};

struct DialogParams {
    WidgetId id;

    DialogTheme* theme;

    DialogType type;
    Rect rect;

    bool dispose;  // Dialog will be disposed on close
    bool run;  // Autorun
    bool modeless;

    StrView title;
    StrView message;

    GridLayoutP grid;

    StrView confirmButtonText;
    StrView rejectButtonText;

    Flags flags;


    SlotVoid onConfirm;
    SlotVoid onReject;


    // Obsolete
    SlotVoid confirmSignal;
    SlotVoid rejectSignal;
};

using DialogParamsA = const DialogParams&;

}  // namespace mangl
