/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_scroll.h"

#include "mangl_label.h"

#include "../mangl_scene.h"

#include <cognit/func.hpp>

#include <limits>

namespace mangl {

void ScrollWidget::onInit()
{
    indicatorFillSprite_ = scene_->newPrim();
    indicatorLeftSprite_ = scene_->newPrim();
    indicatorRightSprite_ = scene_->newPrim();
}


void ScrollWidget::onCreate()
{
    setupIndicators();
    updateContents();
    updateLayout();

    horzHandler_.onStart(0);
    vertHandler_.onStart(0);

    loadSavedState();
}

void ScrollWidget::loadSavedState()
{
    if (statePosition_.hasKey() || !created_ || savedStateLoaded_)
        return;

    int position = statePosition_(-1);
    savedPosition_ = position;
    if (position >= 0)
        scrollToPosition((Cord)position);
    savedStateLoaded_ = true;
}


void ScrollWidget::onDestroy()
{
    if (statePosition_.hasKey() && savedPosition_ != (int)currentPosition()) {
        statePosition_ = (int)currentPosition();
    }
}


void ScrollWidget::updateLayout()
{
    auto& rect = this->r_;
    horzHandler_.windowSize = rect.w;
    vertHandler_.windowSize = rect.h;

    for (auto& c: contents_) {
        if (c.prim) c.prim->setScissor(r_);
        if (c.widget) c.widget->setScissor(r_);
    }
}

void ScrollWidget::updateVisible() {
    for (auto& c: contents_) {
        if (c.prim) c.prim->setVisible(visible_);
        if (c.widget) c.widget->setVisible(visible_);
    }
}

void ScrollWidget::updateAlpha() {
    for (auto& c: contents_) {
        if (alphaFlag_) {
            if (c.prim) c.prim->setAlpha(alpha_);
            if (c.widget) c.widget->setAlpha(alpha_);
        } else {
            if (c.prim) c.prim->clearAlpha();
            if (c.widget) c.widget->clearAlpha();
        }
    }
}




ScrollWidget *ScrollWidget::setIndicator(TextureA fillTex, TextureA leftTex, TextureA rightTex, Cord width)
{
    indicatorFillTex_ = fillTex;
    indicatorLeftTex_ = leftTex;
    indicatorRightTex_ = rightTex;
    indicatorWidth_ = width;


    setupIndicators();
    updateContents();
    return this;
}


ScrollWidget *ScrollWidget::setContentsSize(Size size)
{
    horzHandler_.contentsSize = size.w;
    vertHandler_.contentsSize = size.h;
    updateContents();
    return this;
}



ScrollWidgetP ScrollWidget::addWidget(Widget *widget, RectA rect)
{
    Content c;
    widget->scrollMember_ = true;
    widget->scrollWidget_ = this;
    c.widget = widget;
    c.addRect = rect;
    addContent(c);
    return this;
}

ScrollWidgetP ScrollWidget::addWidget(LabelP label, PointA pos) {
    Content c;
    label->scrollMember_ = true;
    label->scrollWidget_ = this;
    c.widget = label;
    c.addPos = pos;
    c.addRect = label->getRect();
    addContent(c);
    return this;
}


ScrollWidgetP ScrollWidget::addPrim(PrimP prim, RectA rect)
{
    Content c;
    c.prim = prim;
    c.addRect = prim->getRect();
    addContent(c);
    return this;
}



ScrollWidgetP ScrollWidget::offsetContents(Point point)
{
    for (auto& c: contents_) {
        if (c.prim) {
            if (Prim::Type::Triangle == c.prim->type) {
                for (auto& p: c.tri)
                    p += point;

                c.prim->setTriangle(c.tri[0], c.tri[1], c.tri[2]);
            } else {
                c.r += point;
                c.prim->move(c.r);
            }
        } else if (c.widget) {
            c.r += point;
            c.widget->move(c.r);
        }
    }

    updateContents();

    return this;
}



void ScrollWidget::addContent(Content c)
{
    contents_.push_back(c);

    if (pagingEnabled_ && pages_.empty()) {
        if (orientationFlags_ & M::OrientationHorz)
            addPage(c.r.w);
        else if (orientationFlags_ & M::OrientationVert)
            addPage(c.r.h);
    }

    updateStats();
    updateContents();
    updateLayout();
}

ScrollWidgetP ScrollWidget::addPage(Cord extent, PrimP fullPrim, PrimP emptySprite)
{
    Page p;

    if (orientationFlags_ & M::OrientationHorz) {
        horzHandler_.contentsSize += extent;
        p.width = extent;
        horzHandler_.addPage(p.width);
    }

    if (orientationFlags_ & M::OrientationVert) {
        vertHandler_.contentsSize += extent;
        p.height = extent;
        vertHandler_.addPage(p.height);
    }

    p.fullPrim = fullPrim;
    p.emptyPrim = emptySprite;

    pages_.push_back(p);

    updateContents();

    return this;
}

void ScrollWidget::scrollToPage(int pageIndex)
{
    if (pageIndex >= (int)pages_.size())
        pageIndex = (int)pages_.size() - 1;
    if (pageIndex < 0)
        pageIndex = 0;

    if (orientationFlags_ & M::OrientationHorz)
        horzHandler_.scrollToPage(pageIndex);
    else if (orientationFlags_ & M::OrientationVert)
        vertHandler_.scrollToPage(pageIndex);

    updateContents();
}



bool ScrollWidget::onTouchDown(TouchA touch)
{
    auto& p = touch.p;
    auto& touchTimestamp = touch.timestamp;

    if (r_.contains(p)) {
        tracking_ = true;

        if (orientationFlags_ & M::OrientationHorz)
            horzHandler_.touchesBegin(p.x, touchTimestamp);

        if (orientationFlags_ & M::OrientationVert)
            vertHandler_.touchesBegin(p.y, touchTimestamp);

//        if (ti.count == 1) {
        moved_ = false;
        movedMaxDistance_ = 0;
        movedBeginPoint_ = p;
//        }
    }

    return false;  // pass the message below
}

bool ScrollWidget::onTouchMove(TouchA touch)
{
    const auto& p = touch.p;

    if (tracking_) {

        if (orientationFlags_ & M::OrientationHorz)
            horzHandler_.touchesMove(p.x, touch.timestamp);

        if (orientationFlags_ & M::OrientationVert)
            vertHandler_.touchesMove(p.y, touch.timestamp);

        moved_ = true;

        // TODO: Convert to movedMaxDistanceSquared
        movedMaxDistance_ = cognit::hypot(p.x - movedBeginPoint_.x, p.y - movedBeginPoint_.y);

        updateWidgets();
        updateIndicators();
    }

    return tracking_;
}

bool ScrollWidget::onTouchUp(TouchA touch)
{
    bool selected = false;

    if (tracking_) {

        selected = true;

        if (orientationFlags_ & M::OrientationHorz)
            horzHandler_.touchesEnd(touch.p.x, touch.timestamp);

        if (orientationFlags_ & M::OrientationVert)
            vertHandler_.touchesEnd(touch.p.y, touch.timestamp);

        tracking_ = false;
    }

    if (!moved_)
        return false;  // pass the touch

    //if (movedMaxDistance_ < maxDistance_)
    //	return false;  // pass the touch

    moved_ = false;

    return selected;
}


void ScrollWidget::onTimer(Timestamp timestamp)
{
    if (!created_)
        return;

    currentTimestamp_ = timestamp;

    // TODO: if ( horzHandler_.onTimer(timestamp) || vertHandler_.onTimer(timestamp) ) {
    if (vertHandler_.onTimer(timestamp)) {
        updateContents();
        updateIndicators();
    }

    lastTimestamp_ = timestamp;
}


void ScrollWidget::updateStats()
{
    if (!created_)
        return;

    Cord minX = std::numeric_limits<Cord>::max() - 1;
    Cord maxX = std::numeric_limits<Cord>::lowest() + 1;
    Cord minY = std::numeric_limits<Cord>::max() - 1;
    Cord maxY = std::numeric_limits<Cord>::lowest() + 1;

    for (auto& c: contents_) {
        Rect r;

        if (c.prim) {
            r = c.addRect; //prim->getRect();
        } else if (c.widget) {
            if (c.widget->isLabel()) {
                auto label = dynamic_cast<LabelP>(c.widget);
                r = c.addRect; //label->getBoundingBox();
                //r.x = r_.x;
                //r.w = r_.w;
                //r.h = label->getFont().hei
                //r.y -= c.widget->
            } else {
                r = c.addRect; //c.widget->getRect();
            }
        } else {
            r = {};
            MDebugTrap();
        }

        if (r.x < minX) minX = r.x;
        if (r.right() > maxX) maxX = r.right();
        if (r.y < minY) minY = r.y;
        if (r.top() > maxY) maxY = r.top();
    }


    //minX += contentsPadding_.left + contentsPadding_.right;
    maxX += contentsPadding_.horz2();

    horzHandler_.contentsMin = minX;
    horzHandler_.contentsMax = maxX;
    horzHandler_.contentsSize = std::abs(maxX - minX);

    maxY += contentsPadding_.vert2();

    vertHandler_.contentsSize = std::abs(maxY - minY);
    vertHandler_.contentsMin = minY;
    vertHandler_.contentsMax = maxY;
}

void ScrollWidget::updateWidgets()
{
    if (!created_)
        return;

    // Horizontal coordinate is the negative of the position
    Point offset{horzHandler_.currentOffset, vertHandler_.currentOffset};
    Point coffset{origin_.x + r_.x + offset.x, origin_.y + r_.y + offset.y};


    // coffset.x += contentsPadding_.left;
    // coffset.y += contentsPadding_.top;

    coffset.y += r_.h;

    for (auto& c: contents_) {
        if (c.prim) {
            if (Prim::Type::Triangle == c.prim->type)
                c.prim->setTriangle(
                    c.tri[0].off(coffset),
                    c.tri[1].off(coffset),
                    c.tri[2].off(coffset));
            else {
                c.r = c.addRect;
                c.r += coffset;
                c.prim->move(c.r);
            }

            //MDebugPrint("prim: %z", c.prim->getRect());

        } else if (c.widget) {
            c.r = c.addRect;
            c.r += coffset;
            c.widget->move(c.r);
        }
    }
}


void ScrollWidget::setupIndicators()
{
    if (!created_)
        return;

    if (indicatorLocation_ == IndicatorLocation::None) {
        indicatorFillSprite_->setVisible(false);
        indicatorLeftSprite_->setVisible(false);
        indicatorRightSprite_->setVisible(false);
        return;
    }

    if (indicatorFillTex_.valid()) {
        indicatorFillSprite_->setTexture(indicatorFillTex_);
        indicatorLeftSprite_->setTexture(indicatorLeftTex_);
        indicatorRightSprite_->setTexture(indicatorRightTex_);
    }

    if (indicatorFillTex_.valid()) {
        if (indicatorWidth_ == 0) {
            indicatorWidth_ = indicatorFillTex_.h();
        }
        if (indicatorTexScale_ == 0) {
            indicatorTexScale_ = indicatorWidth_ / indicatorFillTex_.h();
        }

        auto fillFrame = Rect{r_.x, r_.y, r_.w, indicatorWidth_};
        if (indicatorLeftTex_.valid())
            fillFrame.x += indicatorLeftTex_.w() * indicatorTexScale_, fillFrame.w -= indicatorLeftTex_.w() * indicatorTexScale_;
        if (indicatorRightTex_.valid())
            fillFrame.w -= indicatorRightTex_.w() * indicatorTexScale_;

        indicatorFillSprite_->move(fillFrame)->setTiledHorz(true)->setVisible(visible_);
        if (indicatorLeftTex_.valid())
            indicatorLeftSprite_->move(r_.x, r_.y, indicatorLeftTex_.w() * indicatorTexScale_, indicatorLeftTex_.h() * indicatorTexScale_)->setVisible(visible_);
        if (indicatorRightTex_.valid())
            indicatorRightSprite_->move(r_.right() - indicatorRightTex_.w() * indicatorTexScale_, r_.y, indicatorRightTex_.w() * indicatorTexScale_, indicatorRightTex_.h() * indicatorTexScale_)->setVisible(visible_);
    }

    // handle indicator
    Cord totalExtent{};
    for (auto& page: pages_) {
        if (page.fullPrim) {
            totalExtent += (orientationFlags_ & M::OrientationHorz) ? page.fullPrim->getRect().w : page.fullPrim->getRect().h;
        }
    }

    auto current = r_.pos();

    if (orientationFlags_ & M::OrientationHorz)
        current.x += (r_.w - totalExtent)/2;
    else
        current.y += (r_.h - totalExtent)/2;

    for (auto& page : pages_) {
        if (!page.fullPrim)
            continue;

        page.fullPrim->move(current);

        if (page.emptyPrim)
            page.emptyPrim->move(current)->setVisible(visible_);

        if (orientationFlags_ & M::OrientationHorz)
            current.x += page.fullPrim->getRect().w;
        else
            current.y += page.fullPrim->getRect().h;
    }

}

void ScrollWidget::updateIndicators()
{
    if (!created_)
        return;

    if (pagingEnabled_ && !pages_.empty()) {
        for (auto& p: pages_) {
            if (p.fullPrim)
                p.fullPrim->setVisible(false);
        }

        if (orientationFlags_ & M::OrientationHorz && pages_[horzHandler_.currentPage].fullPrim)
            pages_[horzHandler_.currentPage].fullPrim->setVisible(visible_);
        if (orientationFlags_ & M::OrientationVert && pages_[vertHandler_.currentPage].fullPrim)
            pages_[vertHandler_.currentPage].fullPrim->setVisible(visible_);
    }
}


ScrollWidgetP ScrollWidget::scrollToEnd()
{
    if (orientationFlags_ & M::OrientationHorz) {
        horzHandler_.scrollToEnd();
    }
    if (orientationFlags_ & M::OrientationVert) {
        vertHandler_.scrollToEnd();
    }
    updateContents();
    return this;
}

ScrollWidgetP ScrollWidget::scrollToPosition(Cord position)
{
    if (orientationFlags_ & M::OrientationHorz) {
        horzHandler_.scrollToPosition(position);
    }
    if (orientationFlags_ & M::OrientationVert) {
        vertHandler_.scrollToPosition(position);
    }

    updateContents();
    return this;
}

}  // namespace mangl
