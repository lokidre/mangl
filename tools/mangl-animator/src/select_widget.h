/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "animator_types.h"

#pragma warning(push, 0)
#include <QListView>
#include <QPushButton>
#include <QTabBar>
#pragma warning(pop)

#include "animator.h"
#include "storage.h"


namespace animator {

class SelectListDataModel;


class SelectListWidget: public QListView {
    using Base = QListView;
    using Self = SelectListWidget;
};


class SelectWidget: public QWidget {
    using Base = QWidget;
    using Self = SelectWidget;

public:
    SelectWidget(QWidget* parent = 0);

    void updateSetup();
    void updateContents();
    void setup();

    int currentTab() { return tabbar_->currentIndex(); }

private:
    Settings *settings{Settings::instance()};

    SelectListDataModel* dataModel_{};
    SelectListWidget* listWidget_{};

    QPushButton* addButton_, * removeButton_, * clearButton_;
    QTabBar* tabbar_;

    Animator* animator_;

    void onAddButton();
    void onRemoveButton();
    //void onClearButton() ;
    void onEditButton();

    void onAnimationClicked(const QModelIndex&);
    void onAnimationDoubleClicked(const QModelIndex&);

    void onTabbarCurrentChanged(int);

    auto& getItems() { return animator_->items_[currentTab()].items; }
};


class SelectListDataModel: public QAbstractTableModel {
    using Base = QAbstractTableModel;

    Animator const* animator_{Animator::instance()};
    SelectWidget* selectWidget_;
    
    auto& getItems() const noexcept {
        return animator_->items_[selectWidget_->currentTab()].items;
    }

public:
    SelectListDataModel(SelectWidget* selectWidget): selectWidget_{selectWidget}{}
    
    void beginUpdates() { beginResetModel(); }
    void endUpdates() { endResetModel(); }


protected:
    int rowCount(const QModelIndex& parent) const override {
        return (int)getItems().size();
    }

    int columnCount(const QModelIndex& parent) const override {
        if (parent.isValid())
            return 0;
        return 1;
    }

    Qt::ItemFlags flags(const QModelIndex& index) const override {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }


    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        int row = index.row();

        if (Qt::DisplayRole == role)
            return toQt(getItems()[row].title);

        //if ( Qt::ForegroundRole == role ) {
        //	if ( row == 0 || row == 2 || row == levels_title_row ) {
        //		v = QBrush(QColor(0,0,255)) ;
        //	}
        //}

        //if ( Qt::BackgroundRole == role ) {
        //	if ( row == 0 || row == 2 || row == levels_title_row ) {
        //		v = QBrush(QColor(200,200,200)) ;
        //	}
        //}


        if (Qt::TextAlignmentRole == role)
            return (int)(Qt::AlignLeft | Qt::AlignVCenter);

        return {};
    }

};

}  // namespace animator

