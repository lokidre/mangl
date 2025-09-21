/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_texture.h"
#include "mangl_color.h"

#include <cmath>

#include <cognit/func.hpp>

namespace mangl {

struct Model;
using ModelP = Model*;

struct MatrixHashedGL;

using Triangle = std::array<Point, 3>;

// Move out PrimType::Triangle
enum class PrimType { //: int {
    None = 0,  // The value of the first is set when the .clear() command is issued

    Phony,  // Doesn't get rendered. Used as parent, positional, etc.

    // Primitives
    Triangle,  // triangle
    Line,  // line
    Point, // point

    // Composites
    Box,  // filled box

    Rect,  // rectangle (border, not filled)

    Texture,  // rectangular area with texture

    // Complex shapes
    Circle,
    Arc,

    Disk,  // filled
    Sector,

    Bezier,

    Model,
};

struct PrimBezierParams {
    Point a{M::InvalidCoord, M::InvalidCoord};
    Point b{M::InvalidCoord, M::InvalidCoord};
    Point c{M::InvalidCoord, M::InvalidCoord};
    Point d{M::InvalidCoord, M::InvalidCoord};

    std::initializer_list<Point> points;

    Color color{M::White};
    Cord width{1};

    Flags flags{};
};

struct Prim;
using PrimP = Prim*;

struct Prim {
    using Self = Prim;
    using SelfP = Self*;

    using T = RealGL;

    using Type = PrimType;         //  obsolete, remove

    using TextureIndex = uint16_t; // unsigned short;


    //
    // Hierarchy
    //
    PrimP parent; // Parent prim. visible, alpha, position are reflected


    PrimP prev, next; // Next and prev prims in chain


    // Model
    ModelP model;


    //
    // Data
    //

    // Geometry
    MatrixHashedGL* modelMtx;
    RectGL r; // rectangle
    PointGL tri[3];

    bool radiusFlag_;
    RealGL radius_, radiusRightTop_, radiusRightBottom_, radiusLeftBottom_;


    // Thickness
    RealGL width_;
    RectGL scissorRect_;


    M::Orientation orientation_; // clockwise: 1 - pi/4, 2 - pi, 3 - 3pi/2

    RealGL rotationAngle_;       // rotation angle in radians
    RealGL rotationCos_, rotationSin_;

    M::Orientation beginSegment_, endSegment_; // 0 - bottom, 1 - right, 2 - top, 3 - left


    //
    // Transparency
    RealGL alpha_;


    //
    // Colors
    //
    Color color_, color2_, color3_, color4_;


    // Textures
    Texture texture_;
    TextureIndex textureIndex_; // TODO: use short
    PointGL texturePoints_[4];
    RealGL textureScale_;


    // First is the prim type
    PrimType type;

    SizeGL elSize_; // Tiling element size

    // Blurring effect
    RealGL blur;
    RealGL blurOpacity;


    //
    // Rendering Runtime calculations
    //
    RealGL rtMeasure_;
    RealGL rtData1_, rtData2_;


    //
    // All the flags
    //

    bool visible_; // Visible flag

    bool widthFlag_;

    bool scissorFlag_; // whether the prim needs to be cut

    bool textureFlag_; // prim has texture

    bool orientationFlag_;
    bool rotationFlag_;

    bool alphaFlag_;
    bool colorFlag_, colorFlag2_, colorFlag3_, colorFlag4_;


    // Tiling (applies to textures)
    bool tiledHorz_, tiledVert_;

    // Flipping
    bool flipHorz_, flipVert_; // if the prim needs to be flipped

    bool blurFlag_;


    //
    // Operations
    //

    // Construction
    auto clear() noexcept {
        //*this = {};
        memset(this, 0, sizeof(*this));
        return this;
    }

    [[nodiscard]] bool valid() const noexcept { return type != Type::None; }
    [[nodiscard]] bool isValid() const noexcept { return type != Type::None; }

    [[deprecated]] auto visible() const noexcept { return visible_; }
    [[nodiscard]] auto isVisible() const noexcept { return visible_; }
    auto setVisible(bool f = true) noexcept {
        visible_ = f;
        return this;
    }
    auto setHidden(bool f = true) noexcept {
        visible_ = !f;
        return this;
    }

    auto unhide() noexcept {
        visible_ = true;
        return this;
    }

    auto show() noexcept {
        visible_ = true;
        return this;
    }

    auto hide() noexcept {
        visible_ = false;
        return this;
    }

    [[nodiscard]] auto getParent() const noexcept { return parent; }
    auto setParent(PrimP par) noexcept {
        parent = par;
        return this;
    }


    // RectA rect() const { return r; }
    [[nodiscard]] Rect getRect() const noexcept { return r.convert<Rect>(); }
    // RectA contentsFrame() const { return r; }
    // Cord x() const { return r.x; }
    // Cord y() const { return r.y; }
    // Cord w() const { return r.w; }
    // Cord h() const { return r.h; }


    //
    // Flipping the prim
    //
    auto setFlipHorz(bool f = true) noexcept {
        flipHorz_ = f;
        return this;
    }
    auto setFlipVert(bool f = true) noexcept {
        flipVert_ = f;
        return this;
    }
    [[nodiscard]] auto isFlipHorz() const noexcept { return flipHorz_; }
    [[nodiscard]] auto isFlipVert() const noexcept { return flipVert_; }

    //
    // Orientation (faster than rotation)
    //
    auto setOrientation(M::Orientation o) noexcept {
        orientation_ = o;
        orientationFlag_ = true;
        return this;
    }
    [[deprecated]] [[nodiscard]] auto orientation() const noexcept { return orientation_; }
    auto clearOrientation() noexcept {
        orientationFlag_ = false;
        return this;
    }
    [[nodiscard]] auto getOrientation() const noexcept { return orientation_; }

    //
    // Sprite rotation
    //
    [[nodiscard]] auto isRotation() const noexcept { return rotationFlag_; }
    [[nodiscard]] auto getRotationAngle() const noexcept { return rotationAngle_; }

    auto setRotation(Real angle) noexcept {
        rotationFlag_ = true;
        rotationAngle_ = static_cast<T>(angle);
        rotationCos_ = std::cos(rotationAngle_);
        rotationSin_ = std::sin(rotationAngle_);
        return this;
    }

    // 0 degrees = v{0,1}
    auto setRotationVector(Point v) noexcept {
        rotationFlag_ = true;
        auto norm = v.norm();
        rotationCos_ = (T)(v.y / norm);
        rotationSin_ = (T)(-v.x / norm);
        return this;
    }

    auto clearRotation() noexcept {
        rotationFlag_ = false;
        return this;
    }

    //
    // Width, for things like line, triangle, rectangle
    //
    [[deprecated]] constexpr auto defaultWidth() const noexcept { return M::One; }
    constexpr static auto getDefaultWidth() noexcept { return M::One; }
    [[deprecated]] auto width() const noexcept { return widthFlag_ ? width_ : M::One; } // obsolete
    [[deprecated]] auto widthFlag() const noexcept { return widthFlag_; }               // obsolete

    [[nodiscard]] auto getWidth() const noexcept { return widthFlag_ ? width_ : getDefaultWidth(); }
    [[nodiscard]] auto isWidth() const noexcept { return widthFlag_; }

    auto setWidth(Cord w) noexcept {
        width_ = (T)w;
        widthFlag_ = true;
        return this;
    }
    auto clearWidth() noexcept {
        widthFlag_ = false;
        return this;
    }


    constexpr auto getDefaultAlpha() const noexcept { return M::One; }

    //[[deprecated]] Real alpha() const noexcept { return alphaFlag_ ? alpha_ : M::One; }
    [[deprecated]] auto alphaFlag() const noexcept { return alphaFlag_; }

    [[nodiscard]] auto getAlpha() const noexcept { return alphaFlag_ ? alpha_ : M::One; }
    [[nodiscard]] auto isAlpha() const noexcept { return alphaFlag_; }

    auto setAlpha(Real value) noexcept {
        alphaFlag_ = true;
        alpha_ = (RealGL)value;
        // color_.a = (RealGL)value;
        return this;
    }

    auto setAlpha(bool enabled, Real value) noexcept {
        alphaFlag_ = enabled;
        if (enabled)
            setAlpha(value);
        return this;
    }

    auto clearAlpha() noexcept {
        alphaFlag_ = false;
        return this;
    }


    auto moveGL(T x, T y, T w, T h) noexcept {
        r.x = x, r.y = y, r.w = w, r.h = h;

        if (type == Type::Circle || type == Type::Disk) {
            radius_ = w / 2;
            tri[0].x = r.x + radius_;
            tri[0].y = r.y + radius_;
        }
        return this;
    }

    auto moveGl(RectGL _r) noexcept { return moveGL(_r.x, _r.y, _r.w, _r.h); }


    auto move(Cord x, Cord y, Cord w, Cord h) noexcept { return moveGL((T)x, (T)y, (T)w, (T)h); }

    auto setPos(Point p) noexcept { return moveGL((T)p.x, (T)p.y, r.w, r.h); }
    auto repos(Point p) noexcept { return setPos(p); }
    auto move(Point p) noexcept { return setPos(p); }


    auto move(Point p, Size s) noexcept { return move(p.x, p.y, s.w, s.h); }
    auto move(RectA _r) noexcept { return move(_r.x, _r.y, _r.w, _r.h); }


    // auto move(Cord x, Cord y)  noexcept { return move(x, y, r.w, r.h); }
    // auto pos(Point p)  noexcept { return move(p.x, p.y); }
    // Self* geo(RectA _r)  noexcept { return move(_r.x, _r.y, _r.w, _r.h); }

    auto setFrame(RectA _r) noexcept { return move(_r); }
    auto reframe(RectA _r) noexcept { return move(_r); }


    auto setElSize(SizeA s) noexcept {
        elSize_ = s.convert<SizeGL>();
        return this;
    }

    auto moveCentered(Point p) noexcept {
        return moveGL(T(p.x) - r.w / 2, T(p.y) - r.w / 2, r.w, r.h);
    }

    auto setSize(Size s) noexcept { return moveGL(r.x, r.y, (T)s.w, (T)s.h); }

    auto resize(Size s) noexcept { return setSize(s); }


    // Self* resize(Cord w, Cord h) { return resize({w,h}); }

    //
    // Scissor
    //
    auto setScissor(RectA rect) noexcept {
        scissorFlag_ = true;
        scissorRect_ = rect.convert<RectGL>();
        return this;
    }

    auto clearScissor() noexcept {
        scissorFlag_ = false;
        return this;
    }

    auto setScissor(bool flag, RectA rect) noexcept {
        if (flag)
            return setScissor(rect);
        return clearScissor();
    }


    //
    // Textures
    //
    auto setTexture(TextureA tex, Index index = {}) noexcept {
        type = Type::Texture;

        textureFlag_ = true;
        texture_ = tex;
        textureIndex_ = (TextureIndex)index;

        return this;
    }

    auto setRectangleTexture(TextureA tex, Index index = 0) noexcept {
        textureFlag_ = true;
        texture_ = tex;
        textureIndex_ = (TextureIndex)index;
        return this;
    }

    auto setTriangleTexture(TextureA tex, Point texA, Point texB, Point texC) noexcept {
        texture_ = tex;
        textureIndex_ = 0;
        textureFlag_ = true;
        texturePoints_[0] = texA.convert<PointGL>();
        texturePoints_[1] = texB.convert<PointGL>();
        texturePoints_[2] = texC.convert<PointGL>();
        return this;
    }

    auto clearTexture() noexcept {
        textureFlag_ = false;
        return this;
    }

    TextureA texture() const noexcept { return texture_; }
    [[nodiscard]] const auto& getTexture() const noexcept { return texture_; }

    auto setTextureIndex(Index idx) noexcept {
        textureIndex_ = (TextureIndex)idx;
        return this;
    }
    auto textureIndex() const noexcept { return textureIndex_; }

    auto setTextureScale(Real s) noexcept {
        textureScale_ = (T)s;
        return this;
    }


    //
    // Tiling and repetitions
    //
    auto setTiled(bool f = true) noexcept {
        tiledHorz_ = tiledVert_ = f;
        return this;
    }
    auto setTiledHorz(bool f = true) noexcept {
        tiledHorz_ = f;
        return this;
    }
    auto setTiledVert(bool f = true) noexcept {
        tiledVert_ = f;
        return this;
    }

    // Blurring
    auto setBlur(Cord blurDistance, Cord aBlurOpasity) {
        blurFlag_ = true;
        blur = (RealGL)blurDistance;
        blurOpacity = (RealGL)aBlurOpasity;
        return this;
    }

    auto clearBlur() {
        blurFlag_ = false;
        return this;
    }



    //
    // Colors
    //
    auto clearColor()  noexcept { colorFlag_ = false;  return this; }

    auto clearColors() noexcept {
        colorFlag_ = colorFlag2_ = colorFlag3_ = colorFlag4_ = false;
        return this;
    }

    auto clearGradient() noexcept { return clearColors(); }

    auto setColor(ColorA c) noexcept {
        colorFlag_ = true;
        color_ = c;
        colorFlag2_ = colorFlag3_ = colorFlag4_ = false;
        return this;
    }

    auto setColors(ColorA ac, ColorA bc) noexcept {
        colorFlag_ = true;
        colorFlag2_ = true;
        color_ = ac;
        color2_ = bc;

        colorFlag3_ = colorFlag4_ = false;

        return this;
    }


    auto setColors(ColorA ac, ColorA bc, ColorA cc) noexcept
    {
        colorFlag_ = true;
        colorFlag2_ = true;
        colorFlag3_ = true;
        color_ = ac;
        color2_ = bc;
        color3_ = cc;

        colorFlag4_ = false;

        return this;
    }

    auto setColors(ColorA ac, ColorA bc, ColorA cc, ColorA dc) noexcept {
        colorFlag_ = true;
        color_ = ac;

        colorFlag2_ = true;
        color2_ = bc;

        colorFlag3_ = true;
        color3_ = cc;

        colorFlag4_ = true;
        color4_ = dc;

        return this;
    }

    auto setColors(ColorGradientA colors) noexcept {
        return setColors(colors[0], colors[1], colors[2], colors[3]);
    }

    auto setGradient(ColorGradientA gradient) noexcept { return setColors(gradient); }

    auto setColors(bool enabled, ColorGradientA colors) noexcept {
        return enabled ? setColors(colors) : clearColors();
    }



    //[[deprecated]] ColorA color() const noexcept { return color_; }
    [[nodiscard]] const auto& getColor() const noexcept { return color_; }

    //[[deprecated]] auto colorFlag() const  noexcept { return colorFlag_; }
    [[nodiscard]] auto isColor() const noexcept { return colorFlag_; }




    //
    // Box
    //
    auto setBox(RectA rect) noexcept {
        type = Type::Box;
        r = rect.convert<RectGL>();
        return this;
    }


    // Rectangle
    auto setRect(RectA rect) noexcept {
        type = Type::Rect;
        r = rect.convert<RectGL>();
        return this;
    }

    //
    // Points
    //
    auto setPoint(Point p) noexcept {
        type = Type::Point;
        r.x = (T)p.x, r.y = (T)p.y;
        return this;
    }


    //
    // Line
    //
    //auto setLine(MFloat ax, MFloat ay, MFloat bx, MFloat by)  noexcept {
    //    type = Type::Line;
    //    r = {ax, ay, bx - ax, by - ay};
    //    return this;
    //}

    auto setLine(Point a, Point b) noexcept { 
        type = Type::Line;
        r = {(T)a.x, (T)a.y, (T)(b.x - a.x), (T)(b.y - a.y)};
        return this;
    }


    //
    // Bezier curve
    //
    auto setBezier(Point a, Point b, Point c, Point d) noexcept {
        type = Type::Bezier;
        r.x = (T)a.x, r.y = (T)a.y;
        r.w = (T)d.x, r.h = (T)d.y;
        tri[0] = {(T)b.x, (T)b.y};
        tri[1] = {(T)c.x, (T)c.y};
        return this;
    }

    // TODO: better points
    Self* setBezier(const Point* b)  noexcept { return setBezier(b[0], b[1], b[2], b[3]); }

    //
    // circle
    //
    auto initCircle(Point _c, Cord _radius)  noexcept {
        auto c = _c.convert<PointGL>();
        auto rr = (T)_radius;
        type = Type::Circle;
        tri[0].x = c.x, tri[0].y = c.y;
        radius_ = rr;
        r.x = c.x - rr, r.y = c.y - rr, r.w = rr * 2, r.h = rr * 2;
        return this;
    }

    auto setCircle(Point center, Cord radius) noexcept { return initCircle(center, radius); }

    auto setDisk(Point c, Cord radius) {
        initCircle(c, radius);
        type = Type::Disk;
        return this;
    }

    auto setSegments(M::Orientation segBegin, M::Orientation segEnd) noexcept {
        beginSegment_ = segBegin;
        endSegment_ = segEnd;
        return this;
    }

    auto setSegment(Point c, Cord radius, M::Orientation segBegin, M::Orientation segEnd) noexcept {
        return setDisk(c, radius)->setSegments(segBegin, segEnd);
    }

    auto setArc(Point c, Cord radius, M::Orientation segBegin, M::Orientation segEnd)  noexcept {
        return initCircle(c, radius)->setSegments(segBegin, segEnd);
    }

    auto setRadius(Cord rad) noexcept {
        if (rad == 0) {
            radiusFlag_ = false;
            return this;
        }
        radiusFlag_ = true;
        radius_ = static_cast<T>(rad);
        radiusRightTop_ = static_cast<T>(rad);
        radiusRightBottom_ = static_cast<T>(rad);
        radiusLeftBottom_ = static_cast<T>(rad);
        return this; 
    }

    auto setRadiuses(Cord leftTop, Cord rightTop, Cord rightBottom, Cord leftBottom) noexcept { 
        radiusFlag_ = true;
        radius_ = (T)leftTop; 
        radiusRightTop_ = (T)rightTop;
        radiusRightBottom_ = (T)rightBottom;
        radiusLeftBottom_ = (T)leftBottom;
        return this; 
    }

    auto clearRadius() noexcept { 
        radiusFlag_ = false;
        radius_ = 0; 
        return this;  
    }

    //
    // Triangle
    //
    auto setTriangle(Point a, Point b, Point c) noexcept {
        type = Type::Triangle;
        tri[0] = a.convert<PointGL>();
        tri[1] = b.convert<PointGL>();
        tri[2] = c.convert<PointGL>();
        return this;
    }

    template <class Container>
    auto setTriangle(const Container& pts) noexcept {
        return setTriangle(pts[0], pts[1], pts[2]);
    }

    Triangle getTriangle() const noexcept {
        Triangle t{};
        t[0] = {tri[0].x, tri[0].y};
        t[1] = {tri[1].x, tri[1].y}; 
        t[2] = {tri[2].x, tri[2].y};
        return t;
    }



    //
    // Model
    //
    auto setModel(ModelP m) noexcept {
        type = Type::Model;
        model = m;
        return this;
    }

};


using ScenePrimsContainer = std::vector<Prim>;
using ScenePrimsContainerP = ScenePrimsContainer*;



// Optional class 
// TODO: try see if it works
class MPrimOpt {
    PrimP  prim{};

    MPrimOpt(): prim(nullptr) {}
    explicit MPrimOpt(PrimP p): prim(p) {}

};

}  // namespace mangl

