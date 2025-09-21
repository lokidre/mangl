/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_background.h"

namespace mangl {


void MBackground::onStart(Timestamp timestamp)
{
    current_timestamp_ = last_timestamp_ = timestamp;

    std::seed_seq sseq{1, 2};
    mt_.seed(sseq);

    bool positive = direction_.y == 0 ? (direction_.x > 0) : (direction_.y > 0);

    //
    // Allocate screen elements for each background layer
    //
    Cord Rect:: *rectPosition = direction_.y == 0 ? &Rect::x : &Rect::y;
    Cord Size:: *rectExtent = direction_.y == 0 ? &Size::w : &Size::h;


    // Estimate elements per layer based on the speed, period and texture sizes
    for (auto &layer : layers_) {

        auto textureExtent = 0.;

        if (layer.elSize.w == 0) {
            layer.elSize = layer.textures[0].texture.size();
        }

        if (layer.elScale == 0) {
            layer.elScale = layer.elSize.w / layer.textures[0].texture.size().w;
        }

        // find the smallest and largest texture
        auto textureSize = layer.elSize;
        auto largestTextureRect = layer.elSize;
        for (auto &tex : layer.textures) {
            if (tex.texture.size().*rectExtent * layer.elScale < textureSize.*rectExtent)
                textureSize = tex.texture.size() * layer.elScale;
            if (tex.texture.size().*rectExtent * layer.elScale > largestTextureRect.*rectExtent)
                largestTextureRect = tex.texture.size() * layer.elScale;
            //if ( tex.texture.size().*rectExtent < textureSize.*rectExtent )
            //	textureSize = tex.texture.size() ;
            //if ( tex.texture.size().*rectExtent > largestTextureRect.*rectExtent )
            //	largestTextureRect = tex.texture.size() ;
        }

        auto totalExtent = viewport_.size().*rectExtent * layer.elScale + largestTextureRect.*rectExtent;

        textureExtent = (direction_.y == 0) ? textureSize.w : textureSize.h;

        // handle big textures
        if (textureExtent >= totalExtent / 2 || layer.full_screen) {
            layer.elementIndexEnd = 2;
            if (layer.single)
                layer.elementIndexEnd = 1;
            if (layer.speed.set)
                layer.elementIndexEnd = 3;
            if (layer.tile)
                layer.elementIndexEnd += int((viewport_.h/textureSize.h + 1) * (viewport_.w/textureSize.w + 1));

            if (!isNull(layer.speed))
                layer.elementIndexEnd *= 2;
        } else {
            layer.elementIndexEnd = 3;

            if (isNull(layer.period)) {
                if (layer.tile || layer.pattern) {
                    auto horzOverlap = isNull(layer.patternHorzOverlap) ? 100. : maxValue(layer.patternHorzOverlap);
                    auto vertOverlap = isNull(layer.patternVertOverlap) ? 100. : maxValue(layer.patternVertOverlap);
                    auto h = textureSize.h / 100. * vertOverlap;
                    auto w = textureSize.w / 100. * horzOverlap;
                    // multiply by 2 - overlap from both sides
                    w /= 2, h /= 2;
                    layer.elementIndexEnd += int(viewport_.w/w+1)*int(viewport_.h/h+1);
                } else {
                    layer.elementIndexEnd += int(totalExtent/textureSize.*rectExtent);
                }
            } else {
                auto periodVal = minValue(layer.period);
                auto speedVal = minValue(layer.speed);
                auto deltaT = speedVal * periodVal;
                deltaT /= 4;  // both ends
                layer.elementIndexEnd += int((totalExtent + largestTextureRect.*rectExtent) / deltaT + 1);
            }
        }

        layer.elementIndexBegin = 0;
    }


    // Initializa all elements
    for (auto &el : elements_)
        el.clear();

    Index currentIndex{};

    if (backgroundColorFlag_) {
        auto &el = elements_[currentIndex];
        el.layerIndex = -1;
        currentIndex++;

        el.so->setColor(backgroundColor_)->setBox(viewport_)->setVisible(true);
        el.valid = true;
    }

    if (!gradients_.empty()) {

        auto gIt = gradients_.begin(), gEnd = gradients_.end();

        Point a, b, c, d;
        Color gc = gIt->color;

        if (gradientVertical_) {
            a = {0, gIt->position * viewport_.h}, b = {viewport_.w, gIt->position * viewport_.h};
        } else {
            a = {gIt->position * viewport_.w, 0}, b = {gIt->position * viewport_.w, viewport_.h};
        }

        for (++gIt; gIt != gEnd; a = d, b = c, gc = gIt->color, ++gIt) {

            auto &p = elements_[currentIndex++];
            auto &q = elements_[currentIndex++];

            p.layerIndex = q.layerIndex = -1;
            p.valid = q.valid = true;

            if (gradientVertical_) {
                c = {viewport_.w, gIt->position * viewport_.h}, d = {0, gIt->position * viewport_.h};
            } else {
                c = {gIt->position * viewport_.w,  viewport_.h}, d = {gIt->position * viewport_.w, 0};
            }

            p.so->setTriangle(a, b, d)->setColors(gc, gc, gIt->color)->setVisible();
            q.so->setTriangle(b, c, d)->setColors(gc, gIt->color, gIt->color)->setVisible();
        }
    }

    for (auto& l: layers_) {
        l.valid = true;

        l.elementIndexBegin = currentIndex;
        l.elementIndexEnd += l.elementIndexBegin;

        currentIndex = l.elementIndexEnd;

        if (l.elementIndexEnd > elements_.size())
            throw std::runtime_error("MBackground::Element buffer is not large enough");
    }


    // place initial elements for background
    for (auto &layer : layers_) {

        Cord start_coord = layer.startXPercent * 0.01 * viewport_.size().*rectExtent;

        if (layer.position.x.set)
            start_coord += value(layer.position.x);

        auto startPos = start_coord + viewport_.*rectPosition;
        auto endPos = viewport_.*rectPosition + viewport_.size().*rectExtent;
        auto positionAdvance = 1.;
        if (positive) {
            std::swap(startPos, endPos);
            positionAdvance = -1;
        }

        for (auto currentPos = startPos; currentPos * positionAdvance < endPos * positionAdvance; ) {

            auto elRect = appear_element(layer, {currentPos,0}, positionAdvance);

            if (!isNull(layer.speed)) {
                currentPos += isNull(layer.period) ? elRect.size().*rectExtent : value(layer.period) * value(layer.speed) * speed_k * positionAdvance;
            } else {
                currentPos += elRect.size().*rectExtent * positionAdvance;
            }


            if (layer.single)
                break;
        }
    }

    started_ = true;
}

}
