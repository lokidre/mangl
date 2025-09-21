/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_framework.h"

namespace mangl {

class MainBase {
public:
    virtual ~MainBase() = default;
    virtual Framework *onInstance() { return nullptr; };
};

extern MainBase *mangl_instance_main_;

template <class F>
class Main: public MainBase {
public:
    Main() {
        mangl_instance_main_ = this;
    }

    Framework *onInstance() override { return new F; }
};

void mangl_main();

}  // namespace mangl

