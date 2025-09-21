/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "properties_widget.h"

#include "state.hpp"
#include "main_data.h"
#include "theme.hpp"
#include "pubsub.hpp"

#include <apeal/string.hpp>

namespace toolkit {

void PropertiesWidget::onCreate() {

    auto theme = ToolkitTheme::instance();

    setVExpand(true);
    setSizeRequest(350, 400);

    mainLayout_.create()->setSpacing(theme->layoutColSpacing)->setMargin(theme->layoutHorzPadding);
    setChild(&mainLayout_);


    //    orientationCombo_.create();
    //    mainLayout_.append(&orientationCombo_);


    auto state = State::instance();

    layoutGrid_.create()
        ->setColumnSpacing(theme->layoutColSpacing)
        ->setRowSpacing(theme->layoutRowSpacing);
    // layoutGrid_.setHExpand();
    mainLayout_.append(&layoutGrid_);

    int row = 0;
    int cols = 4;

    std::vector<String> resolutionStrings;
    for (auto& ri: resolutionInfos_) {
        auto& device = resolutionFamilies_[ri.family];
        auto label = apl::fmt("%s %dx%d", device, (int)ri.layout.w, (int)ri.layout.h);
        resolutionStrings.push_back(label);
    }
    gw::Label resolutionLabel{"Resolution:"};
    resolutionLabel.setXAlign(1);
    layoutGrid_.attach(&resolutionLabel, 0, row);
    resolutionDropdown_.create(resolutionStrings)->setHExpand();
    resolutionDropdown_.itemSelectedSignal = [this](int idx) { int a = 0; };
    layoutGrid_.attach(&resolutionDropdown_, 1, row, cols-1);
    ++row;


    const char* orientations[] = {"Vertical", "Horizontal", nullptr};
    gw::Label orientationLabel{"Orientation:"};
    orientationLabel.setXAlign(1);
    layoutGrid_.attach(&orientationLabel, 0, row);
    orientationDropdown_.create(orientations)->setHExpand();
    orientationDropdown_.itemSelectedSignal = [this](int idx) { int a = 0; };
    layoutGrid_.attach(&orientationDropdown_, 1, row, cols-1);
    ++row;


    std::vector<String> zoomStrings;
    for (auto zoom: layoutZooms_) {
        zoomStrings.push_back(apl::fmt("%d%%", zoom));
    }

    gw::Label zoomLabel{"Zoom:"};
    zoomLabel.setXAlign(1);
    layoutGrid_.attach(&zoomLabel, 0, row);
    zoomDropdown_.create(zoomStrings)->setHExpand();
    layoutGrid_.attach(&zoomDropdown_, 1, row, cols-1);
    ++row;


    wireframeCheckbox_.create();
    wireframeCheckbox_.setChecked(state->model.wireframe);
    wireframeCheckbox_.onToggle = [this](bool active) {
        State::instance()->model.wireframe = active;
        Pubsub::instance()->viewOptionsChanged();
    };
    layoutGrid_.attach(&wireframeCheckbox_, 0, row, 2);

    verticesCheckbox_.create();
    verticesCheckbox_.setChecked(state->model.vertices);
    verticesCheckbox_.onToggle = [this](bool active) {
        State::instance()->model.vertices = active;
        Pubsub::instance()->viewOptionsChanged();
    };
    layoutGrid_.attach(&verticesCheckbox_, 2, row, 2);

    faceCullingCheckbox_.create();
    faceCullingCheckbox_.setChecked(state->model.faceCulling);
    faceCullingCheckbox_.onToggle = [this](bool active) {
        State::instance()->model.faceCulling = active;
        Pubsub::instance()->viewOptionsChanged();
    };
    layoutGrid_.attach(&faceCullingCheckbox_, 4, row, 2);
    ++row;

    depthTestCheckbox_.create();
    depthTestCheckbox_.setChecked(state->model.depthTest);
    depthTestCheckbox_.onToggle = [this](bool active) {
        State::instance()->model.depthTest = active;
        Pubsub::instance()->viewOptionsChanged();
    };
    layoutGrid_.attach(&depthTestCheckbox_, 0, row, 2);

    lightingCheckbox_.create();
    lightingCheckbox_.setChecked(state->model.lighting);
    lightingCheckbox_.onToggle = [this](bool active) {
        State::instance()->model.lighting = active;
        Pubsub::instance()->viewOptionsChanged();
    };
    layoutGrid_.attach(&lightingCheckbox_, 2, row, 2);


    ++row;


    //
    // auto dropDown = gtk_drop_down_new_from_strings(orientations);
    //    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropDown), (int)state->screenLayout.orientation);
    //    gtk_widget_set_hexpand(dropDown, true);
    //
    //    layoutGrid_.gattach(dropDown, 1, row, 3);

    //    mainLayout_.gappend(dropDown);


    //    gwidget_ = gtk_frame_new("Properties");
    //    //gtk_widget_set_hexpand(gwidget_, true);
    //    gtk_widget_set_vexpand(gwidget_, true);
    //
    //    gtk_widget_set_size_request(GTK_WIDGET(gwidget_), 300, 400);
}

}  // namespace toolkit
