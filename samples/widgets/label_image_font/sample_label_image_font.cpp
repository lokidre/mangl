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

    auto scene = App::getScene();
    auto layout = Env::getSafeArea();

    auto center = layout.center();

    auto line = center;


    scene->newBox(M::Navy, Env::getFullArea());

    scene->newLine({center.x, 0}, {center.x, layout.h}, M::DimGrey);
    scene->newLine({0, center.y}, {layout.w, center.y}, M::DimGrey);

    // scene->newLabel("Center", center, 16, M::White, M::AlignCenterAll);
    scene->newLabel({.text = "ABC",
                     .pos = center,
                     .font = &texCommon.imageFont,
                     .textSize = 40,
                     .flags = M::AlignCenterAll});


    // scene->newLabel(
    //     {.text = "Align Left",
    //      .pos = center.off({.y = 30}),
    //      .flags = M::AlignLeft | M::AlignVCenter}
    //);
    // scene->newLabel(
    //     {.text = "Align Right",
    //      .pos = center.off({.y = -30}),
    //      .flags = M::AlignRight | M::AlignVCenter}
    //);


    // line.y = line.y * 0.5;

    // scene->newLine({0, line.y}, {layout.w, line.y}, M::DimGrey, 1);
    // scene->newLabel(
    //     {.text = "Align Bottom",
    //      .pos = line, //.off({.y = 30}),
    //      .color = M::Yellow,
    //      .flags = M::AlignBottom | M::AlignLeft}
    //);

    // scene->newLabel(
    //     {.text = "Align Top",
    //      .pos = line, //.off({.y = -30}),
    //      .color = M::Yellow,
    //      .flags = M::AlignTop | M::AlignRight}
    //);


    // line.y = center.y * 1.5;
    // scene->newLine({0, line.y}, {layout.w, line.y}, M::DimGrey, 1);

    // scene->newLabel({
    //     .text = "Background Effect",
    //     .pos = line.off({.x = -center.x, .y = 30}),
    //     .color = M::White,
    //     .effect = {
    //         .type = LabelEffectType::Shadow,
    //         .color = M::LightGray,
    //         .offset = {1, -2},
    //     },
    // });

    // scene->newLabel({
    //     .text = "Outer Glow Effect",
    //     .pos = line.off({.x = -center.x, .y = 70}),
    //     .color = M::Blue,
    //     .effect = {
    //         .type = LabelEffectType::OuterGlow,
    //         .color = M::White,
    //         .blur = 2,
    //     },
    // });

    // scene->newLabel({
    //     .text = "Spacing S",
    //     .pos = line.off({.y = -30}),
    //     .color = M::Yellow,
    //     .spacing = 5,
    // });
}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
} // namespace