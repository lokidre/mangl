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
#include "mangl_label.h"

namespace mangl {

struct TableWidgetCellInfo {
    WidgetP widget{};
    Padding padding{};
    Cord height{};
    int col{}, row{};
};

struct TableSeparatorInfo {
    enum class Type {
        HorzBorder, VertBorder, HorzSep, VertSep,
    };

    Type type{};
    int index{};
    Cord width{};
    PrimP prim{};

    bool isBorderHorz() const noexcept { return Type::HorzBorder == type; }
    bool isBorderVert() const noexcept { return Type::VertBorder == type; }
    bool isSeparatorHorz() const noexcept { return Type::HorzSep == type; }
    bool isSeparatorVert() const noexcept { return Type::VertSep == type; }
};

struct TableWidgetModel {
    friend class TableWidget;

    virtual ~TableWidgetModel(){}

protected:
    virtual int columnCount() = 0;
    virtual int rowCount() = 0;
    virtual void onCreateSeparator(TableSeparatorInfo &sep) {}
    virtual void onCreateCell(TableWidgetCellInfo &cell) = 0;
    virtual void onUpdateData(TableWidgetCellInfo &cell) = 0;
};


class TableWidget: public WidgetTemplate<TableWidget> {
public:
    typedef std::vector<TableWidgetCellInfo> cell_list_type;

    struct RowInfo {
        int row_index = -1;
        cell_list_type cells;
    };

    typedef std::vector<RowInfo> row_list_type;

    struct ColumnInfo {
        Cord width{};
        int alignment{-1};
    };

    typedef std::vector<ColumnInfo> column_list_type;
    typedef std::vector<TableSeparatorInfo> separator_list_type;


    TableWidgetP setModel(TableWidgetModel *model) { 
        model_ = model;  
        return this; 
    }

    TableWidget *updateData();


private:
    TableWidgetModel *model_{};

    int colsCount_{}, rowsCount_{};

    void updateTable();

    row_list_type data_;
    column_list_type columns_;
    separator_list_type separators_;


protected:
    void onCreate() override;
    void onMove() override { updateTable(); }
    void onVisible(bool visible) override { updateTable(); }
};

}  // namespace mangl
