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
    StackWidget* stack_{};
    Rect stackRect_{};

    Rect stackTrackingRect_{0, 0, 30, 30};
    bool stackTracking_{};
    Point stackTrackingLast_{};

    void moveStack(RectA r) {
        stackTrackingRect_.x = r.right() - stackTrackingRect_.w;
        stackTrackingRect_.y = r.y; // top() - stackTrackingRect_.h;

        stackRect_ = r;

        stack_->move(stackRect_);
    }

protected:
    void onStart() override;
    void onTouchesBegin(TouchesA touches); //override;
    void onTouchesMove(TouchesA touches);// override;
    void onTouchesEnd(TouchesA touches) ;//override;
};


void TestScreen::onStart() {

    auto scene = App::getScene();
    auto layout = Env::getSafeArea();

    auto center = layout.center();

    scene->newBox(Theme::body.backgroundColor, Env::getFullArea());

    stack_ = scene->newStackWidget({
        //.rect = stackRect_,
    });

    stackRect_ = layout;
    stackRect_.shrinkX(layout.w * 0.1);
    stackRect_.shrinkW(layout.w * 0.1);
    stackRect_.shrinkY(layout.h * 0.2);
    stackRect_.shrinkH(layout.h * 0.2);

    moveStack(stackRect_);


    // Background
    stack_->addPrim(scene->newBox(M::Gray));

    // Border
    stack_->addPrim(scene->newRect(M::Blue));

    // Center rectangle
    stack_->addPrim(scene->newRect(M::Yellow), {
                                                   .left = 0.5,
                                                   .bottom = 0.5,
                                                   .width = 30,
                                                   .height = 40,
                                                   .flags = M::AlignCenterAll,
                                               });

    // Rect rect{layout.x + 10, layout.top() - 50, 120, 40};

    // auto button = scene->newButton({
    //     .text = "Button",
    //     .rect = rect,
    //     .pushSignal = []() {
    //         MDebugPrint("Button pressed.");
    //     },
    // });

    //    rect.y -= 50;
    //
    //    scene->newButton({.text = "Button T", .rect = rect});


    // Resize grip
    stack_->addPrim(scene->newBox(M::LightGray)->setAlpha(0.4), {
                                                                    .right = 0,
                                                                    .bottom = 0,
                                                                    .width = stackTrackingRect_.w,
                                                                    .height = stackTrackingRect_.h,
                                                                });
}


void TestScreen::onTouchesBegin(TouchesA touches) {
    auto& p = touches.touches[0].p;
    if (stackTrackingRect_.contains(p)) {
        stackTracking_ = true;
        stackTrackingLast_ = p;
    }
}

void TestScreen::onTouchesMove(TouchesA touches) {
    auto& p = touches.touches[0].p;
    if (stackTracking_) {
        auto delta = p - stackTrackingLast_;
        stackRect_.expandW(delta.x);
        stackRect_.shrinkY(delta.y);
        moveStack(stackRect_);

        stackTrackingLast_ = p;
    }
}

void TestScreen::onTouchesEnd(TouchesA touches) { stackTracking_ = false; }


void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

} // namespace

