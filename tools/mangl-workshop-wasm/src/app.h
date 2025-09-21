/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <mangl/app.h>

using namespace mangl;

class App: public AppBase<App> {
private:
    void setupLayout();

protected:
    void onInit() override;
    void onStart() override;
};

//inline Size sampleApplicationSize_ = {414, 896};
inline Size sampleApplicationSize_ = {320, 480};
inline Padding sampleScreenPadding_ = {44, 0, 34, 0};
inline Size sampleApplicationLayout_ = {320, 0};
inline InterfaceIdiom sampleInterfaceIdiom_ = InterfaceIdiom::Phone;

