/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

#include <mangl/dispatch.h>

namespace sample {


struct ElementInfo {
    PrimP prim{};
    bool visible{};
    Rect rect{};
    Color color{};
};
using ElementsContainer = std::vector<ElementInfo>;


class TestScreen: public Screen {

    ElementsContainer elements_;
    void threadFunc();

    std::atomic_bool thSetTextFlag_{};
    LabelP testLabel_{};

protected:
    void onStart() override;
};

void TestScreen::onStart()
{
    // Calling 
    buildSampleBackground();


    auto scene = App::getScene();
    auto safe = Env::getSafeArea();

    int numPrims = 30;
    for (int i = 0; i < numPrims; ++i) {
        auto& el = elements_.emplace_back();
        el.color = M::Red;
        el.rect = {20, 20, 30, 40};
        el.prim = scene->newBox(el.color, el.rect)->setVisible(false);
    }



    gridAddScreenButton({
        .text = "TH SET TEXT",
        .onPush = [this] { thSetTextFlag_ = true; },
    });


    testLabel_ = scene->newLabel({
        .text = "Main Thread Label",
        .pos = safe.position({0.5, 0.4}),
        .flags = M::AlignCenterAll,
    });


    std::thread th([this] { threadFunc();});
    th.detach();
}


void TestScreen::threadFunc()
{
    // Main cycle
    for(;;) {
        M::sleep(0.2);

        Dispatch::asyncMain([this] {
            auto safe = Env::getSafeArea();
            auto scene = App::getScene();

            for (auto& el: elements_) {
                el.visible = true;

                M::random.range(20, 100, &el.rect.w);
                M::random.range(20, 100, &el.rect.h);

                M::random.range(safe.x, safe.w + safe.x - el.rect.w, &el.rect.x);
                M::random.range(safe.y, safe.h + safe.y - el.rect.h, &el.rect.y);


                M::random.range(0.4, 1, &el.color.r);
                M::random.range(0.4, 1, &el.color.g);
                M::random.range(0.4, 1, &el.color.b);

                el.prim->move(el.rect)->setColor(el.color)->setVisible(el.visible);
            }
        });

        if (thSetTextFlag_) {
            thSetTextFlag_ = false;
            testLabel_->setText("Thread text");
        }

    }

}





void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

}  // namespace

