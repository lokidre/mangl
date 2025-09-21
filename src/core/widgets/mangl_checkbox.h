/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../effects/mangl_frames_effect.h"
#include "../audio/mangl_sound.h"
#include "../mangl_styles.h"
#include "../mangl_framework.h"
#include "mangl_checkbox_params.h"

namespace mangl {

class Checkbox: public WidgetTemplate<Checkbox> {
    friend class Scene;

    using Base = WidgetTemplate<Checkbox>;
    using Self = Checkbox;
    using SelfP = Checkbox*;

    using SelfRet = Checkbox*;


public:
    //
    // Signals
    //

    using CheckSignalId = SignalIdBool;
    using CheckSignal = SignalBool;

    CheckSignalId checkSignalId;
    CheckSignal checkSignal;

    SignalBool pressSignal;


    void disconnectSignals() {
        checkSignalId = nullptr;
        checkSignal = nullptr;
        pressSignal = nullptr;
    }


    SelfRet setPrim(PrimP prim) {
        prim_ = prim;
        updateContents();
        return static_cast<SelfRet>(this);
    }

    SelfRet setAnimation(FramesEffectP animation) {
        animation_ = animation;

        if (prim_ && prim_->isVisible() && animation_) {
            animation_->attach(prim_);
            animation_->start();
        }
        return static_cast<SelfRet>(this);
    }

//    SelfRet setBackground(WidgetP widget) {
//        backgroundWidget_ = widget;
//        updateContents();
//        return static_cast<SelfRet>(this);
//    }
//
//    SelfRet setWidget(WidgetP buttonWidget) {
//        contentsWidget_ = buttonWidget;
//        updateContents();
//        return static_cast<SelfRet>(this);
//    }
//
//    SelfRet setPressedWidget(WidgetP widget) {
//        pressedWidget_ = widget;
//        updateContents();
//        return static_cast<SelfRet>(this);
//    }
//
//    [[nodiscard]] auto getBackgroundWidget() noexcept { return backgroundWidget_; }
//    [[nodiscard]] auto getContentsWidget() noexcept { return contentsWidget_; }
//    [[nodiscard]] auto getPressedWidget() noexcept { return pressedWidget_; }


    SelfRet setMargins(MarginsA margins) {
        margins_ = margins;
        updateSensRect();
        return static_cast<SelfRet>(this);
    }

    //    SelfRet setId(WidgetId id) {
    //        Base::setId(id);
    //        return static_cast<SelfRet>(this);
    //    }

    SelfRet setPressedColor(ColorA color) {
        pressedColorEnabled_ = true;
        pressedColor_ = color;
        return static_cast<SelfRet>(this);
    }

    SelfRet clearPressedColor() {
        pressedColorEnabled_ = false;
        return static_cast<SelfRet>(this);
    }

    SelfRet setColor(ColorA color) {
        buttonColorFlag_ = true;
        buttonColor_ = color;
        updateContents();
        return static_cast<SelfRet>(this);
    }

    SelfRet clearColor() {
        buttonColorFlag_ = false;
        updateContents();
        return static_cast<SelfRet>(this);
    }

//    SelfRet setTextLabel(LabelP textLabel, LabelP pressedTextLabel = nullptr) {
//        textLabel_ = textLabel;
//        pressedTextLabel_ = pressedTextLabel;
//        updateContents();
//        return static_cast<SelfRet>(this);
//    }
//
//    [[nodiscard]] auto getTextLabel() noexcept { return textLabel_; }
//
//    SelfRet setText(StrView text) {
//        apl::verify(textLabel_, "Attempted to set text without label");
//        textLabel_->setText(text);
//        if (pressedTextLabel_)
//            pressedTextLabel_->setText(text);
//        updateContents();
//        return static_cast<SelfRet>(this);
//    }
//
//    const auto& getText() const noexcept { return textLabel_->getText(); }
//
//    SelfRet clearText() {
//        textLabel_->clearText();
//        if (pressedTextLabel_)
//            pressedTextLabel_->clearText();
//        updateContents();
//        return static_cast<SelfRet>(this);
//    }

    SelfRet setSound(SfxId resid) {
        soundId_ = resid;
        return static_cast<SelfRet>(this);
    };

    [[nodiscard]] auto& sensFrame() const noexcept { return sensRect_; }

    // Checks if the widget isn't empty (has either text or texture)
    [[nodiscard]] bool empty() const noexcept {
        if (textLabel_ && !textLabel_->empty())
            return false;
//        if (texButton_.valid())
//            return false;
        return true;
    }

    SelfRet setKey(KeyCode key) {
        clearKey();
        return addKey(key);
    }
    SelfRet addKey(KeyCode key) {
        if (keysCount_ < maxKeys_) {
            activateKeys_[keysCount_++] = key;
        } else {
            activateKeys_[keysCount_ - 1] = key; // just keep replacing the last one
        }
        return (SelfRet)this;
    }
    SelfRet clearKey() {
        keysCount_ = 0;
        return this;
    }


    void setTextureBody(TextureA onButtonTex, TextureA onPressedTex, TextureA onDisabledTex,
                        TextureA offButtonTex, TextureA offPressedTex, TextureA offDisabledTex) {

        checkOnTex_ = onButtonTex,
        checkOnPressedTex_ = onPressedTex,
        checkOnDisabledTex_ = onDisabledTex,

        checkOffTex_ = offButtonTex,
        checkOffPressedTex_ = offPressedTex,
        checkOffDisabledTex_ = offDisabledTex,

//        texButton_ = checkOnTex_;
//        texPressed_ = checkOnPressedTex_;
//        texDisabled_ = checkOnDisabledTex_;

        updateContents();
    }

    SelfRet setTexture(TextureA onButtonTex, TextureA onPressedTex, TextureA onDisabledTex,
                       TextureA offButtonTex, TextureA offPressedTex, TextureA offDisabledTex) {
        setTextureBody(onButtonTex, onPressedTex, onDisabledTex, offButtonTex, offPressedTex, offDisabledTex);
        return this;
    }

//    SelfRet setTexture(TextureA buttonTex, TextureA pressedTex) {
//        setTexture(buttonTex, pressedTex, pressedTex);
//        return static_cast<SelfRet>(this);
//    }

    SelfRet setTexture(TextureA onTexture, TextureA offTexture) {
        ADebugTrapAssert(onTexture.count == offTexture.count);

        if (onTexture.count >= 3) {
            setTexture(onTexture[0], onTexture[1], onTexture[2], offTexture[0], offTexture[1], offTexture[2]);
            return this;
        }

        if (onTexture.count >= 2) {
            setTexture(onTexture[0], onTexture[1], onTexture[1], offTexture[0], offTexture[1], offTexture[1]);
            return this;
        }

        if (onTexture.count == 1) {
            setTexture(onTexture[0], onTexture[0], onTexture[0], offTexture[0], offTexture[0], offTexture[0]);
            return this;
        }

        return this;
    }


    SelfRet setTexture(TextureA texture) {
        if (texture.count >= 6) {
            setTexture(texture[0], texture[1], texture[2], texture[3], texture[4], texture[5]);
            return this;
        }

        if (texture.count >= 4) {
            //setTexture(texture[0], texture[2], texture[2], texture[1], texture[3], texture[3]);
            setTexture(texture[0], texture[1], texture[1], texture[2], texture[3], texture[3]);
            return this;
        }

        if (texture.count >= 2) {
            setTexture(texture[0], texture[0], texture[0], texture[1], texture[1], texture[1]);
            return this;
        }

        if (texture.count == 1) {
            setTexture(texture, texture, texture, texture, texture, texture);
            return this;
        }

        return this;
    }


private:
    CheckboxP onNew(CheckboxParamsA params);


    bool pressed_{};  // widget is in pressed state
    bool pushed_{};   // widget was pushed and signal emitted
    bool tracking_{}; // tracking started

    TouchInfo beginTouches_{};

    PrimP prim_{}, checkPrim_{};

    Margins margins_{};

    Color pressedColor_{M::DarkGrey};
    bool pressedColorEnabled_{};

    Color buttonColor_{};
    bool buttonColorFlag_{};

    FramesEffectP animation_{};

    bool sensRectValid_{false};
    Rect sensValidatedRect_{};

    String text_;
    LabelP textLabel_{}, pressedTextLabel_{};

    //Texture texButton_{}, texPressed_{}, texDisabled_{};
    //WidgetP backgroundWidget_{}, onWidget_{}, offWidget_{};
    //WidgetP contentsWidget_{}, pressedWidget_{};

    Cord radius_{-1};

    //
    // Checkbox support
    //
    Texture checkOnTex_{}, checkOnPressedTex_{}, checkOnDisabledTex_{};
    Texture checkOffTex_{}, checkOffPressedTex_{}, checkOffDisabledTex_{};

    WidgetP checkOnWidget_{}, checkOnPressedWidget_{}, checkOnDisabledWidget_{};
    WidgetP checkOffWidget_{}, checkOffPressedWidget_{}, checkOffDisabledWidget_{};


    SfxId soundId_{};


    Rect sensRect_{};

    Stat<int> statChecked_;
    bool stateDefaultChecked_{false};
    bool stateChanged_{false};
    bool savedStateChecked_{false};
    bool savedStateLoaded_{false};
    bool savedStateSet_{false};


    void loadSavedState();




    //
    // Keyboard
    //
    constexpr static int maxKeys_ = 3;
    std::array<KeyCode, maxKeys_> activateKeys_{};
    int keysCount_{};


public:
    [[nodiscard]] bool pressed() const noexcept { return pressed_; }
    [[nodiscard]] bool pushed() const noexcept { return pushed_; }

    void ack() noexcept { pressed_ = pushed_ = false; }


    [[nodiscard]] bool checked() const noexcept { return checked_; }

    SelfRet setChecked(bool ch);

    SelfRet setStateTag(StrView tag, bool defaultState = false) {
        if (tag.empty()) return this;
        statChecked_.setKey("mangl_widget_checkbox_checked:").setSuffix(tag);
        stateDefaultChecked_ = defaultState;
        loadSavedState();
        return this;
    }


private:

    void updateContents();
    void updateSensRect();
    void updateVisible();
    void updatePressed(bool pressed);
    void updateTextures();
    void emitAction();
    void emitSignals();


protected:
    void onCreate() override;

    void onVisible(bool visible) override {
        if (!this->created_)
            return;

        if (animation_ && visible) {
            animation_->attach(prim_);
            animation_->start();
        }

        updateVisible();
    }

    void onEnable(bool enabled) override { updateContents(); }
    void onMove() override { updateContents(); }
    void onScissor(bool scissorFlag, RectA scissorRect) override {
        updateContents();
    }

    bool onTouchDown(TouchA) override;
    bool onTouchMove(TouchA) override;
    bool onTouchUp(TouchA) override;
    bool onTouchCancel(TouchA) override;

    bool onKeyDown(KeyCode key) override;
    bool onKeyUp(KeyCode key) override;

    void onSetZIndexFront() override;

};


}  // namespace namesapce
