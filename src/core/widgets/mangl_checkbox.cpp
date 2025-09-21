/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_checkbox.h"

#include "../mangl_scene.h"

namespace mangl {

CheckboxP Scene::newCheckbox(CheckboxParamsA params) {
    return newWidget<Checkbox>(params.flags)->onNew(params)->create();
}


CheckboxP Scene::newCheckbox(TextureA onTex, TextureA offTex, Point pos, Size size, Flags flags) {
    auto rect = Rect::make(pos, size);

    auto checkbox = newWidget<Checkbox>(flags)->move(rect)->setTexture(onTex, offTex);

    // TODO:
    return checkbox->create();
}

CheckboxP Scene::newCheckbox(Rect rect, TextureA onTex, TextureA offTex, Flags flags) {

    // TODO:
    auto checkbox = newWidget<Checkbox>(flags)->move(rect)->setTexture(onTex, offTex);

    return checkbox->create();
}

CheckboxP Checkbox::onNew(CheckboxParamsA params) {

    type = WidgetType::Checkbox;

    auto& theme = M::Styles::checkbox;

    if (params.rect.w > 0) {
        move(params.rect);
    }

    checked_ = params.checked;


    checkOnWidget_ = params.onWidget;
    checkOnPressedWidget_ = params.onPressedWidget;
    checkOnDisabledWidget_ = params.onDisabledWidget;
    checkOffWidget_ = params.offWidget;
    checkOffPressedWidget_ = params.offPressedWidget;
    checkOffDisabledWidget_ = params.offDisabledWidget;


    if (params.texture) {
        setTexture(params.texture);
        //setTexture(params.texture[0], params.texture[1]);
    } else {
        if (params.onTexture) {
            ADebugTrapAssert(params.onTexture.valid() && params.offTexture.valid());
            setTexture(params.onTexture, params.offTexture);
        }
    }
    if (!checkOnTex_ && !checkOnWidget_ && theme.texture) {
        setTexture(theme.texture);
    }


    if (params.checkSignal) checkSignal = params.checkSignal;
    if (params.onCheck) checkSignal = params.onCheck;

    if (params.checkSignalId) checkSignalId = params.checkSignalId;
    if (params.onCheckId) checkSignalId = params.onCheckId;

    setStateTag(params.stateTag, params.defaultChecked);

    return this;
}


void Checkbox::onCreate() {
    auto scene = this->scene_;
    auto& sscheckbox = M::Styles::checkbox;

    if (animation_ || checkOnTex_.valid()) {
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

    if (!prim_ && !checkOnWidget_) {
        if (!buttonColor_.valid()) {
            buttonColor_ = sscheckbox.backgroundColor;
            buttonColorFlag_ = true;
        }

        prim_ = scene->newRect(buttonColor_, this->r_);
        prim_->setWidth(sscheckbox.borderWidth);

        if (radius_ < 0 || radius_ > 0) {
            prim_->setRadius(radius_ > 0 ? radius_ : sscheckbox.borderRadius);
        }

        if (!checkOnTex_) {
            //auto radius = std::hypot(this->r_.w, this->r_.h) * 0.2;
            checkPrim_ = scene->newDisk(buttonColor_, this->r_.center(), 0);
        }
        //checkPrim_->setVisible(this->visible_ && checked_);

        pressedColorEnabled_ = true;
        pressedColor_ = sscheckbox.pressedBackgroundColor;
    }

//    if (!textLabel_ && !text_.empty()) {
//        textLabel_ = scene->newLabel({
//            .text = text_,
//            .pos = this->r_.center(),
//            .color = Theme::checkbox.textColor,
//            .textSize = Theme::checkbox.textSize,
//            .flags = M::AlignCenterAll,
//        });
//    }
//
//    if (textLabel_) {
//        textLabel_->create();
//    }

//    if (backgroundWidget_) {
//        backgroundWidget_->create();
//    }
//
//    if (contentsWidget_) {
//        contentsWidget_->create();
//    }
//
//    if (pressedWidget_) {
//        pressedWidget_->create();
//    }


    loadSavedState();
    updateContents();
}


void Checkbox::updateContents() {
    if (!this->created_)
        return;

    updateSensRect();

    auto& rect = this->r_;


    if (checkOnWidget_)
        checkOnWidget_->move(rect)->setScissor(this->scissorFlag_, this->scissorRect_);
    if (checkOnPressedWidget_)
        checkOnPressedWidget_->move(rect)->setScissor(this->scissorFlag_, this->scissorRect_);
    if (checkOnDisabledWidget_)
        checkOnDisabledWidget_->move(rect)->setScissor(this->scissorFlag_, this->scissorRect_);
    if (checkOffWidget_)
        checkOffWidget_->move(rect)->setScissor(this->scissorFlag_, this->scissorRect_);
    if (checkOffPressedWidget_)
        checkOffPressedWidget_->move(rect)->setScissor(this->scissorFlag_, this->scissorRect_);
    if (checkOffDisabledWidget_)
        checkOffDisabledWidget_->move(rect)->setScissor(this->scissorFlag_, this->scissorRect_);


//    if (backgroundWidget_)
//        backgroundWidget_->move(rect)->setScissor(this->scissorFlag_, this->scissorRect_);
//
//    if (contentsWidget_)
//        contentsWidget_->move(rect)->setScissor(this->scissorFlag_, this->scissorRect_);
//
//    if (pressedWidget_)
//        pressedWidget_->move(rect)->setScissor(this->scissorFlag_, this->scissorRect_);

    updateTextures();

    if (prim_) {
        prim_->move(rect);

        if (buttonColorFlag_)
            prim_->setColor(buttonColor_);

        prim_->setScissor(this->scissorFlag_, this->scissorRect_);
    }

    if (checkPrim_) {
        //auto radius = std::hypot(rect.w, rect.h) * 0.2;
        auto radius = std::min(rect.w, rect.h) * 0.4;
        checkPrim_->setDisk(rect.center(), radius);
        checkPrim_->setScissor(this->scissorFlag_, this->scissorRect_);
    }

//    if (textLabel_)
//        textLabel_->move(rect)->setScissor(this->scissorFlag_, this->scissorRect_);
//
//    if (pressedTextLabel_)
//        pressedTextLabel_->move(rect)->setScissor(this->scissorFlag_, this->scissorRect_);

    updateVisible();
}

void Checkbox::loadSavedState()
{
    if (!statChecked_.hasKey() || !created_ || savedStateLoaded_)
        return;

    savedStateChecked_ = statChecked_.get(stateDefaultChecked_);

    setChecked(savedStateChecked_);
    savedStateSet_ = true;

    savedStateLoaded_ = true;

    emitSignals();
}


void Checkbox::updateSensRect() {

    const auto& rect = this->r_;

    sensRect_ = margins_.outerRect(rect);
    sensRectValid_ = true;
    sensValidatedRect_ = rect;
}

void Checkbox::updateTextures() {
    if (!checkOnTex_ || !prim_)
        return;

    if (this->disabled_) {
        prim_->setTexture(checked_ ? checkOnDisabledTex_ : checkOffDisabledTex_);
    } else if (this->pressed_) {
        prim_->setTexture(checked_ ? checkOnPressedTex_ : checkOffPressedTex_);
    } else {
        prim_->setTexture(checked_ ? checkOnTex_ : checkOffTex_);
    }
}


void Checkbox::updateVisible() {
    auto& vis = this->visible_;

    if (prim_)
        prim_->setVisible(vis);

    if (checkPrim_)
        checkPrim_->setVisible(vis && checked_);


    if (checkOnWidget_) checkOnWidget_->setVisible(false);
    if (checkOnPressedWidget_) checkOnPressedWidget_->setVisible(false);
    if (checkOnDisabledWidget_) checkOnDisabledWidget_->setVisible(false);
    if (checkOffWidget_) checkOffWidget_->setVisible(false);
    if (checkOffPressedWidget_) checkOffPressedWidget_->setVisible(false);
    if (checkOffDisabledWidget_) checkOffDisabledWidget_->setVisible(false);

    if (vis) {
        if (checked_) {
            if (pressed_) {
                if (checkOnPressedWidget_)
                    checkOnPressedWidget_->setVisible(true);
                else
                    if (checkOnWidget_) checkOnWidget_->setVisible(true);
            } else {  // not pressed
                if (checkOnWidget_) checkOnWidget_->setVisible(true);
            }
        } else {  // not checked
            if (pressed_) {
                if (checkOffPressedWidget_)
                    checkOffPressedWidget_->setVisible(true);
                else
                    if (checkOffWidget_) checkOffWidget_->setVisible(true);
            } else {  // not pressed
                if (checkOffWidget_) checkOffWidget_->setVisible(true);
            }
        }
    }


//    if (backgroundWidget_)
//        backgroundWidget_->setVisible(vis);
//
//    if (contentsWidget_)
//        contentsWidget_->setVisible(vis);
//
//    if (pressedWidget_) {
//        pressedWidget_->setVisible(vis && pressed_);
//
//        if (contentsWidget_)
//            contentsWidget_->setVisible(vis && !pressed_);
//    }


//    if (textLabel_)
//        textLabel_->setVisible(vis);
//
//    if (pressedTextLabel_) {
//        pressedTextLabel_->setVisible(vis && pressed_);
//
//        if (textLabel_)
//            textLabel_->setVisible(vis && !pressed_);
//    }
}

void Checkbox::updatePressed(bool pressed) {
    if (!this->created_)
        return;

    auto changed = pressed != pressed_;
    pressed_ = pressed;

    updateTextures();

    updateVisible();

    if (pressed) {
        if (prim_) {
            if (pressedColorEnabled_) {
                prim_->setColor(pressedColor_);

                if (checkPrim_) checkPrim_->setColor(pressedColor_);
            }
        }

        if (animation_) animation_->pause();

        if (changed && pressSignal) pressSignal(pressed_);
    } else {  // Depressed
        if (prim_) {
            if (pressedColorEnabled_) {
                if (buttonColorFlag_) {
                    prim_->setColor(buttonColor_);

                    if (checkPrim_)
                        checkPrim_->setColor(buttonColor_);
                } else {
                    prim_->clearColor();
                }
            }
        }

        if (animation_)  animation_->resume();

        if (changed && pressSignal) pressSignal(pressed_);
    }
}

CheckboxP Checkbox::setChecked(bool ch) {
    checked_ = ch;

    updateContents();
    updateVisible();

    return this;
}

void Checkbox::emitSignals() {
    if (checkSignal)  checkSignal(checked_);
    if (checkSignalId)  checkSignalId(this->controlId_, checked_);
}


void Checkbox::emitAction() {
    emitSignals();

    pushed_ = true;

    // Deal with the sounds
    if (soundId_)
        Sfx::play(soundId_);
    else if (mangl_internal::defaultButtonSound_ != mangl_internal::InvalidSoundButton)
        Sfx::play(mangl_internal::defaultButtonSound_);
    else if (Framework::instance()->appParams_.defaultButtonSound && Framework::instance()->appParams_.defaultButtonSound != mangl_internal::InvalidSoundButton)
        Sfx::play(Framework::instance()->appParams_.defaultButtonSound);
}




bool Checkbox::onTouchDown(TouchA touch) {
    pushed_ = false;

    beginTouches_ = touch;

    if (!sensRectValid_)
        updateSensRect();

    if (sensRect_.contains(touch.p)) {
        if (this->disabled_)
            return false;

        tracking_ = true;
        updatePressed(true);
    }

    updateVisible();

    if (this->scrollMember_)
        return false;

    return tracking_;
}

bool Checkbox::onTouchMove(TouchA touch) {
    using namespace apl;

    if (this->disabled_)
        return false;


    const auto& p = touch.p;
    const auto& bp = beginTouches_.p;

    if (!sensRectValid_)
        updateSensRect();

    if (this->scrollMember_) {
        if (pressed_ && (p - bp).norm() > 4 * Env::pixelDensity) { // TODO: deal with the scroll
            updatePressed(false);
        } else
            ;

    } else {
        if (sensRect_.contains(p) && !pressed_ && tracking_) {
            pressed_ = true;
            updatePressed(true);
        }

        if (!sensRect_.contains(p) && tracking_) {
            updatePressed(false);
        }
    }

    updateVisible();

    if (this->scrollMember_)
        return false;


    return tracking_;
}

bool Checkbox::onTouchUp(TouchA touch) {
    if (this->disabled_)
        return false;


    auto& p = touch.p;
    auto& bp = beginTouches_.p;

    if (!sensRectValid_)
        updateSensRect();


    if (pressedColorEnabled_) {
        if (prim_) {
            if (buttonColorFlag_)
                prim_->setColor(buttonColor_);
            else
                prim_->clearColor();
        }
    }

    if (this->scrollMember_) {
        if ((p - bp).norm() > 10 * Env::pixelDensity) // TODO: get rid of upscale
            pressed_ = false;
    }

    // it's imperative to change the texture and then send the signal, since the user might
    // change the texture on gwidget end
//    if (prim_ && checkOnTex_.valid())
//        prim_->setTexture(pressed_ ? checkOnTex_ : checkOffTex_);


    auto haveAction = pressed_ && sensRect_.contains(p) && sensRect_.contains(bp);

    if (pressed_) {
        setChecked(!checked_);
    }

    if (pressed_)
        updatePressed(false);

    if (haveAction)
        emitAction();

    tracking_ = false;

    if (statChecked_.hasKey())
        statChecked_.set(checked_);

    // if ( pressedSignal )
    //	pressedSignal(pressed_) ;

    updateVisible();

    return pushed_;
}

bool Checkbox::onTouchCancel(TouchA touch) {
    if (this->disabled_)
        return false;

    pressed_ = false;
    tracking_ = false;

    if (prim_ && checkOnTex_.valid())
        prim_->setTexture(checked_ ? checkOnTex_ : checkOffTex_);

    if (animation_)
        animation_->stop();

    if (pressSignal)
        pressSignal(pressed_);

    updateVisible();

    return false;
}

bool Checkbox::onKeyDown(KeyCode key) {
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

bool Checkbox::onKeyUp(KeyCode key) {
    if (keysCount_ == 0)
        return false;

    if (pressed_) {
        for (int i = 0; i < keysCount_; ++i) {
            if (activateKeys_[i] == key) {
                updatePressed(false);
                emitAction();
                if (statChecked_.hasKey())
                    statChecked_.set(checked_);
                return true;
            }
        }
    }

    return false;
}

void Checkbox::onSetZIndexFront() {
    auto scene = this->scene_;

    scene->moveToFront(prim_);
    scene->moveToFront(checkPrim_);

    scene->setZIndexFront(checkOnWidget_);
    scene->setZIndexFront(checkOnPressedWidget_);
    scene->setZIndexFront(checkOnDisabledWidget_);

    scene->setZIndexFront(checkOffWidget_);
    scene->setZIndexFront(checkOffPressedWidget_);
    scene->setZIndexFront(checkOffDisabledWidget_);


}


} // namespace mangl
