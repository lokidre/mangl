/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "texmake.h"

void TexMake::markImagePlaced(ImageSource& src) {

    src.placed = true;

    auto outSize = outputImage_.getSize();
    auto maskBuffer = imageMask_.data();

    // // Old code
    // for (int y = 0; y < img.targetRect.h; ++y) {
    //     auto m = maskBuffer + (y + img.targetRect.y) * outSize.w + img.targetRect.x;
    //     std::memset(m, 1, img.targetRect.w);
    // }


    // New code
    auto prow = maskBuffer + src.targetRect.y * outSize.w;
    for (int j = 0; j < src.targetRect.h; ++j) {
        auto pm = prow + src.targetRect.x;
        std::memset(pm, 1, src.targetRect.w);
        prow += outSize.w;
    }
}



/*#include "texmake.h"


struct TexArea {
    Index x, y, w, h;
};

struct TexNode {
    //std::vector<TexNode> children;
    TexArea area;
    //bool available = true;
};

void TexMake::buildPlacements()
{
    //brute force method
    using namespace apl;

    auto outSize = outputImage_.getSize();

    // mask
    imageMask_.alloc(outputImage_.size.w * outputImage_.size.h);
    imageMask_.zero();
    imageMask_[0] = 1; // the very first pixel (0,0) is reserved

    // sort all the images by maximum side
    std::multimap<Index, Index, std::greater<Index>> texturesBySide;

    Index index{};
    for (auto& tex : textures_) {
        if (tex.phonyImage) {
            ++index;
            continue;
        }      
        auto max = std::max(tex.sourceRect.h, tex.sourceRect.w);
        texturesBySide.insert({ max, index });
        ++index;
    }

    // continue while there are textures left
    PointI current{ 1, 1 };

    std::multimap<Index, TexNode, std::greater<Index>> texNodes;

    TexNode iniNode;
    iniNode.area = { 0, 0, outputImage_.size.w, outputImage_.size.h };
    Index nodeSide = std::min(iniNode.area.w, iniNode.area.h);
    texNodes.insert({ nodeSide, iniNode });


    Index nodeIndex{};
    for (auto& it: texturesBySide) {
        auto& tex = textures_[it.second];
        if (tex.phonyImage) {
            ++nodeIndex;
            continue;
        }

        auto node = texNodes.begin()->second;
        texNodes.erase(texNodes.begin());

        Index bleed = 1;

        tex.targetRect.x = node.area.x + bleed;
        tex.targetRect.y = node.area.y + bleed; 
        tex.targetRect.w = tex.sourceRect.w;
        tex.targetRect.h = tex.sourceRect.h;

        tex.placed = true;

        TexNode northNode{}, eastNode{}, northEastNode{};


        auto enX = tex.targetRect.x + tex.targetRect.w + bleed;
        auto nY = tex.targetRect.y + tex.targetRect.h + bleed;

        eastNode.area = {enX, node.area.y, node.area.w - enX, tex.targetRect.h + bleed * 2};
        
        northNode.area = {node.area.x, nY, tex.targetRect.w + bleed * 2, node.area.h - nY};

        northEastNode.area = {enX, nY, node.area.w - enX, node.area.h - nY};

        Index northNodeSide = std::min(northNode.area.w, northNode.area.h);
        Index eastNodeSide = std::min(eastNode.area.w, eastNode.area.h);
        Index northEastSide = std::min(northEastNode.area.w, northEastNode.area.h);

        if (eastNodeSide > 3) {
            texNodes.insert({eastNodeSide, eastNode});
        }
        if (northNodeSide > 3) {
            texNodes.insert({northNodeSide, northNode});
        }
        if (northEastSide > 3) {
            texNodes.insert({northEastSide, northEastNode});
        }
       
        
        
        ++nodeIndex;
    }
}

*/