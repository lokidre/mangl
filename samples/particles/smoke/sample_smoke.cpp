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
    
}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);
    switchScreen(new TestScreen);
}

} // namespace
