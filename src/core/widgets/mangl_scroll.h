/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <utility>

#include "mangl_widget.h"

#include "mangl_scroll_handler.h"

#include "../mangl_storage.h"

namespace mangl {

class ScrollWidget: public WidgetTemplate<ScrollWidget> {
    friend struct ScrollWidgetHandler;

    using Self = ScrollWidget;
    using Sret = ScrollWidgetP;

    ScrollWidgetHandler horzHandler_{this}, vertHandler_{this};

    struct Content {
        Rect addRect{};
        Triangle addTriangle{};
        Point addPos{};

        Rect r{};
        std::array<Point, 3> tri{};
        PrimP prim{};
        WidgetP widget{};
    };

    std::vector<Content> contents_;

    void addContent(Content c);

    struct Page {
        Cord width{}, height{};
        PrimP emptyPrim{}, fullPrim{};
    };

    using PagesList = std::vector<Page>;

    bool pagingEnabled_ = false;
    PagesList pages_;

    Point origin_{};

public:
    enum class IndicatorLocation {
        None, Bottom, Top, Left, Right
    };

private:
    IndicatorLocation indicatorLocation_{IndicatorLocation::None};
    Cord indicatorWidth_{}, indicatorTexScale_{};
    PrimP indicatorFillSprite_{}, indicatorLeftSprite_{}, indicatorRightSprite_{};
    Texture indicatorFillTex_{}, indicatorLeftTex_{}, indicatorRightTex_{};

    int orientationFlags_{M::OrientationHorz};

    Cord movedMaxDistance_{};
    Point movedBeginPoint_{};

    Padding contentsPadding_{};

    Point start_position_, start_velocity_;  // the state at the beginning of the movement
    Timestamp start_timestamp_;
    Point target_;  // target where the control should be moved

    Timestamp currentTimestamp_{}, lastTimestamp_{};

    auto updateContents() {
        updateStats();
        updateWidgets();
        updateIndicators();
        return this;
    }
    void updateWidgets();
    void updateStats();

    void setupIndicators();
    void updateIndicators();
    void updateLayout();
    void updateVisible();
    void updateAlpha();

    bool stateChanged_{false};
    bool savedStateLoaded_{false};
    int savedPosition_{-1};
    void loadSavedState();


public:
    ScrollWidgetP addPrim(PrimP prim, RectA rect);
    auto addPrim(PrimP prim) { return addPrim(prim, prim->getRect()); }

    ScrollWidgetP addWidget(WidgetP widget, RectA rect);
    ScrollWidgetP addWidget(LabelP widget, PointA pos);
    auto addWidget(WidgetP widget) { return addWidget(widget, widget->getRect()); }

    Sret addPage(Cord width, PrimP full = nullptr, PrimP empty = nullptr);
    auto setOrigin(Point origin) {
        origin_ = origin;
        return this;
    }
    Sret setContentsSize(Size size);
    Sret offsetContents(Point point);
    Sret setIndicator(TextureA fillTex, TextureA leftTex, TextureA rightTex, Cord width = 0);
    Sret setIndicatorLocation(IndicatorLocation location) { indicatorLocation_ = location;  return this; }

    auto setContentsPadding(PaddingA padding) {
        contentsPadding_ = padding;
        return updateContents();
    }


    template <class Container>
    auto addWidgets(Container&& widgets) {
        for (auto& widget: widgets)
            addWidget(widget);
        return this;
    }

    template <class Container>
    auto addPrims(Container&& prims) {
        for (auto& prim: prims)
            addPrim(prim);
        return this;
    }

    auto setOrientation(int o) noexcept {
        orientationFlags_ = o;
        return updateContents();
    }


    void scrollToPage(int pageIndex);

    auto enablePaging(bool flag = true) {
        horzHandler_.pagingEnabled = vertHandler_.pagingEnabled = pagingEnabled_ = flag;
        return this;
    }

    auto setSnapToEnd(bool flag = true) {
        horzHandler_.snapToEnd = vertHandler_.snapToEnd = flag;
        return this;
    }

    ScrollWidgetP scrollToEnd();
    ScrollWidgetP scrollToPosition(Cord position);
    ScrollWidgetP scrollToBegin();



    [[nodiscard]] Cord currentPosition() const noexcept {
        return (orientationFlags_ & M::OrientationHorz) ? horzHandler_.currentOffset : vertHandler_.currentOffset;
    }

    //
    // Status functions
    //
    [[nodiscard]] int currentPage() const {
        return (orientationFlags_ & M::OrientationHorz) ? horzHandler_.currentPage : vertHandler_.currentPage;
    }
    [[nodiscard]] int totalPages() const { return static_cast<int>(pages_.size()); }
    [[nodiscard]] bool scrolling() const { return horzHandler_.moving() || vertHandler_.moving(); }

    using PositionSignal = std::function<void(WidgetId, int)>;
    using AnonPositionSignal = std::function<void(int)>;

    PositionSignal pageSelectedSignal{}, positionSignal{};
    AnonPositionSignal anonPageSelectedSignal{}, anonPositionSignal{};

    Self *pageSignalConnect(PositionSignal f) {
        anonPageSelectedSignal = nullptr;
        pageSelectedSignal = std::move(f);
        return this;
    }
    auto pageSignalConnect(AnonPositionSignal f) {
        pageSelectedSignal = nullptr;
        anonPageSelectedSignal = f;
        return this;
    }


    auto setStateTag(StrView stateTag) {
        statePosition_.setKey("mangl_widget_scroll_position:").setSuffix(stateTag);
        loadSavedState();
        return this;
    }

    Setting<int> statePosition_;


public:
    void onInit() override;



protected:
    void onCreate() override;
    void onDestroy() override;
    void onMove() override { updateLayout(); }

    void onVisible(bool visible) override { updateVisible(); }
    void onAlpha() override { updateAlpha(); }


    bool onTouchDown(TouchA) override;
    bool onTouchMove(TouchA) override;
    bool onTouchUp(TouchA) override;

    void onTimer(Timestamp timestamp) override;


};


}  // namespace mangl
