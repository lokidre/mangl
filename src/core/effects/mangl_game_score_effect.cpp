/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_game_score_effect.h"

namespace mangl {

GameScoreEffect& GameScoreEffect::create(
    SceneP scene, RectA contentsFrame, const Font* font, Cord fontHeight, Flags textAlignment
) {
    //
    // two parameters we need to deduce: start and end positions
    //
    frame_ = contentsFrame;

    // traveling distance
    fontHeight_ = fontHeight; // font.height() ;
    if (distance_ == 0)
        distance_ = fontHeight_ * 3 / 2;

    for (int i = 0; i < WidgetsCount; ++i) {
        effectLabel_[i] = scene->newWidget<Label>()
                              ->setFont(font)
                              ->setTextHeight(fontHeight)
                              ->move(contentsFrame)
                              ->setTextAlignment(textAlignment)
                              ->create();
        fadeEffect_[i] =
            scene->newEffect<FadeEffect>()->setDuration(duration_ * 1.5)->attach(effectLabel_[i]);
        moveEffect_[i] = scene->newEffect<MoveEffect>()
                             ->setOffset({0, distance_})
                             ->setDuration(duration_)
                             ->setDisappear()
                             ->attach(effectLabel_[i]);
    }

    created_ = true;
    
    return *this;
}

GameScoreEffect& GameScoreEffect::setDuration(Timestamp duration) {
    duration_ = duration;
    if (!created_)
        return *this;
    
    for (int i = 0; i < WidgetsCount; ++i) {
        moveEffect_[i]->setDuration(duration);
        fadeEffect_[i]->setDuration(duration * 1.5);
    }
    return *this;
}

GameScoreEffect& GameScoreEffect::setDistance(Cord distance) {
    if (!created_)
        return *this;
    
    distance_ = distance;
    for (int i = 0; i < WidgetsCount; ++i) {
        moveEffect_[i]->setOffset({0., distance});
    }
    return *this;
}


GameScoreEffect& GameScoreEffect::move(RectA contentsFrame) {
    if (!created_)
        return *this;
    for (int i = 0; i < WidgetsCount; ++i) {
        effectLabel_[i]->move(contentsFrame);
    }
    return *this;
}

GameScoreEffect& GameScoreEffect::start(int score) {
    if (!created_)
        return *this;
    auto contentsFrame = frame_;

    int labelIndex = 0;
    if (moveEffect_[labelIndex]->active()) {
        labelIndex++;
        contentsFrame.y -= fontHeight_;
    }

    if (score > 0)
        effectLabel_[labelIndex]->setText("+%d", score);
    else
        effectLabel_[labelIndex]->setInt(score);

    effectLabel_[labelIndex]->clearAlpha()->move(contentsFrame)->setVisible();
    fadeEffect_[labelIndex]->start();
    moveEffect_[labelIndex]->start();

    return *this;
}

} // namespace mangl
