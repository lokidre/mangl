/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_gradient_widget.h"

#include "../mangl_scene.h"

namespace mangl {


GradientWidget *GradientWidget::addGradient(GradientA gradient)
{
    if (created_)
        throw std::runtime_error("Adding gradient after gwidget was created");

    GradientItem item{gradient};
    item.valid = true;

    items_.insert(items_pair(gradient.position, item));

    return this;
}

GradientWidget *GradientWidget::reserveGradient(GradientA gradient)
{
    if (created_)
        throw std::runtime_error("Reserving gradient after gwidget was created");

    GradientItem item{gradient};
    item.valid = false;

    items_.insert(items_pair(gradient.position, item));

    return this;
}


void GradientWidget::onCreate() {
    spritesCount_ = apl::count(items_) * 2;
    prims = scene_->newPrimArray(spritesCount_);
    updateLayout();
}



void GradientWidget::updateLayout()
{
    if (!created_)
        return;

    auto spritePtr = prims->begin();
    auto spritesEnd = spritePtr + spritesCount_;

    if (!visible_) {
        for (; spritePtr != spritesEnd; ++spritePtr)
            spritePtr->setVisible(false);
        return;
    }


    Point pa{r_.x, r_.y}, pb{r_.x + r_.w, r_.y}, pc{}, pd{};
    auto h = r_.h;
    if (h == 0)
        return;

    Color pcolor{};
    Cord pposition = 0;

    for (const auto& itemIt : items_) {
        if (!itemIt.second.valid)
            continue;

        const auto &item = itemIt.second;
        const auto &position = item.gradient.position;
        if (position != pposition) {

            if (M::OrientationVert & orientationFlags_) {
                auto y = r_.y + position*r_.h;
                pc = {r_.x,y}, pd = {r_.x+r_.w,y};
            }

            if (M::OrientationHorz & orientationFlags_) {
                // TODO: test
                auto x = r_.x + position*r_.w;
                pc = {x,r_.y}, pd = {x,r_.x+r_.w};
            }

            spritePtr->setTriangle(pa, pb, pd)->setColors(pcolor, pcolor, item.gradient.color)->setVisible(true);
            ++spritePtr;

            spritePtr->setTriangle(pa, pd, pc)->setColors(pcolor, item.gradient.color, item.gradient.color)->setVisible(true);
            ++spritePtr;

            pa = pc, pb = pd;
            pposition = position;
        }

        pcolor = item.gradient.color;
    }

    for (; spritePtr != spritesEnd; ++spritePtr)
        spritePtr->setVisible(false);
}

}  // namespace mangl
