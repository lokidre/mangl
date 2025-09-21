/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_widget.h"
#include "../mangl_texture.h"
#include "../mangl_storage.h"
#include "mangl_segmented_params.h"
#include "mangl_segment_widget.h"

namespace mangl {



class Segmented: public WidgetTemplate<Segmented> {
    friend class Scene;

    using Base = Widget;
    using Self = Segmented;

    using SelfRet = Segmented *;


    struct SegmentItem {
        SegmentWidget* segment{};

        int index{};
        Rect rect{};
    };

    using ItemsList = std::vector<SegmentItem>;
    using RowsList = std::vector<ItemsList>;

    M::OrientationFlag orientation_ = M::OrientationHorz;
    RowsList rows_;

    [[nodiscard]] SegmentItem& getItemByIndex(int index) {
        int current = 0;
        for (auto& row: rows_) {
            for (auto& item: row) {
                if (index == current)
                    return item;
                ++current;
            }
        }
        mthrowError("Invalid segmented index");
    }

    [[nodiscard]] int getTotalItems() {
        int current = 0;
        for (auto& row: rows_) {
            for (auto& item: row) {
                ++current;
            }
        }
        return current;
    }

    int selectedSegment_ = -1;
    int pressedSegment_ = -1;

    int trackingStartSegment_ = -1, trackingSegment_ = -1;

    Widget *backgroundWidget_{};
    Prim *backgroundPrim_{};

    Stat<int> stateSegment_;


    int stateDefaultSegment_{0};
    bool stateChanged_{false};
    int savedStateIndex_{-1};
    bool savedStateLoaded_{false};
    bool savedStateSet_{false};

    int soundId_{};

    SegmentedParams params_{};



    SegmentedP onNew(const SegmentedParams& params);
    void setParams(const SegmentedParams& params) {
        params_ = params;

        setStateTag(params.stateTag, params.defaultIndex);
    }

    void updateLayout();
    void updateScene();
    void loadSavedState();


public:
    SignalIdInt selectedSignalId;
    SignalInt selectedSignal;

    SlotIdInt onSelectId{};
    SlotInt onSelect{};


    SelfRet setOrientation(M::OrientationFlag orientation) { orientation_ = orientation;  return this; }

    SelfRet setBackground(Widget *widget) { backgroundWidget_ = widget; updateLayout();  return this; }
    SelfRet setBackground(Prim *prim) { backgroundPrim_ = prim; updateLayout();  return this; }

    SegmentedP addRow() {
        rows_.emplace_back();
        return this;
    }

    SegmentedP addSegment(const SegmentWidgetParams& params);

    SelfRet addSegment(String title) {
        return addSegment({.text = title});
    }



    SelfRet addSegment(TextureA segmentTex) {
        const auto& buttonTex = segmentTex.contentsFrame(0);
        const auto& pressedTex = segmentTex.count >= 2 ? segmentTex.contentsFrame(1) : buttonTex;
        const auto& selectedTex = segmentTex.count >= 3 ? segmentTex.contentsFrame(2) : pressedTex;
        return addSegment(buttonTex, pressedTex, selectedTex);
    }


    SelfRet addSegment(Widget* segmentWidget, Widget* pressedWidget, Widget* selectedWidget) {
        return addSegment({
            .widget = segmentWidget,
            .pressedWidget = pressedWidget,
            .selectedWidget = selectedWidget,
        });
    }


    SelfRet addSegment(TextureA segmentTex, TextureA pressedTex, TextureA selectedTex) {
        return addSegment({
            .segmentTex = segmentTex,
            .pressedTex = pressedTex,
            .selectedTex = selectedTex,
        });
    }

    using WidgetsTuple = std::tuple<WidgetP, WidgetP, WidgetP>;

    //SelfRet addSegment(WidgetsTuple widgets, Size segSize) {
    //    return addSegment(std::get<0>(widgets), std::get<1>(widgets), std::get<2>(widgets), segSize);
    //}

    SelfRet select(int index) {
        selectedSegment_ = index;
        updateScene();
        return this;
    }

    [[nodiscard]] int selectedSegment() const { return selectedSegment_; }


    SelfRet setStateTag(StrView tag, int defaultSegment = 0) {
        stateSegment_.setKey("mangl_widget_segmented_selected:").setSuffix(tag);
        stateDefaultSegment_ = defaultSegment;
        loadSavedState();
        return this;
    }

    SelfRet setSound(int resid) {  soundId_ = resid; return this;  }


protected:
    void onCreate() override;
    void onDestroy() override;
    void onVisible(bool visible) override;
    void onMove() override { updateLayout(); }
    void onAlpha() override;
    void onEnable(bool enabled) override;

    bool onTouchDown(TouchA) override;
    bool onTouchMove(TouchA) override;
    bool onTouchUp(TouchA) override;
    bool onTouchCancel(TouchA) override;


};

using SegmentedP = Segmented*;

}  // namespace mangl

