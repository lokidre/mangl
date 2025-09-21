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

#include <map>

namespace mangl {

class GradientWidget: public WidgetTemplate<GradientWidget> {
    using Base = WidgetTemplate<GradientWidget>;
    using Self = GradientWidget;

public:
    struct Gradient {
        int id{};
        Color color;
        Cord position{}; // (0-1)
        Real alpha{1};

        Gradient(int i, ColorA c, Cord p): id{i}, color{c}, position{p} {}
        Gradient(ColorA c, Cord p): Gradient(0, c, p){}
    };
    using GradientA = const Gradient &;

    Self *addGradient(GradientA it);
    Self *addGradient(ColorA c, Cord pos) { return addGradient({c,pos}); }
    Self *reserveGradient(GradientA it);

    Self *setOrientation(int o) {
        orientationFlags_ = o;
        updateContents();
        return this;
    }


private:

    struct GradientItem {
        Gradient gradient;
        bool valid{true};
    };

    std::multimap<Cord, GradientItem> items_;
    using items_pair = std::pair<Cord, GradientItem>;

    int spritesCount_{};
    ScenePrimsContainerP prims{};

    int orientationFlags_{M::OrientationVert};

    void updateLayout();
    void updateContents() { updateLayout(); }

protected:
    void onCreate() override;


};

}  // namespace mangl


