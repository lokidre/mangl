/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <main_types.hpp>

#include <apeal/singleton.hpp>
#include <apeal/signal.hpp>

namespace toolkit {

class Pubsub: public apl::Singleton<Pubsub> {
public:
    SlotVoid projectFileSelected;
    SlotVoid mainWindowDestroy;
    SlotVoid viewOptionsChanged;
};

}  // namespace toolkit
