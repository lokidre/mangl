/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <test_app.h>

class TestScreen: public Screen {
    LabelP touchLabels_[MaxTouches];
    
protected:
    void onStart() override;
    void onTouchDown(TouchA touch) override;
    void onTouchMove(TouchA touch) override;
    void onTouchUp(TouchA touch) override;
    void onTouchCancel(TouchA touch) override;
};

void TestScreen::onStart()
{
    buildSampleBackground();
    
    auto scene = App::getScene();
    auto safe = Env::getSafeArea();
    
    
    auto current = safe.position({0.1, 0.9});
    auto stepY = 30;
    
    for (auto& l: touchLabels_) {
        l = scene->newLabel({
            .text = "-", //apl::fmt("Touch %g", current.y),
            .pos = current,
        });
        
        current.y -= stepY;
    }
    


}

void TestScreen::onTouchDown(TouchA touch) {
    MDebugFunc("i: %d, c: %d, p: %g, %g", touch.index, touch.count, touch.p.x, touch.p.y);
    
    touchLabels_[touch.index]->setText("Down: i: %d; c: %d; p: %g, %g", touch.index, touch.count, touch.p.x, touch.p.y);
}

void TestScreen::onTouchMove(TouchA touch) {
//    MDebugFunc("i: %d, c: %d, p: %g, %g", touch.index, touch.count, touch.p.x, touch.p.y);

    touchLabels_[touch.index]->setText("Move: i: %d; c: %d; p: %g, %g", touch.index, touch.count, touch.p.x, touch.p.y);
}

void TestScreen::onTouchUp(TouchA touch) {
    MDebugFunc("i: %d, c: %d, p: %g, %g", touch.index, touch.count, touch.p.x, touch.p.y);
    
    touchLabels_[touch.index]->setText("Up: i: %d; c: %d; p: %g, %g", touch.index, touch.count, touch.p.x, touch.p.y);
   
}

void TestScreen::onTouchCancel(TouchA touch) {
    MDebugFunc("i: %d, c: %d, p: %g, %g", touch.index, touch.count, touch.p.x, touch.p.y);

    touchLabels_[touch.index]->setText("Cancel: i: %d; c: %d; p: %g, %g", touch.index, touch.count, touch.p.x, touch.p.y);
}


void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
