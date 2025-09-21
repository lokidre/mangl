/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_scene.h"

#include "../mangl_widgets.h"
#include "../mangl_framework.h"

namespace mangl {

bool Scene::onTouchDown(TouchA touch)
{
    M::debugAssertMainThread();

    if (interactionDisabled_)
        return false;

    if (modalWidget_) {
        if (modalWidget_->created_)
            modalWidget_->onTouchDown(touch);

        //MDebugFunc("modal widdget: %d", modalWidget_ ? 1: 0);

        return true;
    }

    for (auto w = widgets_; w; w = w->next) {
        if (!w->visible_ || !w->created_ || w->interactionDisabled_)
            continue;
        if (w->onTouchDown(touch)) {
            //MDebugFunc("touch true");
            return true;
        }
    }

    return false;
}

bool Scene::onTouchMove(TouchA touch)
{
    M::debugAssertMainThread();

    if (interactionDisabled_)
        return false;

    if (modalWidget_) {
        if (modalWidget_->created_)
            modalWidget_->onTouchMove(touch);
        return true;
    }

    for (auto w = widgets_; w; w = w->next) {
        if (!w->visible_ || !w->created_ || w->interactionDisabled_)
            continue;
        auto processed = w->onTouchMove(touch);
        if (processed && !w->passthroughTouchesMove_)
            return true;
    }

    return false;
}

bool Scene::onTouchUp(TouchA touch)
{
    M::debugAssertMainThread();

    if (interactionDisabled_)
        return false;

    if (modalWidget_) {
        if (modalWidget_->created_)
            modalWidget_->onTouchUp(touch);
        return true;
    }

    //for (auto &c: widgets_) {
    for (auto w = widgets_; w; w = w->next) {
        if (!w->visible_ || !w->created_ || w->interactionDisabled_)
            continue;
        if (w->onTouchUp(touch))
            return true;
    }

    return false;
}

bool Scene::onTouchCancel(TouchA touch)
{
    M::debugAssertMainThread();

    if (interactionDisabled_)
        return false;

    if (modalWidget_) {
        if (modalWidget_->created_)
            modalWidget_->onTouchCancel(touch);
        return true;
    }


    //for (auto &c: widgets_) {
    for (auto w = widgets_; w; w = w->next) {
        if (!w->visible_ || !w->created_ || w->interactionDisabled_)
            continue;
        if (w->onTouchCancel(touch))
            return true;
    }

    return false;
}


bool Scene::onKeyDown(KeyCode key)
{
    M::debugAssertMainThread();

    if (interactionDisabled_)
        return false;

    if (modalWidget_) {
        if (modalWidget_->created_)
            modalWidget_->onKeyDown(key);
        return true;
    }


    //for (auto &w: widgets_) {
    for (auto w = widgets_; w; w = w->next) {
        if (!w->visible_ || !w->created_ || w->interactionDisabled_)
            continue;
        if (w->onKeyDown(key))
            return true;
    }

    return false;
}

bool Scene::onKeyUp(KeyCode key)
{
    M::debugAssertMainThread();

    if (interactionDisabled_)
        return false;

    if (modalWidget_) {
        if (modalWidget_->created_)
            modalWidget_->onKeyUp(key);
        return true;
    }

    //for (auto &w: widgets_) {
    for (auto w = widgets_; w; w = w->next) {
        if (!w->visible_ || !w->created_ || w->interactionDisabled_)
            continue;
        if (w->onKeyUp(key))
            return true;
    }

    return false;
}

bool Scene::onMouseWheel(Cord delta, Flags keys, Point pos)
{
    M::debugAssertMainThread();

    if (interactionDisabled_)
        return false;

    if (modalWidget_) {
        if (modalWidget_->created_)
            modalWidget_->onMouseWheel(delta, keys, pos);
        return true;
    }

    //for (auto &w: widgets_) {
    for (auto w = widgets_; w; w = w->next) {
        if (!w->visible_ || !w->created_ || w->interactionDisabled_)
            continue;
        if (w->onMouseWheel(delta, keys, pos))
            return true;
    }

    return false;
}

}  // namespace mangl
