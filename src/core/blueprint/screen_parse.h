/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once 

#include <mangl/mangl.h>

namespace mangl {

enum class CoordUnit {
    Point, Percent,
};  


enum class BorderStyle {
    Solid,
};

struct CoordMajor {
    RealGL value{};
    CoordUnit units{CoordUnit::Point};
    RealGL max{};
};


struct ViewPort {
    RealGL x{};
    RealGL y{};
    RealGL width{};
    RealGL height{};
};

struct Border {
    RealGL size;
    Color color;
    BorderStyle style;
};

struct ComponentLayout {
    CoordMajor left;
    CoordMajor right;
    CoordMajor top;
    CoordMajor bottom;
    CoordMajor width;
    CoordMajor height;
    int alignHorz{};
    int alignVert{};
    RealGL x{};
    RealGL y{};
};







//Project loadProject(String filename);
//
//CoordMajor parseCoords(String str, RealGL max);
//
//Color parseColor(String color);
//
//std::set<String> loadFontNames(const Project& project);
//
////String getText(const Project& project);
//
////std::set<Font> loadFonts(const TextureAtlas& gameTex, const Project& project);
//
//Rect calcCoords(const Component& elem, const Project& project, const Screen& screen);
//
//Point calcPointCoords(const Component& elem, const Project& project, const Screen& screen);
//
//void createScreenComponents(const Project& project, Index screenIndex, const TextureAtlas& gameTex);
//
////void createRect(const Project& project);
//
////void createLabel(const Project& project);
//
//void parseHorzAlign(StrView value, int& alignHorz);
//
//void parseVertAlign(StrView value, int& alignVert);
}

