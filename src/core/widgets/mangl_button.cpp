/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_button.h"
#include "mangl_checkbox.h"

namespace mangl {

ButtonP Scene::newButton(ButtonParamsA params) {
    auto button = newWidget<Button>(params.flags);
    return setupButton(button, params);
}

ButtonP Scene::setupButton(ButtonP button, ButtonParamsA params) {
    return button->processNew(params)->create();
}


ButtonP Button::processNew(ButtonParamsA params) {
    type = WidgetType::Button;

    params_ = params;
    style_ = params.style ? *params.style : M::Styles::button;
    if (params.widget) {
        mainWidget_ = params.widget;
    } else {
        mainWidget_ = defWidget_ = this->scene_->newWidget<DefaultButtonContentWidget>();
    }

    mainWidget_->buttonParams_ = &params_;
    mainWidget_->buttonStyle_ = &style_;

    addChildItem(mainWidget_);

    if (params.id) setId(params.id);

    if (!params.textView.empty()) params_.text = params.textView;


    move(params.rect);
    setMargins(params.margins);


    //mainWidget_ = params.widget ? params.widget : this;
    //mainWidget_->onSetText(params.text);

    if (params.hidden)
        hide();

    setKey(params.keyCode);

    onPush = params.onPush;
    onPushId = params.onPushId;
    onPress = params.onPress;
    onCheck = params.onCheck;
    onCheckId = params.onCheckId;

    checked_ = params.checked;


    /*
    text_ = params.text;

    textLabel_ = params.textLabel;
    pressedTextLabel_ = params.pressedTextLabel;


    if (params.backgroundColor.valid()) {
        setColor(params.backgroundColor);
    }

    radius_ = params.radius;


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


    if (params.widget) setWidget(params.widget);

    if (params.backgroundWidget) setBackground(params.backgroundWidget);
    pressedBackgroundWidget_ = params.pressedBackgroundWidget;
    disabledBackgroundWidget_ = params.disabledBackgroundWidget;

    if (params.contentsWidget) setWidget(params.contentsWidget);
    */

    return this;
}

void Button::onCreate(SceneP scene) {
    mainWidget_->create();

    updateLayout();

    pressed_ = false;
    mainWidget_->onPressedState(pressed_);

    /*
    if (backgroundWidget_) backgroundWidget_->create();
    if (pressedBackgroundWidget_) pressedBackgroundWidget_->create();
    if (disabledBackgroundWidget_) disabledBackgroundWidget_->create();
    if (pressedWidget_) pressedWidget_->create();


    if (!texButton_.valid() && style_->texture.valid()) {
        setTexture(style_->texture);
    }

    if (animation_ || texButton_.valid() || texPressed_.valid()) {
        prim_ = scene->newPrim();
        auto flags = this->flags_;

        if (flags & M::FlipHorz)
            prim_->setFlipHorz();
        if (flags & M::FlipVert)
            prim_->setFlipVert();

        if (animation_ && this->visible_) {
            animation_->attach(prim_);
            animation_->start();
        }
    }

    if (!prim_ && !backgroundWidget_) {
        if (!buttonColor_.valid()) {
            buttonColor_ = style_->backgroundColor;
            buttonColorFlag_ = true;
        }

        prim_ = scene->newBox(buttonColor_);

        if (radius_ < 0 || radius_ > 0) {
            prim_->setRadius(radius_ > 0 ? radius_ : style_->border.radius);
        }

        if (!style_->border.strikes.empty()) {
            borderRuntime_.assign(style_->border);
            borderRuntime_.create(scene, M::Hidden);
            borderRuntime_.setVisible(this->visible_);
        }


        pressedColorEnabled_ = true;
        pressedColor_ = style_->pressedBackgroundColor;
    }

    if (!textLabel_ && !text_.empty()) {
        textLabel_ = scene->newLabel({
            .text = text_,
            .pos = this->r_.center(),
            .color = style_->textColor,
            .textSize = style_->textSize,
            .flags = M::AlignCenterAll,
        });

        if (style_->textGradient[0])
            textLabel_->setColors(style_->textGradient);
    }


    if (contentsWidget_)
        contentsWidget_->create();


    updateLayout();
    updateVisible();
    updateScissor();

    // updateSensRect();
    */
}


ButtonP Button::updateLayout() {
    if (!this->created_)
        return this;

    updateSensRect();
    mainWidget_->move(this->r_);

    /*
    auto& rect = this->r_;

    if (backgroundWidget_) backgroundWidget_->move(rect)->setOrientationBase(orientation_);;
    if (pressedBackgroundWidget_) pressedBackgroundWidget_->move(rect)->setOrientationBase(orientation_);
    if (disabledBackgroundWidget_) disabledBackgroundWidget_->move(rect)->setOrientationBase(orientation_);
    if (contentsWidget_) contentsWidget_->move(rect)->setOrientationBase(orientation_);
    if (pressedWidget_) pressedWidget_->move(rect)->setOrientationBase(orientation_);

    if (prim_) {
        if (texButton_) prim_->setTexture(texButton_)->setOrientation(orientation_);

        if (this->disabled_ && texDisabled_)
            prim_->setTexture(texDisabled_);

        prim_->move(rect);

        if (buttonColorFlag_) prim_->setColor(buttonColor_);

        borderRuntime_.move(rect);
    }

    if (textLabel_) {
        textLabel_->move(rect)->setOrientation(orientation_);
        if (buttonTextColorFlag_) textLabel_->setColor(buttonTextColor_);
    }

    if (pressedTextLabel_) pressedTextLabel_->move(rect)->setOrientation(orientation_);

    updateVisible();
    updateScissor();
    */
    return this;
}


//ButtonP Button::updateScissor() {

    /*
    if (backgroundWidget_) backgroundWidget_->setScissor(this->scissorFlag_, this->scissorRect_);
    if (pressedBackgroundWidget_) pressedBackgroundWidget_->setScissor(this->scissorFlag_, this->scissorRect_);
    if (disabledBackgroundWidget_) disabledBackgroundWidget_->setScissor(this->scissorFlag_, this->scissorRect_);
    if (contentsWidget_) contentsWidget_->setScissor(this->scissorFlag_, this->scissorRect_);
    if (pressedWidget_) pressedWidget_->setScissor(this->scissorFlag_, this->scissorRect_);
    if (prim_) prim_->setScissor(this->scissorFlag_, this->scissorRect_);
    borderRuntime_.setScissor(this->scissorFlag_, this->scissorRect_);
    if (textLabel_) textLabel_->setScissor(this->scissorFlag_, this->scissorRect_);
    if (pressedTextLabel_) pressedTextLabel_->setScissor(this->scissorFlag_, this->scissorRect_);
    */
//     return this;
// }


ButtonP Button::updateSensRect() {

    const auto& rect = this->r_;

    sensRect_ = margins_.outerRect(rect);

    sensRectValid_ = true;
    sensValidatedRect_ = rect;

    return this;
}


// void Button::updateVisible() {
//     mainWidget_->setVisible(this->visible_);

    /*
    auto visible = this->visible_;

    // TODO: bring back
    borderRuntime_.setVisible(visible_);

    if (prim_)
        prim_->setVisible(visible);

    if (backgroundWidget_) backgroundWidget_->setVisible(visible);

    if (pressedBackgroundWidget_) {
        pressedBackgroundWidget_->setVisible(visible && pressed_);
        if (backgroundWidget_) backgroundWidget_->setVisible(visible && !pressed_);
    }

    if (pressed_) {
        if (disabledBackgroundWidget_) disabledBackgroundWidget_->setVisible(false);
    } else {
        if (disabled_) {
            if (disabledBackgroundWidget_) {
                disabledBackgroundWidget_->setVisible(visible);
                if (backgroundWidget_) backgroundWidget_->setVisible(false);
            } else {
                if (style_->disabledBackgroundColor) {
                    if (prim_) prim_->setColor(style_->disabledBackgroundColor);
                }
            }

        } else {
            if (disabledBackgroundWidget_) disabledBackgroundWidget_->setVisible(false);
            if (backgroundWidget_) backgroundWidget_->setVisible(visible);
        }
    }


    if (contentsWidget_) contentsWidget_->setVisible(visible);

    if (pressedWidget_) {
        pressedWidget_->setVisible(visible && pressed_);

        if (contentsWidget_) contentsWidget_->setVisible(visible && !pressed_);
    }


    if (textLabel_) textLabel_->setVisible(visible);

    if (pressedTextLabel_) {
        pressedTextLabel_->setVisible(visible && pressed_);

        if (textLabel_) textLabel_->setVisible(visible && !pressed_);
    }
    */
// }

void Button::updatePressed(bool pressed) {
    if (!created_)
        return;

    if (pressed == pressed_)
        return;

    pressed_ = pressed;
    mainWidget_->onPressedState(pressed_);

    /*
    if (contentsWidget_)
        contentsWidget_->onPressedState(pressed_);

    // if (pressedBackgroundWidget_) {
    //     pressedBackgroundWidget_->setVisible(visible_ && pressed_);
    //     if (backgroundWidget_) backgroundWidget_->setVisible(visible_ && !pressed_);
    // }


    if (pressed) {
        if (prim_) {
            if (texPressed_)
                prim_->setTexture(texPressed_);
            if (pressedColorEnabled_)
                prim_->setColor(pressedColor_);
        }

        if (animation_)
            animation_->pause();
    } else {
        if (prim_) {
            if (texButton_)
                prim_->setTexture(texButton_);

            if (pressedColorEnabled_) {
                if (buttonColorFlag_)
                    prim_->setColor(buttonColor_);
                else
                    prim_->clearColor();
            }
        }

        if (animation_)
            animation_->resume();
    }
    */

    if (onPress) onPress(pressed_);
}

void Button::emitAction() {
    if (onPush) onPush();
    if (onPushId) onPushId(controlId_);

    if (params_.checkbox) {
        checked_ = !checked_;
        if (onCheck) onCheck(checked_);
        if (onCheckId) onCheckId(controlId_, checked_);
        mainWidget_->onCheckedState(checked_);
    }

    pushed_ = true;

    // Deal with the sounds
    if (soundId_)
        Sfx::play(soundId_);
    else if (mangl_internal::defaultButtonSound_ != mangl_internal::InvalidSoundButton)
        Sfx::play(mangl_internal::defaultButtonSound_);
    else if (Framework::instance()->appParams_.defaultButtonSound && Framework::instance()->appParams_.defaultButtonSound != mangl_internal::InvalidSoundButton)
        Sfx::play(Framework::instance()->appParams_.defaultButtonSound);
}


bool Button::onTouchDown(TouchA touch) {
    pushed_ = false;

    beginTouches_ = touch;

    if (!sensRectValid_)
        updateSensRect();

    if (sensRect_.contains(touch.p)) {
        if (disabled_)
            return false;

        tracking_ = true;
        updatePressed(true);
    }

    if (scrollMember_)
        return false;

    return tracking_;
}

bool Button::onTouchMove(TouchA touch) {
    if (disabled_)
        return false;

    const auto& bp = beginTouches_.p;

    if (!sensRectValid_)
        updateSensRect();

    if (this->scrollMember_) {
        if (pressed_ && (touch.p - bp).norm() > 4 * Env::pixelDensity) { // TODO: deal with the scroll
            updatePressed(false);
        } else
            ;

    } else {
        if (tracking_) {
            if (sensRect_.contains(touch.p)) {
                updatePressed(true);
            } else {
                updatePressed(false);
            }
        }
    }

    if (scrollMember_)
        return false;

    return tracking_;
}

bool Button::onTouchUp(TouchA touch) {
    if (this->disabled_)
        return false;

    auto& p = touch.p;
    auto& bp = beginTouches_.p;

    if (!sensRectValid_)
        updateSensRect();


    /*
    // it's imperative to change the texture and then send the signal, since the user might
    // change the texture on gwidget end
    if (prim_ && texButton_.valid())
        prim_->setTexture(texButton_);

    if (pressedColorEnabled_) {
        if (prim_) {
            if (buttonColorFlag_)
                prim_->setColor(buttonColor_);
            else
                prim_->clearColor();
        }
    }
    */

    if (this->scrollMember_) {
        if ((p - bp).norm() > 10 * Env::pixelDensity) // TODO: get rid of upscale
            pressed_ = false;
    }

    auto haveAction = pressed_ && sensRect_.contains(p) && sensRect_.contains(bp);

    if (pressed_)
        updatePressed(false);

    if (haveAction)
        emitAction();

    tracking_ = false;

    //updateVisible();

    return pushed_;
}

bool Button::onTouchCancel(TouchA info) {
    if (this->disabled_)
        return false;

    // if (prim_ && texButton_.valid())
    //     prim_->setTexture(texButton_);

    pressed_ = false;
    tracking_ = false;
    mainWidget_->onPressedState(pressed_);

    //if (animation_) animation_->stop();
    if (onPress) onPress(pressed_);

    //updateVisible();

    return false;
}

bool Button::onKeyDown(KeyCode key) {
    if (keysCount_ == 0)
        return false;

    for (int i = 0; i < keysCount_; ++i) {
        if (activateKeys_[i] == key) {
            updatePressed(true);
            return true;
        }
    }

    return false;
}

bool Button::onKeyUp(KeyCode key) {
    if (keysCount_ == 0)
        return false;

    if (pressed_) {
        for (int i = 0; i < keysCount_; ++i) {
            if (activateKeys_[i] == key) {
                updatePressed(false);
                emitAction();
                return true;
            }
        }
    }

    return false;
}

/*
void Button::onSetZIndexFront() {
    if (prim_) this->scene_->moveToFront(prim_);

    if (backgroundWidget_) backgroundWidget_->setZIndexFront();
    if (pressedBackgroundWidget_) pressedBackgroundWidget_->setZIndexFront();

    if (contentsWidget_) contentsWidget_->setZIndexFront();
    if (pressedWidget_) pressedWidget_->setZIndexFront();

    if (textLabel_) textLabel_->setZIndexFront();
    if (pressedTextLabel_) pressedTextLabel_->setZIndexFront();
}
*/


} // namespace mangl
