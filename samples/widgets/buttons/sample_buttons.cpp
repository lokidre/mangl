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
    ButtonStyle cstyle{};

protected:
    void onStart() override;
};


void TestScreen::onStart()
{
    sampleBuildBackground();
    sampleBuildStatusBar();

    auto scene = App::getScene();

    auto& sbutton = M::Styles::button;

    scene->newButton({
        .text = "BUTTON",
        .rect = sampleGridAddButton(),
        .onPush = [this] {
            sampleSetStatusMessage("Button pressed.");
        },
    });

    scene->newButton({
        .text = "DISABLED",
        .rect = sampleGridAddButton(),
    })->disable();


    sbutton.texture = texCommon.button;

    scene->newButton({
        .text = "Texture",
        .rect = sampleGridAddButton(),
        .onPush = [this] {
            sampleSetStatusMessage("Texture Button pressed.");
        },
    });

    sbutton.texture = texCommon.buttonLabel;

    scene->newButton({
        .rect = sampleGridAddButton(),
        .onPush = [this] {
            setStatusMessage("Styled Button pressed.");
        },
    });

    M::Styles::button.reset();


    scene->newButton({
        .text = "Resetted",
        .rect = sampleGridAddButton(),
        .onPush = [this] {
            setStatusMessage("Style Reset Button pressed.");
        },
    });


    sbutton.reset();
    sbutton.textColor = M::Yellow;
    sbutton.backgroundColor = M::Maroon;
    sbutton.pressedBackgroundColor = M::Olive;
    sbutton.border.radius = 10;
    sbutton.border.strikes = {
        {M::Red, 7, 0},
        {M::Blue, 3, 0},
    };

    scene->newButton({
        .text = "STRIKES",
        .rect = sampleGridAddButton(),
        .onPush = [this] {
            sampleSetStatusMessage("Borders Button pressed.");
        },
    });


    cstyle.reset();
    cstyle.textColor = M::Magenta;

    scene->newButton({
        .style = &cstyle,
        .text = "C Styled",
        .rect = sampleGridAddButton(),
    });


    scene->newButton({
        .text = "Flipped",
        .rect = sampleGridAddButton(),
    })->setOrientation(M::Orientation180);

}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

} // namespace
