/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "test_types.h"

#ifndef MANGL_CONSOLE

#include <mangl/app.h>

namespace sample {

class App: public AppBase<App> {
public:
    App();

private:
    void initStyles();

protected:
    void onInit() override;
    void onStart() override;
};

}  // namespace sample

#endif

