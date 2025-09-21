/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_background.h"

namespace mangl {


void MBackground::onTimer(Timestamp timestamp, Timestamp last_timestamp)
{
    if (!started_)
        return;

    current_timestamp_ = timestamp;
    last_timestamp_ = last_timestamp;

    auto delta_t = current_timestamp_ - last_timestamp_;

    for (auto &layer : layers_) {
        for (auto elIt = elements_.begin() + layer.elementIndexBegin, elEnd = elements_.begin() + layer.elementIndexEnd; elIt != elEnd; ++elIt) {
            auto &el = *elIt;

            if (!el.valid)
                continue;

            if (!el.static_flag && !paused_ && (el.velocity.x != 0 || el.velocity.y != 0)) {

                auto rect = el.so->getRect();

                rect.move({rect.x + el.velocity.x * delta_t * speed_k, rect.y += el.velocity.y * delta_t * speed_k});

                // TODO: check the total layer extent to cover more than the screen


                //
                // Check the border crossing case
                //
                bool fly_out = false;

                if (el.velocity.x < 0 && rect.x + rect.w < viewport_.x)
                    fly_out = true;
                else if (el.velocity.x > 0 && rect.x > viewport_.x + viewport_.w)
                    fly_out = true;
                else if (el.velocity.y < 0 && rect.y + rect.h < viewport_.y)
                    fly_out = true;
                if (el.velocity.y > 0 && rect.y > viewport_.y + viewport_.h)
                    fly_out = true;

                if (fly_out) {
                    el.so->setVisible(false);
                    el.valid = false;
                } else {
                    el.so->move(rect);
                }

                // check if we'd better call the 
                if (!layer.period.set || layer.full_screen)
                    backgroundProcessTimestamp_ = 0;
            }

            // process animation
            if (el.valid && el.texture.count > 1 && current_timestamp_ >= el.texture_timestamp) {
                el.texture_current++;
                if (el.texture_current >= el.texture.count)
                    el.texture_current = 0;

                el.so->setTexture(el.texture, el.texture_current);

                el.texture_timestamp = current_timestamp_ + el.texture_frame;
            }

            // process flicks
            if (el.valid && el.alpha_flick && current_timestamp_ >= el.alpha_flick_timestamp) {
                el.alpha_current_index++;
                if (el.alpha_current_index == el.BK_ELEMENT_FLICKS)
                    el.alpha_current_index = 0;
                el.so->setAlpha(el.alpha_values[el.alpha_current_index]);
                el.alpha_flick_timestamp += el.alpha_period;
            }
        }
    }

    // process next appearing request
    if (current_timestamp_ >= backgroundProcessTimestamp_ && !paused_) {
        process_appear();
        backgroundProcessTimestamp_ = current_timestamp_ + 0.125f;
    }


}

}
