/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_render.h"

#include "../mangl_environment.h"
#include "../mangl_scene.h"

#include "mangl_pl_util_global.h"

#include <apeal/image.hpp>

#include "../mangl_config.h"
#include "../mangl_framework.h"

namespace mangl {

SceneRenderer::SceneRenderer() {
    using namespace render;

    sceneRender_ = this;
    scenes_ = new ScenesContainer;
}

void SceneRenderer::onInit()
{
    //textures_paused_ = false ;
    transparent = false;
    scenePrepared_ = false;
#if APEAL_OS_ANDROID
    android_current_texture_id_ = 1;
#endif

#if APEAL_OS_IPHONE && IOS_TIMER_THREADED
    texture_release_all_queue_ = false;
    texture_reload_queue_ = false;
#endif
}

void SceneRenderer::onRelease()
{
    release();
}

void SceneRenderer::release()
{
#if MANGL_OPENGL
    destroyShaders();
#endif
    
    releaseTexturesAll();
}


SceneP SceneRenderer::newScene() {
    auto scene = new Scene();
    render::scenes_->push_back(scene);
    return scene;
}

void SceneRenderer::deleteScene(SceneP scene)
{
    using namespace render;

    for (auto it = scenes_->begin(), end = scenes_->end(); it != end; ++it) {
        if (*it == scene) {
            scene->release();
            scenes_->erase(it);
            delete scene;
            break;
        }
    }
}

std::vector<SceneP> &SceneRenderer::scenes() noexcept {
    return *render::scenes_;
}

void SceneRenderer::swapScenes(SceneP scene1, SceneP scene2)
{
    using namespace render;

    auto it1 = scenes_->end(), it2 = it1;
    for (auto it = scenes_->begin(), end = scenes_->end(); it != end; ++it) {
        if (*it == scene1)
            it1 = it;
        if (*it == scene2)
            it2 = it;
    }

#if _DEBUG
    if (it1 == scenes_->end() || it2 == scenes_->end()) {
        throw std::runtime_error("swapScenes - invalid scene passed");
    }
#endif

    std::swap(*it1, *it2);
}


//void SceneRenderer::setViewport(RectA viewport)
//{
//    screenViewport_ = viewport.convert<RectGL>();
//
//    if (scenePrepared_) {
//        prepareViewport();
//    }
//}


void SceneRenderer::onTimer(Timestamp timestamp) noexcept
{
    // Run the scene
    for (auto scene: *render::scenes_) {
        if (!scene->visible_)
            continue;
        scene->onTimer(timestamp);
    }
}

bool SceneRenderer::onRenderMaintenance() noexcept
{
    onRenderTexturesMaintenance();
    return true;
}

void SceneRenderer::updateLayout() {
    Rect viewport{};

    if (Env::isDeviceDesktop()) {
        //viewport.setSize(Env::applicationRect.size() * Env::pixelDensity);
        viewport.setSize(Env::applicationRect.size());
    } else {
        viewport.setSize(Env::physScreenSize);
    }


    screenScale_ = static_cast<RealGL>(Env::screenScale);
    screenViewport_ = viewport.convert<RectGL>();
    layoutViewport_ = screenViewport_ / screenScale_;

    if (scenePrepared_) {
        prepareViewport();
    }


    //setViewport(viewport);

    //MDebugPrint("updateLayout() layout: %.0z, scale: %f, screenvp: %.0z, layoutvp: %.0z", Env::layoutSize, screenScale_, screenViewport_, layoutViewport_);

}




void SceneRenderer::prepareViewport()
{
    using namespace render;

    //updateLayout();

    if (ManglConfig::debugPrintGraphics) {
        MDebugFunc("viewport: %.0z; screenLayout: %.0z; scale: %.2f",
                   screenViewport_.size(),
                   layoutViewport_.size(),
                   screenScale_);
    }
   
    
    RealGL left{}, right{layoutViewport_.w};
    RealGL top{layoutViewport_.h}, bottom{};
    
    zFarOrtho_ = -std::max(layoutViewport_.w, layoutViewport_.h);
    zNearOrtho_ = std::max(layoutViewport_.w, layoutViewport_.h);
    
    
    orthoProjMatrix_.ortho(left, right, bottom, top, -zNearOrtho_, -zFarOrtho_);
    orthoProjMatrix_.updateHash();
    
    zFarPerspective_ = zFarOrtho_;
    zNearPerspective_ = zFarOrtho_ / 4;
    
    fovPerspective_ = piGL_ / 4;
    aspectRatioPerspective_ = (right - left) / (top - bottom);
    bottomNearPerspective_ = -std::fabs(zNearPerspective_ * std::tan(fovPerspective_/2));
    leftNearPerspective_ = bottomNearPerspective_ * aspectRatioPerspective_;
    
    perspectProjMatrix_.perspective(fovPerspective_, aspectRatioPerspective_, -zNearPerspective_, -zFarPerspective_);
    perspectProjMatrix_.updateHash();
    
    
    currentProjMtx_ = orthoProjMatrix_;
    
    defaultViewMtx_.eye();
    defaultViewMtx_.updateHash();
    
    defaultModelMtx_.eye();
    defaultModelMtx_.updateHash();
    

    vtxPosRange_ = std::max(layoutViewport_.w, layoutViewport_.h);
    // Add some offscreen rendering
    vtxPosRange_ += vtxPosRange_;
    //vtxPosRange_ += vtxPosRange_ / 2;

    if constexpr (std::is_integral_v<RenderPositionCoord>) {
        vtxPosRegularMaxVal_ = std::numeric_limits<RenderPositionCoord>::max();
    } else {
        vtxPosRegularMaxVal_ = std::numeric_limits<RealGL>::max();
    }

    vtxPosCurrentMaxVal_ = vtxPosRegularMaxVal_;
    
    vtxPosMaxRange_ = vtxPosRegularMaxVal_ / vtxPosRange_;
#if MANGL_OPENGL
    vtxPosRegularScale_ = vtxPosRegularMaxVal_ / vtxPosMaxRange_;
#else
    vtxPosRegularScale_ = 1;
#endif
    
    vtxPosRegularRecip_ = 1 / vtxPosRegularScale_;


    vtxPosCurrentScale_ = vtxPosRegularScale_;
    vtxPosScaleRecip_ = vtxPosRegularRecip_;


#if MANGL_OPENGL
    prepareViewportOpenGL();
#elif MANGL_METAL
    prepareViewportMetal();
#endif
}

void SceneRenderer::prepareScene() {
#if MANGL_OPENGL
    prepareSceneOpenGL();
#elif MANGL_METAL
    prepareSceneMetal();
#endif
}

void SceneRenderer::destroyScene() {
#if MANGL_OPENGL
    destroySceneOpenGL();
#elif MANGL_METAL
//    destroySceneMetal();
#endif

}


void SceneRenderer::processRenderLayoutChange(RectA layout) {
    Env::applicationRect = layout;

    // Logical screen size remains the same.
    // Physical changes
    // Env::physScreenSize = layout.size();
    // Env::logicalScreenSize = Env::physScreenSize / Env::pixelDensity;
    //Env::pixelDensity = Env::physScreenSize.w / Env::logicalScreenSize.w;

    //Env::processScreenLayout();

    Env::screenScale = Env::applicationRect.w / Env::layoutSize.w;
    Env::layoutPadding = Env::screenPadding / Env::screenScale;


    Env::updateConversionData();

    Framework::instance()->onViewport();
}

// TODO: move somewhere else
Rect SceneRenderer::screenshotCurrentApplicationLayout_{};


void SceneRenderer::takeScreenshot() {
    screenshotScheduled_ = true;

    screenshotSize_ = Env::screenshotSize;

    if (!screenshotSize_)
        screenshotSize_ = Env::applicationRect.size();

    screenshotCurrentApplicationLayout_ = Env::applicationRect;
    processRenderLayoutChange({.w = screenshotSize_.w, .h = screenshotSize_.h});


#if MANGL_OPENGL

    using namespace opengl;

    RenderEngineOpenGL::screenshotScheduled = true;
    RenderEngineOpenGL::screenshotSize = screenshotSize_.convert<SizeI>();


    /*
    auto x = static_cast<GLsizei>(screenViewport_.x);
    auto y = static_cast<GLsizei>(screenViewport_.y);
    auto width = static_cast<GLsizei>(screenViewport_.w);
    auto height = static_cast<GLsizei>(screenViewport_.h);

    int bytesPerRow = width * 4;

    auto image = Image::make(ColorModel::RGBA, {width, height});

    ByteBuffer data;
    data.alloc(height * bytesPerRow);


    glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    // flip vertical
    auto srcPtr = data.data();
    auto dstPtr = image.data() + (height - 1) * bytesPerRow;

    for (int row = 0; row < height; ++row) {
        std::memcpy(dstPtr, srcPtr, bytesPerRow);

        srcPtr += bytesPerRow;
        dstPtr -= bytesPerRow;
    }

    saveImage(filepath, image);
    */

#endif
}

void SceneRenderer::processTakenScreenshot(const void* data, apl::ColorModel cm, int width, int height) {
    using namespace apl;

    processRenderLayoutChange(screenshotCurrentApplicationLayout_);

    auto time = getLocalTime();
    auto timestamp = formatTimestamp(time);
    auto filename = fmt("screenshot_%s-%dx%d-%s.png", Env::appName, width, height, timestamp);
    auto filepath = makeFilePath(Env::docDir, filename);

    mkdirIfNotExistsParents(Env::docDir);


    //
    // Flip vertical
    //
#if 0  // Just flip
    auto image = Image::make(ColorModel::RGBA, {width, height});
    auto srcPtr = static_cast<const uint8_t*>(data);
    auto dstPtr = image.data() + (height - 1) * image.bytesPerRow;

    for (int row = 0; row < height; ++row) {
        std::memcpy(dstPtr, srcPtr, image.bytesPerRow);

        srcPtr += image.bytesPerRow;
        dstPtr -= image.bytesPerRow;
    }
#endif

    //
    // Flip vertical and convert to RGB
    //
    auto image = Image::make(ColorModel::RGB, {width, height});
    auto srcBytesPerRow = 4 * width;
    auto srcPtr = static_cast<const uint8_t*>(data);
    auto dstPtr = image.data() + (height - 1) * image.bytesPerRow;
    for (int row = 0; row < height; ++row) {
        auto src = srcPtr;
        auto dst = dstPtr;
        for (int col = 0; col < width; ++col) {
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            ++src;
        }

        srcPtr += srcBytesPerRow;
        dstPtr -= image.bytesPerRow;
    }


    //auto rgb = Image::make(ColorModel::RGB, {width, height});

    //image.convert(rgb);

    //auto rgb = image.clone(ColorModel::RGB);


    saveImage(filepath, image);
    //saveImage(filepath, rgb);

}


}  // namespace mangl
