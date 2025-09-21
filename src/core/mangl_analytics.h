/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_types.h"

#include <apeal/singleton.hpp>

namespace mangl {

struct Analytics: public apl::Singleton<Analytics> {
    using Params = std::unordered_map<StrView, StrView>;

#if APEAL_OS_IPHONE && MANGL_FIREBASE
    static void log(StrView name, const Params &params);
    static void levelStart(StrView name);
    static void levelEnd(StrView name, bool success);
#else
    static void log(StrView name, const Params &params){}
    static void levelStart(StrView name){}
    static void levelEnd(StrView name, bool success){}
#endif
    
    template <class ...Args>
    static void logParam(StrView name, StrView param, StrView fmt, Args&&... args) {
        auto val = apl::fmt(fmt, std::move(args)...);
        log(name, {{param, val}});
    }

    static void log(StrView name) { log(name, {}); }
};

}  // namespace mangl

#if _MSC_VER
#define MAnalyticsException(...) Analytics::log("exception", {{"what", ::apl::fmt(__VA_ARGS__)}, {"func", __FUNCTION__}})
#else
#define MAnalyticsException(...) Analytics::log("exception", {{"what", ::apl::fmt(__VA_ARGS__)}, {"func", __PRETTY_FUNCTION__}})
#endif
