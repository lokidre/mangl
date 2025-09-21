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

    scene->newBox(Theme::body.backgroundColor, Env::getFullArea());


    auto center = layout.center();

    Rect checkboxRect{center.x + layout.w * 0.25, layout.top() - 50, 30, 30};
    Point labelPos = {10, checkboxRect.center().y};
    LabelP label{};

    auto advance = [&]() {
        labelPos.y -= 50;
        checkboxRect.y -= 50;
    };

#if 0
    label = scene->newLabel({
        .text = "Standard...",
        .pos = labelPos,
        .flags = M::AlignLeft | M::AlignVCenter,
    });

    scene->newCheckbox({
        .rect = checkboxRect,
        .checked = true,
        .checkSignal = [label](bool checked) {
            label->setText(checked ? "Checked" : "Unchecked");
        },
    });

    advance();


    label = scene->newLabel({
        .text = "Texture...",
        .pos = labelPos,
        .flags = M::AlignLeft | M::AlignVCenter,
    });


    scene->newCheckbox({
        .rect = checkboxRect,
        .texture = texCommon.checkbox,
        .checked = true,
        .checkSignal = [label](bool checked) {
            label->setText(checked ? "Checked" : "Unchecked");
        },
    });

    advance();


    label = scene->newLabel({
        .text = "Theme texture...",
        .pos = labelPos,
        .flags = M::AlignLeft | M::AlignVCenter,
    });

    Theme::checkbox.texture = texCommon.checkbox;

    scene->newCheckbox({
        .rect = checkboxRect,
        .checked = true,
        .checkSignal = [label](bool checked) {
            label->setText(checked ? "Checked" : "Unchecked");
        },
    });

    advance();
#endif

    label = scene->newLabel({
        .text = "Widgets...",
        .pos = labelPos,
        .flags = M::AlignLeft | M::AlignVCenter,
    });


    auto onWidget = scene->newStackWidget({});
    onWidget->addPrim(scene->newBox(M::Blue, {}));

    auto offWidget = scene->newStackWidget({});
    offWidget->addPrim(scene->newBox(M::Red, {}));

    // auto offWidget = scene->newStackWidget({});

    scene->newCheckbox({
        .rect = checkboxRect,
        .onWidget = onWidget,
        .offWidget = offWidget,
        .checked = true,
        .checkSignal = [label](bool checked) { label->setText(checked ? "Checked" : "Unchecked"); },
    });

    advance();
}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

} //namespace