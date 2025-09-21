/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <mangl/screen.h>
#include <mangl/scene.h>
#include <mangl/animation.h>

#include "screen_model.h"
#include "app.h"


using namespace mangl;

class Screen: public ScreenBase {
private:

    auto getScene() { return App::getScene(); }
    auto getRenderer() { return App::getRenderer(); }


    void buildEmptyBackground();


    TouchesInfo lastTouches_;
    const Real pi_ = cognit::constants::pi<Real>;

    TextureAtlas guiTex_;

//
    // Timer
    //
    Timestamp lastTimestamp_{};

    //
    // Animation tab
    //
    void buildAnimation();
    Animation animation_;
    TextureAtlas animationTex_;
    String animationTextureFile_;

    void buildModel();
    void updateModelMaterial() {
        model_.setMaterial(
            {.ambient = ambientColor,
             .diffuse = diffuseColor,
             .specular = specularColor,
             .shininess = shininess}
        );
    }


    Model model_;
    Fog fog_;
    uint32_t ambientColor{M::Black}, diffuseColor{M::Red}, specularColor{M::White};
    RealGL shininess{32};
    MatrixHashedGL orgModelMatrix_, modelMatrix_, viewMatrix_;
    PrimP modelPrim_{};
    Cord modelTextureData_[4];
    Cord modelScale_, modelSphereRadius_;
    Point3D modelCentroid_;
    // Box3D bbox = model_.getBoundingBox();
    Cord maxFace_{}; // = bbox.max();
    Cord modelSphereSize_{};
    String modelData_{};
    String animationData_{};


    RotationAngle isometricAngle_, originalAngle_; // Isometric and original angles
    RotationAngle currentAngle_;                   // Current angle

    Point3DGL calcLightPosition_{};
    Point3DGL lightPosition_{};
    Point3DGL userLightPosition_{};
    bool userLightEnabled_{};


protected:
    void onStart() override;

    void onTouchesBegin(TouchesA) override;
    void onTouchesMove(TouchesA) override;
    void onTouchesEnd(TouchesA) override;

    void onTimer(Timestamp) override;

    String onCall(StrView name, StrView data) override;

};
