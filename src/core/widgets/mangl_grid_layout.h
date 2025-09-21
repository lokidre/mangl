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
#include "mangl_grid_layout_params.h"


namespace mangl {

struct GridLayoutItem {
    PointI pos{};
    PrimP prim{};
    WidgetP widget{};
    Size size{}, minSize{}, maxSize{};
    Flags flags{};

    PrimP debugPrim{};

    [[nodiscard]] bool empty() const noexcept { return !prim && !widget; }
};

struct GridLayoutColumn {
    Cord pos{};
    Cord width{};
    Cord minWidth{}, maxWidth{};
};

struct GridLayoutRow {
    Cord pos{};
    Cord height{};
    Cord minHeight{}, maxHeight{};
};


struct GridLayoutItemParam {
    PointI pos{};
    PrimP prim{};
    WidgetP widget{};
    Size size{}, minSize{}, maxSize{};
    Flags flags{};

    PrimP debugPrim{};
};

class GridLayout: public WidgetTemplate<GridLayout> {
    friend class Scene;

    using Base = WidgetTemplate<GridLayout>;
    using Self = GridLayout;
    using SelfP = Self*;

    using ItemsContainer = std::vector<GridLayoutItem>;

public:
    SelfP addItem(const GridLayoutItemParam& param);

    SelfP addNext(GridLayoutItemParam param) {
        param.pos = currentPos_;

        currentPos_.x++;
        if (maxColumns_ > 0 && currentPos_.x >= maxColumns_) {
            currentPos_.x = 0;
            currentPos_.y++;
        }

        return addItem(param);
    }

    Size estimateSize();

    ItemsContainer& getItems() noexcept { return items_; }

    auto clear() {
        items_.clear();
        columns_.clear();
        rows_.clear();
        return this;
    }


private:
    GridLayoutP onNew(GridLayoutParamsA params);

    GridLayoutParams params_{};

    PointI currentPos_{};
    int maxColumns_{};

    using ColumnsContainer = std::vector<GridLayoutColumn>;
    using RowsContainer = std::vector<GridLayoutRow>;

    ColumnsContainer columns_;
    RowsContainer rows_;

    ItemsContainer items_;

    bool debug_{};
    PrimP debugPrim_{};


    void updateLayout();
    void updateVisible();

protected:
    void onMove() override {  updateLayout();  }
    void onVisible(bool visible) override { updateVisible(); }
    void onSetZIndexFront() override;

};

using GridLayoutP = GridLayout*;



}  // namespace mangl
