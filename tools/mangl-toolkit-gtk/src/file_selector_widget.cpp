/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "file_selector_widget.h"

#include "main_application.hpp"

#include "main_data.h"
#include "pubsub.hpp"

namespace toolkit {

void FileSelectorWidget::create() {
    auto state = State::instance();

    filesBox_.create();
    filesBox_.setSizeRequest(400, 400);

    projectTabs_.create()->setVExpand();
    projectTabs_.blockSignals();
    filesBox_.append(&projectTabs_);


    for (int tabIndex = 0; tabIndex < std::size(projectCollections_); ++tabIndex) {
        auto& tabInfo = projectCollections_[tabIndex];

        auto& listView = projectTabListViews_[tabIndex];
        listView.create();


        listView.blockSignals();
        auto& cont = state->projectFiles.fileContainers[tabIndex];

        if (!cont.files.empty()) {
            for (auto& file: cont.files) {
                listView.addItem(file.name);
            }

            if (cont.selectedIndex >= 0 && cont.selectedIndex < (int)cont.files.size()) {
                listView.selectItem(cont.selectedIndex);
            }
        }

        listView.unblockSignals();

        listView.itemSelected = [](int index) {
            auto state = State::instance();
            auto& projectFiles = state->projectFiles;
            auto& collection = projectFiles.fileContainers[(int)projectFiles.selectedCat];
            collection.selectedIndex = index;
            Pubsub::instance()->projectFileSelected();
        };


        projectTabs_.addTab(tabInfo.title, &listView);
    }

    auto currentTab = static_cast<int>(state->projectFiles.selectedCat);
    projectTabs_.setCurrentTab(currentTab);


    projectTabs_.unblockSignals();

    projectTabs_.tabSelected = [this](int index) {
        auto& projectFiles = State::instance()->projectFiles;
        projectFiles.selectedCat = (ProjectCategory)index;
        actionButtonRemove_.setEnabled(!projectFiles.fileContainers[index].files.empty());
        Pubsub::instance()->projectFileSelected();
    };


    filesBox_.append(actionBar_.create());

    actionButtonRemove_.create();
    actionBar_.packEnd(&actionButtonRemove_);

    actionButtonRemove_.setEnabled(!state->projectFiles.fileContainers[currentTab].files.empty());

    actionButtonRemove_.clicked = [this]() {
        auto state = State::instance();
        auto& projectFiles = state->projectFiles;
        auto tabIndex = static_cast<int>(projectFiles.selectedCat);

        auto& currentCollection = projectFiles.fileContainers[tabIndex];
        auto idx = currentCollection.selectedIndex;

        if (idx >= 0 && idx < (int)currentCollection.files.size()) {
            currentCollection.files.erase(currentCollection.files.begin() + idx);

            auto& listView = projectTabListViews_[tabIndex];
            listView.blockSignals();


            if (idx > 0) {
                --idx;
            }

            listView.clearItems();
            for (auto& file: currentCollection.files) {
                listView.addItem(file.name);
            }
            listView.unblockSignals();

            listView.selectItem(idx);
        }

    };



    fileAddDialog_.setSelectMultiple();
    //fileAddDialog_.addFilterSuffix("obj");
    fileAddDialog_.addFilterSuffixes({"obj", "ini", "yaml"});

    fileAddDialog_.responseSignal = [this](const gw::FileChooserResult& result) {
        auto state = State::instance();
        auto& projectFiles = state->projectFiles;

        auto tabIndex = static_cast<int>(projectFiles.selectedCat);
        auto& tabInfo = projectCollections_[tabIndex];
        auto& listView = projectTabListViews_[tabIndex];
        listView.blockSignals();

        auto& currentCollection = projectFiles.fileContainers[tabIndex];
        currentCollection.currentDirectory = result.currentDirectory;

        for (auto& file: result.files) {
            ProjectFileState fs;
            fs.title = file.baseName;
            fs.name = file.baseName;
            fs.path = file.path;

            currentCollection.files.emplace_back(fs);
        }

        listView.clearItems();

        for (auto& file: currentCollection.files) {
            listView.addItem(file.name);
        }

        listView.unblockSignals();

        currentCollection.selectedIndex = (int)(currentCollection.files.size()-1);
        listView.selectItem(currentCollection.selectedIndex);

        Pubsub::instance()->projectFileSelected();
    };

    actionButtonAdd_.create();
    actionButtonAdd_.clicked = [this] {
        auto state = State::instance();
        auto& projectFiles = state->projectFiles;
        auto tabIndex = static_cast<int>(projectFiles.selectedCat);
        auto& currentCollection = projectFiles.fileContainers[tabIndex];
        auto currentDirectory = currentCollection.currentDirectory;
        if (currentDirectory.empty()) {
            currentDirectory = apl::FS::getCurrentDirectory();
        }

        fileAddDialog_.setCurrentDirectory(currentDirectory);

        fileAddDialog_.show(&(MainApplication::instance()->mainWindow));
    };
    actionBar_.packEnd(&actionButtonAdd_);


}

}  // namespace toolkit
