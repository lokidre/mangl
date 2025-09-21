/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "animator.h"

APEAL_QT_HEADERS_BEGIN
#include <QDir>
APEAL_QT_HEADERS_END

#include "animation_screen.h"
#include "main.h"
#include "main_window.h"
#include "app.h"
#include "storage.h"

#include <apeal/file.hpp>
#include <apeal/parse/ini.hpp>

namespace animator {

Animator::Animator()
{
    using namespace apl;

    lastOpenDirectory_ = settings->lastOpenDir(fromQt(QDir::currentPath()));
    stateFilename_ = makeFilePath(Env::instance()->libDir, "animator_state.ini");
    loadState();
}


void Animator::saveState()
{
    std::ofstream f(stateFilename_, std::ios_base::out | std::ios_base::binary);

    f << "[ANIMATOR-STATE]" << '\n';
    f << '\n';

    for (auto& it: items_[TabIndexBackground].items) {
        f << "[BACKGROUND]" << '\n';
        f << "file=" << it.file << '\n';
        f << '\n';
    }

    for (auto& it: items_[TabIndexAnimation].items) {
        f << "[ANIMATION]" << '\n';
        f << "file=" << it.file << '\n';
        f << '\n';
    }

    for (auto& it: items_[TabIndexBlueprint].items) {
        f << "[LAYOUT]" << '\n';
        f << "file=" << it.file << '\n';
        f << "texdir=" << it.textureDir << '\n';
        f << '\n';
    }

    for (auto& it : items_[TabIndexModel].items) {
        f << "[MODEL]" << '\n';
        f << "file=" << it.file << '\n';
        f << '\n';
    }
}


void Animator::loadState()
{
    using namespace apl;
    using namespace apl::literals;

    try {
        auto ini = loadIni(stateFilename_);

        for (auto& sec: ini.sec) {
            int tab = -1;
            if ("BACKGROUND" == sec.name)	tab = TabIndexBackground;
            else if ("ANIMATION" == sec.name)	tab = TabIndexAnimation;
            else if ("LAYOUT" == sec.name)	tab = TabIndexBlueprint;
            else if ("MODEL" == sec.name)	tab = TabIndexModel;

            if (tab < 0)
                continue;

            String file, texDir;

            for (auto&& el: sec.el) {
                switch (pshHash(el.name)) {
                case "file"_psh: file = el.value; break;
                case "texdir"_psh: texDir = el.value; break;
                }
            }

            if (!file.empty() && appendFile(tab, file)) {
                auto& bi = items_[tab].items.back();
                bi.textureDir = texDir;
            }
        }
    }
    catch (...) {
    }
}


bool Animator::appendFile(int tab, StrView file)
{
    // check if the file is already loaded
    for (auto& bi: items_[tab].items) {
        if (bi.file == file)
            return false;
    }

    auto& bi = items_[tab].items.emplace_back();

    bi.file = file;
    bi.title = apl::extractFileName(file);

    return true;
}

void Animator::start(int tabIndex, int itemIndex) //StrView filename)
{
    using namespace apl;

    currentTab_ = tabIndex;
    currentItemIndex_ = itemIndex;

    auto& items = items_[tabIndex];
    auto& item = items.items[itemIndex];

    auto& filename = item.file;

    auto watcher = mainWindow_->fsWatcher_;


    if (!items.currentFilename.empty())
        watcher->removePath(toQt(items.currentFilename));

    for (auto& dep: items.deps)
        watcher->removePath(toQt(dep.filepath));

    auto screen = App::instance()->animatorScreen;

    auto [fn, ext] = splitFileExt(filename);

    // run a preprocessor, to check the dependencies
    if (ext == "ini") {
        Preprocessor preprocessor;
        auto filedir = extractDirName(filename);
        preprocessor.search_dirs.push_back(String{filedir});

        String preprocessed;
        preprocessor.load(filename, preprocessed);
        items.deps = preprocessor.deps;


        watcher->addPath(toQt(filename));
        for (auto& dep: items.deps)
            watcher->addPath(toQt(dep.filepath));
    }
        
    if (ext == "yaml") {
        auto& deps = screen->blueprintEngine.loadDeps(filename);

        items.deps.clear();
        for (auto& dep: deps) {
            auto &d = items.deps.emplace_back();
            d.filepath = dep.second->path;
            d.line = dep.second->line;
        }

        //watcher->addPath(toQt(filename));
        for (auto& dep: items.deps)
            watcher->addPath(toQt(dep.filepath));
    }

    items.currentFilename = filename;

    screen->switchScreen(tabIndex, filename);

}

}  // namespace animator
