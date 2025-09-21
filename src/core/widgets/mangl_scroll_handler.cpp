/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_scroll_handler.h"

#include "mangl_scroll.h"

#include <cmath>

namespace mangl {


void ScrollWidgetHandler::onStart(Timestamp timestamp)
{
    //windowSize = scroll_->r_.w ;
}

bool ScrollWidgetHandler::onTimer(Timestamp timestamp)
{
    using namespace cognit;

    bool moved = false;

    auto deltat = timestamp - lastTimestamp_;

    if (moving_) {

        if (targetMoving_) {
            Cord halfDistance = abs(targetPosition_ - startPosition_) / 2;
            Cord distanceFromStart = abs(currentOffset - startPosition_);
            Cord distanceFromCenter = abs(halfDistance - distanceFromStart);

            acceleration_ = halfDistance / 7 + 5 * (halfDistance + 1) / (distanceFromCenter + 1);

            acceleration_ *= acceleration_;

            velocity_ = acceleration_ * direction_;
        }

        moved = true;
    }

    if (moving_) {
        if (inertiaMoving_) {

            // Check the boundaries
            // If we are out of boundaries, then quickly stop the inertia and scroll back
            //if ( currentOffset < -windowSize * 0.8 || currentOffset >= contentsSize - windowSize + windowSize * 0.2 ) {
            if (currentOffset < -windowSize * 0.1 || currentOffset >= contentsSize - windowSize + windowSize * 0.1) {
                scrollToSnap();
            } else {

                auto x = (timestamp - inertiaStartTimestamp_);
                auto at = std::pow(0.2, x);

                velocity_ = inertiaSpeed_ * at;

                if (velocity_ < windowSize) {
                    inertiaMoving_ = false;
                    velocity_ = 0;
                    if (!pagingEnabled) {
                        targetPosition_ = currentOffset;
                    }
                    scrollToSnap();
                } else {
                    velocity_ *= direction_;
                }
            }
        }
    }


    if (moving_) {
        if (inertiaMoving_ || targetMoving_)
            currentOffset += deltat * velocity_;
    }

    if (moving_) {
        //
        // Update page information
        //
        if (!inertiaMoving_ && !touchesBegan_) {

            // Check the end condition
            if (tiny(currentOffset - targetPosition_)) {
                moving_ = false;
                currentOffset = targetPosition_;
            } else {
                if (direction_ > 0) {
                    if (currentOffset > targetPosition_) {
                        moving_ = false;
                        currentOffset = targetPosition_;
                    }
                } else {
                    if (currentOffset < targetPosition_) {
                        moving_ = false;
                        currentOffset = targetPosition_;
                    }
                }
            }

        }

        if (pagingEnabled) {
            // determine the current page
            auto centerLine = currentOffset + windowSize/2;
            auto& current = pages[currentPage];
            if (centerLine < current.start || centerLine >= current.start + current.size) {
                int index = 0;
                for (auto &page: pages) {
                    if (centerLine >= page.start && centerLine < page.start + page.size) {
                        currentPage = index;
                        break;
                    }
                    ++index;
                }
            }

            if (currentPage != prevPageIndex) {
                if (scroll_->pageSelectedSignal)
                    scroll_->pageSelectedSignal(scroll_->controlId_, currentPage);
                if (scroll_->anonPageSelectedSignal)
                    scroll_->anonPageSelectedSignal(currentPage);

                prevPageIndex = currentPage;
            }
        }


    }


    lastTimestamp_ = timestamp;

    return moved;
}

void ScrollWidgetHandler::touchesBegin(Cord position, Timestamp timestamp)
{
    beginTouchPosition_ = lastTouchPosition_ = position;
    touchesBegan_ = true;
    moving_ = true;
    targetMoving_ = inertiaMoving_ = false;
    lastTouchTimestamp_ = timestamp;
    inertiaMeasuresCount_ = 0;
}

void ScrollWidgetHandler::touchesMove(Cord position, Timestamp timestamp)
{
    auto deltaS = position - lastTouchPosition_;

    currentOffset += deltaS;

    auto deltaT = timestamp - lastTouchTimestamp_;

    if (deltaT > 0.001) {
        auto v = deltaS / deltaT;

        if (inertiaMeasuresCount_ < InertiaMeasures)
            inertiaMeasuresCount_++;

        // shift the velocities
        for (int i = inertiaMeasuresCount_ - 1; i > 0; --i) {
            inertiaVelocityMeasures_[i] = inertiaVelocityMeasures_[i-1];
        }

        inertiaVelocityMeasures_[0] = v;

        // verify the direction, if the direction changes - erase the rest
        for (int i = 1; i < inertiaMeasuresCount_; ++i) {
            if (std::signbit(inertiaVelocityMeasures_[i]) != std::signbit(v)) {
                inertiaMeasuresCount_ = i;
                break;
            }
        }

    }

    lastTouchPosition_ = position;
    lastTouchTimestamp_ = timestamp;
}

void ScrollWidgetHandler::touchesEnd(Cord position, Timestamp timestamp)
{
    touchesBegan_ = false;

    // If the paging is enabled

    // 
    // estimate inertia velocity
    //
    auto deltaT = timestamp - lastTouchTimestamp_;

    if (inertiaMeasuresCount_ > 0 && deltaT < 0.1) {
        Timestamp v = 0;
        auto d = (Cord)inertiaMeasuresCount_;

        for (int i = 0; i < inertiaMeasuresCount_; ++i) {
            v += inertiaVelocityMeasures_[i] / d;
        }
        //MDebugPrint("Inertia. Count: %d, velocity: %d", inertiaMeasuresCount_, v);

        inertiaMeasuresCount_ = 0;

        inertiaStartTimestamp_ = lastTimestamp_;

        velocity_ = v;
        direction_ = velocity_ < 0 ? -1 : 1;

        inertiaSpeed_ = velocity_ * direction_;

        inertiaMoving_ = true;
        moving_ = true;
    }


    // If at the end of the touch the scroll will not be moving
    // check if there is need for automatic moving towards the boundaries
    if (!inertiaMoving_) {
        if (!pagingEnabled) {
            //if (currentOffset < 0)
            //    targetPosition_ = 0;
            //else if (currentOffset > contentsSize - windowSize)
            //    targetPosition_ = contentsSize - windowSize;
            //else
                targetPosition_ = currentOffset;
        }
        scrollToSnap();
    }


    lastTouchTimestamp_ = timestamp;
}


void ScrollWidgetHandler::scrollToPage(int pageIndex)
{
    scrollToPageBody(pageIndex);
}

void ScrollWidgetHandler::scrollToPageBody(int pageIndex)
{
    int index = 0;
    startPosition_ = currentOffset;
    Cord target = 0;

    for (auto &p : pages) {
        if (index == pageIndex)
            break;
        target += p.size;
        ++index;
    }

    scrollToTarget(target);
}

void ScrollWidgetHandler::scrollToPosition(Cord position)
{
    scrollToTarget(position);
}

void ScrollWidgetHandler::scrollToTarget(Cord target)
{
    targetPosition_ = target;

    moving_ = true;
    targetMoving_ = true;
    targetMovingInitialized_ = false;

    startPosition_ = currentOffset;

    direction_ = targetPosition_ - startPosition_ < 0 ? -1 : 1;
    velocity_ = 10 * direction_;
    acceleration_ = 2;
    targetMovingInitialized_ = true;
}

void ScrollWidgetHandler::scrollToSnap()
{
    inertiaMoving_ = false;

    if (contentsSize <= windowSize) {
        if (snapToEnd)
            scrollToTarget(contentsSize - windowSize);
        else
            scrollToTarget(0);
        return;
    }

    bool snapped = false;

    if (currentOffset > contentsSize - windowSize) {
        snapped = true;
        scrollToTarget(contentsSize - windowSize);
        //scrollToTarget(0);
    } else if (currentOffset < 0) {
        snapped = true;
        //scrollToTarget(-contentsSize + windowSize);
        scrollToTarget(0);
    }

    if (!snapped && pagingEnabled && !pages.empty()) {
        auto halfOffset = currentOffset + pages[0].size/2;
        for (auto &page : pages) {
            if (halfOffset >= page.start && halfOffset < page.start + page.size && halfOffset != page.start) {
                scrollToTarget(page.start);
                break;
            }
        }
    }
}


void ScrollWidgetHandler::scrollToBegin()
{
    scrollToTarget(0);
}

void ScrollWidgetHandler::scrollToEnd()
{
    scrollToTarget(contentsSize - windowSize);
}

}  // namespace mangl
