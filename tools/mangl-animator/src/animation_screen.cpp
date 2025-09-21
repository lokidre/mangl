/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "animation_screen.h"

#include "animator_mode.h"
#include "res.h"

#include <cognit/mtx.hpp>


namespace animator {

constexpr static auto animatorTexture_ = ":tex_animator";

void AnimatorScreen::onStart()
{
    if (!app->guiTex.valid() )
        app->guiTex.load(animatorTexture_);

    resetBackground();
}


void AnimatorScreen::onRelease()
{
    app->getScene()->release();
    app->getRenderer()->releaseTexture(app->guiTex);
    app->guiTex.setValid(false);
}


void AnimatorScreen::onTimer(Timestamp timestamp)
{
    using namespace apl;

    if (timerDisabled_)
        return;

    try {
        if (switchScheduled_) {
            // 
            // Parse the background description file
            //
            animation_.clear();

            auto scene = app->getScene();
            scene->release();

            resetBackground();

            apl::FileSystem fs;

            // Parameters
            std::unordered_map<StrView, StrView> params;
            auto mode = AnimatorMode::instance();
            using RF = AnimatorMode::ResolutionFamily;
            switch (mode->currentFamily) {
            case RF::Square:		
                break;
            case RF::Banner:	
                params["BANNER"] = "1";
                break;
            case RF::Phone:
                params["PHONE"] = "1";
                Env::interfaceIdiom = InterfaceIdiom::Phone;
                break;
            case RF::Pad:
                params["PAD"] = "1";
                Env::interfaceIdiom = InterfaceIdiom::Pad;
                break;
            case RF::Desktop:
                params["DESKTOP"] = "1";
                Env::interfaceIdiom = InterfaceIdiom::Desktop;
                break;
            default:
                break;
            }

            params["ANIMATOR"] = "1";

#if MANGL_ADS && MANGL_ADS_BANNER
            params["BANNER"] = "1";

            if (env_->currentFamily == ManglEnvironment::ResolutionFamily::RES_BANNER) {
                app_->setBannerHeight(framework_->layout.h/2);
            } else {
                app_->setBannerHeight(framework_->layout.w>=640 ? 100 : 50);
            }
#endif


            if (!screenFilename_[TabIndexBackground].empty()) {

                auto&& filename = screenFilename_[TabIndexBackground];

                background_.preload(filename);

                String textureImageFile, textureAtlasFile;

                auto dirname = extractDirName(filename);

                std::vector<String> dirs;
                if (!background_.textureDir.empty())
                    dirs.push_back(background_.textureDir);
                dirs.push_back(".");


                for (auto& dir: dirs) {
                    String finalDirname{dirname};

                    if (dir != ".")
                        finalDirname = makeFilePath(dirname, dir);

                    textureImageFile = makeFilePathExt(finalDirname, background_.texture, "png");
                    textureAtlasFile = makeFilePathExt(finalDirname, background_.texture, "atlas");

                    if (apl::fileExists(textureImageFile))
                        break;
                }

                background_.setParameters(params);
                app->backgroundTex.load(textureAtlasFile);

                background_.setTexture(app->backgroundTex);
                background_.load(filename);
                background_.onStart(timestamp);
            }


            if (!screenFilename_[TabIndexBlueprint].empty()) {
                loadBlueprint();
            }

            if (!screenFilename_[TabIndexAnimation].empty()) {
                loadAnimation(timestamp, params);
            }

            if (!screenFilename_[TabIndexModel].empty()) {
                loadModel(timestamp);
            }


            switchScheduled_ = false;
        }

        //blueprintEngine.onTimer(timestamp);
        background_.onTimer(timestamp, lastTimestamp);
        animation_.onTimer(timestamp);

        lastTimestamp = timestamp;
    }
    catch (const std::exception& e) {
        timerDisabled_ = true;
        errorMessage = e.what();
        errorFlag = true;
    }
    catch (...) {
        timerDisabled_ = true;
        errorMessage = "Unhandled Exception!";
        errorFlag = true;
    }
}


void AnimatorScreen::loadBlueprint()
{
    using namespace apl;

    auto& filename = screenFilename_[TabIndexBlueprint];

    auto animator = Animator::instance();
    auto& item = animator->getCurrentItem();

    auto& engine = blueprintEngine;

    engine.reset();
    engine.load(filename);

    // prepare directory list
    std::vector<String> dirs;
    dirs.push_back("");  // Current dir
    if (!item.textureDir.empty())
        dirs.push_back(item.textureDir);


    // Load the textures
    for (auto& texIt: engine.getTextures()) {
        auto& tex = texIt.second;

        if (tex->file.front() == ':') {
            if (tex->file == animatorTexture_) {
                engine.setTexture(tex->name, &app->guiTex);
                continue;
            }
        }

        for (auto& dir: dirs) {
            auto atlasFilePath = makeFilePathExt(dir, tex->file, M::DefaultTextureAtlasExt);
            if (fileExists(atlasFilePath)) {
                TextureAtlas* atlas{};

                // Check if texture is already loaded
                // TODO: check for file change time
                //       for now just reload all over again
                if (auto t = loadedTextures_.find(atlasFilePath); t != loadedTextures_.end()) {
                    t->second->release();
                    loadedTextures_.erase(t);
                }

                if (!atlas) {
                    atlas = new TextureAtlas;
                    atlas->load(atlasFilePath);
                    loadedTextures_.insert({atlasFilePath, atlas});
                }

                engine.setTexture(tex->name, atlas);
                break;
            }
        }


        //atlasFile = makeFilePathExt(finalDirname, animationTextureFile, M::DefaultTextureAtlasExt);

        //if (fileExists(atlasFile))
        //    break;

    }


    engine.setViewport(Env::getFullArea(), Env::getSafePadding());
    engine.create(app->getScene());
    engine.start();
}


void AnimatorScreen::loadAnimation(Timestamp timestamp, std::unordered_map<StrView, StrView>& params)
{
    using namespace apl;

    auto& filename = screenFilename_[TabIndexAnimation];

    auto layout = Env::fullLayout();
    auto viewport = layout;
    //auto viewport = Env::applicationRect;
    //viewport.move({0, 0});

    auto scene = app->getScene();


    // Get the texture file used in animation
    animation_.clear();
    animation_.setParameters(params);


    animation_.preload(filename);
    auto animationTextureFile = animation_.atlasFile;

    // Set up the animation
    animation_.clear();
    animation_.setParameters(params);


#if MANGL_ADS && MANGL_ADS_EMBED
    if (env_->currentFamily == ManglEnvironment::ResolutionFamily::BANNER) {
        viewport.h /= 2;
        viewport.y = viewport.h;
    }
#endif

    animation_.setViewport(viewport, Env::layoutPadding);
    animation_.load(filename);


    if (!animationTextureFile.empty() && animationTextureFile != animatorTexture_) {
        String atlasFile;
        if (animationTextureFile.front() != ':') {
            auto dirname = extractDirName(filename);
            std::vector<String> dirs;
            if (!animation_.textureDir.empty())
                dirs.push_back(animation_.textureDir);
            dirs.push_back(".");

            for (auto& dir: dirs) {
                String finalDirname;

                // Two variations: relative and absolute paths
                if (dir == ".")
                    finalDirname = dirname;
                else if (dir.size() >= 2 && dir.substr(0, 2) == "..")
                    finalDirname = makeFilePath(dirname, dir);
                else
                    finalDirname = dir;

                atlasFile = makeFilePathExt(finalDirname, animationTextureFile, M::DefaultTextureAtlasExt);

                if (fileExists(atlasFile))
                    break;
            }
        } else {
            atlasFile = makeFileName(animationTextureFile, M::DefaultTextureAtlasExt);
        }

        if (!app->animationTex.loaded(atlasFile))
            app->animationTex.load(atlasFile);

        animation_.create(scene, &app->animationTex);
    } else {
        // If animation has no 
        animation_.create(scene, &app->guiTex);  // For text purposes
    }

    //animation_.setScale(ratio) ;
    animation_.start(timestamp);
}


void AnimatorScreen::loadModel(Timestamp)
{
    using namespace apl;

    auto layout = Env::fullLayout();
    auto scene = app->getScene();


    auto renderer = app->renderer();

    auto&& modelFilename = screenFilename_[TabIndexModel];
    model_.loadObj(modelFilename);

    // look for texture file
    auto [name, ext] = splitFileExt(modelFilename);
    auto [dir, basename] = splitFilePath(name);
    auto textureFilename = makeFilePathExt(dir, basename, "png");

    //model_.normalize(true);
    //model_.scale(framework_->layout.w * 0.8f) ;
    if (!model_.normalsGenerated())
        model_.generateNormals();

    //Color modelColor{M::Silver};
    //Color modelColor{M::White};
    
    //Color ambient{M::Silver};
    //Color ambient{M::Yellow};
    //Color ambient{M::White};
    Color ambient{M::Black};

    //Color diffuse{M::Red};
    //Color diffuse{M::Silver};
    Color diffuse{M::White};
    //Color specular{M::Blue};
    Color specular{M::Silver};
    //RealGL shininess{32};
    RealGL shininess{32};

    model_.setMaterial({.ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess});



    auto bbox = model_.getBoundingBox();
    auto maxFace = bbox.max();

    modelCentroid_ = bbox.centroid();

    Cord modelSphereSize = std::min(layout.w, layout.h) - 20; //(renderer->->xLeft()+render->xRight())*0.8 / stretchValue ;
    modelSphereRadius_ = modelSphereSize/2;

    modelScale_ = modelSphereSize / maxFace;
    modelScale_ *= 0.8;


    //if ( !lightPositionChanged_ )
    auto screenBoxSize = (RealGL)std::max(renderer->width(), renderer->height());
    calcLightPosition_ = {-screenBoxSize, screenBoxSize, screenBoxSize};
    if (!userLightPosition_)
        userLightPosition_ = calcLightPosition_;
    if (!lightPosition_)
        lightPosition_ = calcLightPosition_;

    model_.lightPosition = &lightPosition_;

    //scene->setLightPosition() ;

    //scene->setLightPosition ({ -modelSphereSize*2, modelSphereSize*2, -modelSphereSize*2 }) ;

    auto isoRho = -std::atan(1 / cognit::constants::sqrt2<Real>);
    auto isoTheta = -pi_/4;

    isometricAngle_ = {isoTheta, isoRho};

    originalAngle_ = isometricAngle_;

    orgModelMatrix_.scaling(modelScale_);
    orgModelMatrix_.translate(-modelCentroid_);
    originalAngle_.rotateMatrix(orgModelMatrix_);


    currentAngle_ = originalAngle_;
    modelMatrix_ = orgModelMatrix_;

    //modelMatrix_.zoom(modelSphereSize) ;
    //modelMatrix_.scaling(modelScale_) ;
    //currentAngle_.rotateMatrix(modelMatrix_) ;

    viewMatrix_.translation(renderer->center3D() - bbox.centroid());

    model_.modelMatrix = &modelMatrix_;
    model_.viewMatrix = &viewMatrix_;
    //model_.faceCulling = true;
    model_.depthTest = true;


    modelPrim_ = scene->newModel(&model_);
    //modelPrim_->setPointSize(2) ;
    modelPrim_->setWidth(1);
    //modelPrim_->setColor(0xFFFF00);

    //modelPrim_->hide();
}


void AnimatorScreen::resetBackground()
{
    auto scene = app->scene();

    // Window generic background
    auto& backgroundTex = app->guiTex("checkerboard");
    auto layout = Env::fullLayout();
    backgroundPrim_ = scene->newImage(layout, backgroundTex, M::Tiled);
    //backgroundPrim_ = scene->newBox(M::Red, {10, 10, 50, 50});

    // background
    //background_.clear();
}


void AnimatorScreen::switchScreen(int tab, StrView filename)
{
    switchScheduled_ = true;
    screenFilename_[tab] = filename;
    timerDisabled_ = false;
}


void AnimatorScreen::onTouchesBegin(TouchesA touches)
{
    lastTouches_ = touches;
}

void AnimatorScreen::onTouchesMove(TouchesA touches)
{
    if (modelPrim_) {

        auto delta = touches.touches[0].p - lastTouches_.touches[0].p;

        if (!delta)
            return;

        auto d = delta / (modelSphereRadius_ * 2);
        auto k = d * pi_;  // to radians

        currentAngle_ += RotationAngle{k.x, k.y};
        modelMatrix_.scaling(modelScale_);
        modelMatrix_.translate(-modelCentroid_);
        currentAngle_.rotateMatrix(modelMatrix_);

        //modelMatrix_.translate(-modelCentroid_);
        //modelMatrix_.updateHash();
    }

    lastTouches_ = touches;
}

void AnimatorScreen::onTouchesEnd(TouchesA touches)
{
}

}  // namespace animator
