/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_table_widget.h"

#include <numeric>

namespace mangl {

void TableWidget::onCreate() {
    apl::verify(model_, "TableWidget::model is not set");

    colsCount_ = model_->columnCount();
    rowsCount_ = model_->rowCount();

    if (0 == colsCount_ || 0 == rowsCount_)
        return;

    data_.resize(rowsCount_);

    columns_.resize(colsCount_);

    int rowIndex = 0;
    for (auto& row: data_) {
        row.cells.resize(colsCount_);
        row.row_index = rowIndex;
        int colIndex = 0;
        for (auto& cell: row.cells) {
            cell.col = colIndex, cell.row = rowIndex;
            model_->onCreateCell(cell);
            model_->onUpdateData(cell);
            ++colIndex;
        }
        ++rowIndex;
    }

    separators_.resize(2 + 2 + rowsCount_ + colsCount_);

    int index = 0;

    for (auto& sep: separators_) {
        if (index < 2) {
            sep.type = TableSeparatorInfo::Type::HorzBorder;
            sep.index = index;
        } else if (index < 2 + 2) {
            sep.type = TableSeparatorInfo::Type::VertBorder;
            sep.index = index - 2;
        } else if (index < 2 + 2 + colsCount_) {
            sep.type = TableSeparatorInfo::Type::VertSep;
            sep.index = index - 4;
        } else {
            sep.type = TableSeparatorInfo::Type::HorzSep;
            sep.index = index - 4 - colsCount_;
        }
        sep.prim = nullptr;
        model_->onCreateSeparator(sep);
        ++index;
    }

    updateTable();
}


TableWidget* TableWidget::updateData() {
    for (auto& row: data_)
        for (auto& cell: row.cells)
            model_->onUpdateData(cell);

    updateTable();
    return this;
}


void TableWidget::updateTable() {
    if (!created_)
        return;

    for (auto& row: data_)
        for (auto& cell: row.cells)
            if (cell.widget) cell.widget->hide();

    for (auto& sep: separators_)
        if (sep.prim) sep.prim->hide();

    if (!visible_)
        return;


    auto currentY = r_.top();
    Cord currentX{};

    auto& topBorder = separators_[0];
    auto& bottomBorder = separators_[1];
    auto& leftBorder = separators_[2];
    auto& rightBorder = separators_[3];
    auto vertSepBegin = separators_.begin() + 4;
    auto vertSepEnd = vertSepBegin + colsCount_;
    auto horzSepBegin = vertSepEnd;


    auto renderHorzSeparator = [this](auto& sep, Real y) {
        if (auto prim = sep.prim) {
            if (PrimType::Line == prim->type)
                prim->setLine({r_.x, y + sep.width / 2}, {r_.right(), y + sep.width / 2})->setWidth(sep.width);
            else
                prim->move({r_.x, y, r_.w, sep.width});

            prim->unhide();
        }
    };

    auto renderVertSeparator = [this](auto& sep, Real x, Real y) {
        auto prim = sep.prim;
        if (prim) {
            if (PrimType::Line == prim->type)
                prim->setLine({x + sep.width / 2, y}, {x + sep.width / 2, r_.top()})->setWidth(sep.width);
            else
                prim->move({x, y, sep.width, r_.top() - y});

            prim->unhide();
        }
    };


    if (topBorder.width && topBorder.prim) {
        currentY -= topBorder.width;
        renderHorzSeparator(topBorder, currentY);
    }

    // estimate the columns screenLayout
    auto totalSeparatorWidth = std::accumulate(
        vertSepBegin, vertSepEnd, leftBorder.width + rightBorder.width,
        [](auto w, auto& sep) { return w + sep.width; }
    );

    auto colWidth = (r_.w - totalSeparatorWidth) / colsCount_;
    for (auto& column: columns_) {
        column.width = colWidth;
    }

    bool tableInterrupted{};
    int rowsRendered = 0;
    //Cord lastCurrentY = currentY;
    bool firstRow = true;

//    Cord bottomBorderWidth = bottomBorder.width;

    for (auto& row: data_) {
        currentX = r_.x;
        currentX += leftBorder.width;
        int cellIndex = 0;

        // Calculate the row height
        Cord maxCellHeight{};
        for (auto& cell: row.cells)
            if (cell.widget && cell.height > maxCellHeight)
                maxCellHeight = cell.height + cell.padding.vert2();


        if (currentY - maxCellHeight - bottomBorder.width < r_.y) {
            break;
        }

        // Separator from the previous row
        if (!firstRow) {
            auto& horzSep = *(horzSepBegin + rowsRendered);
            if (horzSep.width > 0 && horzSep.prim) {
                currentY -= horzSep.width;
                renderHorzSeparator(horzSep, currentY);
            }
        }

        currentY -= maxCellHeight;


        for (auto& cell: row.cells) {
            if (!cell.widget)
                continue;

            auto& column = columns_[cellIndex];

            cell.widget->move(
                {currentX + cell.padding.left(), currentY + cell.padding.bottom(),
                 column.width - cell.padding.horz2(), cell.height}
            );
            cell.widget->setVisible(true);

            currentX += column.width;
            cellIndex++;
        }

        rowsRendered++;
        firstRow = false;
    }

    if (!tableInterrupted && bottomBorder.width && bottomBorder.prim) {
        currentY -= bottomBorder.width;
        renderHorzSeparator(bottomBorder, currentY);
    }

    // Render vertical separators and borders
    {
        currentX = r_.x;
        if (leftBorder.width) {
            renderVertSeparator(leftBorder, currentX, currentY);
            currentX += leftBorder.width;
        }

        auto sepIt = vertSepBegin;
        for (int cindex = 0; cindex < colsCount_ - 1; ++cindex, ++sepIt) {
            currentX += columns_[cindex].width;
            if (sepIt->width) {
                renderVertSeparator(*sepIt, currentX, currentY);
                currentX += sepIt->width;
            }
        }

        if (rightBorder.width)
            renderVertSeparator(rightBorder, r_.right() - rightBorder.width, currentY);
    }
}

} // namespace mangl
