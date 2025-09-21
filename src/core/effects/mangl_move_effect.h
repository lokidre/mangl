/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_effect.h"
#include "mangl_move_effect_params.h"

namespace mangl {

struct MoveEffect: public GuiEffectTemplate<MoveEffect> {
    friend class Scene;

    auto setOffset(Point off) noexcept { 
        offset_ = off;  
        return this; 
    }
    [[nodiscard]] auto getOffset() const noexcept { return offset_; }
    [[nodiscard]] auto offset() const noexcept { return offset_; }

private:
    Point offset_{};  // relative distance in pixels

    struct SavedInfo {
        Rect rect{};
        Point startPos{}, endPos{};
        PrimP prim{};
        WidgetP widget{};
    } savedInfo_;

    MoveEffectP onNew(const MoveEffectParams& params);

protected:
    void onStart() override;
    void onStop() override;
    void onTimer(Timestamp lambda) override;
};


}  // namespace mangl
