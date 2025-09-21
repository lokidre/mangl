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
#include "mangl_background_widget_params.h"


namespace mangl {

struct BackgroundWidget: public WidgetTemplate<BackgroundWidget> {
    friend class Scene;

    using Base = WidgetTemplate<BackgroundWidget>;
    using Self = BackgroundWidget;
    using SelfP = Self*;



    BackgroundWidgetP setParams(const BackgroundWidgetParams& params);


private:

    BackgroundWidgetParams params_{};

    static constexpr int TotalPrims = 1 + 2 + 4 + 4; // Background, corners, endings, borders
    int primsCount_{};
    ScenePrimsContainerP prims_{};
    PrimP backgroundPrim_{}, cornerPrims_{}, borderPrims_{};
    PrimP endingPrims_{};


    BackgroundWidgetP onNew(const BackgroundWidgetParams& params);


    void updateLayout(RectA rect);
    void updateScissor();


protected:
    void onCreate(SceneP scene) override;
    void onMove(RectA rect) override {  updateLayout(rect);  }
    void onVisible(bool visible) override;
    void onScissor(bool scissorFlag, RectA scissorRect) override {
        for (auto prim = prims_->begin(), end = prim + primsCount_; prim != end; ++prim) {
            prim->setScissor(scissorFlag, scissorRect);
        }
    }
    void onSetZIndexFront() override;

};

}  // namespace mangl

