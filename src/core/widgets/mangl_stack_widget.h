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
#include "mangl_stack_widget_params.h"

namespace mangl {

class StackWidget: public WidgetTemplate<StackWidget> {
    friend class Scene;

    StackWidgetParams params_{};

    WidgetItem &addItem(PrimP prim, WidgetP widget);

    void updateLayout(RectA rect);
    void updateContents();
    //void updateVisible();
    void updateScissor();

    StackWidgetP onNew(StackWidgetParamsA params);

public:
    WidgetId addPrim(PrimP prim, WidgetLayoutParamsA layout);  // Set fixed size and position or stretch
    WidgetId addPrim(PrimP prim) { return addPrim(prim, {}); }

    WidgetId addWidget(WidgetP widget, WidgetLayoutParamsA layout);
    WidgetId addWidget(WidgetP widget) { return addWidget(widget, {}); }


    [[nodiscard]] auto count() const noexcept { return childItems_.size(); }
    [[nodiscard]] auto empty() const noexcept { return childItems_.empty(); }


protected:
    void onCreate(SceneP scene) override;
    void onMove(RectA rect) override { updateLayout(rect); }
    void onOrientation(M::Orientation orientation) override;

};

}  // namespace mangl
