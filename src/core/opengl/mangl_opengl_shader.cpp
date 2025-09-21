/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_opengl_shader.h"

#include "../mangl_common.h"

#if MANGL_OPENGL

#include "../mangl_render.h"
#include "../mangl_environment.h"
#include "../mangl_scene.h"

#include <algorithm>
#include <cctype>
#include <unordered_set>


#if MANGL_DEBUG_DYNAMIC_SHADERS
#include <apeal/file.hpp>
#endif



// Print shader loading diagnostics
//#define MANGL_PRINT_PROGRAM_DIAGS 1

#include "../render/mangl_pl_util_global.h"

#include "mangl_opengl.h"
#include "mangl_opengl_shader_sources.h"

namespace mangl {

String SceneRenderer::generateShaderHeader(int shaderType)
{
    String header;

#if _DEBUG && MANGL_DEBUG_LEGACY_SHADERS
    legacyShaders = true;
#if MANGL_DEBUG_LEGACY_SHADER_VERSION
    int effectiveGLSL = MANGL_DEBUG_LEGACY_SHADER_VERSION;
#else
    int effectiveGLSL = 120;
#endif
#else  // Release
    int effectiveGLSL = supportedGLSL;
#endif


    bool addVersion = true;

    if (legacyShaders) {
        // #version directive was introduced in 120
        if (effectiveGLSL < 120)
            addVersion = false;
    }

    if (addVersion) {
        header = apl::fmt("#version %d", effectiveGLSL);
        if (isGLSLES)
            header += " es";
        header += '\n';
        header += '\n';
    }
    
    //header += "#pragma debug(on)\n";
    //header += "#pragma optimize(off)\n";

    if (shaderType == GL_VERTEX_SHADER)
        header += "#define IS_VERTEX 1\n";

    if (shaderType == GL_FRAGMENT_SHADER)
        header += "#define IS_FRAGMENT 1\n";

    header += '\n';

    return header;
}



String SceneRenderer::generateShaderConditions(unsigned pmuhash)
{
    String cond;

    opengl::ShaderPermutation pmu{};
    pmu.hash = pmuhash;

    auto processEnabled = [&cond](StrView name, unsigned flag) {
        if (flag) {
            apl::str_format_append(cond, "#define COND_%s_ON  1\n", name);
        } else {
            apl::str_format_append(cond, "#define COND_%s_OFF 1\n", name);
        }
    };


    processEnabled("TEX", pmu.bits.tex);
    processEnabled("FOG", pmu.bits.fog);
    processEnabled("LINE", pmu.bits.line);
    processEnabled("POINT", pmu.bits.point);
    processEnabled("SHADING", pmu.bits.shading);

    cond += '\n';

    return cond;
}


unsigned SceneRenderer::compileShader(unsigned pmu, std::vector<String> &shaderSource, int shaderType)
{
    auto shaderHandle = glCreateShader(shaderType); ManglAssertOpenGL();

    if (!shaderHandle)
        return 0;

    auto header = generateShaderHeader(shaderType);
    shaderSource[0] = header;

    auto cond = generateShaderConditions(pmu);
    shaderSource[1] = cond;


    const int srcCount = apl::count(shaderSource);
    std::vector<const char *> srcPtrs;
    std::vector<int> srcLengths;

    for (auto&& src: shaderSource) {
        srcPtrs.push_back(src.c_str());
        srcLengths.push_back(apl::count(src));
    }

    glShaderSource(shaderHandle, srcCount, srcPtrs.data(), srcLengths.data()); ManglAssertOpenGL();

    glCompileShader(shaderHandle); ManglAssertOpenGL();

    GLint status{};
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status); ManglAssertOpenGL();

    if (GL_FALSE == status) {
        constexpr int bufferSize = 1024;
        GLchar buffer[bufferSize + 1];
        glGetShaderInfoLog(shaderHandle, bufferSize, nullptr, &buffer[0]);
        auto types = (shaderType == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
        mthrowError("Shader Error (%s): %s", types, buffer);
    }

    return shaderHandle;
}

void SceneRenderer::loadShaders()
{
    using namespace opengl;
    using namespace render;
    

    destroyShaders();

    struct ShaderInfo {
        std::vector<String> sources;
        GLuint type{};
        GLuint handle{};
    };

    ShaderInfo shaders[2];  // Vertex, Fragment

    shaders[0].type = GL_VERTEX_SHADER;
    shaders[1].type = GL_FRAGMENT_SHADER;

    for (auto& sh: shaders) {
        sh.sources.emplace_back();  // header
        sh.sources.emplace_back();  // conditions
    }



#if _DEBUG && MANGL_DEBUG_DYNAMIC_SHADERS
    String dir = MANGL_DEBUG_DYNAMIC_SHADERS_DIR;

    auto src = apl::loadFile(dir + "/conditions.glsl");
    for (auto& sh: shaders)
        sh.sources.push_back(src);

    src = apl::loadFile(dir + "/types.glsl");
    for (auto& sh: shaders)
        sh.sources.push_back(src);

    src = apl::loadFile(dir + "/vertex.glsl");
    shaders[0].sources.push_back(src);

    src = apl::loadFile(dir + "/fragment.glsl");
    shaders[1].sources.push_back(src);
#else
    legacyShaders = supportedGLSL < 300;

    for (auto& sh: shaders) {
        sh.sources.push_back(shader_conditions_);
        sh.sources.push_back(shader_types_);
    }

    shaders[0].sources.push_back(shader_vertex_);
    shaders[1].sources.push_back(shader_fragment_);
#endif

    std::vector<ShaderPermutation> shaderPermutations {
        {.bits = {.tri = 1}},
        {.bits = {.tri = 1, .tex = 1}},

        {.bits = {.tri = 1, .shading = 1}},
        {.bits = {.tri = 1, .tex = 1, .shading = 1}},

        {.bits = {.line = 1}},
        {.bits = {.point = 1}},
    };

    // add fog for every variation
    for (auto total = apl::count(shaderPermutations), i{0}; i < total; ++i ) {
        auto pmu = shaderPermutations[i];
        pmu.bits.fog = true;
        shaderPermutations.push_back(pmu);
    }

    //std::vector<ShaderPermutation> shaderPermutations {
    //    {.hash = 0xFFFF},
    //};


//    auto getAttribLocation = [](auto shader, auto name) {
//        auto slot = glGetAttribLocation(shader->handle, name);
//        if (slot < 0)
//            MDebugFunc("Cannot Get Shader Attribute: %s", name);
//        return slot;
//    };

    auto getUniformLocation = [](auto shader, auto szname) {
        auto sname = std::string{"un_"} + szname;
        auto name = sname.c_str();
        auto slot = glGetUniformLocation(shader->handle, name);
#if MANGL_PRINT_PROGRAM_DIAGS
        if (slot < 0) {
            MDebugFunc("Cannot Get Shader Uniform: %s", name);
        }
#endif
        return slot;
    };

    bool mtxTransposeSupported{true};
    if (isGLSLES && supportedOpengGL <= 200)
        mtxTransposeSupported = false;
        

    // Create shader for each permutation
    for (auto&& pmu: shaderPermutations ) {
        auto shader = programs_.insertShader(pmu);


        shader->handle = glCreateProgram(); ManglAssertOpenGL();
        
        apl::verify(shader->handle, "Cannot create GLSL Shader");
        
        for (auto& si: shaders) {
            si.handle = compileShader(pmu.hash, si.sources, si.type);
            glAttachShader(shader->handle, si.handle);  ManglAssertOpenGL();
        }

        shader->shaderHandles[0] = shaders[0].handle;
        shader->shaderHandles[1] = shaders[1].handle;


        // Must be called before linking
        glBindAttribLocation(shader->handle, shader->vtx_pos.slot, shader->vtx_pos.name); ManglAssertOpenGL();
        glBindAttribLocation(shader->handle, shader->vtx_clr.slot, shader->vtx_clr.name); ManglAssertOpenGL();
        glBindAttribLocation(shader->handle, shader->vtx_tex.slot, shader->vtx_tex.name); ManglAssertOpenGL();
        glBindAttribLocation(shader->handle, shader->vtx_nrm.slot, shader->vtx_nrm.name); ManglAssertOpenGL();


        glLinkProgram(shader->handle); ManglAssertOpenGL();

        GLint linkStatus{};
        glGetProgramiv(shader->handle, GL_LINK_STATUS, &linkStatus); ManglAssertOpenGL();


#if _DEBUG
        GLint logLength{};
        glGetProgramiv(shader->handle, GL_INFO_LOG_LENGTH, &logLength);  ManglAssertOpenGL();

        if (logLength) {
            std::basic_string<GLchar> buffer;
            buffer.resize(logLength + 1);

            glGetProgramInfoLog(shader->handle, logLength, 0, buffer.data());
#if MANGL_PRINT_PROGRAM_DIAGS
            if (std::strlen(buffer.data())) {
                MDebugFunc("Shader Info::LinkProgram: %s", buffer);
            }
#endif
        }

        MDebugAssert(linkStatus == GL_TRUE);
#endif
        glUseProgram(shader->handle);   ManglAssertOpenGL();


        for (auto& uni: shader->uniforms) {
            uni.var->transposeSupported = mtxTransposeSupported;
            uni.var->slot = getUniformLocation(shader, uni.name);
            uni.var->init();
        }

        shader->clearSetup();
    }
}


void SceneRenderer::destroyShadersOpenGL()
{
    using namespace render;

    for (auto& shaderIt: programs_.programs) {
        auto shader = shaderIt.second.get();

        for (auto& h: shader->shaderHandles) {
            glDetachShader(shader->handle, h);
            glDeleteShader(h);
        }

        glDeleteProgram(shader->handle);
        shader->handle = {};
    }

    programs_.programs.clear();
}

}  // namespace mangl

#endif  // OpenGL ES 2.0+



