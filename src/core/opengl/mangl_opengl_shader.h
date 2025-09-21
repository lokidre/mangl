/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_common.h"

#if MANGL_OPENGL

#include "mangl_opengl.h"

namespace mangl::opengl {


struct ShaderVarBase {
    using GlSlotType = GLint;
    //using GlSlotType = GLuint;

    constexpr static auto InvalidSlot = static_cast<GlSlotType>(-1);
    GlSlotType slot{InvalidSlot};

    [[nodiscard]] bool isValid() const noexcept {  return slot != InvalidSlot;  }

    virtual void init() = 0;
    explicit ShaderVarBase(const char *iname) noexcept: name{iname}{}

    const char *name;
    bool transposeSupported{};
    
protected:
    ~ShaderVarBase() noexcept = default;
};


template <typename T>
struct Uniform: ShaderVarBase {
    virtual ~Uniform() = default;

    Uniform() = delete;
    Uniform(const char *iname, const T& iinitValue) noexcept:
        ShaderVarBase{iname}, initValue{iinitValue} {}

    T value{}, initValue{};

    const T &operator () () noexcept { return value; }
};

template <typename T>
struct Attribute: ShaderVarBase {
    virtual ~Attribute() = default;

    Attribute() = delete;
    explicit Attribute(const char *name) noexcept: ShaderVarBase{name} {}
};



//
// Shader Uniform Types
//

struct UniInt final: Uniform<int> {
    using T = int;

    UniInt() = delete;
    explicit UniInt(const char *n, T i = 0) noexcept: Uniform<T>(n, i) {}

    void set(T v) noexcept {
        if (InvalidSlot == slot || v == value)
            return;
        glUniform1i(slot, v); ManglAssertOpenGL();
        value = v;
    }

    UniInt& operator=(T v) noexcept { set(v); return *this; }

    void init() override {
        glUniform1i(slot, initValue); ManglAssertOpenGL();
        value = initValue;
    }
};

struct UniFloat final: Uniform<GLfloat> {
    using T = GLfloat;

    UniFloat() = delete;
    explicit UniFloat(const char *n, T i = {}): Uniform<T>{n, i} {}

    void set(T v) noexcept
    {
        if (InvalidSlot == slot || v == value)
            return;
        glUniform1f(slot, v); ManglAssertOpenGL();
        value = v;
    }

    UniFloat& operator = (T v) noexcept { set(v); return *this; }
    bool operator != (T v) const noexcept { return value != v; }

    void init() override {
        glUniform1f(slot, initValue); ManglAssertOpenGL();
        value = initValue;
    }
};

struct UniBool final: Uniform<bool> {
    using Self = UniBool;
    using T = bool;

    UniBool() = delete;
    explicit UniBool(const char *n, T i = {}): Uniform<T>{n, i} {}
    void set(T v) noexcept {
        if (InvalidSlot == slot || v == value)
            return;
        glUniform1i(slot, v ? 1 : 0); ManglAssertOpenGL();
        value = v;
    }

    explicit operator bool() const noexcept { return value; }
    UniBool& operator=(T v) noexcept { set(v); return *this; }

    void init() override { 
        glUniform1i(slot, initValue ? 1 : 0);   ManglAssertOpenGL();
        value = initValue; 
    }
};

struct UniColor final: Uniform<ColorHashed> {
    using T = ColorHashed;

    UniColor() = delete;
    explicit UniColor(const char *name, const T &i = {1,1,1,1}): Uniform{name, i} {
        invalidColorValue_.updateHash();
    }

    void set(const T& c) noexcept {
        // check for invalid value
        if (InvalidSlot == slot)
            return;
        if (c.valid() != isColorValid_ && isColorValid_ ) {
            isColorValid_ = false;
            value = invalidColorValue_;
            set(invalidColorValue_);
            return;
        }

        if (c.hash == value.hash)
            return;
        value = setColorValue(c);
    }
    UniColor& operator = (const T &v) noexcept { set(v); return *this; }
    bool operator != (const T &v) const noexcept { return value != v; }

    void init() override { value = setColorValue(initValue); }

private:
    const T& setColorValue(const T &v) const {
        glUniform4f(slot, v.r, v.g, v.b, v.a); ManglAssertOpenGL();
        return v;
    }

    bool isColorValid_{false};

    // invalid value is treated as totally black
    ColorHashed invalidColorValue_{0,0,0,0};

};

struct UniVec2 final: public Uniform<PointGL> {
    using Vec2 = PointGL;
    using T = Vec2;

    UniVec2() = delete;
    explicit UniVec2(const char *name): Uniform(name, {}) {}

    void set(PointGL c) noexcept {
        if (InvalidSlot == slot || c == value)
            return;
        glUniform2f(slot, c.x, c.y); ManglAssertOpenGL();
        value = c;
    }

    void operator = (T v) noexcept { set(v); }
    bool operator != (const T &v) const noexcept { return value != v; }

    void set(SizeGL s) noexcept { set(T{s.w,s.h}); }
    void operator = (SizeGL s) noexcept  { set(T({s.w,s.h})); }

    void init() override {
        auto ptr = initValue.data();
        glUniform2f(slot, ptr[0], ptr[1]); ManglAssertOpenGL();
        value = initValue;
    }
};

struct UniVec2I final: Uniform<PointI> {
    using Vec2 = PointI;
    using T = Vec2;

    UniVec2I() = delete;
    UniVec2I(const char *name): Uniform<T>(name, {}) {}

    void set(T c) noexcept {
        if (InvalidSlot == slot || c == value)
            return;
        glUniform2i(slot, c.x, c.y); ManglAssertOpenGL();
        value = c;
    }

    void operator = (T v) noexcept { set(v); }
    bool operator != (const T &v) const noexcept { return value != v; }

    void init() override {
        auto ptr = initValue.data();
        glUniform2i(slot, ptr[0], ptr[1]); ManglAssertOpenGL();
        value = initValue;
    }
};



struct UniVec3 final: Uniform<Point3DGL> {
    using Vec3 = Point3DGL;
    using T = Vec3;

    UniVec3() = delete;
    UniVec3(const char *name) noexcept: Uniform<T>(name, {}) {}
    void set(T c) noexcept {
        if (InvalidSlot == slot || c == value)
            return;
        setImpl(c);
    }

    void operator = (T v) noexcept { set(v); }
    bool operator != (T v) const noexcept { return value != v; }

    void setImpl(T c) noexcept {
        glUniform3f(slot, c.x, c.y, c.z); ManglAssertOpenGL();
        value = c;
    }
    
    void init() override {
        auto ptr = initValue.data();
        glUniform3f(slot, ptr[0], ptr[1], ptr[2]);  ManglAssertOpenGL();
        value = initValue;
    }
};

struct UniMat4 final: public Uniform<MatrixHashedGL> {
private:
    using Self = UniMat4;

public:
    using T = MatrixHashedGL;

    constexpr static int Width = 4;
    constexpr static int Height = 4;

    UniMat4() = delete;
    UniMat4(const char *name) noexcept: Uniform<T>{name, {}} {
        identityMatrix_.eye();
        identityMatrix_.updateHash();

        initValue = identityMatrix_;
    }
    void setIdentity() noexcept {
        setUniformMatrix(identityMatrix_);
        isIdentity_ = true;
    }

    bool identity() const noexcept { return isIdentity_; }

    //void setScale(bool scaleFlag, MFloat scaleValue = 1) noexcept {
    //    scaleFlag_ = scaleFlag;
    //    scaleValue_ = scaleValue;
    //}

    void set(const T& m) noexcept {
        if (hash == m.hash)
            return;

        setUniformMatrix(m);
        hash = m.hash;
        isIdentity_ = false;
    }

    void operator=(const T& v) noexcept { set(v); }
    void operator=(const T* p) noexcept { set(*p);  }

    void operator=(std::nullptr_t) {   MDebugTrap();  }
    void operator=(bool) {  MDebugTrap(); }

    void init() override {
        setUniformMatrix(initValue); ManglAssertOpenGL();
        value = initValue;
    }

    void setUniformMatrix(const T &m) noexcept {
        if (InvalidSlot == slot)
            return;

        if (transposeSupported) {
            
            glUniformMatrix4fv(slot, 1, GL_TRUE, m.data()); ManglAssertOpenGL();
            
        } else { // Transpose and convert
            
            auto a = m.data();
            for (int j = 0; j < Height; ++j) {
                auto d = matrixBuffer_ + j;
                for (int i = 0; i < Width; ++i, ++a, d += Width)
                    *d = *a;
            }
            glUniformMatrix4fv(slot, 1, GL_FALSE, matrixBuffer_); ManglAssertOpenGL();
        }
    }


    MatrixHashedGL identityMatrix_;
    bool isIdentity_{false};
    Real scaleValue_{};
    Hash hash{};

    GLfloat matrixBuffer_[Width * Height];
};

struct ArrayAtt: public Attribute<int> {
    ArrayAtt() = delete;
    ArrayAtt(const char *name, int loc) noexcept: Attribute<int>{name} {
        slot = loc;
    }
    void init() override {
        arrayEnabled = false;
    }
    void enable(bool f = true) noexcept {
        if (f == arrayEnabled)
            return;
        if (f) {
            glEnableVertexAttribArray(slot);
        } else {
            glDisableVertexAttribArray(slot);
        }
        arrayEnabled = f;
    }

    void disable() noexcept { enable(false); }

private:
    bool arrayEnabled = false;
};


struct ShaderProgram {

    GLuint handle{};

    //ShaderInfo shaders[2];  // Vertex, Fragment
    GLuint shaderHandles[2]{};


    // Vertex Array Attributes
    constexpr static int VtxPosSlot = 0;
    constexpr static int VtxClrSlot = 1;
    constexpr static int VtxTexSlot = 2;
    constexpr static int VtxNrmSlot = 3;

    ArrayAtt vtx_pos{"in_vtx_pos", VtxPosSlot};
    ArrayAtt vtx_clr{"in_vtx_clr", VtxClrSlot};
    ArrayAtt vtx_tex{"in_vtx_tex", VtxTexSlot};
    ArrayAtt vtx_nrm{"in_vtx_nrm", VtxNrmSlot};

    // Vertex Array helper
    UniFloat vtx_pos_scale{"vtx_pos_scale", 1};//, vtx_nrm_scale{"vtx_nrm_scale", 1};


    // Rendering primitive type
    UniInt prim_type{"prim_type"};

    // Common sceen status
    UniFloat screen_scale{"screen_scale"};

    // Environment
    UniVec2 viewport{"viewport"};


    // Model/View/Projection matrices
    UniMat4 model_mtx{"model_mtx"};
    UniMat4 view_mtx{"view_mtx"};
    UniMat4 proj_mtx{"proj_mtx"};

    // Scene parameters
    UniColor screen_color_effect{"scene_color_effect", {1, 1, 1, 1}};

    // Textures
    UniBool tex_enabled{"tex_enabled"};
    UniInt tex_sampler{"tex_sampler"};

    // Lighting
    UniBool shading_enabled{"shading_enabled"};
    UniVec3 light_pos{"light_pos"};

    // Material support
    //UniColor ambientMaterial{"ambientMaterial"};
    UniColor spec_mtl{"spec_mtl"}, diff_mtl{"diff_mtl"};
    UniFloat shininess{"shininess"};

    // Point primitive
    //UniFloat pointSize{"pointSize", 1};

    // Line primitive
#if 0
    UniFloat line_width{"line_width"};
    UniFloat line_edge_inner{"line_edge_inner"};
    UniFloat line_edge_outer{"line_edge_outer"};
#else
    UniVec3 line_params{"line_params"};
#endif


    // Fog
    UniBool fog_enabled{"fog_enabled"};
    UniColor fog_color{"fog_color"};
#if 0
    UniInt fog_type{"fog_type"};  // 0: linear, 1: exponential, 2: exp squared
    UniInt fog_depth{"fog_depth"};  // 0: z plane, 1: distance
    UniFloat fog_density{"fog_density"};
    UniFloat fog_near{"fog_near"}, fog_far{"fog_far"};
#else
    UniVec2I fog_iparams{"fog_iparams"};
    UniVec3 fog_params{"fog_params"};
#endif

    ShaderPermutation permutation{};

    explicit ShaderProgram(ShaderPermutation pmu) noexcept {
        permutation = pmu;

        // Register attributes
        //regAtt(vtx_pos);
        //regAtt(vtx_clr);
        //if (pmu.bits.tex)
        //    regAtt(vtx_tex);
        //if (pmu.bits.shading)
        //    regAtt(vtx_nrm);

        // Attribute processing
        regUni(vtx_pos_scale);


        //regUni(prim_type);
        //regUni(tex_enabled);


        // Register uniforms
        regUni(screen_scale);
        regUni(viewport);
        regUni(model_mtx);
        regUni(view_mtx);
        regUni(proj_mtx);

        regUni(screen_color_effect);

        if (pmu.bits.line || pmu.bits.point) {
            regUni(line_params);
        }

        if (pmu.bits.tex) {
            regUni(tex_sampler);
        }

        if (pmu.bits.fog) {
            regUni(fog_color);
            regUni(fog_iparams);
            regUni(fog_params);
        }

        if (pmu.bits.shading) {
            regUni(light_pos);

            regUni(diff_mtl);
            regUni(spec_mtl);
            regUni(shininess);
        }
    }

    void regUni(ShaderVarBase& var) noexcept {
        uniforms.push_back({var.name, std::addressof(var)});
    }

    void regAtt(ShaderVarBase& var) noexcept {
        attributes.push_back({var.name, std::addressof(var)});
    }

    void clearSetup() noexcept {
        uniforms.clear();
        uniforms.shrink_to_fit();
        attributes.clear();
        attributes.shrink_to_fit();
//        std::vector<VarInfo> tuni, tatt;
//        uniforms.swap(tuni);
//        attributes.swap(tatt);
    }

    struct VarInfo {
        const char *name;
        ShaderVarBase *var;
    };

    std::vector<VarInfo> uniforms, attributes;
};

}  // namespace mangl::opengl

#endif
