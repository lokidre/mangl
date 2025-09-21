/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_framework.h"

#ifndef MANGL_CONSOLE
#include "../render/mangl_pl_util_global.h"
#endif

#include <apeal/string.hpp>
#include <apeal/parse/json.hpp>

namespace mangl {

//
// Touches
//
void Framework::onTouchDown(TouchA touch) {
    if (!interactionEnabled_)
        return;

#ifndef MANGL_CONSOLE
    auto& scenes = render_->scenes();

    for (auto scene: scenes) {
        if (scene->interactionDisabled_ || !scene->visible_)
            continue;
        if (scene->onTouchDown(touch)) {
            //MDebugFunc();
            return;
        }
    }

    if (currentScreen_) {
        currentScreen_->frameworkKeyBegan_ = true;
        currentScreen_->onTouchDown(touch);
    }
#endif
}

void Framework::onTouchMove(TouchA touch) {
    if (!interactionEnabled_)
        return;

#ifndef MANGL_CONSOLE

    auto& scenes = render_->scenes();

    for (auto scene: scenes) {
        if (scene->interactionDisabled_ || !scene->visible_)
            continue;
        if (scene->onTouchMove(touch))
            return;
    }

    if (currentScreen_ && currentScreen_->frameworkKeyBegan_)
        currentScreen_->onTouchMove(touch);
#endif
}

void Framework::onTouchUp(TouchA touch) {
    if (!interactionEnabled_)
        return;

#ifndef MANGL_CONSOLE
    auto& scenes = render_->scenes();

    for (auto scene: scenes) {
        if (scene->interactionDisabled_ || !scene->visible_)
            continue;
        if (scene->onTouchUp(touch))
            return;
    }

    if (currentScreen_ && currentScreen_->frameworkKeyBegan_) {
        currentScreen_->onTouchUp(touch);
        currentScreen_->onTouchUpPostfix(touch);
    }
#endif
}

void Framework::onTouchCancel(TouchA touch) {
    if (!interactionEnabled_)
        return;

#ifndef MANGL_CONSOLE
    auto& scenes = render_->scenes();

    for (auto scene: scenes) {
        if (scene->interactionDisabled_ || !scene->visible_)
            continue;
        if (scene->onTouchCancel(touch))
            return;
    }

    if (currentScreen_ && currentScreen_->frameworkKeyBegan_)
        currentScreen_->onTouchCancel(touch);
#endif
}


void Framework::onKeyDown(KeyCode key) {
    if (!interactionEnabled_)
        return;

#ifndef MANGL_CONSOLE

    auto& scenes = render_->scenes();

    for (auto scene: scenes) {
        if (scene->interactionDisabled_ || !scene->visible_)
            continue;
        if (scene->onKeyDown(key))
            return;
    }

    if (currentScreen_)
        currentScreen_->onKeyDown(key);
#endif
}

void Framework::onKeyUp(KeyCode key) {
    if (!interactionEnabled_)
        return;

#ifndef MANGL_CONSOLE
    for (auto scene: *render::scenes_) {
        if (scene->interactionDisabled_ || !scene->visible_)
            continue;
        if (scene->onKeyUp(key))
            return;
    }

    if (currentScreen_)
        currentScreen_->onKeyUp(key);
#endif
}

void Framework::onMouseWheel(Cord delta, Flags keys, Point pos) {
    if (!interactionEnabled_)
        return;

#ifndef MANGL_CONSOLE

    for (auto scene: *render::scenes_) {
        if (scene->interactionDisabled_ || !scene->visible_)
            continue;
        if (scene->onMouseWheel(delta, keys, pos))
            return;
    }

    if (currentScreen_)
        currentScreen_->onMouseWheel(delta, keys, pos);
#endif
}


// TODO: remove processCall eventually
String Framework::processCall(StrView name, StrView data) {
    try {
        auto result = onCall(name, data);

        if (currentScreen_) {
            auto res = currentScreen_->onCall(name, data);
            if (!res.empty())
                result = res;
        }

        return result;
    }
    catch(const std::exception& e) {
        //return apl::fmt(R"({"error": "Unknown call name: '%s'"})", name);
        return apl::fmt(R"({"error": "%s"})", apl::JsonBuilder::escape(e.what()));
    }
    catch(...) {
        return R"({"error": "Unhandled Exception"})";
    }
}


} // namespace mangl

