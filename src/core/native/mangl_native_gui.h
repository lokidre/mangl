/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"
#include "../mangl_color.h"
#include "../mangl_environment.h"

namespace mangl {

struct NativeGui: public apl::Singleton<NativeGui> {
    using Self = NativeGui;
    using SelfRet = NativeGui*;

    struct AlertParam {
        StrView title, message;
    };
    using AlertParamCR = const AlertParam&;

    static void alert(AlertParamCR parms);

    using InputDialogSignal = apl::Signal<bool, StrView, bool*>;
    
    struct InputDialogParam {
        StrView prompt;
        StrView title;
        StrView placeholder;
        StrView text; // Initial text
        StrView ok{"OK"};
        StrView cancel;
        InputDialogSignal::Func signal;
    };
    
    using InputDialogParamA = const InputDialogParam&;
    
    static WidgetId inputDialog(InputDialogParamA);
//    static MWidgetId inputDialog(MStringA prompt, MStringA title, InputDialogSignal::F signal) {
//        return inputDialog(prompt, title, "Confirm", "Cancel", signal);
//    }


    static WidgetId createInput(StrView text, RectA rect, Flags flags = {});
    static void setInputPlaceholder(WidgetId widgetId, StrView text);

    SignalIdStrView textChangedSignal{};

    void onTextChanged(int widgetId, StrView text) {
        textChangedSignal(widgetId, text);
    }


    SignalId alertConfirmedSignal{};

    void onAlertConfirmed(WidgetId widgetId) {
        alertConfirmedSignal(widgetId);
    }


    static WidgetId createLabel(StrView text, RectA rect, Flags flags = {});
    static WidgetId createLabel(StrView text, Point point, Flags flags = {});

    SelfRet clearWidgets();

    SelfRet setText(WidgetId widgetId, StrView text);
    SelfRet setVisible(WidgetId widgetId, bool visible = true);
    SelfRet move(WidgetId widgetId, RectA rect);
    SelfRet move(WidgetId widgetId, Point pos);
    SelfRet setTextColor(WidgetId widgetId, ColorA color);
    SelfRet setBackgroundColor(WidgetId widgetId, ColorA color);
    SelfRet setFont(WidgetId widgetId, Cord size, StrView fontName = {});

    String getText(WidgetId widgetId);

    auto clearCallbacks() {
        textChangedSignal = nullptr;
        alertConfirmedSignal = nullptr;
        return this;
    }

//private:
//    Env *env_ = Env::instance();
    
};

}  // namespace mangl
