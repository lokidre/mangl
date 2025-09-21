/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <apeal/geo.hpp>

#include "../mangl_types.h"

#include "../mangl_font.h"
#include "../mangl_scene.h"
#include "mangl_move_effect.h"
#include "mangl_fade_effect.h"

namespace mangl {


class GameScoreEffect {
    using Self = GameScoreEffect;

private:
    static constexpr int WidgetsCount = 2;

    LabelP effectLabel_[WidgetsCount]{};

    bool created_{};
    Timestamp duration_{1.2};
    Cord distance_{};
    Rect frame_{};
    Cord fontHeight_{};

    FadeEffect* fadeEffect_[WidgetsCount]{};
    MoveEffect* moveEffect_[WidgetsCount]{};

public:
    Self &create(SceneP scene, RectA r, const Font* font, Cord fontHeight, Flags textAlignment = M::AlignCenterAll);

    Self &move(RectA contentsFrame);

    Self &start(int score);

    Self &setDuration(Timestamp duration);
    Self &setDistance(Cord distance);

    Label *label() { return effectLabel_[0]; }

};

}  // namespace mangl

