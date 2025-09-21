/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_animation.h"

#include "../effects/mangl_frames_effect.h"


#include <apeal/time.hpp>

namespace mangl {


void Animation::onTimer(Timestamp timestamp)
{
    using LOOP = MAnimationLayer::Loop;
    using TYPE = MAnimationKeyframe::Type;
    using PTYPE = MAnimationKeyframe::ParamType;
    using PTAG = MAnimationKeyframe::ParameterTag;

    if (finished || !started)
        return;

    auto animationTimestamp = timestamp - restartTimestamp_;

    // Check the animation restart conditions
    if (animationTimestamp >= length) {
        if (!repeat) {
            finished = true, started = false;
            return;
        }

        // Restart the animation
        restartTimestamp_ = timestamp;
        animationTimestamp = 0;

        for (auto &layer: layers) {
            layer.finished = layer.started = false;

            for (auto &keyframe: layer.keyframes)
                keyframe.setSpriteVisible(false);
        }

    }

    // 
    // For each layer of the animation
    //
    for (auto &layer: layers) {

        // Skip layers that are not active
        if (!layer.visible || layer.finished || layer.keyframes.empty())
            continue;

        if (animationTimestamp < layer.start)
            continue;

        auto total_keyframes = (int)layer.keyframes.size();

        if (!layer.started) {
            layer.started = true;
            layer.startTimestamp = layer.layerRestartTimestamp = layer.periodRestartTimestamp = timestamp;
            layer.currentKeyframe = 0;
            layer.lastKeyframe = -1;
            layer.nextKeyframe = (layer.currentKeyframe + 1 < (int)layer.keyframes.size()) ? layer.currentKeyframe + 1 : layer.currentKeyframe;
            layer.adjacentKeyframe = layer.nextKeyframe;
            layer.keyframe_direction = true;

            if (debugVerbose_) {
                MDebugFunc("Starting layer: '%s'", layer.id);
            }

            // 
            // Compute keyframe positions, colors, etc.
            //
            for (auto &keyframe: layer.keyframes) {
                if (keyframe.computedFlag)
                    continue;
                keyframe.computedFlag = true;

                // go through all the parameters
                for (auto &pit : keyframe.params) {
                    auto &p = pit.second;
                    if (!p.setFlag)
                        continue;
                    p.computedFlag = true;

                    switch (p.type) {
                    case PTYPE::Distance:
                        p.computedFloatValue = xVal(p.distanceValue);
                        break;
                    case PTYPE::Point:
                        p.computedPointValue.x = xVal(p.pointValue.x);
                        p.computedPointValue.y = yVal(p.pointValue.y);
                        break;
                    default:
                        break;
                    }
                }


                if (0 == keyframe.alignHorz)
                    keyframe.alignHorz = alignHorzDefault_;
                if (0 == keyframe.alignVert)
                    keyframe.alignVert = alignVertDefault_;

                // first determine the keyframe size
                switch (keyframe.type) {
                case TYPE::IMAGE:
                    if (keyframe.size.w.null() && keyframe.size.h.null()) {
                        keyframe.size.w = keyframe.images[0].texture.w();
                        keyframe.size.h = keyframe.images[0].texture.h();
                    }

                    //if ( keyframe.elsize.w.null() && keyframe.elsize.h.null() )
                    //	keyframe.elsize = keyframe.size ;
                    //else if ( keyframe.elsize.w.null() )
                    //	keyframe.elsize.w = keyframe.elsize.h. * keyframe.images[0].texture.w() / keyframe.images[0].texture.h() ;
                    break;
                default:
                    break;
                }

                keyframe.computedFrame.w = xVal(keyframe.size.w);
                keyframe.computedFrame.h = yVal(keyframe.size.h);

                if (keyframe.type == TYPE::IMAGE) {
                    if (keyframe.computedFrame.w == 0)
                        keyframe.computedFrame.w = keyframe.computedFrame.h * keyframe.images[0].texture.w() / keyframe.images[0].texture.h();
                    else if (keyframe.computedFrame.h == 0)
                        keyframe.computedFrame.h = keyframe.computedFrame.w * keyframe.images[0].texture.h() / keyframe.images[0].texture.w();
                }

                // now dermine the position
                auto pos = Point{xVal(keyframe.position.x), yVal(keyframe.position.y)};

                if (M::AlignRight == keyframe.alignHorz)
                    pos.x -= keyframe.computedFrame.w;
                else if (M::AlignCenter == keyframe.alignHorz)
                    pos.x -= keyframe.computedFrame.w/2;

                if (M::AlignTop == keyframe.alignVert)
                    pos.y -= keyframe.computedFrame.h;
                else if (M::AlignVCenter == keyframe.alignVert)
                    pos.y -= keyframe.computedFrame.h/2;

                pos += { xVal(defaultOrigin_.x), yVal(defaultOrigin_.y) };
                pos += viewport_.position();
                pos += { xVal(layer.origin.x), yVal(layer.origin.y) };

                keyframe.computedFrame.move(pos);

                for (auto &image : keyframe.images) {
                    image.computedPosition = {xVal(image.position.x), yVal(image.position.y)};
                }

                for (auto &text : keyframe.texts) {
                    text.computedPosition = {xVal(text.position.x), yVal(text.position.y)};
                    text.widget->setTextAlignment(keyframe.alignHorz | keyframe.alignVert);
                    if (keyframe.textHeightFlag)
                        text.widget->setTextHeight(keyframe.textHeight*scale_);
                }

                for (auto &pt : keyframe.pts) {
                    pt.computedPosition = pointVal(pt.position);
                    pt.computedPosition += keyframe.computedFrame.position();
                }

                if (keyframe.type ==  TYPE::IMAGE) {
                    if (keyframe.elSize.w.null() && keyframe.elSize.h.null())
                        keyframe.computedElSize = keyframe.computedFrame.size();
                    else if (keyframe.elSize.w.null())
                        keyframe.computedElSize = {yVal(keyframe.elSize.h) * keyframe.images[0].texture.w() / keyframe.images[0].texture.h(), yVal(keyframe.elSize.h)};
                    else if (keyframe.elSize.h.null())
                        keyframe.computedElSize = {xVal(keyframe.elSize.w), yVal(keyframe.elSize.h) * keyframe.images[0].texture.h() / keyframe.images[0].texture.w()};
                    else
                        keyframe.computedElSize = {xVal(keyframe.elSize.w), yVal(keyframe.elSize.h)};
                }

                if (debugVerbose_) {
                    MDebugFunc("keyframe '%s' computed rect: %.2f", keyframe.id, keyframe.computedFrame);
                }

            }
        }

        auto layerTimestamp = timestamp - layer.layerRestartTimestamp;
        auto periodTimestamp = timestamp - layer.periodRestartTimestamp;


        // If the layer finished
        //if ( ( layer.length != 0 && layerTimestamp >= layer.length ) || ( layer.period != 0 && periodTimestamp >= layer.period ) ) {
        if (layerTimestamp >= layer.length || periodTimestamp >= layer.period) {

            // If the period is larger than the length, then stop the layer
            switch (layer.loop) {
            case LOOP::NONE:
                layer.started = false;
                layer.finished = true;
                layer.keyframes[layer.currentKeyframe].setSpriteVisible(false);
                break;

            case LOOP::CYCLE:
                layer.keyframes[layer.currentKeyframe].setSpriteVisible(false);

                layer.periodRestartTimestamp = timestamp;
                layer.lastKeyframe = -1;
                layer.currentKeyframe = 0;
                layer.adjacentKeyframe = layer.nextKeyframe = (layer.currentKeyframe + 1 < total_keyframes) ? layer.currentKeyframe + 1 : layer.currentKeyframe;
                layer.layerRestartTimestamp = layer.periodRestartTimestamp = timestamp;
                break;

            case LOOP::PINGPONG:
                layer.keyframes[layer.currentKeyframe].setSpriteVisible(false);

                layer.keyframe_direction = !layer.keyframe_direction;

                layer.periodRestartTimestamp = timestamp;

                if (layer.keyframe_direction) {
                    layer.nextKeyframe = 1;
                    layer.adjacentKeyframe = 1;
                    layer.currentKeyframe = 0;
                } else {
                    layer.currentKeyframe = 0;
                    layer.nextKeyframe = total_keyframes - 1;
                    layer.adjacentKeyframe = total_keyframes - 1;
                }
                layer.lastKeyframe = -1;
                layer.layerRestartTimestamp = layer.periodRestartTimestamp = timestamp;
                break;
            }

            //}
        //}

            if (layer.period > layer.length) {

                layer.started = false;

                // If the layer not in the loop finish it
                if (layer.loop == LOOP::NONE) {
                    layer.finished = true;
                }

                layer.startTimestamp = animationTimestamp + layer.period - layer.length;

            } //else {

                // If layer finished
                /*
                if ( layerTimestamp >= layer.length && layer.start != 0 ) {
                    // figure out if we should stop animating the layer
                    layer.started = false ;
                    if ( layer.loop == LOOP::NONE ) {
                        layer.finished = true ;
                    }
                    layer.keyframes[layer.currentKeyframe].setSpriteVisible(false) ;

                } else {
                */


                //periodTimestamp = 0 ;
        } else { // The layer is still in progress
            if (layer.nextKeyframe != layer.currentKeyframe) {

                // select the correct keyframe
                if (layer.keyframe_direction) {
                    if (periodTimestamp >= layer.keyframes[layer.nextKeyframe].time) {
                        layer.keyframes[layer.currentKeyframe].setSpriteVisible(false);

                        layer.lastKeyframe = layer.currentKeyframe;
                        layer.currentKeyframe = layer.nextKeyframe;

                        if (layer.currentKeyframe + 1 < total_keyframes) {
                            layer.adjacentKeyframe = layer.nextKeyframe = layer.currentKeyframe + 1;
                        } else {
                            layer.nextKeyframe = layer.currentKeyframe;
                            layer.adjacentKeyframe = layer.loop == LOOP::NONE ? layer.currentKeyframe : 0;
                        }

                    } else
                        ;
                } else {  // backwards
                    if (periodTimestamp >= layer.period - layer.keyframes[layer.nextKeyframe].time) {
                        layer.keyframes[layer.currentKeyframe].setSpriteVisible(false);

                        layer.lastKeyframe = layer.currentKeyframe;
                        layer.currentKeyframe = layer.nextKeyframe;

                        if (layer.currentKeyframe > 0) {
                            layer.adjacentKeyframe = layer.nextKeyframe = layer.currentKeyframe - 1;
                        } else {
                            layer.nextKeyframe = layer.currentKeyframe;
                            layer.adjacentKeyframe = layer.loop == LOOP::NONE ? layer.currentKeyframe : total_keyframes - 1;
                        }

                    } else
                        ;
                }
            }
        }

        if (!layer.started)
            continue;


        //
        // Process the new keyframe
        //
        if (layer.currentKeyframe != layer.lastKeyframe) {
            auto &keyframe = layer.keyframes[layer.currentKeyframe];

            keyframe.startTimestamp = timestamp;


            using T = MAnimationKeyframe::Type;

            switch (keyframe.type) {
            case T::IMAGE:
                for (auto &image : keyframe.images) {
                    image.prim->setTexture(image.texture)->setTextureScale(scale_);
                    image.prim->setTiledHorz(keyframe.tiledHorz)->setTiledVert(keyframe.tiledVert);

                    if (image.orientationFlag)
                        image.prim->setOrientation(image.orientation);
                    else
                        image.prim->clearRotation();

                    if (keyframe.framesEffectFlag)
                        image.framesEffect->start();

                    if (keyframe.colorFlag)
                        image.prim->setColor(keyframe.color);
                    else
                        image.prim->clearColor();

                }
                keyframe.moveSprite(keyframe.computedFrame);
                break;

            case T::BOX:
                keyframe.prims[0]->setBox(keyframe.computedFrame);
                break;

            case T::RECTANGLE:
                keyframe.prims[0]->setRect(keyframe.computedFrame);

                if (auto &param = keyframe.params[PTAG::RADIUS]; param.setFlag)
                    keyframe.prims[0]->setRadius(param.computedFloatValue);
                break;

            case T::LINE:
                keyframe.prims[0]->setLine(keyframe.pts[0].computedPosition, keyframe.pts[1].computedPosition);
                if (!keyframe.images.empty())
                    keyframe.prims[0]->setRectangleTexture(keyframe.images[0].texture);
                break;

            case T::BEZIER:
                keyframe.prims[0]->setBezier(keyframe.pts[0].computedPosition, keyframe.pts[1].computedPosition, keyframe.pts[2].computedPosition, keyframe.pts[3].computedPosition);
                if (!keyframe.images.empty())
                    keyframe.prims[0]->setRectangleTexture(keyframe.images[0].texture);
                break;

            case T::CIRCLE:
                keyframe.prims[0]->initCircle(keyframe.params[PTAG::POSITION].computedPointValue, keyframe.params[PTAG::RADIUS].computedFloatValue);

                if (keyframe.fillColorFlag && keyframe.colorFlag) {
                    keyframe.prims[1]->initCircle(keyframe.params[PTAG::POSITION].computedPointValue, keyframe.params[PTAG::RADIUS].computedFloatValue);
                    keyframe.prims[1]->setSegments(M::Orientation90, M::Orientation180);
                }

                if (!keyframe.images.empty())
                    keyframe.prims[0]->setRectangleTexture(keyframe.images[0].texture);
                break;

            default:
                keyframe.moveSprite(keyframe.computedFrame);
                break;
            }


            keyframe.updateSpriteWidth(scale_);
            keyframe.setSpriteVisible(keyframe.visibleFlag);
            keyframe.updateSpriteColor();
            //keyframe.updateSpriteFillColor() ;


            keyframe.alphaFlag ? keyframe.setSpriteAlpha(keyframe.alphaValue) : keyframe.clearSpriteAlpha();
            //keyframe.skewFlag ? keyframe.setSpriteSkew(keyframe.skewValue) : keyframe.clearSpriteSkew() ;
            keyframe.rotationFlag ? keyframe.setSpriteRotation(keyframe.rotationValue) : keyframe.clearSpriteRotation();

            if (keyframe.soundId)
                Sfx::play(keyframe.soundId);

            if (!keyframe.id.empty())
                keyframesInfo_[keyframe.id] = keyframe.computedFrame;

            layer.lastKeyframe = layer.currentKeyframe;
        }


        if (MAnimationLayer::Transition::LINEAR == layer.transition && layer.currentKeyframe != layer.adjacentKeyframe) {
            auto &keyframe = layer.keyframes[layer.currentKeyframe];
            const auto &adjacentKeyframe = layer.keyframes[layer.adjacentKeyframe];

            Real adjacentKeyframeK = 0, adjacentKeyframeK_1 = 1;

            Timestamp timeRho, timeDelta;

            timeDelta = timestamp - keyframe.startTimestamp;//periodTimestamp - keyframe.time ;
            timeRho = std::abs(adjacentKeyframe.time - keyframe.time);

            /*
            if ( layer.keyframe_direction ) {
                timeRho = adjacentKeyframe.time - keyframe.time ;

                if ( timeRho <= 0 )
                    timeRho += layer.period ;
            } else {
                timeRho = keyframe.time - adjacentKeyframe.time ;
                //timeDelta = periodTimestamp - (timestamp - keyframe.startTimestamp) ;//(layer.period - keyframe.time) ;
                //timeDelta = 1. - (timestamp - keyframe.startTimestamp) ;

                if ( timeRho <= 0 ) {
                    timeRho += layer.period ;
                    //timeDelta += layer.period ;
                }
            }
            */

            adjacentKeyframeK = cognit::tiny(timeRho) ? 0 : Cord(timeDelta >= timeRho ? 1 : timeDelta / timeRho);

            adjacentKeyframeK_1 = 1. - adjacentKeyframeK;


            if (keyframe.computedFrame != adjacentKeyframe.computedFrame) {
                auto r = keyframe.computedFrame;
                r.mix(adjacentKeyframe.computedFrame, adjacentKeyframeK);
                keyframe.moveSprite(r);
            }


            if (adjacentKeyframe.colorFlag) {
                //debug_printf("k: %d -> %d: %f", layer.currentKeyframe, layer.adjacentKeyframe, adjacentKeyframeK) ;
                auto&& c = lerp(keyframe.color, adjacentKeyframe.color, adjacentKeyframeK);
                keyframe.setSpriteColor(c);
            }

            if (adjacentKeyframe.alphaFlag && keyframe.alphaValue != adjacentKeyframe.alphaValue) {
                auto alpha = keyframe.alphaValue * adjacentKeyframeK_1 + adjacentKeyframe.alphaValue * adjacentKeyframeK;
                keyframe.setSpriteAlpha(alpha);
            }

            for (auto &text: keyframe.texts) {
                if (keyframe.textHeightFlag && adjacentKeyframe.textHeightFlag)
                    text.widget->setTextHeight((keyframe.textHeight * adjacentKeyframeK_1 + adjacentKeyframe.textHeight * adjacentKeyframeK) * scale_);
            }


        }  // keyframes


    }  // for layers


}

}
