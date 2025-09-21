/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

#include <thread>
#include <chrono>

namespace sample {

struct ElementInfo {
    bool changed{};
    PrimP prim{};
    bool visible{};
    Rect rect{};
    Color color{};
};
using ElementsContainer = std::vector<ElementInfo>;


struct WorkerContext {
    ElementsContainer workerElements, renderElements;
    std::mutex mutex;
    std::thread thread;
};
using WorkersContainer = std::vector<std::shared_ptr<WorkerContext>>;


class TestScreen: public Screen {

    using Self = TestScreen;


    WorkersContainer workerContexts_;

    std::atomic_bool finished_;

    void worker1();
    void worker2();
    void worker3();

protected:
    void onStart() override;
    void onTimer(Timestamp) override;
};

void TestScreen::onStart()
{
    buildSampleBackground();

    auto scene = App::getScene();
    auto safe = Env::getSafeArea();

    auto initCtx = [&](WorkerContext* ctx) {
        int numPrims = 30;
        for (int i = 0; i < numPrims; ++i) {
            auto& el = ctx->workerElements.emplace_back();
            el.color = M::Red;
            el.rect = {20, 20, 30, 40};
            el.prim = scene->newBox(el.color, el.rect)->setVisible(false);
        }
    };

    auto ctx = std::make_shared<WorkerContext>();
    initCtx(ctx.get());
    workerContexts_.emplace_back(ctx);

    ctx = std::make_shared<WorkerContext>();
    initCtx(ctx.get());
    workerContexts_.emplace_back(ctx);


    workerContexts_[0]->thread = std::thread(&Self::worker1, this);
    workerContexts_[1]->thread = std::thread(&Self::worker2, this);

    buildRenderStats();
}


void TestScreen::onTimer(Timestamp)
{
    for (auto& ctx: workerContexts_) {
        std::scoped_lock lock{ctx->mutex};
        for (auto& el: ctx->renderElements) {
            if (!el.changed)
                continue;
            el.changed = false;
            el.prim->move(el.rect)->setColor(el.color)->setVisible(el.visible);
        }
    }

}


void TestScreen::worker1() {
    auto ctx = workerContexts_[0].get();

    auto safe = Env::getSafeArea();

    while (!finished_) {
        for (auto& el: ctx->workerElements) {
            el.changed = true;
            el.visible = true;

            M::random.range(20, 100, &el.rect.w);
            M::random.range(20, 100, &el.rect.h);

            M::random.range(safe.x, safe.w + safe.x - el.rect.w, &el.rect.x);
            M::random.range(safe.y, safe.h + safe.y - el.rect.h, &el.rect.y);


            M::random.range(0.4, 1, &el.color.r);
            M::random.range(0.4, 1, &el.color.g);
            M::random.range(0.4, 1, &el.color.b);
        }


        {
            std::scoped_lock lock{ctx->mutex};
            ctx->renderElements = ctx->workerElements;
        }

        M::sleep(0.02);
    }
}

void TestScreen::worker2() {
    auto ctx = workerContexts_[1].get();

    auto safe = Env::getSafeArea();


    while (!finished_) {
        for (auto& el: ctx->workerElements) {
            el.changed = true;
            el.visible = true;

            M::random.range(20, 100, &el.rect.w);
            M::random.range(20, 100, &el.rect.h);

            M::random.range(safe.x, safe.w + safe.x - el.rect.w, &el.rect.x);
            M::random.range(safe.y, safe.h + safe.y - el.rect.h, &el.rect.y);

            M::random.range(0.4, 1, &el.color.r);
            M::random.range(0.4, 1, &el.color.g);
            M::random.range(0.4, 1, &el.color.b);
        }


        {
            std::scoped_lock lock{ctx->mutex};
            ctx->renderElements = ctx->workerElements;
        }

        M::sleep(0.08);
    }
}

void TestScreen::worker3() {
    while (!finished_) {

        M::sleep(0.1);
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

}

