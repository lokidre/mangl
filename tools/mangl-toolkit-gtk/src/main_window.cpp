/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "main_window.h"
#include "pubsub.hpp"

#include "../src/generated/window_layout_state_pack.hpp"
#include "../src/generated/project_files_state_pack.hpp"
#include "../src/generated/model_view_state_pack.hpp"
#include "../src/generated/screen_layout_state_pack.hpp"

#include <apeal/parse/yaml.hpp>
#include <apeal/file.hpp>

namespace toolkit {

void MainWindow::onCreate() {
    using namespace apl;

    setName("main_window");

    //    auto header = gtk_header_bar_new();
    //    gtk_widget_set_name(GTK_WIDGET(header), "main_titlebar");
    //    gtk_window_set_titlebar(gwindow_, header);


    setTitle("Mangl Toolkit");
    // setDetaultSize(1200, 800);

    auto state = State::instance();

    // auto cacheDir = apl::FileSystem::getHomeDir()+"/.cache" + "/mangl_toolkit";
    // state->cacheDir = cacheDir;
    state->cacheDir = makeFilePath(FS::getHomeDir(), ".cache", "mangl-toolkit");

    state->windowLayoutFilename = makeFilePath(state->cacheDir, "window-layout.yaml");
    state->projectFilesFilename = makeFilePath(state->cacheDir, "project-files.yaml");
    state->modelFilename = makeFilePath(state->cacheDir, "model-state.yaml");
    state->screenLayoutFilename = makeFilePath(state->cacheDir, "screen-layout.yaml");

    FS::mkdirIfNotExistsParents(state->cacheDir);

    unpackFileIfExistsYaml(state->windowLayoutFilename, &state->windowLayout);

    unpackFileIfExistsYaml(state->projectFilesFilename, &state->projectFiles);

    unpackFileIfExistsYaml(state->modelFilename, &state->model);

    unpackFileIfExistsYaml(state->screenLayoutFilename, &state->screenLayout);


    setDetaultSize(state->windowLayout.size.w, state->windowLayout.size.h);

    mainLayout_.create();
    setChild(&mainLayout_);

    createMainMenu();
    createCentralWidget();
    // createStatusbar();


    preset();
}

void MainWindow::onDestroy() {
    auto defaultSize = getDefaultSize();

    auto state = State::instance();
    state->windowLayout.size.w = defaultSize.w;
    state->windowLayout.size.h = defaultSize.h;

    packFileYaml(state->screenLayoutFilename, state->screenLayout);
    packFileYaml(state->windowLayoutFilename, state->windowLayout);
    packFileYaml(state->projectFilesFilename, state->projectFiles);
    packFileYaml(state->modelFilename, state->model);

    Pubsub::instance()->mainWindowDestroy();
}

}  // namespace toolkit
