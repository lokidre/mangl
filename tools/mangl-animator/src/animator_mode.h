/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "animator_types.h"

#pragma warning(push, 0)
#include <QAction>
#include <QString>
#include <QSettings>
#pragma warning(pop)

#include <apeal/singleton.hpp>

namespace animator {

struct AnimatorMode: public apl::Singleton<AnimatorMode, false> {

    enum class ResolutionFamily {
        Square,
        ThreeTwo,
        Banner,
        Phone,
        Pad,
        Desktop,
    };

    ResolutionFamily currentFamily{ResolutionFamily::Phone};

    std::map<ResolutionFamily, String> resolutionFamilies;

    struct ResolutionInfo {
        ResolutionFamily family;
        String title;
        Size layout;
        int upscale;
        Padding padding;
        QAction* action;
    };
    String defaultResolution;
    std::vector<ResolutionInfo> resolutionInfos;

    struct ZoomInfo {
        int zoom;
        QAction* action;
    };

    std::vector<ZoomInfo> zoomInfos;


    AnimatorMode();
    void onUpdate();
};

} // namespace animator

