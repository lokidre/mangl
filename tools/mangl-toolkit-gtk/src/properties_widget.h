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

#include "gwidgets/widgets.hpp"

namespace toolkit {

class PropertiesWidget: public gw::Frame {

    gw::VBoxLayout mainLayout_;

    gw::GridLayout layoutGrid_;
    gw::DropDown resolutionDropdown_;
    gw::DropDown zoomDropdown_;
    gw::DropDown orientationDropdown_;

    gw::CheckBox wireframeCheckbox_{"Wireframe"};
    gw::CheckBox verticesCheckbox_{"Vertices"};
    gw::CheckBox faceCullingCheckbox_{"Face Culling"};
    gw::CheckBox depthTestCheckbox_{"Depth Test"};
    gw::CheckBox lightingCheckbox_{"Lighting"};

protected:
    void onCreate() override;
};

}  // namespace toolkit
