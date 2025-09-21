/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "main_types.hpp"
#include "state.hpp"
#include "game_screen_app.h"
#include "game_screen_model.h"

#include <mangl/animation.h>

namespace toolkit {

class GameScreen: public ScreenBase {
    using Self = GameScreen;
    using SelfP = Self*;


    auto getScene() { return GameScreenApp::getScene(); }
    auto getRenderer() { return GameScreenApp::getRenderer(); }


    void buildScene();
    void buildEmptyBackground();


    TouchInfo lastTouches_{};
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


    //
    // Model tab
    //
    void buildModel();
    void updateModelMaterial() {
        auto& ms = State::instance()->model;
        model_->setMaterial(
            {.ambient = ms.ambientColor,
             .diffuse = ms.diffuseColor,
             .specular = ms.specularColor,
             .shininess = ms.shininess}
        );
    }


    //Model model_;
    std::unique_ptr<Model> model_;
    MatrixHashedGL orgModelMatrix_, modelMatrix_, viewMatrix_;
    PrimP modelPrim_{};
    Cord modelTextureData_[4]{};
    Cord modelScale_{}, modelSphereRadius_{};
    Point3D modelCentroid_{};

    RotationAngle isometricAngle_{}, originalAngle_{};  // Isometric and original angles
    RotationAngle currentAngle_{};  // Current angle

    Point3DGL calcLightPosition_{};
    Point3DGL lightPosition_{};
    Point3DGL userLightPosition_{};
    bool userLightEnabled_{};



protected:
    void onStart() override;
    void onRelease() override;


    void onTouchDown(TouchA) override;
    void onTouchMove(TouchA) override;
    void onTouchUp(TouchA) override;

    void onTimer(Timestamp) override;

protected:
    void onViewOptionsChanged();
};

}  // namespace toolkit
