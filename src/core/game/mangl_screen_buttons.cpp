/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_game_screen.h"

#include "../mangl_styles.h"
#include "../mangl_scene.h"
#include "../mangl_ads.h"



namespace mangl {

ButtonP GameScreenBase::buildNavButton(const GameScreenToolButtonStyles& styles, Point position, ScreenType type) {

    auto size = styles.button.size;

    auto rect = contentArea_.positioned(size, position);

    rect.y = screenTitle_.rect.centered(size).y;

    rect.y += styles.titleOffset.y;
    rect.x += styles.titleOffset.x;

    Margins margins{};

    if (position.x == 1) {
        margins.r = fullArea_.right() - rect.right();
    }
    if (position.x == 1) {
        margins.l = rect.x - fullArea_.x;
    }

    if (position.y == 1) {
        margins.t = fullArea_.top() - rect.top();
    }

    auto button = mainScene_->newButton({
        .rect = rect,
        .margins = margins,
        .texture = styles.button.texture,
        .onPush = styles.onPush,
    });

    return button;
}



void GameScreenBase::layoutScreenButtons(ScreenButtonsParams& buttons, LayoutScreenButtonsParamsA params)
{
    const auto& screenStyles = M::Styles::screen;

    auto textSize = screenStyles.screenButtonHint.size;
    Real textSpacing{4};
    auto textExtent = textSize + textSpacing;
    Real explGap{6};

    //
    // First determine the largest widget height
    //
    //Cord buttonsExtent{};
    //Size buttonExtent {};
    Cord buttonsTotalHeight {};
    Real explExtent{};
    int explCount{};

    for (auto &bi: buttons) {
        //Size buttonExtent {};
        bi.extent = {};

        if (bi.size.w == 0)
            bi.size = screenStyles.screenSelectButton.size;

        auto h = bi.size.h;

        h += bi.margins.top();
        h += bi.margins.bottom();


        if (!bi.rtExplanation.empty()) {
            h += explGap + bi.rtExplanation.size() * textExtent;
        }
        if (h > bi.extent.h)
            bi.extent.h = h;
        if (h - bi.size.h > explExtent)
            explExtent = h - bi.size.h;
        if ((int)bi.rtExplanation.size() >= explCount)
            explCount = (int)bi.rtExplanation.size();


        buttonsTotalHeight += h;
    }

    //
    // Total buttons spacing
    //
    //auto buttonsTotalHeight = buttons.size() * buttonExtent.h;


    //
    // Now calculate the area where we need to fit the buttons
    //
    auto area = Env::getSafeArea();

    if (screenTitle_.rect.h != 0)
        area.h = screenTitle_.rect.y - area.y;

    auto padding = params.margins;

    if (padding.bottom() != 0) {
        area.shrinkY(padding.bottom());
    } else {
        if (Ads::bannerEnabled())
            area.shrinkY(Ads::bannerLayout().top() - area.y);
    }

    // Adjust for padding
    area.shrinkH(padding.top());

    //
    // Now lay the buttons
    //

    // Calculate buttons' starting point and the gap
    Real buttonsGap{}, buttonsOffsetY{.5};

    if (buttonsTotalHeight <= area.h) {
        // for one widget just position it in the center of the screen a little bit up
        if (buttons.size() == 1) {
            buttonsOffsetY = .6;
        }

        // For the two buttons we set them a little close to each other. //equally far from edges and each other
        if (buttons.size() == 2) {
            buttonsGap = (area.h - buttonsTotalHeight) / 5;
            buttonsOffsetY = .65;
        }

        if (buttons.size() >= 3) {
            buttonsGap = (area.h - buttonsTotalHeight) / (buttons.size() + 1 + explCount);
            buttonsOffsetY = .65;
        }
    }

    Size gappedButtonsSize{screenStyles.screenSelectButton.size.w, buttonsTotalHeight + (buttons.size()-1)*buttonsGap};

    // Now we have the whole area for all buttons
    auto bf = area.positioned(gappedButtonsSize, {M::Half, buttonsOffsetY});

    // Start from the top
    bf.shrinkY(bf.h - buttons[0].size.h);

    for (auto &b: buttons) {
        bf.y -= b.margins.top();
        bf.h = b.size.h;
        bf.w = b.size.w;
        bf.x = area.x + (area.w - bf.w) * 0.5;

        b.contentsFrame = b.totalFrame = bf;  // Button itself
        auto p = b.contentsFrame.position({.5, 0});  // center bottom
        p.y -= explGap;

        for (auto &expl: b.rtExplanation) {
            expl.pos = p;
            p.y -= textExtent;
        }

        b.totalFrame.expandY(b.totalFrame.y - p.y);

        bf.y -= b.extent.h + buttonsGap;
        bf.y -= b.margins.bottom();

    }
}



void GameScreenBase::buildScreenButtons(ScreenButtonsParams& buttons) 
{
    const auto& screenStyles = M::Styles::screen;

    for (auto& bi: buttons) {

        bi.button = mainScene_->newButton({
            .text = bi.title,
            .rect = bi.contentsFrame,
            //.texture = Gui::guiTex.screenButton,
            .onPush = bi.onPush,
        });


//        auto button = edition->styleButton(bi.button, GuiButtonPlacement::Screen);
//        button->buttonTitle = bi.title;
//        button->suffixText = bi.suffixText;
//
//        if (bi.buttonTexture)
//            button->buttonTex = bi.buttonTexture;
//
//        if (bi.decorTex)
//            button->decorTex = bi.decorTex;
//
//        button->contentsFrame = bi.contentsFrame;
//        button->action = bi.action;
//
//        button->build(button->screen);

        for (auto& expl: bi.rtExplanation) {
            expl.label = mainScene_->newLabel({
                .text = expl.text,
                .pos = expl.pos,
                .color = screenStyles.screenButtonHint.color,
                .textSize = screenStyles.screenButtonHint.size,
                .flags = M::AlignCenterTop,
            });
        }
    }

}


void GameScreenBase::buildScreenToolButtons() {

    //
    // Determine the location of the center button
    //
    // auto& sp = contentContext_.screenParams;
    auto& tbp = contentContext_.screenParams.toolButtons;

    auto scene = mainScene_;

    auto locationCoeff = tbp.locationCoeff;

    if (locationCoeff.x == 0)  locationCoeff.x = 0.5;
    if (locationCoeff.y == 0)  locationCoeff.y = 0.3;

    Point buttonsPos = contentArea_.position(locationCoeff);
    Size buttonSize{tbp.buttonWidth, tbp.buttonHeight};

    // TODO: back
    if (haveScreenLayoutParams_) {
        if (screenLayoutParams_.even) {
            buttonsPos = screenLayout_.adjustGrid(buttonsPos);
        } else {
            buttonsPos = screenLayout_.adjustGridCenter(buttonsPos);
        }
    }

    if (tbp.buttonSize) buttonSize = tbp.buttonSize;
    if (buttonSize.h == 0)  buttonSize.h = buttonSize.w;
    if (buttonSize.w == 0)  buttonSize.w = buttonSize.h;
    if (!buttonSize && haveScreenLayoutParams_) buttonSize = screenLayout_.gridSize;

    MDebugTrapAssert(buttonSize);

    // buttonsPos.x -= buttonSize.w / 2;
    // buttonsPos.y -= buttonSize.h / 2;


    Point advance{tbp.buttonAdvanceX, tbp.buttonAdvanceY};
    if (tbp.buttonAdvance) advance = tbp.buttonAdvance;
    if (!advance && haveScreenLayoutParams_) advance = screenLayout_.gridSize.point();

    MDebugTrapAssert(advance);


    auto buttonsCount = apl::count(tbp.buttons);
    Rect current = Rect::make(buttonsPos, buttonSize);
    auto centerButton = buttonsCount / 2;
    for (int i = 0; i < centerButton; ++i) {
        current.x -= advance.x;
    }


    /*
    auto buttonsGap = advance.x - buttonSize.w;

    auto totalButtonsWidth = M::sizeCord(tbp.buttons) * buttonSize.w;
    totalButtonsWidth += buttonsGap * (M::sizeCord(tbp.buttons) - 1);

    current.x = buttonsPos.x - totalButtonsWidth/2;

    //Point current = buttonsPos;

    // current.x -= advance.x * std::floor(Cord(buttonsCount) / 2);
    */

    // current.x -= buttonSize.w/2;

    for (auto& bi: tbp.buttons) {
        auto rect = current;

        rect += bi.offset;
        rect.x -= buttonSize.w/2;
        rect.y -= buttonSize.h/2;

        if (bi.onCheck) {
            scene->newCheckbox({
                .rect = rect,
                .texture = bi.texture,
                .checked = bi.checked,
                .onCheck = bi.onCheck,
            });
        } else {
            scene->newButton({
                .rect = rect,
                .texture = bi.texture,
                .onPush = bi.onPush,
            });
        }

        // scene->newRect(M::Red, rect);

        current.x += advance.x;
    }

}


}  // namespace mangl


