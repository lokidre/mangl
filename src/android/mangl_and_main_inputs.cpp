/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_main_engine.h"


#undef MDebugPrint
#define MDebugPrint MDebugNoop

namespace mangl {

namespace mangl_impl_ {

static inline Point screenToPoint(float x, float y) {
    auto px = Cord(x) / Env::screenScale;
    auto py = Cord(y) / Env::screenScale;

    py = Env::layoutSize.h - 1 - py;
    return {px, py};
}

struct TouchStatus {
    bool down;
    float x;
    float y;
};

static std::array<TouchStatus, MaxTouches> touchStatuses_{};

static inline void findAvailTouchStatus(float x, float y, int* index, int* count) {
    float minDistance = std::numeric_limits<float>::max() - 1;

    *index = -1;
    *count = 0;

    int i = 0;
    for (auto& ts: touchStatuses_) {
        if (ts.down) {
            float distance = (ts.x - x) * (ts.x - x) + (ts.y - y) * (ts.y - y);
            if (distance < minDistance) {
                minDistance = distance;
                *index = i;
            }
            ++(*count);
        }
        ++i;
    }
}

}  // namespace mangl_impl_

using namespace mangl_impl_;

void AndroidMainEngine::processMotionEventDown(GameActivityMotionEvent *motionEvent, float x, float y) {
    if (touchesResumed_) {
        for (auto& ts: touchStatuses_)
            ts.down = false;
        touchesResumed_ = false;
    }

    int count = 0;
    for (auto& ts: touchStatuses_)
        if (ts.down)
            ++count;

    int index = 0;
    for (auto& ts: touchStatuses_) {
        if (!ts.down) {
            ++count;

            MDebugPrint("Touches::Down: i: %d; c:%d; p: %g, %g", index, count, x, y);

            ts.x = x;
            ts.y = y;
            ts.down = true;

            framework_->onTouchDown({
                .p = screenToPoint(x, y),
                .timestamp = Framework::timestampAtomic(),
                .index = index,
                .count = count,
            });


            break;
        }
        ++index;
    }


//    findAvailTouchStatus(x, y, &index, &count);
//    MDebugPrint("Touches::Check: i: %d; c:%d; p: %g, %g", index, count, x, y);


}

void AndroidMainEngine::processMotionEventUp(GameActivityMotionEvent* motionEvent, float x, float y) {

    int index, count;

    findAvailTouchStatus(x, y, &index, &count);

    if (index < 0)
        return;

    MDebugPrint("Touches::Up: i: %d; c:%d; p: %g, %g", index, count, x, y);

    auto& ts = touchStatuses_[index];
    ts.down = false;
    ts.x = x;
    ts.y = y;

    framework_->onTouchUp({
            .p = screenToPoint(x, y),
            .timestamp = Framework::timestampAtomic(),
            .index = index,
            .count = count,
    });

}

void AndroidMainEngine::processMotionEventMove(GameActivityMotionEvent *motionEvent, float x, float y) {
    int index, count;

    findAvailTouchStatus(x, y, &index, &count);

    if (index < 0)
        return;

    auto& ts = touchStatuses_[index];
    ts.x = x;
    ts.y = y;

    framework_->onTouchMove({
            .p = screenToPoint(x, y),
            .timestamp = Framework::timestampAtomic(),
            .index = index,
            .count = count,
    });

}


void AndroidMainEngine::processActivityInputs()
{
    auto inputBuffer = android_app_swap_input_buffers(Jni::app);

    if (!inputBuffer)
        return;

    auto framework = Framework::instance();


    if (inputBuffer->keyEventsCount != 0) {
        for (uint64_t i = 0; i < inputBuffer->keyEventsCount; ++i) {
            GameActivityKeyEvent* event = &inputBuffer->keyEvents[i];

            if (event->action == 1 && event->keyCode == AKEYCODE_BACK) {
                Framework::processBack();
            }
            //keyEvent->keyCode

        }

        android_app_clear_key_events(inputBuffer);
    }

    //MDebugFunc("eventsCount: %d", inputBuffer->motionEventsCount);

    if (inputBuffer->motionEventsCount != 0) {

        for (uint64_t i = 0; i < inputBuffer->motionEventsCount; ++i) {
            GameActivityMotionEvent *motionEvent = &inputBuffer->motionEvents[i];

            int action = motionEvent->action;
            int actionMasked = action & AMOTION_EVENT_ACTION_MASK;
            int ptrIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

            auto pointer = &motionEvent->pointers[ptrIndex];

            auto x = GameActivityPointerAxes_getX(pointer);
            auto y = GameActivityPointerAxes_getY(pointer);


            //GameActivityMotionEvent_
            //auto pointerId = pointer->id; //motionEvent->poin

//            TouchesInfo touch{
//                .count = 1,
//                .touches = {
//                    MFrameworkNative::currentTimestamp(),
//                },
//            };
//
//            auto& p = touch.touches[0].p;
//
//            p.x = GameActivityPointerAxes_getX(pointer);
//            p.y = GameActivityPointerAxes_getY(pointer);
//
//            p /= Env::screenScale;
//            p.y = Env::layoutSize.h - 1 - p.y;


            switch (actionMasked) {
            case AMOTION_EVENT_ACTION_DOWN:
                MDebugPrint("AMOTION_EVENT_ACTION_DOWN: %g, %g; id: %d", x, y, pointer->id);
                processMotionEventDown(motionEvent, x, y);
                //framework->onTouchesBegin(touch);
                break;
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
                MDebugPrint("AMOTION_EVENT_ACTION_POINTER_DOWN: %g, %g; id: %d", x, y, pointer->id);
                processMotionEventDown(motionEvent, x, y);
                break;

            case AMOTION_EVENT_ACTION_UP:
                MDebugPrint("AMOTION_EVENT_ACTION_UP: %g, %g; id: %d", x, y, pointer->id);
                processMotionEventUp(motionEvent, x, y);
                break;

            case AMOTION_EVENT_ACTION_POINTER_UP:
                MDebugPrint("AMOTION_EVENT_ACTION_POINTER_UP; id: %d: %g, %g", x, y, pointer->id);
                processMotionEventUp(motionEvent, x, y);
                break;

            case AMOTION_EVENT_ACTION_MOVE:
                //MDebugPrint("AMOTION_EVENT_ACTION_MOVE: %g, %g; id: %d", x, y, pointer->id);
                processMotionEventMove(motionEvent, x, y);
                break;

            default:
                MDebugPrint("Action code: %d", actionMasked);
                break;
            }


            // Didn't belong to a game controller, process it ourselves if it is a
            // touch event
            //_cook_game_activity_motion_event(motionEvent, _cooked_event_callback);
        }
        android_app_clear_motion_events(inputBuffer);
    }

}


}  // namespace mangl



