/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_grid_layout.h"

#include "mangl_label.h"
#include "mangl_button.h"

#include "../mangl_scene.h"

namespace mangl {

GridLayoutP Scene::newGridLayout(GridLayoutParamsA params) {
    return newWidget<GridLayout>(params.flags)->onNew(params)->create();
}


GridLayoutP GridLayout::onNew(GridLayoutParamsA params) {
    params_ = params;

    if (params.rect.w > 0)
        move(params.rect);

    debug_ = params.debug;

    if (debug_) {
        debugPrim_ = scene_->newRect(M::getNextDebugColor(), {});
    }

    return this;
}


GridLayoutP GridLayout::addItem(const GridLayoutItemParam& param) {

    if (param.pos.y >= std::size(rows_)) {
        rows_.resize(param.pos.y + 1);
    }

    if (param.pos.x >= std::size(columns_)) {
        columns_.resize(param.pos.x + 1);
    }

    auto& item = items_.emplace_back();

    item.pos = param.pos;
    item.prim = param.prim;
    item.widget = param.widget;
    item.flags = param.flags;

    item.size = param.size;
    if (!item.size)
        item.size = params_.itemSize;

    item.minSize = param.minSize;
    item.maxSize = param.maxSize;

    if (debug_)
        item.debugPrim = this->scene_->newRect(M::getNextDebugColor(), {});

    updateLayout();

    return this;
}

Size GridLayout::estimateSize() {
    for (auto& column: columns_) {
        column.minWidth = 0;
        column.maxWidth = 10000;
    }

    for (auto& row: rows_) {
        row.minHeight = row.maxHeight = 0;
    }

    for (auto& item: items_) {
        if (item.empty())
            continue;

        auto& column = columns_[item.pos.x];
        auto& row = rows_[item.pos.y];

        auto minSize = item.minSize;
        if (item.size.w != 0)
            minSize.w = item.size.w;
        if (item.size.h != 0)
            minSize.h = item.size.h;

        if (minSize.w == 0) {
            // TODO: mangl - better estimate the width. No by text size height, but width
            // probably find the widest character
            minSize.w = M::Styles::label.text.size;
        }

        if (minSize.h == 0) {
            if (item.prim) {
                minSize.h = M::Styles::label.text.size;
            }
            if (item.widget) {
                switch (item.widget->type) {
                case WidgetType::Label: {
                    auto label = dynamic_cast<LabelP>(item.widget);
                    minSize.h = label->getTextHeight();
                    break;
                }
                case WidgetType::Button: {
                    auto button = dynamic_cast<ButtonP>(item.widget);
                    minSize.h = button->getRect().h;
                    break;
                }
                case WidgetType::Checkbox: {
                    minSize.h = item.widget->getRect().h;
                    break;
                }
                default:
                    break;
                }

            }
        }

        if (column.minWidth < minSize.w)
            column.minWidth = minSize.w;
        if (item.maxSize.w > 0 && column.maxWidth > item.maxSize.w)
            column.maxWidth = item.maxSize.w;

        if (item.size.w > 0 && column.minWidth < item.size.w)
            column.minWidth = item.size.w;


        if (row.minHeight < minSize.h)
            row.minHeight = minSize.h;
        if (item.maxSize.h > 0 && row.maxHeight > item.maxSize.h)
            row.maxHeight = item.maxSize.h;

        if (item.size.h > 0 && row.minHeight < item.size.h)
            row.minHeight = item.size.h;
    }


    // Calculate columns gap
    Cord totalWidth = 0;
    for (auto& col: columns_) {
        totalWidth += col.minWidth;
    }

    Cord totalHeight = 0;
    for (auto& row: rows_) {
        totalHeight += row.minHeight;
    }

    if (columns_.size() > 1) {
        totalWidth += M::Styles::grid.spacing.w * Cord(columns_.size() - 1);
    }

    if (rows_.size() > 1) {
        totalHeight += M::Styles::grid.spacing.h * Cord(rows_.size() - 1);
    }

    return {totalWidth, totalHeight};
}


void GridLayout::updateLayout() {
    if (this->r_.w == 0)
        return;

    if (items_.empty())
        return;

    auto gridSize = estimateSize();

    auto& rect = this->r_;

    Size gap{};

    if (columns_.size() > 1) {
        gap.w = (rect.w - gridSize.w) / Cord(columns_.size());
    }

    if (rows_.size() > 1) {
        gap.h = (rect.h - gridSize.h) / Cord(rows_.size());
    }

    // Calculate column widths
    if (columns_.size() == 1) {
        auto& col = columns_[0];
        col.pos = 0;
        col.width = rect.w;
    } else {
        Cord currentX = 0;
        auto first = true;
        for (auto& col: columns_) {
            if (!first) {
                currentX += M::Styles::grid.spacing.w;
            } else {
                first = false;
            }

            col.pos = currentX;
            col.width = col.minWidth + gap.w;
            currentX += col.width;
        }
    }

    if (rows_.size() == 1) {
        auto& row = rows_[0];
        row.pos = 0;
        row.height = rect.h;
    } else {

        // Flip the rows vertically
        auto first = true;
        Cord currentY = rect.h;
        for (auto& row: rows_) {
            row.height = row.minHeight + gap.h;

            if (!first) {
                currentY -= M::Styles::grid.spacing.h;
            } else {
                first = false;
            }

            row.pos = currentY - row.height;
            currentY = row.pos; // + row.height;
//            currentY -= Theme::grid.spacing.h;
        }

    }


    //
    // Now layout all the elements
    //
    for (auto& item: items_) {
        if (item.empty())
            continue;
        auto& col = columns_[item.pos.x];
        auto& row = rows_[item.pos.y];

        Rect itemRect{}; //{col.pos,
        if (item.size.w > 0) {
            itemRect.w = item.size.w;
            if (item.flags & M::AlignCenter) {
                itemRect.x = col.pos + (col.width - itemRect.w)/2;
            } else if (item.flags & M::AlignRight) {
                itemRect.x = col.pos + (col.width - itemRect.w);
            } else if (item.flags & M::AlignLeft) {
                itemRect.x = col.pos;
            } else {
                itemRect.x = col.pos + (col.width - itemRect.w)/2;
            }

        } else {
            itemRect.w = col.width;
            itemRect.x = col.pos;
        }

        if (item.size.h > 0) {
            itemRect.h = item.size.h;
            if (item.flags & M::AlignVCenter) {
                itemRect.y = (row.height - itemRect.h)/2;
            } else if (item.flags & M::AlignTop) {
                itemRect.y = (row.height - itemRect.h);
            } else {
                itemRect.y = row.pos;
            }
        } else {
            itemRect.h = row.height;
            itemRect.y = row.pos;
        }

        itemRect.x += rect.x;
        itemRect.y += rect.y;

        if (item.prim)
            item.prim->move(itemRect);
        if (item.widget)
            item.widget->move(itemRect);

        if (item.debugPrim)
            item.debugPrim->move(itemRect);
    }


    if (debugPrim_)
        debugPrim_->move(this->r_);

}

void GridLayout::updateVisible() {
    for (auto& item: items_) {
        if (item.prim) item.prim->setVisible(this->visible_);
        if (item.widget) item.widget->setVisible(this->visible_);
        if (item.debugPrim) item.debugPrim->setVisible(this->visible_);
    }

    if (debugPrim_) {
        debugPrim_->setVisible(this->visible_);
    }
}


void GridLayout::onSetZIndexFront() {
    for (auto& item: items_) {
        if (item.prim)
            this->scene_->moveToFront(item.prim);
        if (item.widget)
            this->scene_->setZIndexFront(item.widget);
        if (item.debugPrim)
            this->scene_->moveToFront(item.debugPrim);
    }

    if (debugPrim_)
        this->scene_->moveToFront(debugPrim_);
}



}  // namespace mangl

