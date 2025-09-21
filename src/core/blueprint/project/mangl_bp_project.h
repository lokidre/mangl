/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <apeal/noncopyable.hpp>

#include "mangl_bp_screen.h"
#include "mangl_bp_layout.h"
#include "mangl_bp_world.h"
#include "mangl_bp_container.h"
#include "mangl_bp_component.h"

namespace mangl::bp {

struct Project: public apl::NonCopyable {
    String id;
    String title;
    ScreenLayout layout;

    ContainerById<Screen> screens;
    ContainerById<Layer> layers;
    ContainerById<World> worlds;
    ContainerById<Component> components;


    ~Project() noexcept { clear(); }

    void clear() noexcept {
        screens.clear();
        layers.clear();
        worlds.clear();
    }

    ScreenP addScreen(const String& screenId) { return screens.insert(screenId); }

    LayerP addLayer(const String& layerId, ScreenP screen) {
        auto layer = layers.insert(layerId);

        screen->layers.push_back(layer);
        return layer;
    }

    auto addWorld(const String& worldId, ScreenP screen) {
        auto world = worlds.insert(worldId);

        screen->worlds.push_back(world);
        return world;
    }

    auto addComponent(const String& componentId, LayerP layer) {
        auto component = components.insert(componentId);
    }
};

} // namespace mangl::bp
