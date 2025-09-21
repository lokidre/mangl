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


class SampleSegmentWidget: public SegmentWidget {
    SegmentWidgetParams params_{};
    int index_{}, total_{};
    bool selected_{}, pressed_{};
    PrimP backgroundPrim_{};
    PrimP borderPrim_{};
    PrimP selectedPrim_{};
    LabelP label_{};


    void updateStatus() {
        if (!backgroundPrim_) return;
        backgroundPrim_->setColor(pressed_ ? M::Gray : M::Blue);
        borderPrim_->setColor(pressed_ ? M::Red : M::Green);
        selectedPrim_->setVisible(selected_);
        //label_->setText(text_);
    }

    void updateLayout() {
        if (!backgroundPrim_) return;
        backgroundPrim_->move(this->r_);
        borderPrim_->move(this->r_);
        selectedPrim_->move(this->r_);
        label_->setPos(this->r_.center());
    }

    void updateVisible() {

    }

protected:
    void onSetParams(const SegmentWidgetParams& iparams) override {
        params_ = iparams;
    }

    void onSetIndex(int index, int total) override {
        index_ = index;
        total_ = total;
        updateStatus();
    }

    void onSetSelected(bool selected) override {
        if (selected != selected_) {
            selected_ = selected;
            updateStatus();
        }
    }

    void onSetPressed(bool pressed) override {
        if (pressed != pressed_) {
            pressed_ = pressed;
            updateStatus();
        }
    }


    void onCreate() override {
        auto scene = App::getScene();
        backgroundPrim_ = scene->newBox();
        borderPrim_ = scene->newRect()->setWidth(1);
        selectedPrim_ = scene->newRect(M::Yellow)->setWidth(2);
        label_ = scene->newLabel({
            .text = params_.text,
            .color = M::White,
            .textSize = 15,
            .flags = M::AlignCenterAll,
        });
    }

    void onMove() override { updateLayout(); };
    void onVisible() override { updateVisible(); };
    //void onAlpha() override;

};


void TestScreen::onStart() 
{
    buildSampleBackground();

    auto scene = App::getScene();
    auto layout = Env::getSafeArea();

    buildBackground();

    Size segmentedSize{280, 40};
    SegmentedP segmented{};

    segmented = scene->newSegmented({
        .rect = layout.positioned(segmentedSize, {0.5, 0.8}),
    });

    segmented->addSegment({
        .segment = scene->newWidget<SampleSegmentWidget>(),
        .text = "Left",
    });

    segmented->addSegment({
        .segment = scene->newWidget<SampleSegmentWidget>(),
        .text = "CENTER",
    });

    segmented->addSegment({
        .segment = scene->newWidget<SampleSegmentWidget>(),
        .text = "R",
    });

}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

}
