/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../core/native/mangl_native_gui.h"

#if MANGL_SDL

#include "mangl_sdl.h"


namespace mangl {

void NativeGui::alert(const AlertParam &parms)
{
}

WidgetId NativeGui::inputDialog(InputDialogParamA param)
{
    return {};
}

auto NativeGui::clearWidgets() -> SelfRet
{
    return this;
}

auto NativeGui::setVisible(WidgetId widgetId, bool visible) -> SelfRet
{
    return this;
}

auto NativeGui::move(WidgetId widgetId, RectA rect) -> SelfRet
{
    return this;
}



}  // namespace mangl

#endif
