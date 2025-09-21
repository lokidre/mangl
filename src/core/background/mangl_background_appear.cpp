/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_background.h"

#include "../mangl_framework.h"

#include <apeal/debug.hpp>

#include <algorithm>

namespace mangl {

auto MBackground::init_element(MBackgroundLayer &layer) -> element_list_type::iterator
{
    auto elIt = select_element(layer);

    if (elIt == elements_.end())
        return elIt;

    auto &el = *elIt;

    el.so->clear();
    //el.prim->setWireframe(wireframe_) ;

    int element_texture = irandom((int)layer.textures.size());

    auto &tex = layer.textures[element_texture];

    el.texture = tex.texture;
    el.texture_frame = tex.frame_time;
    el.so->setTexture(el.texture);
    el.texture_current = 0;
    el.texture_timestamp = current_timestamp_ + el.texture_frame;

    if (!layer.period.set) {
        el.texture_current = elements_[layer.elementIndexBegin].texture_current;
        el.texture_timestamp = elements_[layer.elementIndexEnd].texture_timestamp;
    }

    el.so->setFlipHorz(layer.flip_horz);
    el.so->setVisible(true);

    Real speed = value(layer.speed);

    el.velocity.x = direction_.x * speed;
    el.velocity.y = direction_.y * speed;

    el.static_flag = layer.static_flag;


    if (layer.direction.x.set || layer.direction.y.set) {
        Point direction{value(layer.direction.x), value(layer.direction.y)};

        Real norm = cognit::hypot(direction.x, direction.y);
        if (norm > 0.000001) {
            el.velocity.x = direction.x / norm * speed;
            el.velocity.y = direction.y / norm * speed;
        }
    }

    el.alpha_flick = layer.alpha.flick;
    if (el.alpha_flick) {
        el.alpha_period = layer.alpha.period; // 1.0f/layer.alpha_frequency ;
        for (auto &aval: el.alpha_values) {
            M::random.range(layer.alpha.minValue, layer.alpha.maxValue, &aval);
        }
        el.alpha_current_index = 0;
        el.alpha_flick_timestamp = current_timestamp_;
    } else {
        if (layer.alpha.set)
            el.so->setAlpha(value(layer.alpha));
        else
            el.so->clearAlpha();
    }


    el.valid = true;
    el.layerIndex = layer.index;

    return elIt;
}

Rect MBackground::appear_element(MBackgroundLayer &layer, Point position, Real advance)
{
    Rect rect{0,0,viewport_.w,viewport_.h};

    //float MRect::*rectAlongPosition = direction_.y == 0 ? &MRect::x : &MRect::y ;
    auto Rect:: *rectCrossPosition = direction_.y == 0 ? &Rect::y : &Rect::x;
    auto Rect:: *rectCrossHeight = direction_.y == 0 ? &Rect::h : &Rect::w;

    auto minBaseline = minValue(layer.baseline), maxBaseline = maxValue(layer.baseline);
    //float width = viewport_.w, height = viewport_.h ;
    //bool fixedBaseline = false;

    auto current_baseline = position.y;
    Real parallaxBegin{}, parallaxEnd{};

    if (layer.parallaxRange.set) {
        parallaxBegin = minValue(layer.parallaxRange), parallaxEnd = maxValue(layer.parallaxRange);

        if (position.y == 0)
            current_baseline = skewedValueExp(layer.baseline, value(layer.parallaxPeriod));
    } else {
        if (position.y == 0)
            current_baseline = value(layer.baseline);
    }


    bool sortLayer = false;
    auto elIt = elements_.end();

    auto swapEl = [](MBackgroundElement &a, MBackgroundElement &b) {
        auto prevSprite = a.so;
        auto itSprite = b.so;
        std::swap(a, b);
        std::swap(*a.so, *b.so);
        a.so = prevSprite;
        b.so = itSprite;
    };

    auto processSort = [&]() {
        if (!sortLayer)
            return;

        sortLayer = false;

        auto elementsBegin = elements_.begin() + layer.elementIndexBegin;
        auto elementsEnd = elements_.begin() + layer.elementIndexEnd;

        // Insertion sort
        // Make sure all elements above the new elements are larger

        auto nIt = elIt;
        for (++nIt; nIt != elementsEnd; ++nIt) {
            if (!nIt->valid)
                continue;
            if (elIt->depth <= nIt->depth)
                break;
            swapEl(*elIt, *nIt);
            elIt = nIt;
        }

        // Now make sure that all the elements below are farther
        nIt = elIt;
        for (--nIt; nIt != elementsBegin - 1; --nIt) {
            if (!nIt->valid)
                continue;
            if (elIt->depth >= nIt->depth)
                break;
            swapEl(*elIt, *nIt);
            elIt = nIt;
        }
    };


    auto parallaxPosition = [&](const MBackgroundElement &el) {
        auto rect = el.so->getRect();
        auto position = (rect.*rectCrossPosition + rect.*rectCrossHeight/2 - minBaseline) / (maxBaseline - minBaseline);
        return apl::aclamp(position, Real(0), Real(1));
    };

    auto parallaxLambda = [&](const MBackgroundElement &el, Real position) {
        return parallaxBegin * (1 - position) + parallaxEnd * position;
    };

    auto parallaxScale = [&](const MBackgroundElement &el, Real lambda) {
        return minValue(layer.parallaxSize) * (1.f - lambda) + maxValue(layer.parallaxSize) * lambda;
    };

    auto processParallax = [&](MBackgroundElement &el) -> Rect {
        if (layer.parallaxRange.set) {

            sortLayer = true;

            auto rect = el.so->getRect();

            auto position = parallaxPosition(el);
            auto lambda = parallaxLambda(el, position);

            //el.depth = 1.0f - position ; //parallaxLambda ; //1.f - parallaxLambda ;
            el.depth = (maxBaseline - minBaseline)-(rect.*rectCrossPosition + rect.*rectCrossHeight/2 - minBaseline);


            if (layer.parallaxAlpha.set) {
                auto alpha = minValue(layer.parallaxAlpha) * (1- lambda) +  maxValue(layer.parallaxAlpha) * lambda;
                el.so->setAlpha(alpha);
            }

            if (layer.parallaxSize.set) {
                auto scale = parallaxScale(el, lambda);
                auto r = rect;
                r.scale(scale);
                rect = rect.centered(r.size());
            }

            if (layer.parallaxSpeed.set) {
                auto s = minValue(layer.parallaxSpeed) * (1- lambda) + maxValue(layer.parallaxSpeed) * lambda;
                el.velocity.scale(s);
            }

            //			if ( layer.parallaxFog.set ) {
            //				//MFloat fogRange = fabs(maxValue(layer.parallaxFog) - minValue(layer.parallaxFog)) ;
            //				MFloat positionY = (rect.*rectCrossPosition - minBaseline) / ( maxBaseline - minBaseline ) ;
            //				MFloat positionYH = (rect.*rectCrossPosition + rect.*rectCrossHeight/2 - minBaseline) / ( maxBaseline - minBaseline ) ;
            //				MFloat lambdaY = (parallaxBegin * ( 1 - positionY ) + parallaxEnd * positionY ) ;
            //				MFloat lambdaYH = (parallaxBegin * ( 1 - positionYH ) + parallaxEnd * positionYH ) ;
            //
            //				//el.prim->setZ ( 1.f - lambdaY, 1.f - lambdaY, 1.f - lambdaYH, 1.f - lambdaYH ) ;
            //				//el.prim->setFog(layer.parallaxFogColor, fogRange) ;
            //			}

            el.so->move(rect);
        } //else {
        //	el.prim->setZ(0) ;
        //}

        return el.so->getRect();
    };



    if (layer.full_screen) {
        minBaseline = 0, maxBaseline = viewport_.*rectCrossHeight;
        (direction_.y == 0 ? position.x : position.y) = 0;
        //fixedBaseline = true;
    }



    if (layer.tile || layer.pattern) {
//        fixedBaseline = true;

        if (advance < 0)
            position.x = 0;

        if (direction_.y == 0) {
            rect = {position.x, minBaseline, viewport_.w, maxBaseline};
        } else {
            rect = {minBaseline, position.x, maxBaseline, viewport_.h};
        }

        bool first = true;

        Point current{rect.x,rect.y};

        for (;;) {

            elIt = init_element(layer);
            if (elIt == elements_.end())
                return rect;
            auto &el = *elIt;

            auto textureSize = el.texture.size() * layer.elScale;
            auto elementSize = textureSize;

            el.so->move(current, textureSize);

            Cord pscale = 1;

            if (layer.parallaxRange.set && layer.parallaxSize.set) {
                auto pos = parallaxPosition(el);
                auto lambda = parallaxLambda(el, pos);
                pscale = parallaxScale(el, lambda);
                elementSize.scale(pscale);
            }

            if (first) {
                first = false;
                if (direction_.y == 0) {
                    current.y = rect.y + rect.h - elementSize.h;
                    current.x -= elementSize.w * value(layer.patternHorzOverlap) / 100.;
                } else {
                    current.x = rect.x + rect.w - elementSize.w;
                    current.y -= elementSize.h * value(layer.patternHorzOverlap) / 100.;  // maybe vert overlap
                }
            }

            if (layer.pattern && !isNull(layer.patternHorzOverlap)) {
                if (direction_.y == 0) {
                    current.x -= elementSize.w * value(layer.patternHorzOverlap) / 100.;
                } else {
                    current.y -= elementSize.h * value(layer.patternHorzOverlap) / 100.;
                }
            }

            el.so->move(current, textureSize);
            processParallax(el);
            processSort();

            if (direction_.y == 0) {
                current.x += elementSize.w;

                if (current.x >= rect.w) {
                    current.x = rect.x;
                    current.y -= elementSize.h;

                    if (layer.pattern && !isNull(layer.patternHorzOverlap))
                        current.x -= elementSize.w * value(layer.patternHorzOverlap) / 100.;

                    if (layer.pattern && !isNull(layer.patternVertOverlap))
                        current.y += elementSize.h * value(layer.patternVertOverlap) / 100.;

                    if (current.y + elementSize.h <= rect.y)
                        break;
                }
            } else {
                current.y += elementSize.h;

                if (current.y >= rect.h) {
                    current.y = rect.y;
                    current.x -= elementSize.w;

                    if (layer.pattern && !isNull(layer.patternHorzOverlap))
                        current.y -= elementSize.h * value(layer.patternHorzOverlap) / 100.;

                    if (layer.pattern && !isNull(layer.patternVertOverlap))
                        current.x += elementSize.w * value(layer.patternVertOverlap) / 100.;

                    if (current.x + elementSize.w <= rect.x)
                        break;
                }
            }
        }

    } else if (layer.full_screen) {

        elIt = init_element(layer);
        if (elIt == elements_.end())
            return rect;
        auto &el = *elIt;
        rect.resize(viewport_.size());
        if (direction_.y == 0)
            rect.move({position.x, viewport_.y});
        else
            rect.move({viewport_.x, position.x});
        el.so->move(rect);

    } else {

        elIt = init_element(layer);
        if (elIt == elements_.end())
            return rect;
        auto &el = *elIt;

        current_baseline = floor(current_baseline);

        el.so->setTexture(el.texture);

        rect.resize(el.texture.size());


        if (direction_.y == 0) {
            rect.x = position.x, rect.y = current_baseline;
            if (direction_.x > 0)
                rect.x -= rect.w;
        } else {
            rect.x = current_baseline, rect.y = position.x;
            if (direction_.y > 0)
                rect.y -= rect.h;
        }

        if (M::AlignRight == layer.alignHorz)
            rect.x -= rect.w;
        else if (M::AlignCenter == layer.alignHorz)
            rect.x -= rect.w/2;

        if (M::AlignTop == layer.alignVert)
            rect.y -= rect.h;
        else if (M::AlignVCenter == layer.alignVert)
            rect.y -= rect.h/2;

        rect.*rectCrossPosition += value(layer.position.y);
        el.so->move(rect);

        rect = processParallax(el);
        processSort();
    }

    layer.appear_timestamp = -1;

    return rect;
}


void MBackground::process_appear()
{
    std::pair<Real, Real> totalExtent(0, direction_.y == 0 ? viewport_.x + viewport_.w : viewport_.y + viewport_.h);

    // determine the extent of each layer
    //for ( MBackgroundLayer &l : layers_ ) {
    //	l.currentExtent = {totalExtent.second,totalExtent.first} ;
    //}

    bool positive = direction_.y == 0 ? (direction_.x > 0) : (direction_.y > 0);
    /*
        for ( auto &el : elements_ ) {
            if ( el.valid && el.layerIndex >= 0 ) {
                MBackgroundLayer &l = layers_[el.layerIndex] ;
                const auto &r = el.prim->rect() ;
                pair<float,float> elementExtent ( direction_.y == 0 ? r.x: r.y, direction_.y == 0 ? r.x + r.w : r.y + r.h ) ;
                if ( l.appear_timestamp == -1 ) {
                    if ( elementExtent.second > l.currentExtent.second )
                        l.currentExtent.second = elementExtent.second ;
                    if ( elementExtent.first < l.currentExtent.first )
                        l.currentExtent.first = elementExtent.first ;
                }
            }
        }
        */

    for (auto &layer : layers_) {

        layer.currentExtent = {totalExtent.second,totalExtent.first};

        for (auto elIt = elements_.begin() + layer.elementIndexBegin, elEnd = elements_.begin() + layer.elementIndexEnd; elIt != elEnd; ++elIt) {
            if (!elIt->valid)
                continue;
            auto r = elIt->so->getRect();
            std::pair<Real, Real> elementExtent(direction_.y == 0 ? r.x : r.y, direction_.y == 0 ? r.x + r.w : r.y + r.h);
            if (layer.appear_timestamp == -1) {
                if (elementExtent.second > layer.currentExtent.second)
                    layer.currentExtent.second = elementExtent.second;
                if (elementExtent.first < layer.currentExtent.first)
                    layer.currentExtent.first = elementExtent.first;
            }
        }


        if (layer.appear_timestamp == -1) {
            if (!isNull(layer.period)) {
                layer.appear_timestamp = current_timestamp_ + value(layer.period);
            } else {
                bool start = false;
                if (positive) {
                    start = layer.currentExtent.first >= totalExtent.first;
                } else {
                    start = layer.currentExtent.second <= totalExtent.second;
                }

                if (start)
                    layer.appear_timestamp = current_timestamp_;
            }
        }
    }

    // show every element
    for (auto &layer : layers_) {
        if (!layer.single && layer.speed.set && layer.appear_timestamp != -1 && current_timestamp_ >= layer.appear_timestamp) {

            Real pos;

            if (positive)
                pos = !isNull(layer.period) ? totalExtent.first : layer.currentExtent.first;
            else
                pos = !isNull(layer.period) ? totalExtent.second : layer.currentExtent.second;

            Real advance = positive ? -1 : 1;

            appear_element(layer, {pos,0}, advance);
        }
    }


}



auto MBackground::select_element(MBackgroundLayer &layer) -> element_list_type::iterator
{
    for (auto elIt = elements_.begin() + layer.elementIndexBegin + layer.currentElementIndex, elEnd = elements_.begin() + layer.elementIndexEnd; elIt != elEnd; ++elIt, ++layer.currentElementIndex) {
        if (!elIt->valid)
            return elIt;
    }
    layer.currentElementIndex = 0;

    // restart, if we couldn't find available element
    for (auto elIt = elements_.begin() + layer.elementIndexBegin + layer.currentElementIndex, elEnd = elements_.begin() + layer.elementIndexEnd; elIt != elEnd; ++elIt, ++layer.currentElementIndex) {
        if (!elIt->valid)
            return elIt;
    }
    layer.currentElementIndex = 0;

    MDebugFunc("Element buffer exhausted!");

    // if we still couldn't find available element, then replace some element from the given layer
    return elements_.end();
}

}
