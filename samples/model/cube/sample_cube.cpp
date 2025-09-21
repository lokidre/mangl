/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <test_app.h>

#include <mangl/mangl.h>
#include <mangl/scene.h>

constexpr static Cord pi_ = cognit::constants::pi<Cord>;
constexpr auto rad(Cord d) { return d * pi_ / 180; }
//Icon Model
Fog fog_;
Model model_;
PrimP prim_;

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

    void rotateMatrix(MatrixHashedGL &m) const noexcept {
        using namespace cognit;

        MatrixHashedGL xM, yM, zM;
        xM.rotationX(-rho);
        yM.rotationY(-theta);

        mtx_mul(yM, m, zM);
        mtx_mul(xM, zM, m);

        m.updateHash();
    }

    auto rotationMatrix() const noexcept {
        MatrixHashedGL m;
        m.eye();
        rotateMatrix(m);
        return m;
    }

    RotationAngle &operator += (const RotationAngle &o) noexcept {
        theta += o.theta;
        rho += o.rho;
        return *this;
    }

};

auto rotationMatrix(RotationAngle a) noexcept { return a.rotationMatrix(); }

struct Position {
    RotationAngle r;  // rotation
    Point3DGL t;  // translation
};
RotationAngle isoAngle_, orgAngle_;
MatrixHashedGL orgModelMtx_, modelMtx_;  // Original and Current Matrices
MatrixHashedGL viewMtx_; // View matrix

void setTargetRotation(RotationAngle a) noexcept {
    modelMtx_.scaling(68);
    a.rotateMatrix(modelMtx_);
    modelMtx_.updateHash();
}

// State
bool returning_ = false;  // flag shows that the target is returning
Cord rotK_;  // Target Rotation Coefficient
Cord returningAV_;  // Returning Angular velocity (when user interacted)
Cord idleAV_;  // Idle angular velocity
RotationAngle curAngle_{};

bool touchesBegan_{};
RotationAngle touchesBeginA_;
TouchesInfo beginTouches_, lastTouches_;


class TestScreen: public ScreenBase {
protected:
    void onStart() override;
    void onTouchesBegin(TouchesA ti) override;
    void onTouchesMove(TouchesA ti) override;
    void onTouchesEnd(TouchesA ti) override;
};

void TestScreen::onStart()
{
    //using namespace mangl;
    auto scene = App::scene();
    auto layout = Env::safeLayout();
    
    Size cubeSize{100, 100};
    Point cubePos = layout.center();
    
    cubePos.x -= 20;
    cubePos.y -= 100;

    //scene->newCircle ( {80,200}, 40 )->setColor(0xFF0000);//->setFillColor(0xFFFF00)->setWidth(2) ;
    
    scene->newRect (M::Red, Rect::make(cubePos - cubeSize/2, cubeSize))->setWidth(2);

    //->setFillColor(0xFFFF00)->setWidth(2) ;
    Cord modelScale_ = 68;

    auto isoTheta = rad(-45);
    isoAngle_ = {isoTheta, rad(-17)};

    orgAngle_ = isoAngle_;

    returningAV_ = pi_ * 2;
    idleAV_ = pi_ / 32;

    if (curAngle_.theta == 0 && curAngle_.rho == 0)
        curAngle_ = orgAngle_;

    orgModelMtx_.eye();
    orgAngle_.rotateMatrix(orgModelMtx_);

    modelMtx_ = orgModelMtx_;
    modelMtx_.zoom(modelScale_);
    modelMtx_.updateHash();

    viewMtx_.translation({cubePos.x, cubePos.y, 0});
    viewMtx_.updateHash();


    model_.depthTest = true;
    model_.faceCulling = true;
    model_.setLineWidth(1.5);// 2);
    
    fog_.type = Fog::Type::Exp;
    auto fogColor = 0xff0000;
    fog_.set(fogColor, 2, 4.4, -2);
    
    //model_.fog = &fog_;
    
    auto pushLine = [](Model& model, int materialIndex, int vertexIndex, std::initializer_list<int> v) {
        auto &group = model.groups.back();
        auto &line = group.lines.emplace_back();
        line.m = materialIndex;
        for (auto &vi: v)
            line.v.push_back({vertexIndex+vi});
    };

    auto buildCube = [&pushLine](Model& model, int materialIndex, PointGL pos) {
        constexpr RealGL s{1};
        constexpr auto u = s/2;
        ColorA ridgeColor = 0xffffff;//0x00bfbf;
        //Material ridgeMaterial{ridgeColor, ridgeColor, ridgeColor, 1.f};
        Material ridgeMaterial{};
        ridgeMaterial.ambient = ridgeColor;

        model.materials.push_back(ridgeMaterial);
        // Vertex coordinates
        auto &v = model.vertexPositions;
        auto vertexIndex = (int)v.size();

        model.groups.emplace_back();

        v.push_back({pos.x - u, pos.y - u, -u});
        v.push_back({pos.x - u, pos.y + u, -u});
        v.push_back({pos.x + u, pos.y + u, -u});
        v.push_back({pos.x + u, pos.y - u, -u});

        v.push_back({pos.x - u, pos.y - u, u});
        v.push_back({pos.x - u, pos.y + u, u});
        v.push_back({pos.x + u, pos.y + u, u});
        v.push_back({pos.x + u, pos.y - u, u});

        pushLine(model, materialIndex, vertexIndex, {0, 1, 2, 3, 0});
        pushLine(model, materialIndex, vertexIndex, {4, 5, 6, 7, 4});

        pushLine(model, materialIndex, vertexIndex, {0, 4});
        pushLine(model, materialIndex, vertexIndex, {1, 5});
        pushLine(model, materialIndex, vertexIndex, {2, 6});
        pushLine(model, materialIndex, vertexIndex, {3, 7});

        //model.groups.resize(model.groups.size()+1);

    };


    //constexpr auto alpha = 70.53;
    auto materialIndex = apl::count(model_.materials);
    //PointGL a = {100 ,100};
    //model.materials.push_back(ridgeMaterial);
    //buildCube(model_, materialIndex, cubePos.convert<PointGL>()); //{{0, -alpha - 45}}); // 5
    buildCube(model_, materialIndex, {0, 0}); //{{0, -alpha - 45}}); // 5

    model_.modelMatrix = &modelMtx_;
    model_.viewMatrix = &viewMtx_;

   // model_.generateNormals();
    
    scene->newModel(&model_);
    
    //prim_ = scene->newModel(&model_);

}

void TestScreen::onTouchesBegin(TouchesA ti){

    touchesBegan_ = true;
    beginTouches_ = lastTouches_ = ti;

    returning_ = false;
    rotK_ = 1;
    touchesBeginA_ = curAngle_;
}

void TestScreen::onTouchesMove(TouchesA ti){
    using namespace cognit;

    auto delta = ti.touches[0].p - lastTouches_.touches[0].p;

    if (!delta)
        return;

    auto sphereRadius = Env::safeLayout().w/2;

    auto d = delta / (sphereRadius * 2); /// layout_.screen.h ;
    auto k = d * pi_;  // to radians

    curAngle_ += RotationAngle{k.x, k.y};
    setTargetRotation(curAngle_);

    lastTouches_ = ti;
}

void TestScreen::onTouchesEnd(TouchesA ti)
{
    returning_ = true;
    touchesBegan_ = false;
    curAngle_.normalize();
}


void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
