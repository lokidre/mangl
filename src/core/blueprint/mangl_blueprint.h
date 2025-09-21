/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"
#include "../mangl_framework.h"

#include "mangl_blueprint_structure.h"
#include "mangl_blueprint_loader.h"
#include "objects_parse.h"

#include "project/mangl_bp_project.h"
#include "loader/mangl_bp_loader.h"


#include <apeal/noncopyable.hpp>
#include <apeal/fs.hpp>

namespace mangl {

class Blueprint: public apl::NonCopyable {

public:
    bp::Project* project{};

    ~Blueprint() noexcept {
        clear();
    }

    void clear() noexcept {
        if (project) {
            delete project;
            project = nullptr;
        }
    }

public:
    void loadFile(StrView filename) {
        loadString(apl::loadFile(filename));
    }

    void loadString(StrView data) {
        bp::Loader loader;
        loader.load(data, this);
    }


};

}



namespace mangl::blueprint {

class Engine {
public:
    using Deps = DepsContainer;

    //
    // Setup
    //
    using UniformParam = std::pair<StrView, StrView>;
    using UniformsParam = std::vector<UniformParam>;//std::unordered_map<String, String>;
    using SearchDirs = std::vector<String>;

    using ActionFunc = std::function<void()>;
    using Action = std::pair<String, ActionFunc>;
    using Actions = std::unordered_map<String, ActionFunc>;


    void reset();
    void addUniform(const UniformParam& uni) {
        auto& uniforms = blueprint_.uniforms;

        auto& name = uni.first;
        apl::verify(!uniforms.contains(name), "Duplicate uniform: %s", name);

        auto u = uniforms.add(name);
        u->value = uni.second;
    }

    void setUniforms(const UniformsParam& param) {
        auto& uniforms = blueprint_.uniforms;
        uniforms.clear();
        for (auto& uni: param) {
            addUniform(uni);
        }
    }


//    Rect calcCoords(const Component& elem, const Screen& screen);
//    Point calcPointCoords(const Component& elem, const Screen& screen);
    //RealGL evaluateExpression(Expression& exp);
    RealGL calculateValue(Expression& exp);
    RealGL parseValue(Expression& exp);
    void moveComponent(const Args& args);
    TextureAtlas gameTex;

    void setActions(const Actions& actions);
    void addAction(const Action& action);


    void setSearchDirs(const SearchDirs& dirs);
    void addSearchDir(StrView dir);


    const auto& getTextures() const noexcept {
        return blueprint_.textures;
    }
    void setTexture(StrView name, const TextureAtlas* atlas) {
        auto texture = blueprint_.textures.get(name);
        apl::verify(texture, "Texture not found: %s", name);
        texture->atlas = atlas;
    }


    void setViewport(RectA viewport, PaddingA padding) {
        viewport_ = viewport;
        padding_ = padding;
    }

    //
    // Loading
    //
    const Deps& loadDeps(StrView filename) {
        return loader_.loadDeps(filename, &blueprint_);
    }

    void load(StrView filename);

    //
    // Creating
    //
    void create(SceneP scene);



    //
    // Runtime
    //
    void start(Timestamp when = 0);
    void pause() { paused_ = true; }
    void resume() { paused_ = false; }
    void stop();


    //
    // Debugging
    //
    void setDebugBoundsEnabled(bool f) {
        debugUpdatePending_ = true;
        debugBoundsEnabled_ = f;
    }

    void setDebugIdLabelsEnabled(bool f) {
        debugUpdatePending_ = true;
        debugIdLabelsEnabled_ = f;
    }

    //void setDebugEnabled() {
    //    debugBoundsEnabled_ = true;
    //    debugIdLabelsEnabled_ = true;
    //    updateDebugStatus();
    //}

    //void onTimer(Timestamp timestamp);

public: 
    Blueprint blueprint_;
    Loader loader_;
private:
    /*Blueprint blueprint_;
    Loader loader_;*/

    bool loaded_{};
    TimerHandler timerHandler_;
    
    bool created_{};
    SceneP scene_{};

    bool started_{}, startScheduled_{};
    bool paused_{};
    Timestamp startScheduleTime_{};

    Rect viewport_{};
    Padding padding_{};

    
    bool debugUpdatePending_{};
    bool debugBoundsEnabled_{}, debugIdLabelsEnabled_{};
    void updateDebugStatus();




    //
    // Coordinate system
    //
    //Real evalCoord(
    Real evalCoord(SpacialValueP v, CordRange rng) noexcept;
    auto evalX(LayoutMeasureXP lv) noexcept { return evalCoord(lv, {0, viewport_.w}); }
    auto evalY(LayoutMeasureYP lv) noexcept { return evalCoord(lv, {0, viewport_.h}); }

    auto evalPoint(LayoutPointP p) noexcept { return Point{evalX(&p->x), evalY(&p->y)}; }
    auto evalSize(LayoutSizeP s) noexcept { return Size{evalX(&s->w), evalY(&s->h)}; }
    auto evalRect(LayoutPointP p, LayoutSizeP s) noexcept {
        return Rect{evalX(&p->x), evalY(&p->y), evalX(&s->w), evalY(&s->h)};
    }


    //Size evalSize(LayoutSizeA ls) noexcept;
    //Rect evalRect(LayoutPointA p, LayoutSizeA s) noexcept;

    void evalNodeRect(NodeP node) noexcept;

    void evalNodeRectRatioed(NodeP node, SizeA ratio) noexcept;


    //
    // Graphics elements
    //
    void updateNodePrimColors(NodeP node);

protected:
    void onTimerPostfix(Timestamp timestamp) ;
};

}  // namespace mangl::blueprint


namespace mangl {

using BlueprintEngine = blueprint::Engine;

}  // namespace mangl
