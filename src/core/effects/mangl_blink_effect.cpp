/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_blink_effect.h"

namespace mangl {

void BlinkEffect::onStart() {
    blinkVisible_ = true;
    blinkTimestamp_ = currentTimestamp_;
}

void BlinkEffect::onStop() {
    if (!permanent_) {
        // restore the state
        //for ( MSceneSprite
    }
}

void BlinkEffect::onTimer(Timestamp lambda) {
    if (currentTimestamp_ >= blinkTimestamp_) {
        blinkVisible_ = !blinkVisible_;
        blinkTimestamp_ = currentTimestamp_ + blinkPeriod_;
    }

    if (attachedPrim_) attachedPrim_->setVisible(blinkVisible_);
    if (attachedWidget_) attachedWidget_->setVisible(blinkVisible_);
}

} // namespace mangl
