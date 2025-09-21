/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "animator_types.h"

#include <mangl/background.h>
#include <mangl/screen.h>
#include <mangl/layout.h>
#include <mangl/texture.h>
#include <mangl/animation.h>
#include <mangl/blueprint.h>

//#include <apeal/smartptr.hpp>

#include "app.h"
#include "animator.h"

namespace animator {

struct RotationAngle {
    Cord theta;  // Rotation around Y axis (horizontal) (pan)
    Cord rho; // Rotation around X axis (vertical) (tilt)


    // Math
    constexpr static auto pi_ = cognit::constants::pi<Cord>;
    constexpr static auto pi2_ = pi_ * 2;


    void normalize() {
        // normalize
        while (rho >= pi2_)  rho -= pi2_;
        while (rho < -pi2_)  rho += pi2_;

        while (theta >= pi2_)  theta -= pi2_;
        while (theta < -pi2_)  theta += pi2_;
    }

    void rotateMatrix(MatrixHashedGL& m) const {
        using namespace cognit;

        MatrixGL xM, yM, zM;
        xM.rotationX(-rho);
        yM.rotationY(-theta);

        mtx_mul(yM, m, zM);
        mtx_mul(xM, zM, m);

        m.updateHash();
    }

    auto rotationMatrix() const {
        MatrixHashedGL m;
        m.eye();
        rotateMatrix(m);
        return m;
    }

    auto& operator += (const RotationAngle& o) {
        theta += o.theta;
        rho += o.rho;
        return *this;
    }

};

struct Position {
    RotationAngle r;  // rotation
    Point3D t;  // translation
};



class AnimatorScreen: public ScreenBase {
public:
    AnimatorScreen() {
        int a = 0;
    }
    void switchScreen(int tab, StrView file);

    void reload() { switchScheduled_ = true; }

    void clearAll() {
        switchScheduled_ = true;
        for (auto& fn : screenFilename_)
            fn.clear();
    }

    void clearScreen(int tab) {
        switchScheduled_ = true;
        screenFilename_[tab].clear();
    }

    auto& currentScreen(int tab) const { return screenFilename_[tab]; }

    bool errorFlag = false;
    String errorMessage;


public:
    // User interaction
    BlueprintEngine blueprintEngine;

private:
    App* app{App::instance()};
    MBackground background_;
    Animation animation_;
    MLayout layout_;

    std::unordered_map<String, TextureAtlas*> loadedTextures_;




    //
    // 3D model
    //
    constexpr static Cord pi_ = cognit::constants::pi<Cord>;
    constexpr static Cord pi2_ = pi_ * 2;

    RotationAngle isometricAngle_, originalAngle_;  // Isometric and original angles
    RotationAngle currentAngle_;  // Current angle

    Model model_;
    MatrixHashedGL orgModelMatrix_, modelMatrix_, viewMatrix_;
    PrimP modelPrim_{};
    Cord modelTextureData_[4];
    Cord modelScale_, modelSphereRadius_;
    Point3D modelCentroid_;



    PrimP backgroundPrim_;


    Point3DGL calcLightPosition_{};
    Point3DGL lightPosition_{};
    Point3DGL userLightPosition_{};
    bool userLightEnabled_{};


    bool switchScheduled_{};
    String screenFilename_[TabsCount];

    bool timerDisabled_{};
    Timestamp lastTimestamp{};


    enum { MAX_BACKGROUND_ELEMENTS = 10000 };

    void resetBackground();

public:
    void onWireframeEnable(bool state) { model_.wireframe = state; }

    void onVerticesEnable(bool state) { model_.vertices = state; }

    void onBoundsEnable(bool st) { blueprintEngine.setDebugBoundsEnabled(st); }
    void onLabelsEnable(bool st) { blueprintEngine.setDebugIdLabelsEnabled(st); }

    void onLightingEnable(bool state) {
        model_.lightingEnabled = state;
        lightPosition_ = state ? userLightPosition_ : calcLightPosition_;
    }

    void onUserLightPositionEnable(bool state) {
        userLightEnabled_ = state;
        lightPosition_ = state ? userLightPosition_ : calcLightPosition_;
    }

    void setUserLightPositionI(Point3DI p) { 
        setUserLightPosition(p.convert<Point3D>()); 
    }

    void setUserLightPosition(Point3D p) {
        userLightPosition_ = p.convert<Point3DGL>();
        if (userLightEnabled_)
            lightPosition_ = userLightPosition_;
    }


private:
    void loadModel(Timestamp);
    void loadAnimation(Timestamp, std::unordered_map<StrView, StrView>& params);
    void loadBlueprint();


protected:  // GameScreen overrides
    void onStart() override;
    void onRelease() override;
    void onTimer(Timestamp timestamp) override;


private:
    TouchesInfo lastTouches_;

protected:
    void onTouchesBegin(TouchesA) override;
    void onTouchesMove(TouchesA) override;
    void onTouchesEnd(TouchesA) override;
};

}  // namespace animator
