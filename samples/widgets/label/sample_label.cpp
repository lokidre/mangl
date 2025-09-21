/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

#include <mangl/widgets.h>
#include <mangl/theme.h>

namespace sample {

class TestScreen: public Screen {
protected:
    void onStart() override;
};


void TestScreen::onStart() {
    buildSampleBackground();

    auto scene = App::getScene();
    auto safe = Env::getSafeArea();

    auto center = safe.center();

    auto line = center;

    auto gridHeight = safe.h * 0.25;

    LabelP label{};

    scene->newLine(M::DimGrey, {center.x, safe.y}, {center.x, safe.top()}, 1);
    scene->newLine(M::DimGrey, {safe.x, center.y}, {safe.right(), center.y}, 1);

    label = scene->newLabel({
        .text = "Center",
        .pos = center,
        .align = M::AlignCenterAll
    });

    label->setOrientation(M::Orientation180);


    scene->newLabel({
        .text = "Align Left",
        .pos = center.off({.y = 30}),
        .align = M::AlignLeft | M::AlignVCenter
    });
    
    scene->newLabel({.text = "Align Right",
                     .pos = center.off({.y = -30}),
                     .flags = M::AlignRight | M::AlignVCenter});


    line.y = line.y * 0.5;

    scene->newLine(M::DimGrey, {0, line.y}, {safe.w, line.y}, 1);
    scene->newLabel({.text = "Align Bottom",
                     .pos = line, //.off({.y = 30}),
                     .color = M::Yellow,
                     .flags = M::AlignBottom | M::AlignLeft});

    scene->newLabel({.text = "Align Top",
                     .pos = line, //.off({.y = -30}),
                     .color = M::Yellow,
                     .flags = M::AlignTop | M::AlignRight});


    line.y = center.y * 1.5;
    scene->newLine(M::DimGrey, {0, line.y}, {safe.w, line.y}, 1);

    scene->newLabel({
        .text = "Background Effect",
        .pos = line.off({.x = -center.x, .y = 30}),
        .color = M::White,
        .effect =
            {
                .type = LabelEffectType::Shadow,
                .color = M::LightGray,
                .offset = {1, -2},
            },
    });

    scene->newLabel({
        .text = "Outer Glow Effect",
        .pos = line.off({.x = -center.x, .y = 70}),
        .color = M::Blue,
        .effect =
            {
                .type = LabelEffectType::OuterGlow,
                .color = M::White,
                .blur = 2,
            },
    });

    scene->newLabel({
        .text = "Spacing S",
        .pos = line.off({.y = -30}),
        .color = M::Yellow,
        .spacing = 5,
    });


    label = scene->newLabel({
        .text = "az_Orientation 180",
        .pos = line.off({.y = -gridHeight / 2}),
        .color = M::Red,
        .flags = M::AlignCenterAll,
    });

    label->setOrientation(M::Orientation180);
}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

} // namespace
