/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_segment_widget.h"

#include "../audio/mangl_sound.h"
#include "../mangl_framework.h"

namespace mangl {

void DefaultSegmentWidget::onCreate() {

    auto& ssegmented = M::Styles::segmented;

    normalTex.set(params_.segmentTex);
    firstTex.set(params_.segmentFirstTex);
    lastTex.set(params_.segmentLastTex);

    if (normalTex.normal) {
        prim_ = scene_->newPrim()->setTexture(normalTex.normal);
    }

    if (params_.widget)
        switchWidget(params_.widget);
    if (params_.selectedWidget) params_.selectedWidget->setVisible(false);
    if (params_.pressedWidget) params_.pressedWidget->setVisible(false);

    // Check the theme
    if (!prim_ && !params_.widget) {
        if (ssegmented.segmentTex) {
            normalTex.set(ssegmented.segmentTex);
            firstTex.set(ssegmented.segmentFirstTex);
            lastTex.set(ssegmented.segmentLastTex);

            prim_ = scene_->newPrim()->setTexture(normalTex.normal);
        }
    }

    if (normalTex.normal) {
        currTex = normalTex;
        prim_->setTexture(currTex.normal);
    }

    if (!params_.text.empty()) {
        textLabel_ = scene_->newLabel({
            .text = params_.text,
            .pos = this->r_.center(),
            .color = ssegmented.textColor,
            .textSize = ssegmented.textSize,
            .flags = M::AlignCenterAll,
        });
    }

    onSetIndex(index_, total_);


    updateStatus();
    updateLayout();
    updateVisible();

}

void DefaultSegmentWidget::updateLayout() {
    if (prim_)
        prim_->move(this->r_);

    if (currentWidget_)
        currentWidget_->move(this->r_);

    if (textLabel_)
        textLabel_->setPos(this->r_.center());
}

void DefaultSegmentWidget::updateStatus() {
    if (prim_) {
        if (pressed_) {
            if (currTex.pressed)
                prim_->setTexture(currTex.pressed);
        } else {
            if (selected_) {
                if (currTex.selected)
                    prim_->setTexture(currTex.selected);
            } else {
                if (currTex.normal)
                    prim_->setTexture(currTex.normal);
            }
        }
    }

    if (pressed_) {
        switchWidget(params_.pressedWidget);
    } else {
        if (selected_)
            switchWidget(params_.selectedWidget);
        else
            switchWidget(params_.widget);
    }

    if (textLabel_)
        textLabel_->setPos(this->r_.center());
}


void DefaultSegmentWidget::updateVisible() {
    if (prim_) prim_->setVisible(visible_);
    if (currentWidget_) currentWidget_->setVisible(visible_);
    if (textLabel_) textLabel_->setVisible(visible_);
}

void DefaultSegmentWidget::onAlpha()
{
    if (!created_)
        return;

    if (alphaFlag_) {
        if (prim_)
            prim_->setAlpha(alpha_);

        if (currentWidget_)
            currentWidget_->setAlpha(alpha_);

        if (textLabel_)
            textLabel_->setAlpha(alpha_);
    } else {
        if (prim_)
            prim_->clearAlpha();

        if (currentWidget_)
            currentWidget_->clearAlpha();

        if (textLabel_)
            textLabel_->clearAlpha();
    }
}




}  // namespace mangl

