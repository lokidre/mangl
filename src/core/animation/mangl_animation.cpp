/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_animation.h"

#include "../audio/mangl_sound.h"

#include "../effects/mangl_frames_effect.h"


#include "../mangl_debug.h"
#include "../mangl_error.h"

namespace mangl {


MAnimationKeyframe::MAnimationKeyframe()
{
    using enum ParameterTag;
    params[RADIUS] = MAnimationParameter(ParamType::Distance);
    params[POSITION] = MAnimationParameter(ParamType::Point);
}

void Animation::create(Scene *scene, const TextureAtlas *atls)
{
    scene_ = scene;
    primsCount_ = 0;
    setAtlas(atls);

    for (auto &layer: layers) {
        for (auto &keyframe: layer.keyframes) {

            for (auto &image: keyframe.images) {
                image.texture = atlas_->get(image.imageName, image.imageIndex);
            }

            if (keyframe.type == MAnimationKeyframe::Type::IMAGE) {
                for (auto &image : keyframe.images) {
                    image.prim = scene->newPrim();

                    if (keyframe.framesEffectFlag) {
                        image.framesEffect = scene->newEffect<FramesEffect>()
                            ->setTextures(image.texture)
                            ->setDuration(keyframe.framesEffectDuration)
                            ->setRepeat(keyframe.framesEffectRepeat)
                            ->setReverse(keyframe.framesEffectReverse)
                            ->setBounce(keyframe.framesEffectBounce)
                            ->attach(image.prim);
                    }
                }
            } else if (!keyframe.texts.empty()) {
                // search for the font
                auto&& fonts = atlas_->getFonts();
                massert(!fonts.empty(), "Fonts not found in texture: %s", atlas_->atlasFileBaseName());

                //if (keyframe.fontName.empty())
                //    keyframe.fontName = fonts.begin()->first;
                auto&& font = keyframe.fontName.empty() ? fonts.begin()->second : atlas_->getFont(keyframe.fontName);

                for (auto&& text: keyframe.texts) {
                    text.widget = scene->newWidget<Label>()->setFont(&font)->setText(text.text)->create();
                }
            } else {
                keyframe.prims.push_back(scene->newPrim());

                if (keyframe.fillColorFlag && keyframe.colorFlag)
                    keyframe.prims.push_back(scene->newPrim());
            }
        }
    }

}


void Animation::destroy()
{
    clear();
    if (scene_ && prims)
        scene_->deletePrim(prims);
}

void Animation::clear()
{
    for (auto &layer: layers) {
        for (auto &keyframe: layer.keyframes) {
            if (keyframe.soundId)
                Sfx::release(keyframe.soundId);

            for (auto&& img: keyframe.images) {
                if (img.prim) {
                    img.prim->setVisible(false);
                    scene_->deletePrim(img.prim);
                    img.prim = 0;
                }
            }
            for (auto&& text: keyframe.texts) {
                if (text.widget) {
                    text.widget->setVisible(false);
                    scene_->deleteWidget(text.widget);
                    text.widget = 0;
                }
            }

            for (auto& prim: keyframe.prims) {
                if (prim) {
                    prim->setVisible(false);
                    scene_->deletePrim(prim);
                    prim = nullptr;
                }
            }

            keyframe.clear();
        }
        layer.clear();
    }
    layers.clear();

    speedK = 1;
    repeat = false;
    started = false;
    origin = {};
    length = 0;
    variables.clear();
    layoutMatch_ = LayoutMatch::WIDTH;

    atlasFile.clear();
    textureDir.clear();


    Base::clear();
}

}  // namespace mangl

