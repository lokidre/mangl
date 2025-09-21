/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_opengl.h"

#if MANGL_OPENGL

#include "../mangl_render.h"
#include "../render/mangl_pl_util_global.h"
#include "../render/mangl_pl_util_header.h"


namespace mangl {

void SceneRenderer::prepareViewportOpenGL() {

    using namespace opengl;
    using namespace mangl::render;

    RenderEngineOpenGL::cacheScreenScale_ = screenScale_;

    // Sets up matrices and transforms for OpenGL
    auto vp = screenViewport_.convert<apl::Rect<GLsizei>>();
    glViewport(vp.x, vp.y, vp.w, vp.h); ManglAssertOpenGL();

    RenderEngineOpenGL::cacheShader_ = nullptr;

    for (auto& shaderIt: programs_.programs) {
        auto shader = shaderIt.second.get();
        glUseProgram(shader->handle); ManglAssertOpenGL();

        //#if _DEBUG
        //        GLint logLength{};
        //        glGetProgramiv(shader->handle, GL_INFO_LOG_LENGTH, &logLength);  ManglAssertOpenGL();
        //
        //        if (logLength) {
        //            std::basic_string<GLchar> buffer;
        //            buffer.resize(logLength + 1);
        //
        //            glGetProgramInfoLog(shader->handle, logLength, 0, buffer.data());
        //            if (std::strlen(buffer.data())) {
        //                MDebugFunc("Shader Info::LinkProgram: %s", buffer);
        //            }
        //        }
        //#endif

        shader->model_mtx = defaultModelMtx_;
        shader->view_mtx = defaultViewMtx_;
        shader->proj_mtx = currentProjMtx_;
        shader->viewport = screenViewport_.size();
        shader->screen_scale = screenScale_;
    }

}

} // namespace mangl

#endif

