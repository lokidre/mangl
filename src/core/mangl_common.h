/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

// Common classes

#include "mangl_types.h"
#include "mangl_color.h"
#include "mangl_matrix.h"
#include "mangl_schedule_trigger.h"

#include "mangl_touches.h"
#include "mangl_time.h"

#include "mangl_error.h"
#include "mangl_debug.h"
#include "main/mangl_thread.h"

#include "mangl_scene_decl.h"
#include "mangl_widgets_decl.h"



/*
namespace mangl {

class FrameworkTimerProcessor {
protected:
    FrameworkTimerProcessor() noexcept;

public:
    virtual ~FrameworkTimerProcessor() noexcept;

public:
    virtual void onTimerPrefix(Timestamp){};
    virtual void onTimerPostfix(Timestamp){};
};



}  // namespace mangl

*/
