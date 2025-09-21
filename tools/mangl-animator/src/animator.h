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

#include "storage.h"

#include <apeal/singleton.hpp>
#include <apeal/parse/preprocessor.hpp>

APEAL_QT_HEADERS_BEGIN
#include <QFileSystemWatcher>
APEAL_QT_HEADERS_END


namespace animator {

class MainWindow;

enum SelectListTab {
    TabIndexBackground = 0,
    TabIndexBlueprint,
    TabIndexAnimation,
    TabIndexModel,
    TabsCount,
};


struct ListItemInfo {
    String title, file, texture;
    String textureDir;
};

struct ItemList {
    std::vector<ListItemInfo> items;
    String currentFilename;
    apl::Preprocessor::Deps deps;
};

class Animator: public apl::Singleton<Animator> {
public:
    ItemList items_[TabsCount];

    String lastOpenDirectory_;

    Settings* settings{Settings::instance()};


public:
    Animator();

    void saveState();
    void loadState();

    //void load(int tab, StrView filename);
    bool appendFile(int tab, StrView filename);
    void unload(int tab, int idx) { 
        items_[tab].items.erase(items_[tab].items.begin() + idx); 
    }
    //void start(int tab, StrView filename);
    void start(int tabIndex, int itemIndex);

    bool isItemSelected() const noexcept { return currentTab_ >= 0 && currentItemIndex_ >= 0; }
    auto& getCurrentItem() noexcept { return items_[currentTab_].items[currentItemIndex_]; }

private:
    String stateFilename_;


public:
    MainWindow* mainWindow() { return mainWindow_; }
    void setMainWindow(MainWindow* m) { mainWindow_ = m; }

    int currentItemIndex_{};

    int currentTab_{};
    void onTabChanged(int tab) { currentTab_ = tab; }
    int currentTab() const { return currentTab_; }


private:
    MainWindow* mainWindow_;

    //void onFileChanged();

};

} // namespace animator
