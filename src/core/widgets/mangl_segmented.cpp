/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_segmented.h"

#include "../audio/mangl_sound.h"
#include "../mangl_framework.h"

namespace mangl {

SegmentedP Scene::newSegmented(const SegmentedParams& params) {
    return newWidget<Segmented>(params.flags)->onNew(params)->create();
}

SegmentedP Segmented::onNew(const SegmentedParams& params) {

    setParams(params);

    if (params.rect.w > 0)
       move(params.rect);

    for (auto& label: params.labels) {
        addSegment(label);
    }

    for (auto& seg: params.segments) {
        addSegment({
            .text = seg.label,
            .segmentTex = seg.texture,
        });
    }

    onSelect = params.onSelect;
    onSelectId = params.onSelectId;

    return this;
}


void Segmented::onCreate()
{
    for (auto& row: rows_) {
        for (auto& it: row) {
            it.segment->create();
        }
    }

    updateLayout();
    loadSavedState();
}

void Segmented::onDestroy()
{
    if (stateSegment_.hasKey() && stateChanged_) {
        stateSegment_ = selectedSegment_;
    }
}

SegmentedP Segmented::addSegment(const SegmentWidgetParams& params) {
    if (rows_.empty())
        rows_.emplace_back();

    auto& row = rows_.back();

    if (selectedSegment_ == -1 && row.empty() && rows_.size() == 1)
        selectedSegment_ = 0;


    auto& item = row.emplace_back();
    int totalItems = 0;
    for (auto& rowIt: rows_)
        for (auto& itemIt: rowIt)
            totalItems++;
    item.index = totalItems - 1;  // Start from zero


    if (savedStateLoaded_ && !savedStateSet_ && totalItems > savedStateIndex_) {
        selectedSegment_ = savedStateIndex_;
        savedStateSet_ = true;
    }


    if (params.segment) {
        item.segment = params.segment;
    } else {
        item.segment = this->scene_->newWidget<DefaultSegmentWidget>();
    }

    item.segment->onSetParams(params);

    if (created_)
        item.segment->create();

    for (auto& rowIt: rows_) {
        int colIndex = 0;
        for (auto& it: rowIt) {
            it.segment->onSetIndex(colIndex, (int)rowIt.size());
            it.segment->onSetSelected(it.index == selectedSegment_);
            colIndex++;
        }
    }

    updateLayout();

    return this;
}

/*
Segmented *Segmented::addSegment(TextureA buttonTex, TextureA pressedTex, TextureA selectedTex, Size segSize)
{
    if (selectedSegment_ == -1 && items_.empty())
        selectedSegment_ = 0;

    SegmentItem item;

    item.index = (int)items_.size();
    item.buttonTex = buttonTex;
    item.pressedTex = pressedTex;
    item.selectedTex = selectedTex;
    item.size = buttonTex.size().scaled(segSize);

    items_.push_back(item);

    updateLayout();

    return this;
}

Segmented *Segmented::addSegment(Widget *segmentWidget, Widget *pressedWidget, Widget *selectedWidget, Size segSize)
{
    if (selectedSegment_ == -1 && items_.empty())
        selectedSegment_ = 0;

    SegmentItem item;

    item.index = (int)items_.size();
    item.widget = segmentWidget;
    item.pressedWidget = pressedWidget ? pressedWidget : item.widget;
    item.selectedWidget = selectedWidget ? selectedWidget : item.pressedWidget;
    item.size = segSize;

    items_.push_back(item);

    updateLayout();

    return this;
}
*/


void Segmented::updateLayout()
{
    if (!created_)
        return;

    if (backgroundWidget_)
        backgroundWidget_->move(r_);

    if (backgroundPrim_)
        backgroundPrim_->move(r_);

    auto rowHeight = r_.h;
    if (rows_.size() > 1)
        rowHeight = rowHeight / Cord(rows_.size());

    Point current(r_.position());
    current.y = r_.top() - rowHeight;

    for (auto& row: rows_) {
        Size itemSize = r_.size();

        itemSize.h = rowHeight;

        if (M::OrientationHorz == orientation_) {
            current.x = r_.x;
            itemSize.w /= Cord(row.size());
        } else {
            itemSize.h /= Cord(row.size());
            current.y = r_.y;
        }

        for (auto& item: row) {
            item.rect = Rect::make(current, itemSize);
            item.segment->move(item.rect);

            if (M::OrientationHorz == orientation_)
                current.x += item.rect.w;
            else
                current.y += item.rect.h;
        }

        current.y -= rowHeight;
    }

    updateScene();
}

void Segmented::updateScene()
{
    if (!created_)
        return;

    for (auto& row: rows_) {
        for (auto& item: row) {

            if (tracking_ && item.index == pressedSegment_) {
                item.segment->onSetPressed(true);
            } else {
                item.segment->onSetPressed(false);

                if (item.index == selectedSegment_) {
                    item.segment->onSetSelected(true);
                } else {
                    item.segment->onSetSelected(false);
                }
            }
        }
    }
}

void Segmented::loadSavedState()
{
    if (!stateSegment_.hasKey() || !created_ || savedStateLoaded_)
        return;

    savedStateIndex_ = stateSegment_.get(stateDefaultSegment_);

    int totalItems = getTotalItems();

    if (savedStateIndex_ < totalItems) {
        select(savedStateIndex_);
        savedStateSet_ = true;
    }

    savedStateLoaded_ = true;
}


void Segmented::onVisible(bool visible) {
    if (!created_)
        return;

    if (backgroundWidget_) backgroundWidget_->setVisible(visible);
    if (backgroundPrim_) backgroundPrim_->setVisible(visible);

    for (auto& row: rows_) {
        for (auto &item: row) {
            item.segment->setVisible(visible);
        }
    }

}

void Segmented::onEnable(bool enabled) {
    if (!created_)
        return;

    for (auto& row: rows_) {
        for (auto &item: row) {
            item.segment->setEnabled(enabled);
        }
    }
}


void Segmented::onAlpha() {
    if (!created_)
        return;

    if (alphaFlag_) {
        if (backgroundWidget_)
            backgroundWidget_->setAlpha(alpha_);

        if (backgroundPrim_)
            backgroundPrim_->setAlpha(alpha_);


        for (auto& row: rows_) {
            for (auto &item: row) {
                item.segment->setAlpha(alpha_);
            }
        }
    } else {
        if (backgroundWidget_)
            backgroundWidget_->clearAlpha();

        if (backgroundPrim_)
            backgroundPrim_->clearAlpha();


        for (auto& row: rows_) {
            for (auto &item: row) {
                item.segment->clearAlpha();
            }
        }
    }
}


bool Segmented::onTouchDown(TouchA touch)
{
    trackingSegment_ = -1;

    if (r_.contains(touch.p)) {
        // Search for the corresponding gwidget
        for (auto& row: rows_) {
            for (auto &item: row) {
                if (item.rect.contains(touch.p)) {
                    item.segment->onSetPressed(true);
                    trackingSegment_ = item.index;
                    trackingStartSegment_ = item.index;
                    tracking_ = true;
                    break;
                }
            }
        }
    }

    return tracking_;
}

bool Segmented::onTouchMove(TouchA touch)
{
    if (!tracking_)
        return false;

    bool processed = false;

    // search for the corresponding gwidget
    if (r_.contains(touch.p)) {

        // leave the current tracking segment in its original state
        if (trackingSegment_ != -1) {
            auto& trackingItem = getItemByIndex(trackingSegment_);

            trackingItem.segment->onSetPressed(false);

            if (selectedSegment_ == trackingSegment_) {
                trackingItem.segment->onSetSelected(true);
            } else {
                trackingItem.segment->onSetSelected(false);
            }
        }

        for (auto& row: rows_) {
            bool found = false;
            for (auto &item: row) {
                if (item.rect.contains(touch.p)) {
                    trackingSegment_ = item.index;
                    item.segment->onSetPressed(true);
                    found = true;
                    break;
                }
            }
            if (found)
                break;
        }

        processed = true;
    } else {
        if (trackingSegment_ != -1) {
            auto& trackingItem = getItemByIndex(trackingSegment_);

            trackingItem.segment->onSetPressed(false);

            if (selectedSegment_ == trackingSegment_) {
                trackingItem.segment->onSetSelected(true);
            } else {
                trackingItem.segment->onSetSelected(false);
            }
        }
    }

    return processed;
}

bool Segmented::onTouchUp(TouchA touch)
{
    bool processed = false;

    if (r_.contains(touch.p)) {

        // check which 
        if (trackingSegment_ != -1) {
            if (trackingSegment_ == trackingStartSegment_) {
                if (selectedSegment_ != -1) {
                    auto &item = getItemByIndex(selectedSegment_);
                    item.segment->onSetSelected(false);
                    item.segment->onSetPressed(false);
                }

                selectedSegment_ = trackingSegment_;

                auto &item = getItemByIndex(selectedSegment_);
                item.segment->onSetPressed(false);
                item.segment->onSetSelected(true);

                selectedSignalId(controlId_, selectedSegment_);
                selectedSignal(selectedSegment_);

                if (onSelectId) onSelectId(controlId_, selectedSegment_);
                if (onSelect) onSelect(selectedSegment_);


                stateChanged_ = true;

                // deal with the sounds
                if (soundId_)
                    Sfx::play(soundId_);
                else if (mangl_internal::defaultButtonSound_ != mangl_internal::InvalidSoundButton)
                    Sfx::play(mangl_internal::defaultButtonSound_);

            } else {  // we ended up on the wrong segment

                auto& item = getItemByIndex(trackingSegment_);

                item.segment->onSetSelected(selectedSegment_ == trackingSegment_);
            }

        }

        processed = true;
    }

    trackingSegment_ = -1;

    tracking_ = false;

    return processed;
}

bool Segmented::onTouchCancel(TouchA touch)
{
    if (trackingSegment_ != -1) {
        auto& titem = getItemByIndex(trackingSegment_);
        auto& sitem = getItemByIndex(selectedSegment_);

        titem.segment->onSetPressed(false);
        sitem.segment->onSetSelected(true);
    }

    tracking_ = false;
    return false;
}

}  // namespace mangl

