/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

#include <mangl/particles.h>

namespace sample {

class TestScreen: public Screen {
private:
    ParticleSystem engine_;

    Size brokenSize_{};  // size of the broken piece
    SizeI brokenDims_{}; // how many broken elements in one piece
    SizeI objectDims_{}; // how many pieces in the object
    Rect objectFrame_{}; // object frame
    std::vector<std::pair<Point, Point>> startVelocities_;
    App& framework_ = *App::instance();

    void emitParticles();

    Point randPoint(PointA min, PointA max) {
        Point p;

        M::random.range(min.x, max.x, &p.x);
        M::random.range(min.y, max.y, &p.y);

        return p;
    }

protected:
    void onStart() override;
    void onTimer(Timestamp timestamp) override { engine_.onTimer(timestamp); }
    void onTouchDown(TouchA info) override { emitParticles(); }
};

void TestScreen::onStart() {
    sampleBuildBackground();
    
    auto scene = App::getScene();
    //auto env = Env::instance();
    auto layout = Env::getSafeArea();

    brokenSize_ = {2, 2};   //    scene->newBox(0, layout);
    brokenDims_ = {16, 16}; // 1024 broken pieces
    objectDims_ = {1, 1};

    auto gravityY = -layout.h; // gravity force direction
    int maxParticles = 5000;
    auto unitsPerMeter = 1; // 1 pixel = 1 meter
    auto speedK = 15.;       // effect playback speed

    startVelocities_.resize(brokenDims_.w * brokenDims_.h);

    for (auto& sv: startVelocities_) {
        sv = {{-1.8, 3}, {1.8, 5}};
    }

    for (auto& v: startVelocities_) {
        v.first *= unitsPerMeter * 10;  // 10 m/s
        v.second *= unitsPerMeter * 15; //  15 m/s
    }

    objectFrame_ = layout.centered(
        {brokenSize_.w * brokenDims_.w * objectDims_.w,
         brokenSize_.h * brokenDims_.h * objectDims_.h}
    );

    scene->newRect(0xFF, objectFrame_);

    engine_.setup(Env::getFullArea(), scene->newPrimArray(maxParticles), maxParticles);
    engine_.setGravity(gravityY * 5);
    engine_.setUnitsPerMeter(unitsPerMeter);
    engine_.setSpeedK(speedK);

    engine_.particleConditionCallback = [](Particle* particle) { return true; };
}

void TestScreen::emitParticles() {
    for (int objectRow = 0; objectRow < objectDims_.h; ++objectRow) {
        for (int objectCol = 0; objectCol < objectDims_.w; ++objectCol) {
            auto brickPosition = Point{
                objectCol * brokenSize_.w * brokenDims_.h, objectRow * brokenSize_.h * brokenDims_.h
            };
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
                    // particle->callbackCondition =
                    particle->prim->setBox(fragmentFrame)->setColor(0xFFFFF);
                    // grow the particle in size
                    particle->sizeProgress.type = ParticleProgress::Type::Height;
                    particle->sizeProgress.state = 0;
                    particle->sizeProgress.value = 50;
                    particle->callbackCondition = Callback::ParticleCallbackCondition::Time;
                    particle->startTimestamp = 0;
                    particle->callbackTime = 1.5;
                }
            }
        }
    }
}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);
    switchScreen(new TestScreen);
}

} // namespace
