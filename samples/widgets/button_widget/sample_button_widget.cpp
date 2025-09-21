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


class SampleButtonContentsWidget: public ButtonContentWidget {

    PrimP backgroundPrim_{};
    LabelP label_{};

protected:
    void onCreate(SceneP scene) override {
        backgroundPrim_ = scene->newBox(M::Blue);
        addChildItem(backgroundPrim_);

        label_ = scene->newLabel({
            .text = "Button",
            .flags = M::AlignCenterAll,
        });
        addChildItem(label_);
    }

    void onMove(RectA rect) override {
        backgroundPrim_->move(rect);
        label_->setPos(rect.center());
    }

    void onPressedState(bool pressed) override {
        backgroundPrim_->setColor(pressed ? M::Red : M::Blue);
    }

    void onSetText(StrView text) override {
        label_->setText(text);
    }
};

void TestScreen::onStart() {
    sampleBuildBackground();

    sampleGridBuildScreenButton({
        .widget = App::getScene()->newWidget<SampleButtonContentsWidget>(),
        .onPush = [this] {
            sampleSetStatusMessage("Button pressed.");
        },
    });
}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

}  // namespce 
