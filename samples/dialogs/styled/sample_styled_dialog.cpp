/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <test_app.h>

#include <mangl/widgets.h>


class TestScreen: public Screen {
protected:
    void onStart() override;

private:
    void setBorderStyle();
    void setTextureStyle();
    void setDecorStyle();
};


void TestScreen::onStart()
{
    buildSampleBackground();

    auto scene = App::getScene();
    auto safe = Env::getSafeArea();


    Theme::segmented.segmentTex = texCommon.segmentedButton;
    Theme::segmented.segmentFirstTex = texCommon.segmentedButtonFirst;
    Theme::segmented.segmentLastTex = texCommon.segmentedButtonLast;

    auto segmentedSize = Size{safe.w - 20, 40};
    auto segmented = scene->newSegmented({
        .rect = safe.positioned(segmentedSize, {0.5, 0.8}),
    });


    segmented->addSegment("Plain");
    segmented->addSegment("Border");
    segmented->addSegment("Texture");
    segmented->addSegment("Decor");


    scene->newButton({
        .text = "Show",
        .rect = safe.positioned(Theme::dialog.buttonSize, {0.5, 0.6}),
        .pushSignal = [this, segmented]() {
            auto choice = segmented->selectedSegment();
               
            Theme::dialog.clear();

            switch(choice) {
            case 1: setBorderStyle(); break;
            case 2: setTextureStyle(); break;
            case 3: setDecorStyle(); break;
            default: break;
            }

            App::getScene()->newDialog({
                .type = DialogType::YesNo,
                .dispose = true,
                .run = true,
                .title = "Question",
                .message = "Are you sure?",
                .confirmButtonText = "Yes",
                .rejectButtonText = "No",
                .confirmSignal = [this] {
                    setStatusMessage("Confirmed.");
                },
                .rejectSignal = [this] {
                    setStatusMessage("Rejected");
                },
            });
        },
    });

}

void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}



void TestScreen::setBorderStyle() 
{
    auto& dialog = Theme::dialog;

    dialog.backgroundColor = M::Olive;


    auto& border = dialog.border;

    border.radius = 10;
    border.strikes = {
        {M::Red, 7, 0},
        {M::Blue, 3, 0},
    };

    border.radius = 6;
    border.strikes = {
        {M::Red, 2, 0},
    };
}

void TestScreen::setTextureStyle() {


    auto& textureTheme_ = M::Styles::dialog;
    textureTheme_.clear();

    auto& dialogTheme = textureTheme_;

    Size blockSize{60, 60};

    dialogTheme.titleTextColor = 0xd1F1fb;

    dialogTheme.smallSize = {320, 240};

    dialogTheme.backgroundWidgetParams = {
        .backgroundTex = texCommon.dialogBackgroundFill,
        .cornerTex = texCommon.dialogBackgroundCorners,
        .borderTex = texCommon.dialogBackgroundBorders,

        .blockSize = blockSize,
    };

    dialogTheme.padding.set(8);
    dialogTheme.margins.set(24);


    // Decor


    auto& buttonTheme = dialogTheme.button;
    buttonTheme.backgroundColor.clear();
    buttonTheme.textColor = dialogTheme.titleTextColor;
    buttonTheme.pressedBackgroundColor = Color{M::DarkGray, 0.2};
    buttonTheme.border.radius = 8;
    buttonTheme.border.strikes = {
        //{M::Red, 7, 0},
        //{buttonTheme.textColor, 1, 0},
        {0x66a5cf, 1, 0},
    };
}


static WidgetP dialogDecorCallback(DialogP dialog) {
    auto scene = dialog->getScene();
    auto widget = scene->newStackWidget({});

    // Bottom separator
    auto separatorColor = M::Red;

    auto theme = dialog->getTheme();

    auto left = theme->margins.left + theme->padding.left;
    auto right = theme->margins.right + theme->padding.right;
    auto bottom = theme->margins.bottom + theme->padding.bottom;
    auto top = theme->margins.top + theme->padding.top;

    widget->addPrim(scene->newBox(separatorColor, {}), {
        .left = left,
        .right = right,
        .bottom = theme->contentsPadding.bottom,
        .height = 1,
    });


    widget->addPrim(scene->newBox(separatorColor, {}), {
        .left = left,
        .top = theme->contentsPadding.top,
        .right = right,
        .height = 1,
    });


    return widget;
}

void TestScreen::setDecorStyle() {

    auto& decorTheme_ = Theme::dialog;
    decorTheme_.clear();

    auto& dialogTheme = decorTheme_;

    auto& buttonTheme = dialogTheme.button;
    buttonTheme.backgroundColor.clear();
    buttonTheme.textColor = 0xd1F1fb;
    buttonTheme.pressedBackgroundColor = Color{M::DarkGray, 0.2};


    Size blockSize{60, 60};

    dialogTheme.titleTextColor = 0xd1F1fb;

    dialogTheme.smallSize = {320, 240};

    dialogTheme.backgroundWidgetParams = {
        .backgroundTex = texCommon.dialogBackgroundFill,
        .cornerTex = texCommon.dialogBackgroundCorners,
        .borderTex = texCommon.dialogBackgroundBorders,

        .blockSize = blockSize,
    };

    dialogTheme.spacing = {16, 16};
    dialogTheme.padding.set(8);
    dialogTheme.margins.set(24);
    dialogTheme.contentsPadding.set(0, 70);


    // Decor
    dialogTheme.decorCreateCallback = dialogDecorCallback;
}
