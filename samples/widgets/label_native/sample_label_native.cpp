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
    sampleBuildBackground();
   

    auto scene = App::getScene();
    auto safe = Env::getSafeArea();

    auto center = safe.center();

    scene->newLine({center.x, safe.y}, {center.x, safe.top()}, M::DimGray);
    scene->newLine({safe.x, center.y}, {safe.right(), center.y}, M::DimGray);

    // scene->newLabel("Center", center, 16, M::White, M::AlignCenterAll);
    scene->newLabel({
        .text = "Center",
        .pos = center,
        .align = M::AlignCenterAll,
    });


    scene->newLabel({.text = "Align Left",
                     .pos = center.off({.y = 30}),
                     .flags = M::AlignLeft | M::AlignVCenter});
    
    scene->newLabel({
        .text = "Align Right",
        .pos = center.off({.y = -30}),
        .flags = M::AlignRight | M::AlignVCenter
    });


    auto line = center;
    line.y = line.y * 0.5;

    scene->newLine({0, line.y}, {safe.w, line.y}, M::DimGrey);
    scene->newLabel({.text = "Align Bottom",
                     .pos = line, //.off({.y = 30}),
                     .color = 0XFFDE00,
                     .flags = M::AlignBottom | M::AlignLeft});

    scene->newLabel({.text = "Align Top",
                     .pos = line, //.off({.y = -30}),
                     .color = 0XFFDE00,
                     .flags = M::AlignTop | M::AlignRight});


    line.y = center.y * 1.5;
    scene->newLine({0, line.y}, {safe.w, line.y}, M::DimGrey);

    scene->newLabel({
        .text = "Background Effect",
        .pos = line.off({.x = -center.x, .y = 30}),
        .color = M::White,
//        .backgroundEffect =
//            {
//                .color = M::LightGray,
//                .offset = {1, -2},
//            },
    });

    scene->newLabel({
        .text = "Spacing S",
        .pos = line.off({.y = -30}),
        .color = M::Yellow,
        .spacing = 5,
    });
}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen<TestScreen>();
}

} // namespace
