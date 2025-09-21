/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>


namespace sample {

class TestScreen: public Screen {
    LabelP fpsLabel_, loadLabel_, runtimeLabel_, renderLabel_, sceneLabel_;


    Timestamp statsUpdateTimestamp_{};
    void updateDebugStatsScene(Timestamp timestamp);


protected:
    void onStart() override;
    void onTimer(Timestamp) override;
};


void TestScreen::onStart()
{
    auto scene = App::instance()->scene();
    auto safe = Env::getSafeArea();


    //
    // FPS elements
    Color labelColor{M::Silver}, valueColor{M::Green}, backroundColor{M::Black};

    Cord textHeight{10};
    scene->newBox(backroundColor, {safe.x, safe.y, safe.w, textHeight * 8});


    auto setupLabel = [&](StrView title, Cord x) -> LabelP {
        auto y = safe.y + textHeight / 2 + 2;

        scene->newLabel({
            .text = title,
            .pos = {x - 1, y},
            .color = labelColor,
            .textSize = textHeight,
            .flags = M::AlignRightCenter,
            });

        return scene->newLabel({
            .pos = {x + 1, y},
            .color = valueColor,
            .textSize = textHeight,
            .flags = M::AlignLeftCenter,
            });
    };


    fpsLabel_ = setupLabel("FPS:", safe.w * 0.09);
    loadLabel_ = setupLabel("LD%:", safe.w * 0.28);

    runtimeLabel_ = setupLabel("RT:", safe.w * 0.43);
    sceneLabel_ = setupLabel("SC:", safe.w * 0.64);
    renderLabel_ = setupLabel("RR:", safe.w * 0.85);





    auto debugCircle = [&](PointA center, Cord radius) {
        auto&& f = center.rect<Rect>().zoomAdd(radius);
        scene->newRect(M::White, f);
    };

    auto testCircle = [&](PointA center, Cord radius, ColorA color, Cord width = 0) {
        debugCircle(center, radius);

        auto prim = scene->newCircle(color, center, radius);
        if (width)
            prim->setWidth(width);
    };

    auto testDisk = [&](PointA center, Cord radius, ColorA color, Cord width = 0) {
        debugCircle(center, radius);

        auto prim = scene->newDisk(color, center, radius);
        if (width)
            prim->setWidth(width);

    };


    auto testArc = [&](PointA center, Cord radius, ColorA color, M::Orientation beginSeg, M::Orientation endSeg, Cord width = 0) {
        debugCircle(center, radius);

        auto prim = scene->newArc(color, center, radius, beginSeg, endSeg);
        if (width)
            prim->setWidth(width);
    };

    auto testSegment = [&](PointA center, Cord radius, ColorA color, M::Orientation beginSeg, M::Orientation endSeg) {
        debugCircle(center, radius);

        scene->newSegment(color, center, radius, beginSeg, endSeg);
    };


    /*
        testCircle({80,80}, 40, 0x0000FF);
        testCircle({200,80}, 40, 0xFF0000, 3);

        testDisk({80,200}, 40, M::Blue);

        testArc({200, 200}, 40, M::Yellow, M::Orientation90, M::Orientation270);

        //testArc ( {200, 200}, 40, 0xFFFF00, M::ORIENTATION_270, M::ORIENTATION_0, 6 ) ;

        testSegment({80, 300}, 40, 0x00FFFF, M::Orientation90, M::Orientation180);


        // Colors
        scene->newDisk(M::Red, {80, 400}, 40)->setColors(0xFF0000, 0x00FF00);

        scene->newDisk(M::Red, {200, 400}, 40)->setColors({0,0}, 0xFFFF00);


        //scene->newCircle ( {80,80}, 40 )->setColor(0xFF) ;

        //scene->newCircle ( {240,80}, 40 )->setColor(0xFFFF)->setWidth(3) ;

        //scene->newCircle ( {80,200}, 40 )->setColor(0xFF0000)->setFillColor(0xFFFF00)->setWidth(2) ;
        */
}


void TestScreen::updateDebugStatsScene(Timestamp timestamp) {
    // Update the gui every half seconds
    if (timestamp < statsUpdateTimestamp_)
        return;
    statsUpdateTimestamp_ = timestamp + 0.5;

    if (!fpsLabel_)
        return;

    char cbuffer[1001];
    const auto& debugStats = MFrameworkNative::debugStats;

    std::snprintf(cbuffer, 100, "%0.1f", debugStats.fps());
    fpsLabel_->setText(cbuffer);

    std::snprintf(cbuffer, 100, "%0.1f", debugStats.load());
    loadLabel_->setText(cbuffer);

    std::snprintf(cbuffer, 100, "%0.4f", debugStats.runtimeLoad());
    runtimeLabel_->setText(cbuffer);

    std::snprintf(cbuffer, 100, "%0.4f", debugStats.sceneLoad());
    sceneLabel_->setText(cbuffer);

    std::snprintf(cbuffer, 100, "%0.4f", debugStats.renderLoad());
    renderLabel_->setText(cbuffer);
}

void TestScreen::onTimer(Timestamp timestamp) {
    updateDebugStatsScene(timestamp);
}



void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

}

