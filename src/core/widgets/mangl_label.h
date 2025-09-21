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
#include "mangl_label_params.h"

#include "../mangl_font.h"

#include <apeal/buffer.hpp>

namespace mangl {

class Label: public WidgetTemplate<Label> {
    friend class Scene;

    using Base = WidgetTemplate<Label>;
    using Self = Label;

    using SelfRet = Self*;

    LabelParams lp_{};

    ScenePrimsContainerP prims_{};
    // int primsCount_{};
    // int effectPrimsCount_{};

    String text_;
    Cord height_{}, fontHeight_{}, userFontHeight_{};
    // int maxTextLength_{128};  // TODO: make dynamic
    // int totalEffectPrims_{128 * 2};

    // Background effects
    LabelEffectParams effectParams_{};
    ScenePrimsContainerP effectPrims_{};

    ColorGradient colors_{};

    FontCP font_{};
    Cord spacing_{};

    Rect boundingRect_{};
    Flags textAlignment_{};
    bool monospace_{};
    Cord maxCharWidth_{}, monoCharWidth_{}, spaceWidth_{};


    bool nativeWidget_{};
    WidgetId nativeWidgetId_{};
    String nativeFontName_{};
    void createNativeWidget();


    LabelP updateContents() {
        updateLayout();
        updateColors();
        return this;
    }

    void updateAlpha();
    void updateColors();
    void updateVisible();
    LabelP updateLayout();
    void updateLayoutPrims();


    LabelP onNew(LabelParamsA params);

    void setFontParams(FontCP font);

    void setEffectParams(LabelEffectParamsA params) noexcept;
    void setTextImpl(StrView text) noexcept;
    void clearTextImpl() noexcept;
    void managePrims() noexcept;


public:
    //
    // Setup
    //
    LabelP setFont(FontCP font) {
        setFontParams(font);
        return this;
    }

    auto setFontName(StrView fontName) {  
        nativeFontName_ = fontName; 
        return updateContents();
    }

    [[nodiscard]] auto getFont() const noexcept { return font_; }

    auto setColors(ColorA ca, ColorA cb, ColorA cc, ColorA cd) {
        colors_[0] = ca;
        colors_[1] = cb;
        colors_[2] = cc;
        colors_[3] = cd;
        return updateContents();
    }

    auto setColors(ColorGradientA gradient) noexcept {
        colors_ = gradient;
        return updateContents();
    }
    auto setGradient(ColorGradientA gradient) noexcept {
        colors_ = gradient;
        return updateContents();
    }



    //
    // Text layout
    auto setTextAlignment(Flags flags) {
        textAlignment_ = flags;
        return updateContents();
    }

    [[nodiscard]] auto getTextAlignment() const noexcept { return textAlignment_; }

    auto setTextHeight(Cord height) noexcept {
        userFontHeight_ = static_cast<RealGL>(height);
        return updateContents();
    }

    [[nodiscard]] auto getTextHeight() const noexcept { return userFontHeight_; }

    // TODO: implement
    [[nodiscard]] Rect getBoundingBox() const noexcept {
        auto rect = getRect();
        return rect;
    }

    auto setSpacing(Cord s) noexcept {
        spacing_ = static_cast<RealGL>(s);
        return updateLayout();
    }

    auto setMonospace(bool f = true) noexcept {
        monospace_ = f;
        return updateLayout();
    }

    auto setMonoCharWidth(Cord w) noexcept {
        monoCharWidth_ = static_cast<RealGL>(w);
        return updateLayout();
    }

    auto setSpaceWidth(Real w) noexcept {
        spaceWidth_ = static_cast<RealGL>(w);
        return updateLayout();
    }


    //
    // Formatted text
    //
    auto setText(StrView text) noexcept {
        M::debugAssertMainThread();
        setTextImpl(text);
        return this;
    }

    template <class ...Args>
    auto setText(StrView fmt, Args&&... args) noexcept {
        M::debugAssertMainThread();
        setTextImpl(apl::str_format(text_, fmt, std::forward<Args>(args)...));
        return this;
    }

    template <class Int>
    auto setInt(Int value) noexcept {
        M::debugAssertMainThread();
        setTextImpl(std::to_string(value));
        return this;
    }

    auto clearText() noexcept { 
        M::debugAssertMainThread();
        clearTextImpl();
        return this;
    }


    [[nodiscard]] const auto& getText() const noexcept { return text_; }
    [[nodiscard]] auto empty() const noexcept { return text_.empty();  }


    auto setEffect(LabelEffectParamsA params) noexcept {
        setEffectParams(params);
        return updateContents();
    }

    auto clearEffects() noexcept { 
        effectParams_.type = LabelEffectType::None;
        return updateContents();
    }


    Size renderedSize{};  // Will keep total size of the text
    [[nodiscard]] Size getRenderedSize() const noexcept {  return renderedSize;  }


protected:
    void onCreate() override;
    void onVisible(bool visible) override { updateVisible(); }
    void onAlpha() override { updateAlpha(); }
    void onScissor(bool scissorFlag, RectA scissorRect) override {
        updateContents();
    }
    void onColor(bool colorFlag, ColorA color) noexcept override  {
        for (auto& c: colors_)
            c = color_;
        updateColors();
    }
    void onMove(RectA rect) override {
        boundingRect_ = rect;
        updateContents();
    }

    void onOrientation(M::Orientation orientation) override {
        updateContents();
    };
    void onSetZIndexFront() override;

};

} // namespace mangl
