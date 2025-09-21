/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_label.h"

#include "../mangl_scene.h"
#include "../mangl_styles.h"

#include "../native/mangl_native_gui.h"

namespace mangl {

LabelP Scene::newLabel(LabelParamsA lp) {
    return newWidget<Label>(lp.flags)->onNew(lp)->create();
}

Label* Label::onNew(const LabelParams& params) {

    type = WidgetType::Label;
    lp_ = params;

    auto& style = M::Styles::label;
    auto textStyle = params.style;

    if (params.textInt != 0) {
        setTextImpl(std::to_string(params.textInt));
    } else {
        setTextImpl(params.text);
    }

    // text_ = params.text;
    // if (params.textInt != 0)
    //     text_ = std::to_string(params.textInt);

    if (params.color.valid()) {
        setColor(params.color);
    } else {
        if (textStyle && textStyle->color.valid())
            setColor(textStyle->color);
        else
            setColor(style.text.color);
    }

    if (params.textSize > 0) {
        userFontHeight_ = params.textSize;
    } else {
        if (textStyle && textStyle->size > 0)
            userFontHeight_ = textStyle->size;
        else
            userFontHeight_ = style.text.size;
    }

    if (params.pos) {
        move({params.pos.x, params.pos.y, 0, 0});
    }
    if (params.rect.w != 0)
        move(params.rect);

    spacing_ = params.spacing;
    textAlignment_ = params.flags | params.align;

    setFontParams(params.font ? params.font : Font::getDefaultFont());

    if (params.alpha != 0)
        setAlpha(params.alpha);

    if (params.transparent)
        setAlpha(0);

    if (params.effect.type != LabelEffectType::None)
        setEffectParams(params.effect);

    if (params.hidden)
        hide();

    return this;
}


void Label::onCreate() {
    interactionDisabled_ = true;

    effectPrims_ = scene_->newPrimArray();
    prims_ = scene_->newPrimArray();

    if (font_ && !font_->valid()) {
        MDebugThrowError("Label::onCreate:Invalid Font");
    }

    if (!font_)
        setFont(Font::getDefaultFont());

    managePrims();
    updateContents();
}


void Label::setFontParams(FontCP font) {
    font_ = font;

    auto& glyphs = font_->getGlyphs();

    // analyze the font
    maxCharWidth_ = 0;
    for (auto& ch: glyphs) {
        if (ch.second.glyphSize.w > maxCharWidth_)
            maxCharWidth_ = ch.second.texture.w();
    }

    if (monoCharWidth_ == 0)
        monoCharWidth_ = maxCharWidth_;
}


void Label::setEffectParams(LabelEffectParamsA params) noexcept {
    effectParams_ = params;
    switch (effectParams_.type) {
    case LabelEffectType::Shadow:
        if (effectParams_.scale == 0)
            effectParams_.scale = 1;
        break;
    case LabelEffectType::OuterGlow:
        if (effectParams_.blur == 0)
            effectParams_.blur = 4;
        if (effectParams_.opacity == 0)
            effectParams_.opacity = 0.75;
        if (!effectParams_.color.valid())
            effectParams_.color = M::White;
        break;
    default: break;
    }


}

void Label::setTextImpl(StrView text) noexcept {
    text_ = text;
    managePrims();
    updateContents();
}

void Label::clearTextImpl() noexcept {
    text_.clear();
    managePrims();
    updateContents();
}

void Label::managePrims() noexcept {
    if (!prims_) return;


    // std::ranges::for_each(*prims_, [](auto& prim) { prim.hide(); });


    if (text_.empty()) {
        prims_->clear();
        effectPrims_->clear();
        return;
    }

    // MDebugFunc("%s", text_);

    prims_->resize(text_.size());

    if (effectParams_.type == LabelEffectType::None) {
        effectPrims_->clear();
    } else {
        effectPrims_->resize(text_.size() * 2);
    }

}



void Label::updateVisible() {
    if (nativeWidget_) {

        if (prims_)
            std::ranges::for_each(*prims_, [](auto& prim) { prim.hide(); });

        if (effectPrims_)
            std::ranges::for_each(*effectPrims_, [](auto& prim) { prim.hide(); });

        NativeGui::instance()->setVisible(nativeWidgetId_, visible_);
        return;
    }

    if (!prims_ || !effectPrims_)
       return;

    std::ranges::for_each(*prims_, [this](auto& prim) {
        prim.setVisible(visible_)->setScissor(scissorFlag_, scissorRect_);
    });

    std::ranges::for_each(*effectPrims_, [this](auto& prim) {
        prim.setVisible(visible_)->setScissor(scissorFlag_, scissorRect_);
    });
}

void Label::createNativeWidget()
{
    // TODO: Implement correct create native gui
//    nativeWidget_ = true;
//
//    if (!nativeWidgetId_) {
//        auto ng = NativeGui::instance();
//        ng->createLabel(text_, r_);
//    }
}

void Label::updateAlpha() {
    if (nativeWidget_) {
        return;
    }

    // if (alpha_ == 0) {
    //     int a = 0;
    // }
    //
    // if (alpha_ == 1) {
    //     int a = 0;
    // }
    //
    //
    // MDebugFunc("Alpha: %g", alpha_);
    updateColors();
}

void Label::updateColors() {

    if (!prims_)
        return;

    auto textColors = colors_;

    Color effectColor1, effectColor2;

    switch (effectParams_.type) {
    case LabelEffectType::Shadow:
        effectColor1 = effectParams_.color;
        break;
    case LabelEffectType::OuterGlow:
        effectColor1 = effectParams_.color;
        effectColor2 = effectParams_.color;
        break;
    default: break;
    }

    // if (alphaFlag_) {
    //     for (auto& c: textColors) c.scaleAlpha(alpha_);
    //     effectColor1.scaleAlpha(alpha_);
    //     effectColor2.scaleAlpha(alpha_);
    // }

    auto prim = prims_->begin();
    auto bprim = effectPrims_->begin();

    for (const auto &chIt: text_) {
        prim->setColors(colorFlag_, textColors);
        prim->setAlpha(alphaFlag_, alpha_);

        switch (effectParams_.type) {
        case LabelEffectType::Shadow:
            bprim->setColor(effectColor1);
            bprim->setAlpha(alphaFlag_, alpha_);
            ++bprim;
            break;
        case LabelEffectType::OuterGlow:
            bprim->setColor(effectColor2);
            bprim->setAlpha(alphaFlag_, alpha_);
            ++bprim;
            break;
        default:
            break;
        }


        ++prim;
    }
}


LabelP Label::updateLayout() {
    if (nativeWidget_) {
        return this;
    }

    updateLayoutPrims();
    return this;
}

void Label::updateLayoutPrims() {
    if (!prims_) return;

    if (text_.empty()) {
        // primsCount_ = 0;
        // effectPrimsCount_ = 0;

        for (auto& prim: *prims_) {
            prim.hide();
        }

        for (auto& bprim: *effectPrims_) {
            bprim.hide();
        }

        prims_->clear();
        effectPrims_->clear();
        return;
    }

    if (!font_ || !font_->valid()) {
        createNativeWidget();
        //mthrowError("Font not set in Label");
        return;
    }

    auto fontBBoxHeight = font_->bboxHeight();

    auto spaceAdvance = font_->getSpaceAdvance();

    Point current{}, startPos{};
    Cord fontScale = (userFontHeight_ != 0) ? userFontHeight_ / fontBBoxHeight : 1;
    auto scale = fontScale;
    auto spacing = spacing_ * userFontHeight_;


    Size totalSize{};

    auto processText = [&](bool renderFlag) {
        bool firstChar = true;
        // primsCount_ = 0;
        // effectPrimsCount_ = 0;

        auto& charlist = font_->getGlyphs();
        auto prim = prims_->begin();
        auto bprim = effectPrims_->begin();

        auto charlistEnd = charlist.end();
        auto lastChFont = charlistEnd;

        Cord lastAdvance{}, lastBearingX{}, lastGlyphWidth{};


        for (const auto &chIt: text_) {

            auto chFont = charlist.find(chIt);

            if (chFont == charlist.end()) {
                if (!firstChar)
                    current.x += spacing;
                current.x += spaceAdvance * scale;
                lastChFont = charlistEnd;
                continue;
            }

            // if (primsCount_ >= maxTextLength_)
            //     break;

            const auto &ch = chFont->second;

            if (!firstChar && ch.charcode != ' ')
                current.x += spacing;


            Cord horzAdvance{}, horzBearingX{};

            // Check if the user specified special size for the space
            if (ch.charcode == ' ' && spaceWidth_ != 0) {
                horzAdvance = spaceWidth_;
            } else {
                if (monospace_) {
                    horzAdvance = monoCharWidth_ / fontScale;
                    horzBearingX = (horzAdvance - ch.glyphSize.w)/2;
                    horzAdvance -= horzBearingX;
                } else {
                    horzAdvance = ch.advance.x;
                    horzBearingX = ch.horzBearing.x;
                }
            }

            current.x += horzBearingX * scale;

            // adjust for the kerning
            if (!monospace_ && lastChFont != charlistEnd) {
                auto&& kerning = lastChFont->second.kerning;
                if (auto kernCh = kerning.find(ch.charcode); kernCh != kerning.end())
                    current.x += kernCh->second * scale;
            }

            if (ch.glyphSize.w > 0 && ch.glyphSize.h > 0) {

                if (renderFlag) {

                    auto r = Rect::make(current, ch.glyphSize * scale);
                    auto glyphOffsetY = (ch.horzBearing.y - ch.glyphSize.h) * scale;

                    prim->setTexture(chFont->second.texture);

                    if (orientation_ != M::Orientation0) {
                        auto offsetX = r.x - startPos.x;
                        offsetX = totalSize.w - offsetX;
                        offsetX -= r.w;
                        r.x = startPos.x + offsetX;// - totalSize.w/2;

                        r.y -= glyphOffsetY;
                        auto hdiff = totalSize.h - ch.glyphSize.h*scale;
                        r.y += hdiff;

                        prim->move(r);
                        prim->setOrientation(orientation_);
                    } else {
                        r.y += glyphOffsetY;
                        prim->move(r);
                        prim->clearOrientation();
                    }


                    switch (effectParams_.type) {
                    case LabelEffectType::Shadow: {
                        auto sc = static_cast<RealGL>(effectParams_.scale);
                        auto s = r.size() * sc;
                        r.x -= (s.w - r.w)/2;
                        r.y -= (s.h - r.h)/2;
                        r.w *= sc;
                        r.h *= sc;
                        r.x += static_cast<RealGL>(effectParams_.offset.x);
                        r.y += static_cast<RealGL>(effectParams_.offset.y);
                        bprim->setTexture(chFont->second.texture)->move(r);
                        ++bprim;
                        // ++effectPrimsCount_;
                        break;
                    }
                    case LabelEffectType::OuterGlow: {
                        auto blur = effectParams_.blur;
                        auto rr = r;
                        bprim->setTexture(chFont->second.texture)
                            ->move(rr)
                            ->setBlur(effectParams_.blur, effectParams_.opacity);
                        ++bprim;
                        // ++effectPrimsCount_;
                        break;
                    }
                    default:
                        break;
                    }

                    ++prim;
                }

                // ++primsCount_;
            }


            if (monospace_) {
                current.x += horzAdvance * scale;
            } else {
                current.x += (horzAdvance - horzBearingX) * scale;
            }

            if (ch.glyphSize.h * scale > totalSize.h)
                totalSize.h = ch.glyphSize.h * scale;

            lastAdvance = horzAdvance * scale;
            lastBearingX = horzBearingX * scale;
            lastGlyphWidth = ch.glyphSize.w * scale;

            firstChar = false;
            lastChFont = chFont;
        }

        // adjust the last character's advance
        if (monospace_) {
            //currentX -= lastAdvance ;
        } else {
            current.x -= lastAdvance;
            current.x += lastGlyphWidth;
        }

    };

    // Determine the total size of the text
    processText(false);


    // determine the scale
    totalSize.w = current.x;
    scale = fontScale;

    // Place the text
    current = boundingRect_.position();

    if (M::AlignTop & textAlignment_)
        current.y += boundingRect_.h - totalSize.h;
    else if (M::AlignVCenter & textAlignment_)
        current.y += (boundingRect_.h - totalSize.h)/2;

    if (M::AlignRight & textAlignment_)
        current.x += boundingRect_.w - totalSize.w;
    else if (M::AlignCenter & textAlignment_)
        current.x += (boundingRect_.w - totalSize.w)/2;

    startPos = current;

    // Render the text

    processText(true);

    totalSize.w = current.x - startPos.x;


    updateVisible();

    renderedSize = totalSize;
}

void Label::onSetZIndexFront() {
    scene_->moveToFront(effectPrims_);
    scene_->moveToFront(prims_);
}


}  // namespace mangl

