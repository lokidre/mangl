/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_common.h"
#include "../mangl_environment.h"

#include "../audio/mangl_sound.h"
#include "../layout/mangl_layout_parser.h"
#include "../mangl_scene.h"
#include "../widgets/mangl_label.h"

namespace mangl {

class FramesEffect;

struct MAnimationParameter {
    enum class ParameterType { FLOAT, Distance, ANGLE, Point, SIZE, RECT, TEXT, COLOR, };
    bool setFlag{};
    ParameterType type;

    Real floatValue;
    MLayoutCoordinate distanceValue;
    MLayoutPoint pointValue;
    Rect rectValue;
    Color colorValue;


    // calculated values
    bool computedFlag{};
    Real computedFloatValue{};
    Point computedPointValue{};

    MAnimationParameter(ParameterType _t = ParameterType::FLOAT): type{_t} {}

    void clear() noexcept {
        computedFlag = false;
    }
};



struct MAnimationKeyframe {

    String id;

    enum class Type { NONE, IMAGE, BOX, TEXT, RECTANGLE, LINE, CIRCLE, BEZIER, };

    using ParamType = MAnimationParameter::ParameterType;

    struct Image {
        String imageName;
        int imageIndex{};
        MLayoutPoint position{0, 0};
        bool orientationFlag = false;
        M::Orientation orientation{M::Orientation0};

        Texture texture;
        Prim *prim{};

        FramesEffect *framesEffect{};

        Point computedPosition;
    };


    //using Images = std::vector<Image> ;
    std::vector<Image> images;

    struct Text {
        String text;
        MLayoutPoint position{0, 0};
        Label *widget;

        Point computedPosition;
    };


    std::vector<Text> texts;
    String fontName;
    Cord textHeight{0};
    bool textHeightFlag{false};

    bool framesEffectFlag{false};
    Cord framesEffectDuration{};
    bool framesEffectRepeat{true};
    bool framesEffectReverse{false}, framesEffectBounce{false};

    //
    // Points data
    //
    struct LayoutPoint {
        MLayoutPoint position{};
        Point computedPosition{};
    };
    std::vector<LayoutPoint> pts;

    enum class ParameterTag { RADIUS, POSITION, };

    KeyValCont<ParameterTag, MAnimationParameter> params;


    Type type{};
    Timestamp time{};

    // Runtime information
    std::vector<PrimP> prims;

    void moveSprite(RectA r)
    {
        for (auto &s: prims)
            s->move(r);

        for (auto &image : images)
            if (image.prim) {
                image.prim->move({r.x+image.computedPosition.x, r.y + image.computedPosition.y, r.w, r.h})->setElSize(computedElSize);
                //image.prim->elSize_ = { 18, 18 } ; //computedElSize ;
                //image.prim->move({ r.x+image.computedPosition.x, r.y + image.computedPosition.y, computedElSize.w, computedElSize.h }) ;
            }

        for (auto &text: texts)
            text.widget->move({r.x+text.computedPosition.x, r.y + text.computedPosition.y, r.w, r.h});
    }

    void setSpriteAlpha(Cord alpha)
    {
        for (auto &s: prims)
            s->setAlpha(alpha);

        for (auto &image : images)
            if (image.prim)
                image.prim->setAlpha(alpha);

        for (auto &text : texts)
            text.widget->setAlpha(alpha);
    }

    void clearSpriteAlpha()
    {
        for (auto &s : prims)
            s->clearAlpha();

        for (auto &image : images)
            if (image.prim)
                image.prim->clearAlpha();

        for (auto &text : texts)
            text.widget->clearAlpha();
    }

    /*
    void setSpriteSkew ( const Cord skew )
    {
        for ( auto &s : prims )
            s->setSkew(skew) ;

        for ( auto &image : images )
            if ( image.prim )
                image.prim->setSkew(skew) ;

        //for ( auto &text : texts )
        //	text.widget->setAlpha(alpha) ;
    }

    void clearSpriteSkew()
    {
        for ( auto &s : prims )
            s->clearSkew() ;

        for ( auto &image : images )
            if ( image.prim )
                image.prim->clearSkew() ;
    }
    */

    void setSpriteColor(ColorA c) 
    {
        for (auto &s : prims)
            s->setColor(c);
        for (auto &image : images)
            if (image.prim)
                image.prim->setColor(c);
        for (auto &text : texts)
            text.widget->setColor(c);
    }

    void updateSpriteColor()
    {
        if (colorFlag) {
            if (fillColorFlag && colorFlag) {
                prims[1]->setColor(color);
            } else {
                for (auto &s : prims)
                    s->setColor(color);
            }

            for (auto &image : images)
                if (image.prim)
                    image.prim->setColor(color);
            for (auto &text : texts)
                text.widget->setColor(color);
        } else {
            for (auto &s : prims)
                s->clearColor();
            for (auto &image : images)
                if (image.prim)
                    image.prim->clearColor();
            for (auto &text : texts)
                text.widget->clearColor();
        }
    }

    /*
    void updateSpriteFillColor()
    {
        if ( fillColorFlag ) {

            if ( fillColorFlag && colorFlag ) {
                prims[0]->setFillColor(fillColor) ;
            } else {
                for ( auto &s : prims )
                    s->setFillColor(fillColor) ;
            }

            for ( auto &image : images )
                if ( image.prim )
                    image.prim->setFillColor(fillColor) ;
            for ( auto &text : texts )
                text.widget->setColor(fillColor) ;
        } else {
            for ( auto &s : prims )
                s->clearFillColor() ;
            for ( auto &image : images )
                if ( image.prim )
                    image.prim->clearFillColor() ;
        }
    }
    */


    void setSpriteVisible(bool vis) {
        for (auto &s: prims)
            if (s) s->setVisible(vis);

        for (auto &image : images)
            if (image.prim) image.prim->setVisible(vis);

        for (auto &text: texts)
            if (text.widget) text.widget->setVisible(vis);
    }

    void updateSpriteWidth(Cord scale) {
        for (auto &s: prims) {
            if (!s) continue;
            if (widthFlag)
                s->setWidth(widthValue*scale);
            else
                s->clearWidth();
        }
    }

    void setSpriteRotation(Real angle) {
        for (auto &s: prims)
            if (s) s->setRotation(angle);
        for (auto &image: images) {
            if (image.prim) image.prim->setRotation(angle);
        }
    }

    void clearSpriteRotation() {
        for (auto &s: prims)
            if (s) s->clearRotation();
        for (auto &image: images) {
            if (image.prim) image.prim->clearRotation();
        }
    }

    MLayoutPoint position{};
    MLayoutSize size{};
    MLayoutSize elSize{};
    int alignHorz{}, alignVert{};

    bool tiledHorz{}, tiledVert{};

    bool colorFlag{};
    Color color{};

    bool fillColorFlag{};
    Color fillColor{};

    bool rotationFlag{};
    Cord rotationValue{};

    bool alphaFlag{};
    Real alphaValue{};

    //bool skewFlag ;
    //Cord skewValue ;

    bool widthFlag{};
    Cord widthValue{};

    bool visibleFlag{};

    int soundId{};


    //
    // Runtime information
    //
    bool computedFlag{};
    Rect computedFrame{};
    Size computedElSize{};
    Timestamp startTimestamp{};

    MAnimationKeyframe();

    void clear()
    {
        images.clear();

        type = Type::NONE;
        prims.clear();
        alignHorz = alignVert = 0;
        size = {};
        colorFlag = false;
        color = {1,1,1,1};

        fillColorFlag = false;
        //skewFlag = false ;
        //skewValue = 1 ;

        soundId = 0;
        tiledVert = tiledHorz = false;
        time = 0;
        rotationFlag = false;
        rotationValue = 0;
        computedFlag = false;
        visibleFlag = true;
        alphaFlag = false;
        alphaValue = 1;

        widthFlag = false;
        widthValue = 0;

        for (auto &p : params) {
            p.second.clear();
        }
    }

    void setColor(ColorA c)
    {
        colorFlag = true;
        color = c;
    }

    void setFillColor(ColorA c) 
    {
        fillColorFlag = true;
        fillColor = c;
    }
};


struct MAnimationLayer {

    typedef std::vector<MAnimationKeyframe> keyframe_list_type;

    keyframe_list_type keyframes;

    String id;
    bool visible{};

    Timestamp start{}, length{}, period{}, framestep{};
    Timestamp estimatedLength{};

    enum class Loop { NONE, CYCLE, PINGPONG, } loop{Loop::NONE};

    enum class Transition { SQUARE, LINEAR } transition{Transition::LINEAR};


    // Runtime information
    MLayoutPoint origin{};

    bool started{}, finished{};
    int currentKeyframe{}, lastKeyframe{}, nextKeyframe{}, adjacentKeyframe{};
    Timestamp startTimestamp{}, layerRestartTimestamp{}, periodRestartTimestamp{};
    bool keyframe_direction{};

    void clear()
    {
        visible = true;
        finished = false;
        started = false;
        framestep = 0;
        length = 0;
        loop = Loop::NONE;
        transition = Transition::SQUARE;
        keyframe_direction = true;
        start = length = period = 0;
        origin.x = 0;
        origin.y = 0;
    }
};



class Animation: public MLayoutParser {
public:
    using Base = MLayoutParser;
    using Layer = MAnimationLayer;
    using Keyframe = MAnimationKeyframe;
    using ParamTag = MAnimationKeyframe::ParameterTag;

    int alignHorzDefault_{}, alignVertDefault_{};


    int key{};
    String id;
    Timestamp length{};
    Timestamp speedK{};
    bool repeat{};
    Point origin{};
    bool padded{true};  // whether the graphics is all padded

    enum class LayoutMatch {
        NONE, WIDTH, HEIGHT
    } layoutMatch_{LayoutMatch::NONE};


    std::vector<Layer> layers;

    std::map<String, Rect> keyframesInfo_;

    // Runtime information
    bool started{}, finished{};
    Timestamp startTimestamp_{}, restartTimestamp_{};


private:
    SceneP scene_{};
    PrimP prims{};
    int primsCount_{};

    int current_sound_id_{};

public:
    //
    // Initialization
    //
    //~Animation() {  release() ; }

    void preload(StrView filename);
    void load(StrView filename);
    void create(SceneP scene, const TextureAtlas *atlas);
    void destroy();
    //void release() ;


    void clear();

public:
    void start(Timestamp timestamp);
    void stop();
    bool active() const noexcept { return started && !finished; }

    void pause();
    void resume();

public:
    void onTimer(Timestamp timestamp);


private:

    enum { REF_TYPE_KEYFRAME = (int)MLayoutCoordinateValue::RefType::END_ };

    Cord onXVal(const MLayoutCoordinateValue &c) const override
    {
        using T = MLayoutCoordinateValue::Type;
        using R = MLayoutCoordinateValue::RefComponent;

        if (T::REF == c.type && REF_TYPE_KEYFRAME == (int)c.refType) {
            auto&& keyframeIt = keyframesInfo_.find(c.refName);
            if (keyframeIt == keyframesInfo_.end())
                throw std::runtime_error("keyframe not found: " + c.refName);
            auto&& keyframe = keyframeIt->second;
            if (R::WIDTH == c.refComponent) return keyframe.w;
            if (R::HEIGHT == c.refComponent) return keyframe.h;
            // It's important to subtract the viewport offset
            // in the future we will need to maintain "internal" position vs "real"
            if (R::X == c.refComponent) return keyframe.x - viewport_.x;
            if (R::Y == c.refComponent) return keyframe.y - viewport_.y;
            if (R::RIGHT == c.refComponent) return keyframe.x + keyframe.w  - viewport_.x;
            if (R::TOP == c.refComponent) return keyframe.y + keyframe.h  - viewport_.y;
        }
        return 0;
    }

    Cord onYVal(const MLayoutCoordinateValue &c) const override { return onXVal(c); }

    bool onRefParse(StrView tag, MLayoutCoordinateValue &c) const override
    {
        if ("keyframe" == tag) {
            c.type = MLayoutCoordinateValue::Type::REF;
            c.refType = (MLayoutCoordinateValue::RefType)REF_TYPE_KEYFRAME;
            return true;
        }

        return false;
    }


    using ParseLambda = void(StrView, Keyframe &);
    KeyValCont<StrView, std::function<ParseLambda>> kfEls_;
    bool wasImage_{}, wasText_{};


    bool parserLoaded_{};
    void loadParser();

    //
    // Debugging
    //
public:
    void setDebugVerbose(bool f = true) { debugVerbose_ = f; }
private:
    bool debugVerbose_ = false;


};


}  // namespace mangl

