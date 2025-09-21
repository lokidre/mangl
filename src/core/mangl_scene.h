/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_prim.h"
#include "mangl_prim_params.h"

#include "mangl_keyboard.h"
#include "mangl_touches.h"

#include "widgets/mangl_background_widget_params.h"
#include "widgets/mangl_button_params.h"
#include "widgets/mangl_checkbox_params.h"
#include "widgets/mangl_grid_layout_params.h"
#include "widgets/mangl_dialog_params.h"
#include "widgets/mangl_label_params.h"
#include "widgets/mangl_segmented_params.h"
#include "widgets/mangl_slider_params.h"
#include "widgets/mangl_stack_widget_params.h"

#include "effects/mangl_crossfade_effect_params.h"
#include "effects/mangl_fade_effect_params.h"
#include "effects/mangl_move_effect_params.h"
#include "effects/mangl_scale_effect_params.h"
#include "effects/mangl_disappear_effect_params.h"
#include "effects/mangl_rotate_effect_params.h"

#include "design/mangl_design_grid_params.h"


namespace mangl {

struct GuiEffect;
using GuiEffectP = GuiEffect*;

struct SceneRenderObject {
    Prim* prim{};
    ScenePrimsContainer* prims{};
};

using SceneRenderObjectContainer = std::vector<SceneRenderObject>;

struct SceneObject {
    virtual ~SceneObject() = default;
};

namespace render {
struct RenderContext;
static void processPipelineColors(const RenderContext&) noexcept;
static void pipelineRender() noexcept;
}  // namespace render

class Scene {
    friend class SceneRenderer;
    friend class Framework;
    friend void render::pipelineRender() noexcept;
    friend void render::processPipelineColors(const render::RenderContext&) noexcept;

    using Self = Scene;

public:
    using EffectsList = std::vector<GuiEffectP>;
    using WidgetsList = std::vector<WidgetP>;

    ~Scene() noexcept { release(); }

    void release() noexcept;

    SceneRenderObjectContainer objects_;

    Rect frame_{};
    bool frameSet_ = false;

    void clear();

    void setDefaultFlags(Flags flags) noexcept { defaultFlags_ = flags; }
    void clearDefaultFlags() noexcept { defaultFlags_ = 0; }
    Flags defaultFlags_{};



    // Prim manipulation
    //
    // adding/removing the objects
    //
    void addPrim(PrimP prim);  // must be allocated
    PrimP newPrim(Flags flags = {});
    ScenePrimsContainerP newPrimArray(Index count = 0);  // allocates an array of objects

    SceneP deletePrim(PrimP prim);
    SceneP removeSprite(PrimP prim);

    SceneP deleteWidget(WidgetP widget);



    //
    // Shortcuts for convenient prim creation
    //
    PrimP newImage(PrimParamsA params) {
        auto flags = params.flags;
        if (params.hidden)
            flags |= M::Hidden;

        auto prim = newPrim(flags);

        MDebugAssert(params.texture.valid());
        apl::verify(params.texture.valid(), "newImage: Texture is not provided");
        if (params.textureIndex)
            prim->setTexture(params.texture.getFrame(params.textureIndex));
        else
            prim->setTexture(params.texture);

        prim->move(params.rect);
        prim->setElSize(params.elSize);

        if (params.alpha != 0) prim->setAlpha(params.alpha);

        return prim;
    }

    auto newImage(RectA frame, TextureA tex, Flags flags = 0) {
        return newPrim(flags)->setTexture(tex)->move(frame);
    }

    auto newImage(RectA frame, TextureA tex, Size elSize, Flags flags = 0) {
        return newPrim(flags)->setTexture(tex)->move(frame)->setElSize(elSize);
    }

    auto newTexture(TextureA tex, RectA ar, Flags flags = 0) {
        return newPrim(flags)->setTexture(tex)->move(ar);
    }

    auto newTexture(TextureA tex, Flags flags = 0) {
        return newPrim(flags)->setTexture(tex);
    }



    // Primitives
    auto newPoint(Point p, ColorA c, Cord w, Flags flags = {}) {
        return newPrim(flags)->setPoint(p)->setColor(c)->setWidth(w); 
    }

    auto newLine(Point a, Point b, Flags f=0) noexcept { return newPrim(f)->setLine(a, b); }
    auto newLine(ColorA c, Point a, Point b, Cord width = 1, Flags f = {}) noexcept {
        return newLine(a, b, f)->setColor(c)->setWidth(width); 
    }

    auto newTri(Point a, Point b, Point c, Flags f = {}) { return newPrim(f)->setTriangle(a, b, c); }
    auto newTriangle(Point a, Point b, Point c, Flags f = {}) { return newTri(a, b, c, f); }


    // Composites
    auto newRect(ColorA acolor = {}, RectA ar = {}, Flags flags = {}) {
        return newPrim(flags)->setRect(ar)->setColor(acolor); 
    }

    auto newBox(ColorA acolor = {}, RectA ar = {}, Flags flags = {}) {
        return newPrim(flags)->setBox(ar)->setColor(acolor); 
    }

    // Complex shapes
    PrimP newCircle(ColorA color, PointA center, Real radius, Flags flags={}) {
        return newPrim(flags)->initCircle(center, radius)->setColor(color);
    }

    PrimP newArc(ColorA color, PointA center, Real radius, M::Orientation beginSeg, M::Orientation endSeg, Flags f = {}) {
        return newPrim(f)->setArc(center, radius, beginSeg, endSeg)->setColor(color);
    }

    auto newSegment(ColorA color, PointA center, Real radius, M::Orientation beginSeg, M::Orientation endSeg, Flags f = {}) {
        return newPrim(f)->setSegment(center, radius, beginSeg, endSeg)->setColor(color);
    }


    PrimP newBezier(const PrimBezierParams& p) {
        auto prim = newPrim(p.flags);

        Point pts[4]{};
        Index i = 0;
        for (auto pit: p.points) {
            pts[i] = pit;
            ++i;
        }

        if (p.a.x != M::InvalidCoord && p.a.y != M::InvalidCoord) pts[0] = p.a;
        if (p.b.x != M::InvalidCoord && p.b.y != M::InvalidCoord) pts[1] = p.b;
        if (p.c.x != M::InvalidCoord && p.c.y != M::InvalidCoord) pts[2] = p.c;
        if (p.d.x != M::InvalidCoord && p.d.y != M::InvalidCoord) pts[3] = p.d;

        prim->setBezier(pts);

        prim->setColor(p.color);
        prim->setWidth(p.width);

        return prim;
    }

    auto newDisk(ColorA color, PointA center, Cord radius, Flags f = {}) noexcept {
        return newPrim(f)->setDisk(center, radius)->setColor(color);
    }

    // Model
    auto newModel(Model* model, Flags f = {}) { 
        return newPrim(f)->setModel(model); 
    }


    //
    // Widgets
    //
    LabelP newLabel(LabelParamsA lp);

//    LabelP newLabel(RectA rect, StrView text, Cord height, ColorA color, Flags flags = {});

//    LabelP newLabel(StrView text, PointA point, Cord height, ColorA color, Flags flags = {}) {
//        return newLabel({point.x, point.y, 0, 0}, text, height, color, flags);
//    }
//
//    LabelP newLabel(Cord height, ColorA color, Flags flags = {}) {
//        return newLabel(StrView{}, {}, height, color, flags);
//    }
//
//    LabelP newLabel(Point p, Cord height, ColorA color, Flags flags = {}) {
//        return newLabel(StrView{}, p, height, color, flags);
//    }


    ButtonP newButton(ButtonParamsA params);
    ButtonP setupButton(ButtonP button, ButtonParamsA params);

    //template <class ButtonT, class ParamsT>
    //ButtonT* newButtonT(ParamsT params) {
    //    auto button = newWidget<ButtonT>();
    //    setupButton(button, static_cast<ButtonParamsA>(params));
    //    return button;
    //}

    template <class ButtonT>
    ButtonT* newButtonT(ButtonParamsA params) {
        auto button = newWidget<ButtonT>();
        setupButton(button, params);
        return button;
    }


    CheckboxP newCheckbox(CheckboxParamsA params);
    CheckboxP newCheckbox(Rect contentsFrame, TextureA onTex, TextureA offTex, Flags flags = 0);
    CheckboxP newCheckbox(TextureA onTex, TextureA offTex, Point pos, Size size, Flags flags = 0);


    SegmentedP newSegmented(SegmentedParamsA params = {});
    StackWidgetP newStackWidget(StackWidgetParamsA params = {});
    BackgroundWidgetP newBackgroundWidget(BackgroundWidgetParamsA params = {});

    SliderP newSlider(SliderParamsA params);


    DialogP newDialog(DialogParamsA params);

    GridLayoutP newGridLayout(GridLayoutParamsA params = {});







public:
    //
    // Elements order
    //
    void moveToBack(PrimP prim);
    void moveToFront(PrimP prim);
    void moveToFront(ScenePrimsContainer* prims);
    void moveToFront(int index);
    void setZIndexFront(WidgetP widget);

    //
    // Effects
    //
    template <class T>
    auto newEffect() noexcept {
        M::debugAssertMainThread();

        auto e = new T;
        allocatedEffects_.push_back(e);
        return e;
    }

    MoveEffectP newMoveEffect(const MoveEffectParams& params);
    CrossfadeEffectP newCrossfadeEffect(const CrossfadeEffectParams& params);
    FadeEffectP newFadeEffect(FadeEffectParamsA params);
    FadeEffectP newFadeInEffect(FadeEffectParamsA params);
    FadeEffectP newFadeOutEffect(FadeEffectParamsA params);
    ScaleEffectP newScaleEffect(const ScaleEffectParams& params);
    DisappearEffectP newDisappearEffect(const DisappearEffectParams& params);
    RotateEffectP newRotateEffect(RotateEffectParamsA params);


    //
    // Design elements
    //
    DesignGridWidgetP newDesignGridWidget(DesignGridParamsA dgp);



    //
    // Widgets
    //
public:
    bool interactionDisabled_ = false;
    auto setInteractionEnabled(bool f = true) noexcept {
        interactionDisabled_ = !f;
        return this;
    }

    static Flags processVisibilityFlag(Flags flags, Flags defFlags) noexcept {
        if (flags & M::Hidden) defFlags &= ~M::Visible;
        else if (flags & M::Visible) defFlags &= ~M::Hidden;
        else if ((defFlags & M::Visible) || (defFlags & M::Hidden)) {}
        else defFlags |= M::Visible;  // If no flag provided, then make it visible
        flags |= defFlags;
        return flags;
    }


    template <class T>
    T *newWidget(Flags flags = {}) {
        auto w = new T;
        initWidget(w, flags);
        return w;
    }

    template <class T, class... Args>
    T* newWidgetArg(Args... args) {
        auto w = new T{args...};
        initWidget(w, {});
        return w;
    }

private:
    void initWidget(WidgetP w, Flags flags);

    void pushWidgetToList(WidgetP w);
    void removeWidgetFromList(WidgetP w);
    void moveWidgetToFront(WidgetP w);


    // Supporting modal widget
public:
    void setModalWidget(WidgetP w) { modalWidget_ = w; }

private:
    WidgetP modalWidget_{};

    SceneRenderObjectContainer allocatedPrims_;

    EffectsList allocatedEffects_;

    bool widgetsEntered_{};
    WidgetsList allocatedWidgets_; //, widgets_;
    WidgetP widgets_{};


    //
    // General objects, just for memory management
    //
public:
    template <class ObjectT>
    auto newObject() {
        M::debugAssertMainThread();

        auto obj = new ObjectT;
        allocatedObjects_.push_back(obj);
        return obj;
    }

    std::vector<SceneObject *> allocatedObjects_;


    //
    // General effects
    //
private:
    bool visible_ = true;

    bool alphaFlag_{};
    Cord alphaValue_{};

    bool colorEffectFlag_{};
    ColorHashed colorEffectValue_{1, 1, 1, 1};

    bool scissorFlag_{};
    Rect scissorRect_{};


public:
    // Color blend for the whole scene
    auto setColor(ColorA color) noexcept {
        colorEffectFlag_ = true;
        colorEffectValue_ = color;
        return this;
    }

    auto setColor(bool enabled, ColorA color = {}) noexcept {
        colorEffectFlag_ = enabled;
        colorEffectValue_ = color;
        return this;
    }

    auto clearColor() noexcept {
        colorEffectFlag_ = false;
        return this;
    }


    // Visibility
    auto setVisible(bool f = true) noexcept {
        visible_ = f;
        return this;
    }
    auto hide() noexcept {  return setVisible(false);  }
    auto unhide() noexcept {  return setVisible(true);  }

    //
    // Transparency
    //
    auto setAlpha(Cord alpha) noexcept {
        alphaFlag_ = true;
        alphaValue_ = alpha;
        return this;
    }

    auto setAlpha(bool enabled, Cord alpha) noexcept {
        alphaFlag_ = enabled;
        alphaValue_ = alpha;
        return this;
    }

    auto clearAlpha() noexcept {
        alphaFlag_ = false;
        return this;
    }


    //
    // Scissor
    auto setScissor(RectA rect) noexcept {
        scissorFlag_ = true;
        scissorRect_ = rect;
        return this;
    }
    auto setScissor(bool enabled, RectA rect) noexcept {
        scissorFlag_ = enabled;
        scissorRect_ = rect;
        return this;
    }

    auto clearScissor() noexcept {
        scissorFlag_ = false;
        return this;
    }


public:
    void onTimer(Timestamp timestamp) noexcept;


    //
    // Interaction
    //
    bool onTouchDown(TouchA);
    bool onTouchMove(TouchA);
    bool onTouchUp(TouchA);
    bool onTouchCancel(TouchA);


    //
    // Keyboard
    //
    bool onKeyDown(KeyCode key);
    bool onKeyUp(KeyCode key);

    //
    // Mouse
    //
    bool onMouseWheel(Cord delta, Flags keys, Point pos);
};


}  // namespace mangl
