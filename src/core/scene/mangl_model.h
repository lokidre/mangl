/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_setup.h"
#include "../mangl_color.h"
#include "../mangl_matrix.h"

#include "../mangl_texture.h"

#include <apeal/buffer.hpp>

namespace mangl {

struct Box3D {
    Real x, y, z;
    Real w, h, d;

    [[nodiscard]] auto centroid() const noexcept { return Point3D{x + w/2, y + h/2, z + d/2}; }
    [[nodiscard]] auto max() const noexcept {  return std::max(w, std::max(h, d)); }
};


//struct Scene;
class SceneRenderer;

enum class FogType: int { Linear = 0, Exp = 1, ExpSquare = 2 };
enum class FogDepth: int { Distance = 0, Range = 1 };


struct Fog {
    using T = RealGL;

    using Type = FogType;
    using Depth = FogDepth;

    bool enabled{}, stopped{};

    Type type{Type::ExpSquare};
    Depth depth{Depth::Distance};

    ColorHashed color{};
    T znear{}, zfar{}, density{1};

    void set(ColorA c, Real dens = 1, Real zn = {}, Real zf = {}) noexcept
    {
        enabled = true;
        color = c;
        znear = static_cast<T>(zn);
        zfar = static_cast<T>(zf);
        density = static_cast<T>(dens);
        if (zf != 0)
            density /= std::abs(zfar-znear);
    }
};


template <class T>
//using ModelDataVector = std::vector<T> ;
using ModelDataVector = apl::Buffer<T>;


//
// Material
//
struct Material {
    ColorHashed ambient, diffuse, specular;
    RealGL shininess{};
};

struct Model;
using ModelP = Model*;

struct Model {
    using Self = Model;

    using VertexPosition = Point3DGL;
    using TextureVertex = PointGL;


    using VertexVector = ModelDataVector<VertexPosition>;
    using TextureVertexVector = ModelDataVector<TextureVertex>;
    using StringVector = ModelDataVector<String>;
    using StrViewVector = ModelDataVector<StrView>;



    //
    // Data
    //
    VertexVector vertexPositionsOrig;
    TextureVertexVector vertexTexturesOrig;
    VertexVector vertexNormalsOrig;

    VertexVector vertexPositions;
    TextureVertexVector vertexTextures;
    VertexVector vertexNormals;

    std::vector<Material> materials;

    //
    // Elements
    //
    //using Index = int;
    using IndexVector = ModelDataVector<int>;

    struct Line {
        int v[2];  // two vertices
    };

    struct FaceVertex {
        int p, t, n;  // position, texture, normal
    };
    using FaceVertexVector = ModelDataVector<FaceVertex>;


    struct Face {
        FaceVertexVector v;
        int m;  //  material index
        bool ft, fn;  // texture flag, normal flag
    };
    using FaceVector = ModelDataVector<Face>;

    struct Polygon {
        IndexVector v;
        int m;  // material index
    };
    using PolygonVector = ModelDataVector<Polygon>;


    struct Group {
        String name;

        FaceVector faces;
        PolygonVector lines;
        IndexVector points;
    };
    using GroupVector = ModelDataVector<Group>;

    GroupVector groups;

    Texture texture{};
    int textureIndex{};


    Self &clear();
    Self &normalize(bool center = false);

    Model &scale(Cord s);

    auto getBoundingBox() noexcept {
        return boundingBoxCalculated_ ? boundingBox_ : calculateBoundingBox();
    }

    Point getProjection(Point p);

    Model &setTexture(TextureA t) { texture = t;  return *this; }

    //Model &enableLighting( bool f = true ) { lightingEnabled_ = f ; return *this ; }
    //const bool lightingEnabled() const {  return lightingEnabled_ ; } 

    Model &generateNormals();
    [[nodiscard]] bool normalsGenerated() const { return normalsGenerated_; }

    // synchronizes the data, used in rotations to avoid rounding error build up
    Model& sync(const Model& m);

private:
    bool boundingBoxCalculated_{false};
    Box3D boundingBox_{};

    bool normalsGenerated_{false};

    RealGL rotationRho_{}, rotationTheta_{}, rotationKsi_{};

    Box3D calculateBoundingBox(const VertexVector &v);

    Box3D calculateBoundingBox();

public:
    RealGL lineWidth_{1};
    RealGL pointSize_{1};

    Fog *fog{};

public:
    struct MaterialParam {
        Color ambient, diffuse, specular;
        Real shininess{};
    };

    void setMaterial(int index, const Material& material) noexcept {
        if (apl::count(materials) <= index)
            materials.resize(index + 1);
        materials[index] = material;
    }

    void setMaterial(int index, ColorA ambient, ColorA diffuse, ColorA specular, Real shininess) noexcept {
        if (apl::count(materials) <= index)
            materials.resize(index + 1);

        auto& material = materials[index];
        material.ambient = ambient;
        material.diffuse = diffuse;
        material.specular = specular;
        material.shininess = (RealGL)shininess;
    }

    void setMaterial(ColorA ambient, ColorA diffuse, ColorA specular, Real shininess) noexcept {
        setMaterial(0, ambient, diffuse, specular, shininess);
        for (auto &g : groups) {
            for (auto& f: g.faces)
                f.m = {};
            for (auto& l: g.lines)
                l.m = {};
        }
    }

    void setMaterial(const MaterialParam& parm) noexcept {
        setMaterial(parm.ambient, parm.diffuse, parm.specular, parm.shininess);
    }

    void setLineWidth(Cord width) noexcept { lineWidth_ = (RealGL)width; }
    void setPointSize(Cord size) noexcept { pointSize_ = (RealGL)size; }



public:  // Rendering options
    bool depthTest{};
    bool faceCulling{};
    MatrixHashedGLP modelMatrix{}, viewMatrix{}, projMatrix{};

    Point3DGL *lightPosition{};
    Hash lightPositionHash{};
    bool lightingEnabled{false};

    // Debugging
    bool wireframe{false}, vertices{false};

    Model& loadObj(StrView filename) { return loadObjFile(filename); }  // Obsolete
    Model& loadObjFile(StrView filename);
    Model& loadObjData(StrView contents, StrView filename = {});
};

}  // namespace mangl



