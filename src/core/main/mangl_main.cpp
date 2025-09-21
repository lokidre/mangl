/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_main.h"

namespace mangl {

void mangl_main() {
    massert(mangl_instance_main_, "Main<T> is not instantiated");

    Framework::instance_ = mangl_instance_main_->onInstance();
    
    Framework::mainThreadId = M::getCurrentThreadId();

    massert(Framework::instance_, "Main<T> is not instantiated");
}

//Framework* mangl_framework_{};
MainBase* mangl_instance_main_{};

}  // namespace mangl

