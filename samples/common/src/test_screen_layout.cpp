/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "test_screen.h"
#include "test_app.h"

#ifndef MANGL_CONSOLE
namespace sample {

void Screen::gridStart(const GridParams& p) {
    if (p.direction > 0) {
        currGridY_ = gridStartTopY_;
    } else {
        currGridY_ = gridStartBottomY_;
    }

    gridParams_ = p;
    currGridCol_ = 0;
    //currGridRow_ = 8;
    //currGridY_ = 0.9;
}

void Screen::gridAdvanceCol() {
    currGridCol_++;
    if (currGridCol_ >= gridParams_.cols) {
        sampleGridAdvanceRow();
    }
}

void Screen::sampleGridAdvanceRow() {
    currGridCol_ = 0;
    currGridY_ -= 0.1 * static_cast<Cord>(gridParams_.direction);
}


Rect Screen::gridAddButton() {
    Size size = M::Styles::screen.screenChoiceButton.size;

    Point coeff{0, currGridY_};
    coeff.x = currGridCol_ == 1 ? 0.9 : 0.1;
    gridAdvanceCol();
    return Env::getSafeArea().positioned(size, coeff);
}

ButtonP Screen::gridAddButton(ButtonParams p) {
    p.rect = gridAddButton();
    return App::getScene()->newButton(p);
}


Rect Screen::gridAddScreenButton() {
    if (currGridCol_ != 0)
        sampleGridAdvanceRow();

    auto size = M::Styles::screen.screenSelectButton.size;

    Point coeff{0.5, currGridY_};
    sampleGridAdvanceRow();
    return Env::getSafeArea().positioned(size, coeff);
}

Point Screen::sampleGridLayoutScreenLabel() {
    if (currGridCol_ != 0)
        sampleGridAdvanceRow();
    Point coeff{0.5, currGridY_};
    sampleGridAdvanceRow();
    return Env::getSafeArea().position(coeff);
}


ButtonP Screen::gridAddScreenButton(ButtonParams p) {
    p.rect = gridAddScreenButton();
    return App::getScene()->newButton(p);
}

LabelP Screen::sampleGridAddScreenLabel(LabelParams p) {
    p.pos = sampleGridLayoutScreenLabel();
    p.flags = M::AlignCenterAll;
    return App::getScene()->newLabel(p);
}




Rect Screen::gridAddSlider() {
    Size size{280, 40};
    Point coeff{0.5, currGridY_};
    sampleGridAdvanceRow();
    return Env::getSafeArea().positioned(size, coeff);
}



Rect Screen::gridAddCheckbox(StringA title) {
    Size size{30, 30};
    Point coeff{0, currGridY_};
    coeff.x = currGridCol_ == 1 ? 0.9 : 0.4;

    gridAdvanceCol();

    Rect rect = Env::getSafeArea().positioned(size, coeff);

    if (!title.empty()) {
        App::getScene()->newLabel({
            .text = title,
            .pos = {rect.x - 10, rect.center().y},
            .flags = M::AlignRight | M::AlignVCenter,
            });
    }
    return rect;
}

}  // namespace 

#endif
