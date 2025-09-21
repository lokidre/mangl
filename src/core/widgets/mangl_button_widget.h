/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_widget.h"
#include "../mangl_texture.h"
#include "mangl_button_params.h"
#include "mangl_widget_border.h"


namespace mangl {

class ButtonContentWidget: public WidgetTemplate<ButtonContentWidget> {
    friend class Button;

protected:
    ButtonParams* buttonParams_{};
    const ButtonStyle* buttonStyle_{};

protected:
    virtual void onSetText(StrView text){}
    virtual void onSetParams(ButtonParamsA params) {}
    //virtual void onSetPressed(bool pressed) {}
};

class DefaultButtonContentWidget: public ButtonContentWidget {
    Texture texture_{};
    PrimP backgroundPrim_{};
    LabelP textLabel_{};
    WidgetBorderStrikesRuntime borderRuntime_{};

protected:
    void onSetText(StrView text) override;

    //
    // Widget overrides
    //
    void onCreate(SceneP scene) override;
    void onPressedState(bool pressed) override;
    void onEnable(bool enabled) override;
    void onMove(RectA rect) override;

};

}  // namespace mangl


