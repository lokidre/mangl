/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
// #include "common/framework.h"
#include <mangl/screen.h>
#include <mangl/particles.h>
#include "../../mangl/src/core/particles/mangl_particles.h"

#include <test_app.h>

class TestScreen: public ScreenBase {
private:
    ParticleSystem engine_;

    Size brokenSize_{};  // size of the broken piece
    SizeI brokenDims_{}; // how many broken elements in one piece
    SizeI objectDims_{}; // how many pieces in the object
    Rect objectFrame_{}; // object frame
    std::vector<std::pair<Point, Point>> startVelocities_;

    App& framework_ = *App::instance();

    void emitParticles();

    Point randPoint(const Point& min, const Point& max) {
        auto x = framework_.rand_.rangeReal(min.x, max.x);
        auto y = framework_.rand_.rangeReal(min.y, max.y);
        return {x, y};
    }

protected:
    void onStart() override;
    void onTimer(Timestamp timestamp) override { engine_.onTimer(timestamp); }

    void onTouchesBegin(const TouchesInfo& info) override { emitParticles(); }
};


void TestScreen::onStart() {
    auto scene = App::instance()->scene();
    auto env = Env::instance();

    auto layout = env->paddedLayout();

    scene->newBox(0, layout);

    brokenSize_ = {25, 25};
    brokenDims_ = {3, 3}; // 4 broken pieces

    objectDims_ = {5, 1}; //

    int maxParticles = 1;
    auto gravityY = -layout.h;
    auto speedK_ = 1.;
    auto unitsPerMeter = 1; // objectFrame_.y / 2 ;  // dropping from 2 meters
    // fallingSpeed ; // debug
    //   auto pieceMass = 1.;
    //   auto gravityG = 6.6;


    startVelocities_.resize(brokenDims_.w * brokenDims_.h);
    startVelocities_[0] = {{-5, 2}, {-2, 4}}; // lowest left
    if (startVelocities_.size() > 1) {
        startVelocities_[1] = {{2, 2}, {5, 4}}; // lower right
    }
    if (startVelocities_.size() > 2) {
        startVelocities_[2] = {{-5, 4}, {-3, 6}}; // upper row left
        startVelocities_[3] = {{2, 4}, {5, 6}};   // upper row right
    }

    // Adjust for meters
    for (auto& v: startVelocities_) {
        v.first *= unitsPerMeter / 10;
        v.second *= unitsPerMeter / 10;
    }

    objectFrame_ = layout.centered(
        {brokenSize_.w, //* brokenDims_.w * objectDims_.w,
         brokenSize_.h }//* brokenDims_.h * objectDims_.h}
    );

    scene->newRect(0xFF, objectFrame_);

    engine_.setup(env->fullLayout(), scene->newPrimArray(maxParticles), maxParticles);
    engine_.setGravity(gravityY);
    engine_.setUnitsPerMeter(unitsPerMeter);
    engine_.setSpeedK(speedK_);
    // engine_.setParticleMass ( pieceMass ) ;
}

void TestScreen::emitParticles() {
    for (int objectRow = 0; objectRow < objectDims_.h; ++objectRow) {
        for (int objectCol = 0; objectCol < objectDims_.w; ++objectCol) {
            auto brickPosition = MPoint{
                objectCol * brokenSize_.w * brokenDims_.h,
                objectRow * brokenSize_.h * brokenDims_.h};
            for (int row = 0; row < brokenDims_.h; ++row) {
                for (int col = 0; col < brokenDims_.w; ++col) {
                    auto fragmentFrame = Rect{col * brokenSize_.w, row * brokenSize_.h};
                    fragmentFrame.resize(brokenSize_);
                    fragmentFrame += objectFrame_.position();
                    fragmentFrame += brickPosition;
                    auto& startVelocities = startVelocities_[row * brokenDims_.w + col];
                    auto particle = engine_.emit(
                        fragmentFrame, randPoint(startVelocities.first, startVelocities.second)
                    );
                    if (!particle)
                        break;
                    particle->prim->setBox(fragmentFrame)
                        ->setColor(0xFF0000); //->setColor(0xFF) ;//

                    // grow the particle in size
                    particle->sizeProgress.type = ParticleProgress::Type::Height;
                    particle->sizeProgress.state = 0;
                    particle->sizeProgress.value = 1.25;
                }
            }
        }
    }

    // engine_.emit ( concretePiecesTex_.frame(0), bp->frame, framework_.randPoint({6,14}, {4,10}) )
    // ; engine_.emit ( concretePiecesTex_.frame(1), bp->frame, framework_.randPoint({12,14},
    // {28,10}) ) ; engine_.emit ( concretePiecesTex_.frame(2), bp->frame,
    // framework_.randPoint({-4,14}, {-6,10}) ) ; engine_.emit ( concretePiecesTex_.frame(3),
    // bp->frame, framework_.randPoint({-28,14}, {-12,10}) ) ;
}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
