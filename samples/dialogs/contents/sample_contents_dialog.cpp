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
    void buildQuestionDialog();
    void buildPauseDialog();

protected:
    void onStart() override;
};

void TestScreen::onStart()
{
    buildSampleBackground();


    buildQuestionDialog();
    buildPauseDialog();
}


void TestScreen::buildQuestionDialog() 
{
    auto scene = App::getScene();

    auto grid = scene->newGridLayout({
        .debug = true,
    });

    int row = 0;
    grid->addItem({
        .pos = {0, row},
        .prim = scene->newBox(M::Red, {}),
    });

    grid->addItem({
        .pos = {1, row},
        .widget = scene->newLabel({
            .text = "Label 1",
        }),
    });

    ++row;


    grid->addItem({
        .pos = {0, row},
        .widget = scene->newButton({
            .text = "Button",
        }),
        .size = {100, 40},
    });

    grid->addItem({
        .pos = {1, row},
        .prim = scene->newBox(M::Magenta, {}),
    });

    ++row;

    grid->addItem({
        .pos = {0, row},
        .widget = scene->newLabel({
            .text = "Checkbox:",
        }),
    });

    grid->addItem({
        .pos = {1, row},
        .widget = scene->newCheckbox({
            .checked = true,
        }),

        .size = {30, 30},
    });



    auto dialog = scene->newDialog({
        .type = DialogType::YesNo,
        .title = "Question",
        .message = "Are you sure?",
        .grid = grid,
        .confirmButtonText = "Yes",
        .rejectButtonText = "No",
        .onConfirm = [this] {
            sampleSetStatusMessage("Confirmed.");
        },
        .onReject = [this] {
            sampleSetStatusMessage("Rejected");
        },
    });

    gridAddScreenButton({
        .text = "Question...",
        .onPush = [this, dialog] {
            dialog->run();
            sampleSetStatusMessage("Showing...");
        },
    });

}

void TestScreen::buildPauseDialog() 
{
    auto scene = App::getScene();

    //Size toolButtonSize{30, 30};
    auto toolButtonSize = M::Styles::screen.screenToolButton.size;

    auto grid = scene->newGridLayout({
        .debug = true,
    });


    auto onWidget = scene->newStackWidget({

    });

    onWidget->addPrim(scene->newBox(M::Magenta, {}), {
        //.size = toolButtonSize, 
        //.flags = M::AlignCenterAll,
    });


    auto offWidget = scene->newStackWidget({

    });
    offWidget->addPrim(scene->newBox(M::Yellow, {}), {
        //.size = toolButtonSize,
        //.flags = M::AlignCenterAll,
    });

    auto checkbox = scene->newCheckbox({
        .onWidget = onWidget,
        .offWidget = offWidget,
    });

    grid->addItem({
        .pos = {0, 0},
        .widget = checkbox,
        .size = toolButtonSize,
    });

    grid->addItem({
        .pos = {1, 0},
        .prim = scene->newBox(M::Blue, {}),
        .size = toolButtonSize,
    });

    grid->addItem({
        .pos = {2, 0},
        .prim = scene->newBox(M::Magenta, {}),
        .size = toolButtonSize,
    });


    auto dialog = scene->newDialog({
        .type = DialogType::YesNo,
        .title = "Game Paused",
        //.message = "Are you sure?",
        .grid = grid,
        .confirmButtonText = "RESUME",
        .rejectButtonText = "QUIT",
        .onConfirm = [this] {
            setStatusMessage("Resumed.");
        },
        .onReject = [this] {
            setStatusMessage("Quit.");
        },
    });


    gridAddScreenButton({
        .text = "PAUSE ...",
        .onPush = [this, dialog]() {
            dialog->run();
            setStatusMessage("Showing...");
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

} // namespace sample

