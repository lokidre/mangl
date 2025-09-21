/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "main_types.hpp"

#include <apeal/singleton.hpp>

struct ScreenLayoutState {
    mangl::M::Orientation orientation{mangl::M::Orientation0};
    Real zoom{1};
};

struct WindowLayoutState {
    //Rect rect;
    SizeI size{1200, 800};
    //Size size;
};

struct ProjectFileState {
    String title, name, path;
};

using ProjectFilesContainer = std::vector<ProjectFileState>;

struct ProjectCategoryState {
    int selectedIndex{-1};
    String currentDirectory;
    ProjectFilesContainer files;
};


enum class ProjectCategory : int {
    Project = 0, Screen, Animation, Background, Model, Count,
};

struct ProjectFilesState {
    ProjectCategory selectedCat{};
    std::array<ProjectCategoryState, (int)ProjectCategory::Count> fileContainers{};
};

struct ModelViewState {
    //uint32_t ambientColor{M::Black}, diffuseColor{M::White}, specularColor{M::Silver};
    uint32_t ambientColor{M::Black}, diffuseColor{M::Red}, specularColor{M::White};
    RealGL shininess{32};
    bool wireframe{false}, vertices{false};
    bool faceCulling{false};
    bool depthTest{true};
    bool lighting{true};
    RealGL scale{0.8};

};

struct State: public apl::Singleton<State> {
    ScreenLayoutState screenLayout;
    WindowLayoutState windowLayout;
    ProjectFilesState projectFiles;
    ModelViewState model;

    int showBackgroundIndex{-1};

    String cacheDir{};
    String windowLayoutFilename{};
    String projectFilesFilename{};
    String modelFilename{};
    String screenLayoutFilename{};

    Real checkboardSize = 16;
    Size screenSize{320, 568};

    void load() {
    }
};
