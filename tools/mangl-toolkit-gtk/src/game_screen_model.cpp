/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "game_screen.h"

#include "state.hpp"

namespace toolkit {

void GameScreen::buildModel() {

    auto scene = getScene();
    auto renderer = getRenderer();
    auto layout = Env::fullLayout();

    auto state = State::instance();

    auto& projectFiles = state->projectFiles;

    auto& modelsFiles = projectFiles.fileContainers[(int)ProjectCategory::Model];

    if (modelsFiles.selectedIndex < 0)
        return;

    auto& modelFile = modelsFiles.files[modelsFiles.selectedIndex];

    model_ = std::make_unique<Model>();

    auto& model = *model_;

    model.loadObj(modelFile.path);

    if (!model.normalsGenerated())
        model.generateNormals();

    updateModelMaterial();

    auto bbox = model.getBoundingBox();
    auto maxFace = bbox.max();

    modelCentroid_ = bbox.centroid();

    //(renderer->->xLeft()+render->xRight())*0.8 / stretchValue ;
    auto modelSphereSize = std::min(layout.w, layout.h);
    modelSphereRadius_ = modelSphereSize/2;

    modelScale_ = modelSphereSize / maxFace;
    modelScale_ *= state->model.scale;

    auto screenBoxSize = (RealGL)std::max(renderer->width(), renderer->height());
    //calcLightPosition_ = {screenBoxSize, screenBoxSize, screenBoxSize};
    calcLightPosition_ = {-screenBoxSize, screenBoxSize, screenBoxSize};
    if (!userLightPosition_)
        userLightPosition_ = calcLightPosition_;
    if (!lightPosition_)
        lightPosition_ = calcLightPosition_;

    model.lightPosition = &lightPosition_;

    model.lightingEnabled = state->model.lighting;

    //scene->setLightPosition() ;

    //scene->setLightPosition ({ -modelSphereSize*2, modelSphereSize*2, -modelSphereSize*2 }) ;

    auto isoRho = -std::atan(1 / cognit::constants::sqrt2<Real>);
    auto isoTheta = -cognit::constants::pi<Real>/4;

    isometricAngle_ = {isoTheta, isoRho};

    originalAngle_ = isometricAngle_;

    orgModelMatrix_.scaling(modelScale_);
    orgModelMatrix_.translate(-modelCentroid_);
    originalAngle_.rotateMatrix(&orgModelMatrix_);


    currentAngle_ = originalAngle_;
    modelMatrix_ = orgModelMatrix_;

    //modelMatrix_.zoom(modelSphereSize) ;
    //modelMatrix_.scaling(modelScale_) ;
    //currentAngle_.rotateMatrix(modelMatrix_) ;

    viewMatrix_.translation(renderer->center3D() - bbox.centroid());

    model.modelMatrix = &modelMatrix_;
    model.viewMatrix = &viewMatrix_;
    model.faceCulling = state->model.faceCulling;
    model.depthTest = state->model.depthTest;

    model.wireframe = state->model.wireframe;


    scene->deletePrim(modelPrim_);


    modelPrim_ = scene->newModel(model_.get());
    //modelPrim_->setPointSize(2) ;
    modelPrim_->setWidth(1);


}


}  // namespace toolkit
