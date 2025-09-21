/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_game_screen.h"

#include "../mangl_scene.h"
#include "../mangl_environment.h"


namespace mangl {

void GameScreenBase::buildTitle(GameScreenContentContext& ctx, ScreenTitleBuildParamsA params, ScreenType screenType) {

    const auto& sscreen = M::Styles::screen;

    GameScreenTitleStyles stitle{};
    bool isHomeStyle = screenType == ScreenType::Home || screenType == ScreenType::Intro;

    if (isHomeStyle) {
        stitle = sscreen.homeTitle;
    } else {
        stitle = sscreen.title;
    }

    auto& st = ctx.screenTitle;
    auto scene = mainScene_;
    const auto& area = contentArea_;

    auto maxWidth = contentArea_.w;

    maxWidth -= stitle.margins.horz2();
    
    //if (!isHomeStyle)
    //    maxWidth -= screenStyle.screenToolButton.size.w * 2  - 16*2;


    if (!params.text.empty() && !params.texture.valid()) {

        Flags align{M::AlignCenter | M::AlignCenterBottom};
        Point point{};

        auto fontHeight = stitle.text.size;

        point.x = area.center().x;

        point.y = contentArea_.top() - fontHeight;
    
        if (contentArea_.top() == fullArea_.top())
            point.y -= 16;
    
        //if (layoutTall)
        //    point.y -= 20;


        st.label = scene->newLabel({
            .text = params.text,
            .pos = point,
            //.font = screenTitleFont,  // TODO: mangl - title font
            .color = stitle.text.color,
            .textSize = fontHeight,
            // .effect = Gui::theme.screenTitleLabelEffect,  // TODO: mangl - add effect
            .flags = align,
        });

        if (st.label->renderedSize.w > maxWidth) {
            // Make the text a little smaller
            auto newTextSize = fontHeight * maxWidth / st.label->renderedSize.w;
            st.label->setTextHeight(newTextSize);
        }


        st.rect = st.label->rect();
        st.rect.setSize(st.label->getRenderedSize());
        //st.rect.w = st.label->renderedSize.w;
        st.rect.x = area.x + (area.w - st.rect.w)/2;

    }  // If title is text


    if (params.texture.valid()) {
        Size titleSize{};

        if (!titleSize && params.height > 0) {
            titleSize = params.texture.sizeByHeight(params.height);
        }

        if (!titleSize && stitle.width > 0) {
            titleSize = params.texture.sizeByWidth(stitle.width);
        } 
        
        if (!titleSize && stitle.height > 0) {
            titleSize = params.texture.sizeByHeight(stitle.height);
        }


        MDebugTrapAssert(titleSize);

        if (titleSize.w > maxWidth) {
            auto ratio = titleSize.w / maxWidth;
            titleSize /= ratio;
        }

        st.rect = area.positioned(titleSize, {0.5, 1});

        st.rect.y -= stitle.margins.top();

        scene->newImage(st.rect, params.texture);
    }


}


}  // namespace mangl

