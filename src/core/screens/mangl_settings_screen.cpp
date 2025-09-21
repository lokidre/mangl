/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
﻿#include "mangl_settings_screen.h"

#include "../mangl_widgets.h"

#include "../audio/mangl_sound.h"
#include "../audio/mangl_music.h"
#include "../audio/mangl_game_music.h"

namespace mangl {

namespace mangl_impl {

struct SettingLayoutContext {
    GameScreenBaseP screen{};

    //GameSettingsScreenItemParams* item{};
    Cord currentY{};

    //LabelP titleLabel{};
    //CheckboxP checkbox{};
    //SliderP slider{};


    //std::vector<WidgetP> widgets;
    //void clearWidgets() {
    //    widgets.clear();

    //    titleLabel = nullptr;
    //    checkbox = nullptr;
    //    slider = nullptr;
    //}
};

static void settingsCreateCheckbox(SettingLayoutContext& lctx, SettingScreenBaseItem& item, CheckboxParams cp) {

    Cord checkboxHeight{40};


    auto safe = lctx.screen->contentArea_;
    auto scene = lctx.screen->mainScene_;

    auto checkboxTexture = item.params.checkboxTexture ? item.params.checkboxTexture : M::Styles::checkbox.texture;

    Size checkboxSize{checkboxHeight, checkboxHeight};

    if (checkboxTexture)
        checkboxSize = checkboxTexture.sizeByHeight(checkboxHeight);

    lctx.currentY -= checkboxSize.h;

    Rect checkboxRect{safe.x + (safe.w - checkboxSize.w)/2, lctx.currentY, checkboxSize.w, checkboxSize.h};
    
    //auto checkboxRect = safe.positioned(checkboxSize, {0.5, 0.75});
    item.rect = checkboxRect;

    cp.rect = checkboxRect;
    cp.texture = checkboxTexture;

    item.checkbox = scene->newCheckbox(cp);
    item.widgets.push_back(item.checkbox);
}


static void settingsCreateSlider(SettingLayoutContext& lctx, SettingScreenBaseItem& item, SliderParams cp) {
    auto safe = lctx.screen->contentArea_;
    auto scene = lctx.screen->mainScene_;

    Size sliderSize{240, 40};


    //auto sliderRect = safe.positioned(sliderSize, {0.5, 0.5});
    //lctx.currentY -= sliderSize.h;
    //sliderRect.y = lctx.currentY;

    lctx.currentY -= sliderSize.h;

    Rect sliderRect{safe.x + (safe.w - sliderSize.w)/2, lctx.currentY, sliderSize.w, sliderSize.h};

    auto sliderTextures = item.params.sliderTexture ? item.params.sliderTexture : M::Styles::slider.textures;

    item.rect = sliderRect;

    cp.rect = sliderRect;
    cp.textures = sliderTextures;

    item.slider = scene->newSlider(cp);

    //lctx.slider = scene->newSlider({
    //    .rect = sliderRect,
    //    .textures = sliderTextures,
    //    .disabled = !Music::isEnabled(),
    //    .position = Music::settingVolume(),
    //    .onPosition = GameMusic::updateUserVolume,
    //    .onPositionEnd = GameMusic::updateUserVolumeEnd,
    //});

    item.widgets.push_back(item.slider);
}


}  // namespace mangl_impl

using namespace mangl_impl;


void SettingsScreenBaseWidget::setup(GameSettingsScreenParamsA params) {
    //params_ = params;

    StrView titleText = params.titleText;
    if (titleText.empty() && !params.titleTexture)
        titleText = "SETTINGS";

    for (auto& pi: params.items) {
        items_.push_back({
            .params = pi,
        });
    }


    GameScreenParams sp{
        .type = GameScreenType::Settings,
        .titleText = titleText,
        .titleTexture = params.titleTexture,
        .closeButton = true, //params_.closeButton,
    };

    if (popup_) {
        popup_->setup(sp);
    } else {
        screen_->setup(sp);
    }
}





void SettingsScreenBaseWidget::onStart() {
    Size sliderSize{240, 40};
    Cord checkboxHeight{40};

    Cord sliderGap{10};
    Cord settingGap{100};


    auto& ctx = popup_ ? popup_->contentContext_ : screen_->contentContext_;

    auto safe = screen_->contentArea_;
    auto scene = screen_->mainScene_;

    // TODO: mangl settings layout
    //Cord currentY = safe.y + safe.h * 0.75;

    SettingLayoutContext lctx;
    lctx.screen = screen_;
    lctx.currentY = safe.y + safe.h * 0.75;


    for (auto& item: items_) {
        //lctx.item = &item;
        //item.clearWidgets();

        switch (item.params.type) {
        case GameSettingsItemType::Checkbox: {

            break;
        }

        case GameSettingsItemType::Sounds: {
            //auto checkboxTexture = item.checkboxTexture ? item.checkboxTexture : M::Styles::checkbox.texture;

            //Size checkboxSize{checkboxHeight, checkboxHeight};

            //if (checkboxTexture)
            //    checkboxSize = checkboxTexture.sizeByHeight(checkboxHeight);

            //auto checkboxRect = safe.positioned(checkboxSize, {0.5, 0.75});

            //currentY -= checkboxHeight;
            //checkboxRect.y = currentY;

            //scene->newCheckbox({
            //    .rect = checkboxRect,
            //    .texture = checkboxTexture,
            //    .checked = Sfx::isEnabled(),
            //    .onCheck = [this](bool checked) {
            //        Sfx::updateEnabledSetting(checked);
            //        soundsVolumeSlider_->setEnabled(checked);
            //    }
            //});

            lctx.currentY = safe.y + safe.h * 0.75;

            settingsCreateCheckbox(lctx, item, {
                .checked = Sfx::isEnabled(),
                .onCheck = [this](bool checked) {
                    Sfx::updateEnabledSetting(checked);
                    soundsVolumeSlider_->setEnabled(checked);
                }
            });


            lctx.currentY = item.rect.y - sliderGap;

            settingsCreateSlider(lctx, item, {
                .disabled = !Sfx::isEnabled(),
                .position = Sfx::settingVolume(),
                .onPosition = Sfx::updateUserVolumePosition,
                .onPositionEnd = Sfx::updateUserVolumeEnd,
            });

            soundsVolumeSlider_ = item.slider;

            //auto sliderRect = safe.positioned(sliderSize, {0.5, 0.5});
            //lctx.currentY -= sliderSize.h;
            //sliderRect.y = lctx.currentY;

            //auto sliderTextures = item.sliderTexture ? item.sliderTexture : M::Styles::slider.textures;

            //soundsVolumeSlider_ = scene->newSlider({
            //    .rect = sliderRect,
            //    .textures = sliderTextures,
            //    .disabled = !Sfx::isEnabled(),
            //    .position = Sfx::settingVolume(),
            //    .onPosition = Sfx::updateUserVolumePosition,
            //    .onPositionEnd = Sfx::updateUserVolumeEnd,
            //});

            break;
        }

        case GameSettingsItemType::Music: {
#if MANGL_MUSIC
            lctx.currentY = safe.y + safe.h * 0.5;

            settingsCreateCheckbox(lctx, item, {
                .checked = Music::isEnabled(),
                .onCheck = [this](bool checked) {
                    GameMusic::updateUserEnabledSetting(checked);
                    musicVolumeSlider_->setEnabled(checked);
                },
            });

            lctx.currentY = item.rect.y - sliderGap;

            settingsCreateSlider(lctx, item, {
                .disabled = !Music::isEnabled(),
                .position = Music::settingVolume(),
                .onPosition = GameMusic::updateUserVolume,
                .onPositionEnd = GameMusic::updateUserVolumeEnd,
            });

            musicVolumeSlider_ = item.slider;
#endif
            break;
        }

        default:
            MDebugTrap();
            break;
        }

        lctx.currentY -= settingGap;
    }

    //auto checkboxSize = guiTex_.settingsSoundsCheckbox.sizeByHeight(40);
    //auto checkboxRect = safe.positioned(checkboxSize, {0.5, 0.75});


    if (popup_) {
        if (ctx.closeButton) {
            ctx.closeButton->onPush = [this] {
                isFinished_ = true;
                //auto& ctx = popup_->contentContext_;
            };
        }
    }

}


}  // namespace mangl
