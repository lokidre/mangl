/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_widget.h"
#include "mangl_button_params.h"
#include "mangl_button_widget.h"
#include "mangl_label.h"
#include "mangl_widget_border.h"

#include "../effects/mangl_frames_effect.h"

#include "../mangl_types.h"


namespace mangl {

class Button: public WidgetTemplate<Button> {
    friend class Scene;

    using Base = WidgetTemplate<Button>;
    using Self = Button;
    using SelfRet = Button*;
    using SelfP = Button*;


    //
    // Params
    //
    ButtonParams params_{};
    ButtonStyle style_{};
    ButtonContentWidgetP mainWidget_{};

    DefaultButtonContentWidget* defWidget_{};

    //
    // Behaviour
    //
    Margins margins_{};
    Rect sensRect_{};
    SfxId soundId_{};


    //
    // Status
    //
    String text_;
    bool pushed_{};   // widget was pushed and signal emitted
    TouchInfo beginTouches_{};

    bool sensRectValid_{false};
    Rect sensValidatedRect_{};


public:
    //
    // Signals
    //
    using PushSignalId = SlotIdVoid;
    using PushSignal = SlotVoid;
    using PushSignalBool = SlotBool;

    PushSignalBool onPress{};

    PushSignal onPush{};
    PushSignalId onPushId{};
    SlotBool onCheck{};
    SlotIdBool onCheckId{};

    void disconnectSignals() {
        onPushId = nullptr;
        onPush = nullptr;
        onPress = nullptr;
        onCheck = nullptr;
        onCheckId = nullptr;
    }

    auto setMargins(MarginsA margins) {
        margins_ = margins;
        return updateSensRect();
    }

    //
    // Accessing members
    //
    template <class T = ButtonContentWidget>
    auto getMainWidget() {
        return static_cast<T*>(mainWidget_);
    }


    auto setText(StrView text) noexcept {
        text_ = text;
        mainWidget_->onSetText(text);
        return this;
    }
    auto clearText() {
        text_.clear();
        mainWidget_->onSetText({});
        return this;
    }

/*
    auto setAnimation(FramesEffectP animation) {
        animation_ = animation;

        if (prim_ && prim_->isVisible() && animation_) {
            animation_->attach(prim_);
            animation_->start();
        }
        return this;
    }

    SelfRet setBackground(WidgetP widget) {
        backgroundWidget_ = widget;
        updateLayout();
        return static_cast<SelfRet>(this);
    }

    SelfRet setWidget(WidgetP buttonWidget) {
        contentsWidget_ = buttonWidget;
        updateLayout();
        return static_cast<SelfRet>(this);
    }

    SelfRet setPressedWidget(WidgetP widget) {
        pressedWidget_ = widget;
        updateLayout();
        return static_cast<SelfRet>(this);
    }

    [[nodiscard]] auto getBackgroundWidget() noexcept { return backgroundWidget_; }
    [[nodiscard]] auto getContentsWidget() noexcept { return contentsWidget_; }
    [[nodiscard]] auto getPressedWidget() noexcept { return pressedWidget_; }



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
        updateLayout();
        return this;
    }

    SelfRet setTextColor(ColorA color) {
        buttonTextColorFlag_ = true;
        buttonTextColor_ = color;
        updateLayout();
        return static_cast<SelfRet>(this);
    }

    SelfRet clearColor() {
        buttonColorFlag_ = false;
        updateLayout();
        return static_cast<SelfRet>(this);
    }

    SelfRet setTextLabel(LabelP textLabel, LabelP pressedTextLabel = 0) {
        textLabel_ = textLabel;
        pressedTextLabel_ = pressedTextLabel;
        updateLayout();
        return static_cast<SelfRet>(this);
    }

    [[nodiscard]] auto getTextLabel() noexcept { return textLabel_; }

    SelfRet setText(StrView text) {
        apl::verify(textLabel_, "Attempted to set text without label");
        textLabel_->setText(text);
        if (pressedTextLabel_)
            pressedTextLabel_->setText(text);
        updateLayout();
        return static_cast<SelfRet>(this);
    }

    [[nodiscard]] const auto& getText() const noexcept { return textLabel_->getText(); }

    SelfRet clearText() {
        textLabel_->clearText();
        if (pressedTextLabel_)
            pressedTextLabel_->clearText();
        updateLayout();
        return static_cast<SelfRet>(this);
    }
    */

    SelfRet setSound(SfxId resid) {
        soundId_ = resid;
        return static_cast<SelfRet>(this);
    };

    [[nodiscard]] auto& getSensRect() const noexcept { return sensRect_; }

    // Checks if the widget isn't empty (has either text or texture)
    // [[nodiscard]] bool empty() const noexcept {
    //     // if (textLabel_ && !textLabel_->empty())
    //     //     return false;
    //     if (params_.texture.valid())
    //         return false;
    //     if (!text_.empty())
    //         return false;
    //
    //     return true;
    // }


    auto addKey(KeyCode key) noexcept {
        if (KeyCode::None == key) return this;
        if (keysCount_ < maxKeys_) {
            activateKeys_[keysCount_++] = key;
        } else {
            activateKeys_[keysCount_ - 1] = key; // just keep replacing the last one
        }
        return this;
    }
    auto clearKey() noexcept {
        keysCount_ = 0;
        return this;
    }

    auto setKey(KeyCode key) noexcept {
        clearKey();
        return addKey(key);
    }


    /*
    void setTextureBody(TextureA buttonTex, TextureA pressedTex, TextureA disabledTex) {
        texButton_ = buttonTex;
        texPressed_ = pressedTex;
        texDisabled_ = disabledTex;

        updateLayout();
    }

    SelfRet setTexture(TextureA buttonTex, TextureA pressedTex, TextureA disabledTex) {
        setTextureBody(buttonTex, pressedTex, disabledTex);
        return static_cast<SelfRet>(this);
    }

    SelfRet setTexture(TextureA buttonTex, TextureA pressedTex) {
        setTexture(buttonTex, pressedTex, pressedTex);
        return static_cast<SelfRet>(this);
    }

    SelfRet setTexture(TextureA buttonTex) {
        if (buttonTex.count == 1) {
            setTexture(buttonTex, buttonTex, buttonTex);
            return this;
        }

        if (buttonTex.count == 2) {
            setTexture(buttonTex.frame(0), buttonTex.frame(1), buttonTex.frame(1));
            return this;
        }

        setTexture(buttonTex.frame(0), buttonTex.frame(1), buttonTex.frame(2));
        return this;
    }
    */

    [[nodiscard]] bool pressed() const noexcept { return pressed_; }
    [[nodiscard]] bool pushed() const noexcept { return pushed_; }

    void ack() noexcept { pressed_ = pushed_ = false; }

private:

    //PrimP prim_{};


    /*
    Color pressedColor_{M::DarkGrey};
    bool pressedColorEnabled_{};

    Color buttonColor_{};
    bool buttonColorFlag_{};

    Color buttonTextColor_{};
    bool buttonTextColorFlag_{};

    FramesEffectP animation_{};


    String text_;
    LabelP textLabel_{}, pressedTextLabel_{};

    // Button rextures
    Texture texButton_{}, texPressed_{}, texDisabled_{};


    // Button widgets
    WidgetP backgroundWidget_{}, pressedBackgroundWidget_{}, disabledBackgroundWidget_{};
    WidgetP contentsWidget_{}, pressedWidget_{};


    Cord radius_{-1};
    WidgetBorderStrikesRuntime borderRuntime_{};
*/



    //
    // Keyboard
    //
    constexpr static int maxKeys_ = 3;
    std::array<KeyCode, maxKeys_> activateKeys_{};
    int keysCount_{};


    ButtonP processNew(ButtonParamsA params);

    ButtonP updateLayout();
    ButtonP updateSensRect();
    void updatePressed(bool pressed);

    void emitAction();


protected:

    void onCreate(SceneP scene) override;

    // void onVisible() override {
    //     if (!this->created_)
    //         return;
    //
    //     // if (animation_ && this->visible_) {
    //     //     animation_->attach(prim_);
    //     //     animation_->start();
    //     // }
    //
    //     updateVisible();
    // }

    //void onEnable() override { updateLayout(); }
    void onMove() override { updateLayout(); }
    //void onScissor() override { updateScissor(); }

    bool onTouchDown(TouchA) override;
    bool onTouchMove(TouchA) override;
    bool onTouchUp(TouchA) override;
    bool onTouchCancel(TouchA) override;

    bool onKeyDown(KeyCode key) override;
    bool onKeyUp(KeyCode key) override;

    //void onSetZIndexFront() override;
    //void onOrientation(M::Orientation o) override { updateLayout(); }
};


} // namespace mangl
