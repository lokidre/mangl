/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"

#include <vector>
#include <utility>
#include <random>


#include "../mangl_texture_atlas.h"
#include "../mangl_scene.h"
#include "../effects/mangl_effect.h"
#include "../layout/mangl_layout_parser.h"

#include <apeal/random.hpp>

namespace mangl {

struct MBackgroundElement {
    Prim *so;

    Cord depth;

    //size_t spriteIndex ;

    bool valid;
    int layerIndex;

    Point velocity;
    bool static_flag;


    // texture animation
    int texture_current;
    Texture texture;
    Cord texture_frame, texture_timestamp;

    // alpha duplicate for faster processing
    bool alpha_flick;
    Cord alpha_period;
    enum { BK_ELEMENT_FLICKS = 6 };
    Cord alpha_values[BK_ELEMENT_FLICKS];
    int alpha_current_index;
    Cord alpha_flick_timestamp;

    void clear() {
        valid = false;
        depth = 0;
        layerIndex = 0;
        texture_current = 0;
        texture.tid = 0;
        alpha_flick = false;
        alpha_current_index = 0;
        velocity.x = velocity.y = 0;
        static_flag = false;
    }
};


struct MBackgroundTexture {
    Texture texture{};
    int count{};
    Cord frame_time{};

    Cord frame_timestamp{};
    int current_index{};
};

struct MBackgroundParameter {
    bool set{};
    Cord value{};
    Cord minValue{}, maxValue{};
    bool isRandom{};
    bool randomUniform{};
    std::pair<Cord, Cord> randomRange{};
    int alongDirection{};

    void clear() {
        set = false;
        alongDirection = 0;
    }
};

struct MBackgroundParameterPoint {
    MBackgroundParameter x, y;
    void clear() { x.clear(), y.clear(); }
};

struct MBackgroundParameterRange {
    MBackgroundParameter mn, mx;
    void clear() { mn.clear(), mx.clear(); }
};

struct MBackgroundParameterVectorRange {
    MBackgroundParameterPoint mn, mx;
    void clear() { mn.clear(), mx.clear(); }
};


struct MBackgroundParameterAlpha: public MBackgroundParameter {
    bool flick{};
    Timestamp frequency{}, period{};
};


struct MBackgroundLayer {

    typedef std::vector<MBackgroundTexture> texture_list_type;

    texture_list_type textures;

    bool valid;
    int index;

    MBackgroundParameter period;
    MBackgroundParameter speed;

    Size elSize{};
    Cord elScale{};

    bool static_flag;

    Cord startXPercent;

    MBackgroundParameter baseline;
    MBackgroundParameterPoint position, direction;

    bool single;  // the element will appear only once

    bool foreground;

    int alignHorz, alignVert;

    MBackgroundParameterAlpha alpha;

    bool flip_horz;
    bool tile;
    bool full_screen;

    bool pattern;
    MBackgroundParameter patternHorzOverlap, patternVertOverlap;


    //
    // Parallax
    // 
    MBackgroundParameter parallaxRange, parallaxAlpha, parallaxSize, parallaxSpeed, parallaxFog, parallaxPeriod;
    Color parallaxFogColor;

    // Run-time
    size_t elementIndexBegin, elementIndexEnd;
    size_t currentElementIndex{};
    std::pair<Cord, Cord> currentExtent{};
    Timestamp appear_timestamp;

    MBackgroundLayer()
    {
        clear();
    }

    void clear();
};



class MBackground: public MLayoutParser {
private:
    using Base = MLayoutParser;

    enum ParseParam {

        PARAM_NONE = 0,
        PARAM_ALONG = 0x00001,
        PARAM_CROSS = 0x00002,
        PARAM_DIAGONAL = 0x00004,
        PARAM_SCALE = 0x00008,
    };

    //
    // Setup
    //
public:
    bool strictParse = true;

    void preload(StrView filename);
    void load(StrView filename);
    void clear();

    void setSprites(Prim *so, int count);
    void setLayout(RectA r, Real scale = 1 ) { viewport_ = r, scale_ = scale, diagonal_ = hypot(viewport_.w, viewport_.h); }
    void setTexture(const TextureAtlas &textures) { texture_id_ = textures.textureId; textures_ = &textures; }


public:
    Real speed_k{1};
    ParseParam speedUnits_ = PARAM_SCALE;
    String texture, textureDir;

private:
    // textures
    size_t texture_id_ = 0;
    TextureAtlas const *textures_ = 0;

    // layout
    Rect viewport_;
    Real scale_{1};
    Real diagonal_{};

public:
    void onStart(Timestamp timestamp);
    void onTimer(Timestamp timestamp, Timestamp lastTimestamp);

    void onPause() { paused_ = true; }
    void onResume() { paused_ = false; }


private:
    bool started_ = false;

    bool backgroundColorFlag_{};
    Color backgroundColor_{};

    //bool backgroundGradientFlag_{} ;
    struct GradientInfo {
        Cord position;
        Color color;
    };
    bool gradientVertical_;
    std::vector<GradientInfo> gradients_;

    Point direction_;

    typedef std::vector<MBackgroundElement> element_list_type;
    element_list_type elements_;

    Timestamp backgroundProcessTimestamp_{};

    PrimP prims;

    std::mt19937 mt_;
    std::uniform_real_distribution<Real> uniform_{0,1};
    std::uniform_int_distribution<int> uniformInt_;

    auto random(Real range) {
        return uniform_(mt_) * range;
    }
    auto random(Real rmin, Real rmax) {
        return uniform_(mt_) * (rmax-rmin) + rmin;
    }
    int irandom(int range) {
        return uniformInt_(mt_) % range;
    }


    typedef std::vector<MBackgroundLayer> layer_list_type;

    layer_list_type layers_;

    void process_appear();
    Rect appear_element(MBackgroundLayer &layer, Point position, Real advance);

    element_list_type::iterator select_element(MBackgroundLayer &layer);
    element_list_type::iterator init_element(MBackgroundLayer &layer);


    bool paused_ = false;

    Real variation(Real value);


private:
    Timestamp current_timestamp_{}, last_timestamp_{};

    Real computeValueComponent(const MBackgroundParameter &param, Real val);
    Real value(const MBackgroundParameter &param);
    Real skewedValueExp(const MBackgroundParameter &param, Real coeff);
    Real minValue(const MBackgroundParameter &param);
    Real maxValue(const MBackgroundParameter &param);
    bool isNull(const MBackgroundParameter &param) { return !param.set || fabs(maxValue(param)) < 0.000001; }


private: // Parsing
    void parse_point(StrView value, Point &p);


    void parseParam(StrView value, MBackgroundParameter &param, const ParseParam &opts);
    void parseParamPoint(StrView value, MBackgroundParameterPoint &point);
    void parseParamRange(StrView value, MBackgroundParameterRange &range, const ParseParam &opts);
    void parseParamAlpha(StrView value, MBackgroundParameterAlpha &param);

    void parseHorzAlign(StrView value, int &align);
    void parseVertAlign(StrView value, int &align);
    void parseAlign(StrView value, int &halign, int &valign);

};

}  // namespace mangl
