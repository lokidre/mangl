/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_button_widget.h"

#include "../mangl_scene.h"

namespace mangl {

void DefaultButtonContentWidget::onCreate(SceneP scene) {
    auto& params = *buttonParams_;
    auto& ss = *buttonStyle_;

    texture_ = params.texture ? params.texture : ss.texture;
    if (texture_) {
        backgroundPrim_ = scene->newTexture(texture_);
    } else {
        backgroundPrim_ = scene->newBox(ss.backgroundColor);

        backgroundPrim_->setRadius(ss.border.radius);
    }

    addChildItem(backgroundPrim_);

    if (!ss.border.strikes.empty()) {
        borderRuntime_.assign(ss.border);
        borderRuntime_.create(scene, M::Hidden);
        borderRuntime_.setVisible(this->visible_);

        for (auto& strike: borderRuntime_.runtimeStrikes) {
            addChildItem(strike.prim);
        }
    }


    if (!params.text.empty())
        onSetText(params.text);

    //if ( params

    /*
    if (params.texture) {
        if (params.pressedTexture) {
            if (params.disabledTexture) {
                setTexture(params.texture, params.pressedTexture, params.disabledTexture);
            } else {
                setTexture(params.texture, params.pressedTexture);
            }
        } else {
            setTexture(params.texture);
        }
    }
    */

}

void DefaultButtonContentWidget::onSetText(StrView text) {
    if (!textLabel_) {
        textLabel_ = this->scene_->newLabel({
            .text = String{text},
            .pos = this->r_.center(),
            .color = buttonStyle_->textColor,
            .textSize = buttonStyle_->textSize,
            .flags = M::AlignCenterAll,
        });

        if (buttonStyle_->textGradient[0])
            textLabel_->setColors(buttonStyle_->textGradient);

        addChildItem(textLabel_);
    } else {
        textLabel_->setText(text);
    }
}

void DefaultButtonContentWidget::onPressedState(bool pressed) {
    auto& ss = *buttonStyle_;

    if (texture_) {
        if (pressed) {
            if (texture_.count >= 2) {
                backgroundPrim_->setTexture(texture_.getFrame(1));
            } else {
                Color pressedColor{ss.pressedBackgroundColor};

                if (pressedColor) {
                    // TODO: default disabled button background color
                    pressedColor = M::Gray;
                }
                backgroundPrim_->setTexture(texture_)->setColor(pressedColor);
            }
        } else {
            backgroundPrim_->setTexture(texture_)->clearColor();
        }
    } else {
        Color backgroundColor{pressed ? ss.pressedBackgroundColor: ss.backgroundColor};
        backgroundPrim_->setColor(backgroundColor);
    }
}

void DefaultButtonContentWidget::onEnable(bool enabled) {
    auto& ss = *buttonStyle_;

    if (texture_) {
        if (enabled) {
            backgroundPrim_->setTexture(texture_)->clearColor();
        } else {
            if (texture_.count >= 3)
                backgroundPrim_->setTexture(texture_.getFrame(2));
            else {
                Color disabledColor{ss.disabledBackgroundColor};

                if (disabledColor) {
                    // TODO: default disabled button background color
                    disabledColor = M::Gray;
                }
                backgroundPrim_->setTexture(texture_)->setColor(disabledColor);
            }
        }
    } else {
        Color backgroundColor{enabled ? ss.backgroundColor : ss.disabledBackgroundColor};
        backgroundPrim_->setColor(backgroundColor);
    }

    if (textLabel_) {
        if (enabled)
            textLabel_->setColor(buttonStyle_->textColor);
        else
            textLabel_->setColor(buttonStyle_->disabledTextColor);
    }
}

void DefaultButtonContentWidget::onMove(RectA rect) {
    backgroundPrim_->move(rect);
    borderRuntime_.move(rect);

    if (textLabel_) textLabel_->setPos(rect.center());
}



}  // namespace mangl


