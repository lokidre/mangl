/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_stack_widget.h"

#include "../mangl_framework.h"
#include "../mangl_scene.h"
#include "../mangl_util.h"

namespace mangl {

StackWidgetP Scene::newStackWidget(StackWidgetParamsA params) {
    return newWidget<StackWidget>(params.flags)->onNew(params)->create();
}

StackWidgetP StackWidget::onNew(StackWidgetParamsA params) {
    type = WidgetType::Stack;

    setVisible(!params.hidden);
    move(params.rect);

    return this;
}


WidgetItem& StackWidget::addItem(PrimP prim, WidgetP widget) {
    // avoid duplicates
    for (auto& item: childItems_) {
        if (prim && item.prim == prim)
            return item;
        if (widget && item.widget == widget)
            return item;
    }

    // PrimP lastPrim{};
    // WidgetP lastWidget{};
    // if (!childItems_.empty()) {
    //     lastPrim = childItems_.back().prim;
    //     lastWidget = childItems_.back().widget;
    // }


    auto& item = childItems_.emplace_back();
    item.id = M::generateId<WidgetId>();
    item.prim = prim;
    item.widget = widget;

    // auto scene = Framework::getScene();
    // if (lastPrim) {
    //     scene->moveToFront()
    //
    // }

    return item;
}


WidgetId StackWidget::addPrim(PrimP prim, WidgetLayoutParamsA layout) {
    if (!prim)
        return 0;

    auto& item = addItem(prim, nullptr);

    item.layout.setParams(layout);

    updateContents();

    return item.id;
}

//auto StackWidget::addPrim(PrimP prim, RectA rect) -> Id {
//    auto& item = insertItem(prim, nullptr);
//
//    item.contentsFrame = rect;
//    if (item.contentsFrame.w == 0)
//        item.stretchWidth = true;
//    if (item.contentsFrame.h == 0)
//        item.stretchHeight = true;
//
//    updateContents();
//
//    return item.id;
//}

WidgetId StackWidget::addWidget(WidgetP widget, WidgetLayoutParamsA layout) {
    if (!widget)
        return 0;

    auto& item = addItem(nullptr, widget);

    item.layout.setParams(layout);

    updateContents();

    return item.id;
}


void StackWidget::onCreate(SceneP scene) {
    for (auto& item: childItems_) {
        if (item.widget) item.widget->create();
    }

    updateContents();
}


void StackWidget::updateLayout(RectA rect) {
    for (auto& item: childItems_) {
        auto f = item.layout.calcRect(rect, orientation_);

        if (item.prim) item.prim->move(f);
        if (item.widget) item.widget->move(f);
    }
}


void StackWidget::updateContents() {
    updateLayout(getRect());
    //updateVisible();
}

void StackWidget::onOrientation(M::Orientation orientation) {
    for (auto& item: childItems_) {
        if (item.prim) item.prim->setOrientation(orientation);
        if (item.widget) item.widget->setOrientationBase(orientation);
    }

    updateLayout(getRect());
}


} // namespace mangl
