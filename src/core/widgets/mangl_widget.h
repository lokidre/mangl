/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_prim.h"
#include "../mangl_touches.h"
#include "../mangl_keyboard.h"
#include "../mangl_scene_decl.h"
#include "../mangl_widgets_decl.h"
#include "mangl_widget_layout.h"

#include <ranges>

namespace mangl {

enum class WidgetType {
    Unknown,
    Label,
    Button,
    Checkbox,
    Stack,
    Slider,
};

struct WidgetItem {
    WidgetId id{};
    PrimP prim{};
    WidgetP widget{};
    WidgetLayout layout{};
    bool wasVisible{};
    bool visible{true};
};

struct Widget {
    friend struct Dialog;

    using Self = Widget;
    using SelfRet = Self*;

    Widget* prev{};
    Widget* next{};


    virtual ~Widget() = default;

    WidgetType type{WidgetType::Unknown};

    [[nodiscard]] bool isLabel() const noexcept { return WidgetType::Label == type; }



    //
    // Status
    //
    bool created_{};
    bool visible_{};
    bool disabled_{};
    bool checked_{};

    bool tracking_{};
    bool pressed_{};  // Widget is in pressed state

    bool moved_{};
    bool interactionDisabled_{};
    SceneP scene_{};
    Rect r_{};

    Flags flags_{0};

    Real alpha_{1};
    bool alphaFlag_{};

    bool scissorFlag_{};
    Rect scissorRect_{};

    Color color_{};
    bool colorFlag_{};

    bool passthroughTouchesMove_{};

    bool scrollMember_{};
    ScrollWidgetP scrollWidget_{};
    WidgetId controlId_{};

    std::vector<WidgetItem> childItems_;

    M::Orientation orientation_{M::Orientation0};


    auto create() {
        if (!created_) {
            created_ = true;
            onCreate();
            onCreate(this->scene_);
        }
        return this;
    }

    auto setPassthroughTouchesMove(bool pt) {
        passthroughTouchesMove_ = pt;
        return this;
    }

    auto setId(WidgetId wid) noexcept {
        controlId_ = wid;
        return this;
    }
    [[nodiscard]] auto getId() const noexcept { return controlId_; }


    auto& addChildItem(PrimP prim) {
        childItems_.push_back({
            .prim = prim,
            .visible = true,
        });
        return childItems_.back();
    }

    auto& addChildItem(WidgetP widget) {
        childItems_.push_back({
            .widget = widget,
            .visible = true,
        });
        return childItems_.back();
    }

    auto addChildItems(const std::ranges::range auto& children) {
        for (const auto& child: children)
            addChildItem(child);
    }

    auto addChildItems(PrimP prims, int count) {
        for (auto prim = prims, end = prims + count; prim != end; ++prim)
            addChildItem(prim);
    }



    [[nodiscard]] bool visible() const noexcept { return visible_; }
    [[nodiscard]] bool isVisible() const noexcept { return visible_; }

    auto setVisible(bool visible = true) noexcept {
        visible_ = visible;
        onVisible(visible);
        //onVisible();
        return this;
    }
    auto hide() noexcept { return setVisible(false); }
    auto unhide() noexcept { return setVisible(true); }

    [[nodiscard]] auto enabled() const noexcept { return !disabled_; }
    [[nodiscard]] auto disabled() const noexcept { return disabled_; }

    WidgetP setEnabled(bool flag = true) noexcept {
        disabled_ = !flag;
        onEnable(flag);
        // onEnable();
        for (auto& item: childItems_)
            if (item.widget) item.widget->setEnabled(flag);
        return this;
    }

    auto disable() noexcept { return setEnabled(false); }
    auto enable() noexcept { return setEnabled(true); }


    auto setChecked(bool checked) noexcept {
        checked_ = checked;
        onCheckedState(checked);
        return this;
    }
    [[nodiscard]] auto isChecked() const noexcept { return checked_; }

    [[nodiscard]] auto alpha() const { return alpha_; }
    [[nodiscard]] auto getAlpha() const noexcept { return alphaFlag_ ? alpha_ : 1.; }
    auto setAlpha(bool enabled, Real alpha) noexcept {
        alphaFlag_ = enabled;
        alpha_ = alpha;
        onAlpha();
        return this;
    }
    auto setAlpha(Real alpha) noexcept { return setAlpha(true, alpha); }
    auto clearAlpha() noexcept {
        alphaFlag_ = false;
        onAlpha();
        return this;
    }
    [[nodiscard]] auto isAlpha() const noexcept { return alphaFlag_; }


    auto setScissor(RectA r) noexcept {
        scissorFlag_ = true;
        scissorRect_ = r;
        onScissor(scissorFlag_, scissorRect_);
        return this;
    }

    auto clearScissor() noexcept {
        scissorFlag_ = false;
        onScissor(scissorFlag_, scissorRect_);
        return this;
    }

    auto setScissor(bool ena, RectA r) noexcept { return ena ? setScissor(r) : clearScissor(); }

    [[nodiscard]] const auto& getColor() const noexcept { return color_; }
    auto setColor(ColorA color) noexcept {
        color_ = color;
        colorFlag_ = true;
        onColor(colorFlag_, color_);
        // onColor();
        return this;
    }
    auto clearColor() noexcept {
        color_ = {};
        colorFlag_ = false;
        onColor(colorFlag_, color_);
        // onColor();
        return this;
    }
    [[nodiscard]] auto isColor() const noexcept { return colorFlag_; }


    auto setOrientationBase(M::Orientation orientation) {
        orientation_ = orientation;
        onOrientation(orientation);
        return this;
    }


    // SelfRet geo(RectA frame) {
    //     r_ = frame;
    //     onMove();
    //     return this;
    // }
    auto move(RectA r) {
        r_ = r;
        onMove(r);
        onMove();
        return this;
    }
    // SelfRet frame(RectA f) {
    //     r_ = f;
    //     onMove();
    //     return this;
    // }
    // SelfRet pos(Point point) {
    //     r_.move(point);
    //     onMove();
    //     return this;
    // }

    auto move(Point p) {
        r_.move(p);
        onMove(r_);
        onMove();
        return this;
    }
    // SelfRet resize(Size size) {
    //     r_.resize(size);
    //     onMove();
    //     return this;
    // }
    SelfRet move(Point p, Size s) noexcept {
        r_.x = p.x, r_.y = p.y, r_.w = s.w, r_.h = s.h;
        onMove(r_);
        onMove();
        return this;
    }

    SelfRet setPos(Point p) noexcept {
        r_.x = p.x, r_.y = p.y;
        onMove(r_);
        onMove();
        return this;
    }

    SelfRet setPos(Cord x, Cord y) noexcept {
        r_.x = x, r_.y = y;
        onMove(r_);
        onMove();
        return this;
    }

    [[nodiscard]] RectA rect() const noexcept { return r_; }
    [[nodiscard]] auto getRect() const noexcept { return r_; }
    [[nodiscard]] Cord width() const noexcept { return r_.w; }
    [[nodiscard]] Cord height() const noexcept { return r_.h; }
    [[nodiscard]] Cord x() const noexcept { return r_.x; }
    [[nodiscard]] Cord y() const noexcept { return r_.y; }

    void setZIndexFront() {
        onSetZIndexFront();
    }


    [[nodiscard]]
    auto findItem(WidgetId id) noexcept {
        for (auto it = childItems_.begin(), end = childItems_.end(); it != end; ++it)
            if (it->id == id)
                return it;
        return childItems_.end();
    }

    [[nodiscard]]
    auto findItem(PrimP prim) noexcept {
        for (auto it = childItems_.begin(), end = childItems_.end(); it != end; ++it)
            if (it->prim == prim)
                return it;
        return childItems_.end();
    }

    [[nodiscard]]
    auto findItem(WidgetP widget) noexcept {
        for (auto it = childItems_.begin(), end = childItems_.end(); it != end; ++it)
            if (it->widget == widget)
                return it;
        return childItems_.end();
    }

    [[nodiscard]]
    PrimP getItemPrim(WidgetId id) noexcept {
        if (auto it = findItem(id); it != childItems_.end())
            return it->prim;
        return nullptr;
    }

    [[nodiscard]]
    WidgetP getItemWidget(WidgetId id) noexcept {
        if (auto it = findItem(id); it != childItems_.end())
            return it->widget;
        return nullptr;
    }

    auto setItemVisible(WidgetId id, bool b) noexcept {
        if (auto item = findItem(id); item != childItems_.end()) {
            item->visible = b;
            if (item->prim) item->prim->setVisible(b);
            if (item->widget) item->widget->setVisible(b);
        }
        //updateVisible();
        return this;
    }

    auto setItemVisible(PrimP prim, bool b) noexcept {
        if (!prim) return this;
        if (auto item = findItem(prim); item != childItems_.end()) {
            item->visible = b;
            if (item->prim) item->prim->setVisible(b);
            if (item->widget) item->widget->setVisible(b);
        }
        return this;
    }

    auto setItemVisible(WidgetP widget, bool b) noexcept {
        if (!widget) return this;
        if (auto item = findItem(widget); item != childItems_.end()) {
            if (item->prim) item->prim->setVisible(b);
            if (item->widget) item->widget->setVisible(b);
        }
        return this;
    }


    [[nodiscard]] SceneP getScene() const noexcept { return scene_; }


    virtual bool onTouchDown(TouchA) { return false; }
    virtual bool onTouchMove(TouchA) { return false; }
    virtual bool onTouchUp(TouchA) { return false; }
    virtual bool onTouchCancel(TouchA) { return false; }

    virtual bool onKeyDown(KeyCode key) { return false; }
    virtual bool onKeyUp(KeyCode key) { return false; }
    virtual bool onMouseWheel(Real delta, Flags keys, Point pos) { return false; }

    virtual void onTimer(Timestamp) {}

    virtual void onInit() {}
    virtual void onCreate() {}
    virtual void onCreate(SceneP scene) {}
    virtual void onDestroy() {}
    virtual void onMove(RectA) {}
    virtual void onMove() {}

    // virtual void onVisible() {
    //     for (auto& item: childItems_) {
    //         if (item.prim) item.prim->setVisible(visible_);
    //         if (item.widget) item.widget->setVisible(visible_);
    //     }
    // }

    virtual void onVisible(bool visible) {
        for (auto& item: childItems_) {
            if (item.prim) item.prim->setVisible(visible && item.visible);
            if (item.widget) item.widget->setVisible(visible && item.visible);
        }
    }

    virtual void onAlpha() {
        for (auto& item: childItems_) {
            if (item.prim) item.prim->setAlpha(alphaFlag_, alpha_);
            if (item.widget) item.widget->setAlpha(alphaFlag_, alpha_);
        }
    }
    virtual void onScissor(bool scissorEnabled, RectA scissorRect) {
        for (auto& item: childItems_) {
            if (item.prim) item.prim->setScissor(scissorEnabled, scissorRect);
            if (item.widget) item.widget->setScissor(scissorEnabled, scissorRect);
        }
    }

    virtual void onColor(bool flag, ColorA color) {}
    // virtual void onColor() {}

    virtual void onEnable(bool enabled) {}
    // virtual void onEnable() {}
    // virtual void onUpdate(){}

    virtual void onSetZIndexFront();

    virtual void onPressedState(bool pressed) {
        pressed_ = pressed;
    }
    virtual void onCheckedState(bool checked) {}
    virtual void onSelectedState(bool selected) {}

    virtual void onOrientation(M::Orientation orientation) {
        for (auto& item: childItems_) {
            if (item.prim) item.prim->setOrientation(orientation);
            if (item.widget) item.widget->setOrientationBase(orientation);
        }
    }
};


template <class WT>
class WidgetTemplate: public Widget {
    using Base = Widget;

    using B = Base;
    using R = WT*;


public:
    auto create() { return static_cast<R>(B::create()); }
    auto setPassthroughTouchesMove(bool pt) {
        return static_cast<R>(B::setPassthroughTouchesMove(pt));
    }

    auto setId(WidgetId wid) noexcept { return static_cast<R>(B::setId(wid)); }
    
    auto setVisible(bool flag = true) noexcept { return static_cast<R>(B::setVisible(flag)); }
    auto hide() noexcept { return setVisible(false); }
    auto show() noexcept { return setVisible(true); }
    auto unhide() noexcept { return setVisible(true); }

    auto setEnabled(bool flag = true) noexcept { return static_cast<R>(B::setEnabled(flag)); }
    auto enable() noexcept { return setEnabled(true); }
    auto disable() noexcept { return setEnabled(false); }

    auto setChecked(bool checked) noexcept { return static_cast<R>(B::setChecked(checked)); }

    auto setAlpha(Real alpha) { return static_cast<R>(B::setAlpha(alpha)); }
    auto setAlpha(bool enabled, Real alpha) { return static_cast<R>(B::setAlpha(enabled, alpha)); }
    auto clearAlpha() { return static_cast<R>(B::clearAlpha()); }

    R setColor(ColorA color) { return (R)B::setColor(color); }
    R clearColor() { return (R)B::clearColor(); }

    R setScissor(RectA r) noexcept { return (R)B::setScissor(r); }
    R setScissor(bool ena, RectA r) noexcept { return (R)B::setScissor(ena, r); }
    R clearScissor() noexcept { return (R)B::clearScissor(); }

    auto setOrientation(M::Orientation o) { return static_cast<R>(setOrientationBase(o)); }

    // R geo(RectA f) { return (R)B::geo(f); }
    R move(RectA r) { return static_cast<R>(B::move(r)); }
    R move(PointA p) { return static_cast<R>(B::move(p)); }
    R setPos(PointA p) { return static_cast<R>(B::setPos(p)); }
    R setPos(Cord x, Cord y) { return static_cast<R>(B::setPos(x, y)); }
    // R frame(RectA f) { return (R)B::geo(f); }
    // R pos(Point p) { return (R)B::pos(p); }
    //R move(Point p) { return (R)B::move(p); }
    R move(Point p, Size s) { return (R)B::move(p, s); }
    // R resize(Size p) { return (R)B::resize(p); }
};

} // namespace mangl
