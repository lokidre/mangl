/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../widgets/mangl_widget.h"
#include "mangl_design_grid_params.h"

namespace mangl {

struct DesignGridCell;
using DesignGridCellP = DesignGridCell*;

struct DesignGridVertex;
using DesignGridVertexP = DesignGridVertex*;

struct DesignGridPrim {
    PrimType type{PrimType::None};
    PointI pos{};

    Rect rect{};
    Texture texture{};

    int primIndex{};
    //PrimP prim{};
};

struct DesignGridVertex {
    int index{};
    PointI loc{};

    Point point{};
    Rect rect{};

    std::array<DesignGridVertexP, 4> nb{}; // Neighbors: top, right, bottom, left

    Color color;
    DesignGridPrim prim;
};

struct DesignGridCell {
    int index{};
    PointI loc{};
    Rect rect{};

    std::array<DesignGridVertexP, 4> vertices{};  // Vertices: left bottom, left top, right top, right, bottom
    std::array<DesignGridCellP, 4> nb{}; // Neighbors: top, right, bottom, left

    DesignGridPrim prim;
};

class DesignGridWidget: public WidgetTemplate<DesignGridWidget> {
    DesignGridParams dgp_;

    SizeI gridDims_{};
    Rect gridRect_{};

    PrimP cellPrims_{};
    int cellPrimsCount_{};

    ScenePrimsContainerP vertexPrims_{};
    int vertexPrimsCount_{};

    std::vector<DesignGridCell> cells_{};
    std::vector<DesignGridVertex> vertices_{};


public:
    explicit DesignGridWidget(DesignGridParamsA gdp_): dgp_{gdp_} {
    }

    DesignGridCellP getCell(PointI pos) {
        if (pos.x < 0 && pos.x >= gridDims_.w) return nullptr;
        if (pos.y < 0 && pos.y >= gridDims_.h) return nullptr;
        return &cells_[pos.y * gridDims_.w + pos.x];
    }


    DesignGridVertexP getVertex(PointI pos) {
        if (pos.x < 0 && pos.x > gridDims_.w) return nullptr;
        if (pos.y < 0 && pos.y > gridDims_.h) return nullptr;
        return &vertices_[pos.y * (gridDims_.w + 1) + pos.x];
    }

protected:
    void onCreate(SceneP scene) override;

};


} // namespace mangl
