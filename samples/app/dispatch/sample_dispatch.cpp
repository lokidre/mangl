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
    void threadFunc1();
    void threadFunc2();
    void threadFunc3();

protected:
    void onStart() override;
};

void TestScreen::onStart()
{
    // Calling 
    Dispatch::asyncMain([this] {
        buildSampleBackground();
    });


    std::thread thread1([this] { threadFunc1();});
    thread1.detach();


    std::thread thread2([this] { threadFunc2();});
    thread2.detach();

    std::thread thread3([this] { threadFunc3();});
    thread3.detach();


    Dispatch::asyncMain([this] {
        buildRenderStats();
    });
}


void TestScreen::threadFunc1()
{
    ElementsContainer elements;

    // Create scene
    Dispatch::asyncMain([&elements] {
        auto scene = App::getScene();
        int numPrims = 30;
        for (int i = 0; i < numPrims; ++i) {
            auto& el = elements.emplace_back();
            el.color = M::Red;
            el.rect = {20, 20, 30, 40};
            el.prim = scene->newBox(el.color, el.rect)->setVisible(false);
        }
    });


    // Main cycle
    for(;;) {
        M::sleep(0.07);

        Dispatch::asyncMain([&elements] {
            auto safe = Env::getSafeArea();
            auto scene = App::getScene();

            for (auto& el: elements) {
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

    }

}


void TestScreen::threadFunc2()
{

    ElementsContainer elements;


    // Create scene
    Dispatch::asyncMain([&elements] {
        auto scene = App::getScene();
        int numPrims = 30;
        for (int i = 0; i < numPrims; ++i) {
            auto& el = elements.emplace_back();
            el.color = M::Red;
            el.rect = {20, 20, 30, 40};
            el.prim = scene->newBox(el.color, el.rect)->setVisible(false);
        }
    });




    // Main cycle
    for(;;) {
        M::sleep(0.08);

        Dispatch::asyncMain([&elements] {
            auto scene = App::getScene();
            auto safe = Env::getSafeArea();

            for (auto& el: elements) {
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

    }

}


void TestScreen::threadFunc3()
{

    ElementsContainer elements;
    std::atomic_bool ready{false};


    // Create scene
    Dispatch::asyncMain([&elements, &ready] {
        auto scene = App::getScene();
        int numPrims = 30;
        for (int i = 0; i < numPrims; ++i) {
            auto& el = elements.emplace_back();
            el.color = M::Red;
            el.rect = {20, 20, 30, 40};
            el.prim = scene->newRect(el.color, el.rect, 2)->setVisible(false);
        }

        ready = true;
    });


    // Main cycle
    for(;;) {
        M::sleep(0.09);

        if (!ready)
            continue;

        auto safe = Env::getSafeArea();

        for (auto& el: elements) {
            el.visible = true;

            M::random.range(20, 100, &el.rect.w);
            M::random.range(20, 100, &el.rect.h);

            M::random.range(safe.x, safe.w + safe.x - el.rect.w, &el.rect.x);
            M::random.range(safe.y, safe.h + safe.y - el.rect.h, &el.rect.y);


            M::random.range(0.4, 1, &el.color.r);
            M::random.range(0.4, 1, &el.color.g);
            M::random.range(0.4, 1, &el.color.b);

            Dispatch::asyncMain([el] {
                el.prim->move(el.rect)->setColor(el.color)->setVisible(el.visible);
            });
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
