/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once


#include "mangl_types.h"

//#ifndef MANGL_CONSOLE

#include "mangl_matrix.h"
#include "mangl_texture_atlas.h"
#include "mangl_scene.h"

#include <apeal/image.hpp>

namespace mangl {

struct RenderTextureInfo;

class SceneRenderer {
public:
    SceneRenderer();

    void updateLayout();

public:
    bool scenePrepared_{};

    SceneP newScene();
    void deleteScene(SceneP scene);
    void swapScenes(SceneP scene1, SceneP scene2);
    std::vector<SceneP> &scenes() noexcept;

    RectGL screenViewport_{}, layoutViewport_{};
    RealGL zNearOrtho_{}, zFarOrtho_{};
    RealGL screenScale_{1};

    Cord fovPerspective_{}, aspectRatioPerspective_{};
    Cord leftNearPerspective_{}, bottomNearPerspective_{};
    Cord zNearPerspective_{}, zFarPerspective_{};

    void release();

    void onInit();
    void onRelease();


    //void setViewport(RectA viewport);

    [[nodiscard]] auto xLeft() const noexcept { return layoutViewport_.x; }
    [[nodiscard]] auto xRight() const noexcept { return layoutViewport_.x + layoutViewport_.w; }
    [[nodiscard]] auto yBottom() const noexcept { return layoutViewport_.y; }
    [[nodiscard]] auto yTop() const noexcept { return layoutViewport_.y + layoutViewport_.h; }
    [[nodiscard]] auto zNearOrtho() const noexcept { return zNearOrtho_; }
    [[nodiscard]] auto zFarOrtho() const noexcept { return zFarOrtho_; }
    [[nodiscard]] auto width() const noexcept { return layoutViewport_.w; }
    [[nodiscard]] auto height() const noexcept { return layoutViewport_.h; }
    [[nodiscard]] auto depth() const noexcept { return std::abs(zFarOrtho_ - zNearOrtho_); }
    [[nodiscard]] auto center3D() const noexcept {
        return Point3D{
            layoutViewport_.x + layoutViewport_.w / 2, 
            layoutViewport_.y + layoutViewport_.h / 2, 
            (zFarOrtho_ + zNearOrtho_) / 2
        }; 
    }


    [[nodiscard]] auto fovPerspective() const noexcept { return fovPerspective_; }
    [[nodiscard]] auto leftNearPerspective() const noexcept { return leftNearPerspective_; }
    [[nodiscard]] auto bottomNearPerspective() const noexcept { return bottomNearPerspective_; }
    [[nodiscard]] auto zNearPerspective() const noexcept { return zNearPerspective_; }
    [[nodiscard]] auto zFarPerspective() const noexcept { return zFarPerspective_; }

    void invalidate() { scenePrepared_ = false; }

    //
    // Textures
    //
private:
#if ANDROID
    int android_current_texture_id_ = 1;
#endif

    TextureId currentTextureId_ = 1; //0x10000;

    static RenderTextureInfo *texInfoById(TextureId tid) noexcept;
    RenderTextureInfo *texInfoCreate();


    static void texInfoForEach(const std::function<void(RenderTextureInfo *)>& f);
    static bool texInfoFindIf(const std::function<bool(RenderTextureInfo *)>& f) ;


    // Called from onRender
    void onRenderTexturesMaintenance() noexcept;


    void glLoadTexture(RenderTextureInfo *ti, TextureId textureId = 0) {
#if MANGL_OPENGL
        glLoadTextureOpenGL(ti, textureId);
#elif MANGL_METAL
        glLoadTextureMetal(ti);
#endif
    }
    void glReleaseTexture(RenderTextureInfo *ti) {
#if MANGL_OPENGL
        glReleaseTextureOpenGL(ti);
#elif MANGL_METAL
        glReleaseTextureMetal(ti);
#endif
    }

    void loadTextureData(StrView filename, apl::Image* image);

public:
    TextureId loadTexture(StrView filename);
    [[nodiscard]] static bool textureLoaded(StrView filename) ;

    static void releaseTexture(const TextureAtlas &tex) { releaseTexture(tex.textureId); }
    static void releaseTexture(TextureId tid);
    static void releaseTexturesAll();
    static void destroyTexturesAll();


    //
    // Pausing / resuming (reloading) textures
    //
    static void pauseTextures();
    static void resumeTextures();


    // Projection
    MatrixHashedGL currentProjMtx_, orthoProjMatrix_, perspectProjMatrix_;
    MatrixHashedGL defaultModelMtx_, defaultViewMtx_;

    // Rendering
    void prepareViewport();
    void prepareScene();
    void destroyScene();

    // Options
    bool transparent{false};


    //
    // Maintenance
    //
    bool onRenderMaintenance() noexcept;

    //
    // Engine callbacks
    //
    void onTimer(Timestamp timestamp) noexcept;

    void onRender() noexcept;  // The main rendering routine.
    
    
private:
    
#if MANGL_METAL
    //
    // Apple Metal stuff
    //
    void prepareViewportMetal();
    void prepareSceneMetal();
    void glLoadTextureMetal(RenderTextureInfo* ti);
    void glReleaseTextureMetal(RenderTextureInfo* ti);
#endif
    

#if MANGL_OPENGL
    //
    // OpengGL stuff
    //
    void glLoadTextureOpenGL(RenderTextureInfo* ti, TextureId textureId);
    void glReleaseTextureOpenGL(RenderTextureInfo* ti);

    void prepareSceneOpenGL();
    void destroySceneOpenGL();

    void prepareViewportOpenGL();
    
    void destroyShadersOpenGL();

    //
    // Shaders
    //
    String generateShaderHeader(int shaderType);
    String generateShaderConditions(unsigned pmu);
    unsigned compileShader(unsigned pmu, std::vector<String> &shaderSource, int shaderType);
    void loadShaders();
    void destroyShaders() {
        destroyShadersOpenGL();
    }
    
    static bool checked_sscanf(int count, const char *buf, const char *fmt, ...);
    int supportedOpengGL{320};
    int supportedGLSL{100};
    bool isGLSLES{};
    bool legacyShaders{};
    
#endif


    //
    // Screenshots
    //
public:
    void takeScreenshot();
    static void processTakenScreenshot(const void* data, apl::ColorModel cm, int width, int height);

private:
    bool screenshotScheduled_{false};
    Size screenshotSize_{};
    static Rect screenshotCurrentApplicationLayout_;

    // TODO: move to Framework
    static void processRenderLayoutChange(RectA layout);
};

}  // namespace mangl

//#endif
