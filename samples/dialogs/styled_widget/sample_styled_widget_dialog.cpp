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

namespace sample {

class TestScreen: public Screen {
    //void setStyledCallbackTheme();

protected:
    void onStart() override;
};


static WidgetP dialogDecorCallback(DialogP dialog) {
    auto scene = dialog->getScene();
    auto widget = scene->newStackWidget({});

    // Bottom separator
    auto separatorColor = M::Red;
    auto theme = dialog->getTheme();

    auto left = theme->margins.left + theme->padding.left;
    auto right = theme->margins.right + theme->padding.right;

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

static void setStyledCallbackTheme() {
    auto& sdialog = Theme::dialog;
    sdialog.clear();

    auto& sbutton = sdialog.button;
    sbutton.backgroundColor.clear();
    sbutton.textColor = 0xd1F1fb;
    sbutton.pressedBackgroundColor = Color{M::DarkGray, 0.2};


    Size blockSize{60, 60};

    sdialog.titleText.color = 0xd1F1fb;

    sdialog.smallSize = {320, 240};

    sdialog.backgroundWidgetParams = {
        .backgroundTex = texCommon.dialogBackgroundFill,
        .cornerTex = texCommon.dialogBackgroundCorners,
        .borderTex = texCommon.dialogBackgroundBorders,

        .blockSize = blockSize,
    };

    sdialog.spacing = {16, 16};
    sdialog.padding.set(8);
    sdialog.margins.set(24);
    sdialog.contentsPadding.set(0, 70);


    // Decor
    sdialog.decorCreateCallback = dialogDecorCallback;
}



void TestScreen::onStart()
{
    buildSampleBackground();

    auto scene = App::getScene();
    auto safe = Env::getSafeArea();

    auto buttonSize = Theme::dialog.button.size;
    auto buttonRect = safe.positioned(buttonSize, {0.2, 0.8});


    scene->newButton({
        .text = "Decor",
        .rect = buttonRect,
        .onPush = [this] {
            setStyledCallbackTheme();

            App::getScene()->newDialog({
                //.theme = &decorTheme_,
                .type = DialogType::YesNo,
                .title = "Question",
                .message = "Are you sure?",
                .confirmButtonText = "Yes",
                .rejectButtonText = "No",
                .onConfirm = [this] {
                    setStatusMessage("Confirmed.");
                },
                .onReject = [this] {
                    setStatusMessage("Rejected.");
                },
            })->run();
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







} // namespace
