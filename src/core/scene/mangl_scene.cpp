/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_scene.h"

#include "../widgets/mangl_widget.h"
#include "../effects/mangl_effect.h"
#include "../mangl_framework.h"
#include "../native/mangl_native_gui.h"

namespace mangl {

void Scene::clear() {
    
    frameSet_ = false;
    colorEffectFlag_ = false;

    release();
}

void Scene::release() noexcept {
    
    M::debugAssertMainThread();
    
    //widgets_.clear();
    widgets_ = nullptr;

    for (WidgetP& w: allocatedWidgets_) {
        if (w) {
            w->onDestroy();

            delete w;
            w = nullptr;
        }
    }


    modalWidget_ = nullptr;
    allocatedWidgets_.clear();


    for (auto& o: allocatedPrims_) {
        if (o.prim) {
            std::free(o.prim);
            o.prim = nullptr;
        }

        if (o.prims) {
            delete o.prims;
            o.prims = nullptr;
        }
    }

    allocatedPrims_.clear();
    objects_.clear();

    for (auto ef: allocatedEffects_) {
        delete ef;
    }
    allocatedEffects_.clear();

    //
    // Now time to delete the objects if any
    for (auto o: allocatedObjects_) {
        delete o;
    }
    allocatedObjects_.clear();

    defaultFlags_ = 0;
}


void Scene::addPrim(PrimP prim) {
    objects_.push_back({
        .prim = prim,
    });
}


PrimP Scene::newPrim(Flags flags) {
    M::debugAssertMainThread();

    
    // TODO: efficient memory management
    auto primData = std::malloc(sizeof(Prim));
    std::memset(primData, 0, sizeof(Prim));

    auto p = static_cast<PrimP>(primData);

    flags = processVisibilityFlag(flags, defaultFlags_);


    bool vis = flags & M::Hidden ? false : true;
    if (flags & M::Visible)
        vis = true;

    p->setVisible(vis);

    if (flags & M::TiledHorz)
        p->setTiledHorz();
    if (flags & M::TiledVert)
        p->setTiledVert();

    if (flags & M::FlipVert)
        p->setFlipVert();
    if (flags & M::FlipHorz)
        p->setFlipHorz();

    // if ( flags & M::AlignRight )	o->setAlignHorz(M::AlignRight) ;
    // if ( flags & M::AlignCenter ) o->setAlignHorz(M::AlignCenter) ;
    // if ( flags & M::AlignTop )		o->setAlignVert(M::AlignTop) ;
    // if ( flags & M::AlignVCenter ) o->setAlignVert(M::AlignVCenter) ;

    objects_.push_back({
        .prim = p,
    });

    allocatedPrims_.push_back({
        .prim = p,
    });


    return p;
}

ScenePrimsContainerP Scene::newPrimArray(Index count) {
    M::debugAssertMainThread();



    auto& p = objects_.emplace_back();

    p.prims = new ScenePrimsContainer;

    if (count > 0) {
        p.prims->resize(count);
        std::ranges::fill(*p.prims, Prim{});
    }

    allocatedPrims_.push_back(p);

    return p.prims;
}

SceneP Scene::deletePrim(PrimP prim) {
    M::debugAssertMainThread();
    
    if (prim == nullptr)
        return this;

    bool allocatedFound = false;

    for (auto it = allocatedPrims_.begin(), end = allocatedPrims_.end(); it != end; ++it) {
        if (it->prim == prim) {
            allocatedPrims_.erase(it);
            allocatedFound = true;
            break;
        }
    }

    removeSprite(prim);

    if (allocatedFound)
        std::free(prim);

    return this;
}

Scene* Scene::deleteWidget(WidgetP widget) {
    M::debugAssertMainThread();

    removeWidgetFromList(widget);

//    for (auto it = widgets_.begin(); it != widgets_.end(); ++it) {
//        if (*it == widget) {
//            widgets_.erase(it);
//            break;
//        }
//    }

    for (auto& allocatedWidget: allocatedWidgets_) {
        if (allocatedWidget == widget) {
            allocatedWidget->onDestroy();

            delete allocatedWidget;
            allocatedWidget = nullptr;
            // widgets_.erase(it) ;
            break;
        }
    }

    return this;
}

Scene* Scene::removeSprite(PrimP prim) {
    for (auto it = objects_.begin(), end = objects_.end(); it != end; ++it) {
        if (it->prim == prim) {
            objects_.erase(it);
            break;
        }
    }

    return this;
}



void Scene::initWidget(WidgetP w, Flags flags) 
{
    M::debugAssertMainThread();

    w->scene_ = this;

    flags = processVisibilityFlag(flags, defaultFlags_);

    bool vis = (flags & M::Hidden) ? false : true;
    if (flags & M::Visible)
        vis = true;

    w->flags_ = flags;

    {
        if (widgetsEntered_) {
            ADebugTrap();
        }

        // lock_type lock(mutex_) ;
        allocatedWidgets_.push_back(w);
        pushWidgetToList(w);

        w->onInit();
        w->setVisible(vis);
    }
}

void Scene::pushWidgetToList(WidgetP w) {
    M::debugAssertMainThread();

    w->next = widgets_;
    if (w->next)
        w->next->prev = w;
    w->prev = nullptr;
    widgets_ = w;
    // widgets_.push_back(w);
}

void Scene::moveWidgetToFront(WidgetP w){

    M::debugAssertMainThread();

    if (w->next)
        w->next->prev = w->prev;
    if (w->prev)
        w->prev->next = w->next;

    w->next = widgets_;
    w->prev = nullptr;
    widgets_ = w;
}

void Scene::removeWidgetFromList(WidgetP w) {
    if (widgets_ == w) {
        widgets_ = w->next;
        if (widgets_)
            widgets_->prev = nullptr;

        w->next = w->prev = nullptr;
        return;
    }

    if (w->prev)
        w->prev->next = w->next;
    if (w->next)
        w->next->prev = w->prev;
    w->next = w->prev = nullptr;
}




// TODO:
void Scene::moveToFront(ScenePrimsContainerP prims) {
    if (!prims)
        return;

    int index = 0;

    for (auto & object: objects_) {
        if (object.prims == prims) {
            break;
        }
    }

    moveToFront(index);
}

void Scene::moveToFront(PrimP prim) {
    if (!prim)
        return;

    int index = 0;

    for (auto & object: objects_) {
        if (object.prim == prim) {
            break;
        }
        ++index;
    }

    moveToFront(index);
}

void Scene::moveToFront(int index) {
    M::debugAssertMainThread();

    if (index == 0)
        return;


    // Find the prim we want to remove
    auto primEnd = objects_.end();
    auto primCurr = objects_.begin() + index;

    // for (; primCurr != primEnd; ++primCurr) {
    //     if (primCurr->prim == prim)
    //         break;
    // }

    if (primCurr == primEnd) {
        ADebugTrap();
        return;
    }

    auto foundPrim = *primCurr;

    for (auto primNext = primCurr + 1; primNext != primEnd; ++primCurr, ++primNext) {
        *primCurr = *primNext;
    }

    *primCurr = foundPrim;
}

void Scene::setZIndexFront(WidgetP widget) {
    M::debugAssertMainThread();
    
    if (!widget)
        return;

    moveWidgetToFront(widget);
    widget->setZIndexFront();
}



#if 0
bool Scene::order_behind(PrimP prim, PrimP behind)
{
    MPrimsList::reverse_iterator so_it, be_it, so_end = objects_.rend();

    for (so_it = objects_.rbegin(); so_it != so_end; ++so_it) {
        if (so_it->second == prim)
            break;
    }

    if (so_it == so_end)
        return false;

    for (be_it = objects_.rbegin(); be_it != so_end; ++be_it) {
        if (be_it->second == behind)
            break;
    }

    if (be_it == so_end)
        return false;

    // check if the object is not behind already
    if (so_it > be_it)
        return true;


    auto so_value = *so_it;

    auto next_it = so_it;
    ++next_it;
    for (; next_it != be_it; ++so_it, ++next_it) {
        *so_it = *next_it;
    }
    *so_it = *next_it;
    *next_it = so_value;


    return true;
}
#endif

} // namespace mangl
