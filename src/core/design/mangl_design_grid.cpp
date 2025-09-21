/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_design_grid.h"

#include "../mangl_scene.h"
#include "../mangl_environment.h"
#include "../mangl_random.h"

namespace mangl {

DesignGridWidgetP Scene::newDesignGridWidget(DesignGridParamsA dgp) {
    return newWidgetArg<DesignGridWidget>(dgp)->create();
}


void DesignGridWidget::onCreate(SceneP scene) {
    // Setup
    if (dgp_.fullscreen)
        dgp_.rect = Env::getFullArea();
    apl::verify(dgp_.rect.valid(), "Grid rect is empty");

    if (dgp_.gridDims.w != 0 || dgp_.gridDims.h != 0) {
        gridDims_ = dgp_.gridDims;
        if (gridDims_.w != 0) dgp_.gridSize.w = dgp_.rect.w / gridDims_.w;
        if (gridDims_.h != 0) dgp_.gridSize.h = dgp_.rect.h / gridDims_.h;
    }


    if (dgp_.gridSize.h == 0) dgp_.gridSize.h = dgp_.gridSize.w;
    if (dgp_.gridSize.w == 0) dgp_.gridSize.w = dgp_.gridSize.h;

    apl::verify(dgp_.gridSize.valid(), "Grid size is empty");


    gridDims_.w = static_cast<int>(dgp_.rect.w / dgp_.gridSize.w);
    if (dgp_.gridSize.w * gridDims_.w < dgp_.rect.w) gridDims_.w++;

    gridDims_.h = static_cast<int>(dgp_.rect.h / dgp_.gridSize.h);
    if (dgp_.gridSize.h * gridDims_.h < dgp_.rect.h) gridDims_.h++;


    // Layout grid
    cells_.resize(gridDims_.w * gridDims_.h);
    vertices_.resize((gridDims_.w + 1) * (gridDims_.h + 1));

    gridRect_ = dgp_.rect;
    gridRect_.w = dgp_.gridSize.w * gridDims_.w;
    gridRect_.h = dgp_.gridSize.h * gridDims_.h;

    auto diff = gridRect_.top() - dgp_.rect.top();
    gridRect_.y -= diff;

    auto cellIt = cells_.begin();
    auto vertexIt = vertices_.begin();
    Rect cellRect{gridRect_.x, gridRect_.y, dgp_.gridSize.w, dgp_.gridSize.h};
    for (int gridRow = 0; gridRow < gridDims_.h; gridRow++) {
        cellRect.x = gridRect_.x;
        for (int gridCol = 0; gridCol < gridDims_.w; gridCol++) {
            auto& cell = *cellIt++;
            auto& vertex = *vertexIt++;

            cell.loc = {gridCol, gridRow};
            cell.rect = cellRect;

            vertex.loc = {gridCol, gridRow};
            vertex.point = {cellRect.x, cellRect.y};

            cellRect.x += dgp_.gridSize.w;
        };

        auto& nextVertex = *vertexIt++;
        nextVertex.loc = {gridDims_.w, gridRow};
        nextVertex.point = {cellRect.x, cellRect.y};

        cellRect.y += dgp_.gridSize.h;
    }

    // Last row
    cellRect.x = gridRect_.x;
    for (int gridCol = 0; gridCol < gridDims_.w; ++gridCol) {
        auto& vertex = *vertexIt++;
        vertex.loc = {gridCol, gridDims_.h};
        vertex.point = {cellRect.x, cellRect.y};

        cellRect.x += dgp_.gridSize.w;
    }

    // Last vertex
    auto& lastVertex = *vertexIt++;
    lastVertex.loc = {gridDims_.w, gridDims_.h};
    lastVertex.point = {cellRect.x, cellRect.y};


    // Setup verices and neighbors
    for (auto& cell: cells_) {
        cell.vertices[0] = getVertex({cell.loc.x, cell.loc.y});
        cell.vertices[1] = getVertex({cell.loc.x, cell.loc.y + 1});
        cell.vertices[2] = getVertex({cell.loc.x + 1, cell.loc.y + 1});
        cell.vertices[3] = getVertex({cell.loc.x + 1, cell.loc.y});

        cell.nb[0] = getCell({cell.loc.x, cell.loc.y + 1});
        cell.nb[1] = getCell({cell.loc.x + 1, cell.loc.y});
        cell.nb[2] = getCell({cell.loc.x, cell.loc.y - 1});
        cell.nb[3] = getCell({cell.loc.x - 1, cell.loc.y});
    }

    for (auto& vtx: vertices_) {
        vtx.nb[0] = getVertex({vtx.loc.x, vtx.loc.y + 1});
        vtx.nb[1] = getVertex({vtx.loc.x + 1, vtx.loc.y});
        vtx.nb[2] = getVertex({vtx.loc.x, vtx.loc.y - 1});
        vtx.nb[3] = getVertex({vtx.loc.x - 1, vtx.loc.y});
    }



    if (dgp_.vertexType != PrimType::None) {
        vertexPrimsCount_ = (gridDims_.w + 1) * (gridDims_.h + 1);
        vertexPrims_ = scene->newPrimArray(vertexPrimsCount_);
        // addChildItems(vertexPrims_, vertexPrimsCount_);

        //auto primP = vertexPrims_;
        auto primIndex = 0;
        for (auto& vtx: vertices_) {
            //vtx.prim.prim = primP++;
            vtx.prim.primIndex = primIndex++;

            Size vtxSize = dgp_.gridSize;

            if (dgp_.vertexWidthRange.valid())  vtxSize.w = M::random.range(dgp_.vertexWidthRange);
            if (dgp_.vertexHeightRange.valid())  vtxSize.h = M::random.range(dgp_.vertexHeightRange);

            if (vtxSize.w == 0 && vtxSize.h != 0) vtxSize.w = vtxSize.h;
            if (vtxSize.h == 0 && vtxSize.w != 0) vtxSize.h = vtxSize.w;

            if (vtxSize.empty()) vtxSize = dgp_.gridSize;


            vtx.rect = {
                vtx.point.x - vtxSize.w / 2,
                vtx.point.y - vtxSize.h / 2,
                vtxSize.w,
                vtxSize.h
            };

            vtx.prim.rect = vtx.rect;

            vtx.color = dgp_.vertexColor;
        };

        for (auto& vtx: vertices_) {
            auto& vertexPrim = vertexPrims_->at(vtx.prim.primIndex);
            switch(dgp_.vertexType) {
                using enum PrimType;
            case Disk:
                vertexPrim.setDisk(vtx.point, vtx.prim.rect.w / 4)->setColor(vtx.color)->setVisible();
                //scene->newDisk(vtx.color, vtx.point, vtx.prim.rect.w / 2);
                //scene->newRect(M::Red, {10, 10, 400, 400});
                //scene->newRect(vtx.color, vtx.prim.rect);
                // vtx.prim.prim->setRect(vtx.prim.rect)->setColor(vtx.color)->setVisible();
                break;
            default:
                MDebugTrap();
                break;
            }
        }
    }

}



}  // namespace mangl
