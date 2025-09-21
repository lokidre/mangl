/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_background_widget.h"

#include "../mangl_scene.h"

namespace mangl {

BackgroundWidgetP Scene::newBackgroundWidget(BackgroundWidgetParamsA params) {
    return newWidget<BackgroundWidget>(params.flags)->onNew(params)->create();
}

BackgroundWidgetP BackgroundWidget::onNew(BackgroundWidgetParamsA params) {
    setParams(params);

    if (params_.cornerTexture) {
        params_.cornerLeftTopTexture = params_.cornerTexture.getFrame(0);
        params_.cornerRightTopTexture = params_.cornerTexture.getFrame(1);
        params_.cornerRightBottomTexture = params_.cornerTexture.getFrame(2);
        params_.cornerLeftBottomTexture = params_.cornerTexture.getFrame(3);
    }

    return this;
}

BackgroundWidgetP BackgroundWidget::setParams(BackgroundWidgetParamsA params) {
    params_ = params;

    if (params_.rect.w > 0)
        move(params_.rect);

    return this;
}

void BackgroundWidget::onCreate(SceneP scene) {
    prims_ = scene_->newPrimArray(TotalPrims);

    primsCount_ = 0;
    backgroundPrim_ = cornerPrims_ = borderPrims_ = nullptr;

    if (params_.texture || params_.color) {
        backgroundPrim_ = &prims_->at(primsCount_);

        if (params_.texture) {
            backgroundPrim_->setTexture(params_.texture);
            if (params_.blockSize) backgroundPrim_->setElSize(params_.blockSize);
        } else {
            backgroundPrim_->setBox({});
        }

        if (params_.color) backgroundPrim_->setColor(params_.color);

        if (params_.cornerRadius != 0) backgroundPrim_->setRadius(params_.cornerRadius);

        ++primsCount_;
    }

    if (params_.beginTexture || params_.endTexture) {
        endingPrims_ = &prims_->at(primsCount_);

        auto prim = endingPrims_;

        if (params_.beginTexture) prim->setTexture(params_.beginTexture)->setElSize(params_.blockSize)->setTiledHorz();
        ++prim, ++primsCount_;

        if (params_.endTexture) prim->setTexture(params_.endTexture)->setElSize(params_.blockSize)->setTiledHorz();
        ++prim, ++primsCount_;
    }

    if (params_.cornerLeftTopTexture || params_.cornerRightTopTexture || params_.cornerRightBottomTexture || params_.cornerLeftBottomTexture) {
        cornerPrims_ = &prims_->at(primsCount_);

        auto prim = cornerPrims_;
        
        if (params_.cornerLeftTopTexture) prim->setTexture(params_.cornerLeftTopTexture);
        ++prim, ++primsCount_;

        if (params_.cornerRightTopTexture)  prim->setTexture(params_.cornerRightTopTexture);
        ++prim, ++primsCount_;

        if (params_.cornerRightBottomTexture) prim->setTexture(params_.cornerRightBottomTexture);
        ++prim, ++primsCount_;

        if (params_.cornerLeftBottomTexture) prim->setTexture(params_.cornerLeftBottomTexture);
        ++prim, ++primsCount_;
    }

    if (params_.borderTexture) {
        borderPrims_ = &prims_->at(primsCount_);

        for (int idx = 0; idx < 4; ++idx) {
            borderPrims_[idx].setTexture(params_.borderTexture.frame(idx));
        }

        borderPrims_[0].setTiledVert(true);
        borderPrims_[2].setTiledVert(true);
        borderPrims_[1].setTiledHorz(true);
        borderPrims_[3].setTiledHorz(true);

        primsCount_ += 4;
    }

    updateLayout(params_.rect);
}


void BackgroundWidget::updateLayout(RectA rect) {
    if (!prims_) return;

    if (backgroundPrim_) {
        Size shrink{};

        if (backgroundPrim_->type == PrimType::Texture) {
            if (borderPrims_) {
                shrink = params_.blockSize;
            } else if (endingPrims_) {
                shrink.w = params_.blockSize.w;
            }
        }

        auto r = rect;
        r.shrinkX(shrink.w);
        r.shrinkW(shrink.w);
        r.shrinkY(shrink.h);
        r.shrinkH(shrink.h);
        backgroundPrim_->move(r);
    }


    // If the block size is bigger than the object itself then don't show it
    if (params_.blockSize.w > r_.w || params_.blockSize.h > r_.h) {
        auto prim = prims_->begin();

        if (backgroundPrim_)
            ++prim;

        for (auto end = prims_->end(); prim != end; ++prim) {
            prim->setVisible(false);
        }
        return;
    } 
    
    for (auto& prim: *prims_) {
        prim.setVisible(visible_);
    }

    if (endingPrims_) {
        auto bs = params_.blockSize;

        endingPrims_[0].move({r_.x, r_.y, bs.w, r_.h});
        endingPrims_[1].move({r_.x + r_.w - bs.w, r_.y, bs.w, r_.h});
    }

    if (cornerPrims_) {
        auto bs = params_.blockSize;
        auto left = r_.x;
        auto right = r_.x + r_.w - bs.w;

        cornerPrims_[0].move({left, r_.y + r_.h - bs.h, bs.w, bs.h});
        cornerPrims_[1].move({right, r_.y + r_.h - bs.h, bs.w, bs.h});
        cornerPrims_[2].move({right, r_.y, bs.w, bs.h});
        cornerPrims_[3].move({left, r_.y, bs.w, bs.h});
    }

    if (borderPrims_) {
        auto cs = params_.blockSize;
        auto bs = params_.blockSize;

        borderPrims_[0].setElSize(bs)->move({r_.x, r_.y + cs.h, bs.w, r_.h - cs.h*2});
        borderPrims_[1].setElSize(bs)->move({r_.x + cs.w, r_.y + r_.h - bs.h, r_.w - cs.w * 2, bs.h});
        borderPrims_[2].setElSize(bs)->move({r_.x + r_.w - bs.w, r_.y + cs.h, bs.w, r_.h - cs.h*2});
        borderPrims_[3].setElSize(bs)->move({r_.x + cs.w, r_.y, r_.w - cs.w * 2, bs.h});
    }
}


void BackgroundWidget::onVisible(bool visible) {
    if (!prims_) return;
    for (auto prim = prims_->begin(), end = prim + primsCount_; prim != end; ++prim) {
        prim->setVisible(visible);
    }
}


void BackgroundWidget::updateScissor() {
    if (!prims_) return;
    for (auto prim = prims_->begin(), end = prim + primsCount_; prim != end; ++prim) {
        prim->setScissor(scissorFlag_, scissorRect_);
    }
}



void BackgroundWidget::onSetZIndexFront() { 
    scene_->moveToFront(prims_);
}


} // namespace mangl
