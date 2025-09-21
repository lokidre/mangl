/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_background.h"

#include <apeal/parse.hpp>
#include <apeal/file.hpp>
#include <apeal/parse/geo.hpp>

namespace mangl {


//
// Parsing
//


void MBackground::preload (StrView filename )
{
	using namespace apl ;

	setupParameters() ;


	AIniParser parser ;
	AIni level_cfg ;
	parser.load(filename,level_cfg) ;

	for ( auto sec : level_cfg.sec ) {
		if ( "BACKGROUND" == sec.name ) {
			for ( auto el : sec.el ) {
				if ( "texture" == el.name )	texture = el.value ; else
				if ( "texture_dir" == el.name || "texture-dir" == el.name ) textureDir = el.value ; else
				if ( parseVar(name_,value_) ) ; else
					;
			}
		}
	}

}

void MBackground::load (StrView filename )
{
	using namespace apl ;

	auto ini = loadIni ( filename ) ;

	clear() ;
	setupParameters() ;

	String val1, val2 ;


	for ( auto&& sec: ini.sec ) {
		if ( !processCondition ( sec.name, sec_name_ ) )
			continue ;

		//
		// Background
		//
		if ( "BACKGROUND" == sec.name ) {
			for ( auto &el : sec.el ) {
				if ( el.name.empty() )
					continue ;

				line_ = el.line ;

				processElement(el) ;
				if ( !processCondition ( el.name, name_ ) )
					continue ;
				processExpression ( el.value, value_ ) ;

				if ( "color" == name_ ) backgroundColor_ = parseColor(value_), backgroundColorFlag_ = true ; else
				if ( "gradient" == name_ ) {
					auto v = parse_std_vector_trim ( value_, ',' ) ;
					if ( v.size() < 3 )
						raiseError ( "Invalid gradient parameter: %s", value_ ) ;

					auto vIt = v.begin(), vEnd = v.end() ;

					if ( "vertical" == v[0] || "v" == v[0] || "vert" == v[0] ) gradientVertical_ = true ; else
					if ( "horizontal" == v[0] || "h" == v[0] || "horz" == v[0] ) gradientVertical_ = false ; else
						raiseError ( "Invalid gradient orientation: %s", v[0] ) ;
					++vIt ;

					for ( ; vIt != vEnd ; ++vIt ) {
						auto [as,bs] = str_split_trim ( *vIt, ':' ) ;

						GradientInfo gi ;
						gi.position = M::parseCoord(as) ;
						gi.color = parseColor ( bs) ;

						gradients_.emplace_back(gi) ;
					}

				} else
				if ( "direction" == name_ ) parse_point(value_,direction_) ; else 
				if ( "speed-units" == name_ ) { 
					if ( "diagonal" == value_ ) speedUnits_ = PARAM_DIAGONAL ; else
					if ( "scale" == value_ ) speedUnits_ = PARAM_SCALE ; else
						raiseError ( "Invalid speed unit: %s", value_ ) ;
				} else 
				if ( "texture" == name_ ) texture = value_ ; else 
				if ( "texture_dir" == name_ || "texture-dir" == name_ ) textureDir = value_ ; else
				if ( parseVar(name_,value_) ) ; else
				if ( strictParse && !name_.empty() )
					raiseError ( "Invalid parameter: %s in %s", name_, sec.name ) ;
			}
		}

		if ( "LAYER" == sec.name || "BACKGROUND_LAYER" == sec.name || "FOREGROUND_LAYER" == sec.name ) {
			MBackgroundLayer layer ;

			for ( auto &el : sec.el ) {
				if ( el.name.empty() )
					continue ;

				line_ = el.line ;

				processElement(el) ;
				if ( !processCondition ( el.name, name_ ) )
					continue ;
				processExpression ( el.value, value_ ) ;

				if ( "textures" == name_ || "image" == name_ ) {

					auto textureNames = parse_std_vector_trim ( value_, ',' ) ;

					for ( auto &s: textureNames ) {
						auto textureComponents = parse_std_vector_trim ( s, ':' ) ;

						auto [textureName, indexStringComponent] = str_split ( textureComponents[0], '[' ) ;

						const auto &tex = textures_->texture(textureName) ;
						int textureBeginIndex = 0, textureEndIndex = tex.count ;

						if ( !indexStringComponent.empty() ) {
							auto is = str_split ( indexStringComponent, ']' ).first ;
							textureBeginIndex = parseInt(is) ;
							textureEndIndex = textureBeginIndex + 1 ;
						}

						if ( textureComponents.size() >= 2 ) {
							layer.textures.resize(layer.textures.size()+1) ;
							auto &t = layer.textures.back() ;

							auto period = parseFloat ( textureComponents[1] ) ;

							if ( tex.count != 0 )
								t.frame_time = period / tex.count ;

							t.texture = tex ;
							t.count = tex.count ;
							t.frame_timestamp = current_timestamp_ ;
							t.current_index = 0 ;

						} else {

							// If the frequency wasn't provided, then treat the elements as different textures
							for ( int textureIndex = textureBeginIndex ; textureIndex < textureEndIndex ; ++textureIndex ) {
								layer.textures.resize(layer.textures.size()+1) ;
								auto &t = layer.textures.back() ;

								t.count = 1 ;
								t.texture = tex.contentsFrame(textureIndex) ;
							}
						}
					}
				} else

				if ( "elsize" == name_ ) parse(value_, &layer.elSize); else
				if ( "speed" == name_ ) parseParam(value_, layer.speed, speedUnits_) ; else 
				if ( "period" == name_ )  parseParam(value_, layer.period, PARAM_NONE) ;  else
				if ( "baseline" == name_ )  parseParam(value_, layer.baseline, PARAM_CROSS) ;  else
				if ( "direction" == name_ ) parseParamPoint(value_, layer.direction); else
				if ( "position" == name_ ) parseParamPoint(value_, layer.position); else
				if ( "offset" == name_ || "start_x" == name_ )  parse_real(value_, layer.startXPercent) ;  else
				if ( "single" == name_ )  parse(value_, &layer.single) ;  else
				if ( "alpha" == name_ )  parseParamAlpha(value_,layer.alpha) ; else
				if ( "flip_horz" == name_ ) parse(value_, &layer.flip_horz) ;  else
				if ( "tile" == name_ ) parse(value_, &layer.tile) ; else
				if ( "pattern" == name_ ) {
                    layer.pattern = true;
                    auto v = parse_std_vector_trim(value_, ',');
                    for (auto&& e : v) {
                        auto [n, x] = str_split_trim(e, ':');
                        if ("overlap" == n || "horz-overlap" == n) {
                            if (x.empty())
                                raiseError("Empty overlap value");
                            if (x.back() == '%')
                                x.remove_suffix(1); //erase ( x.length()-1, 1 ) ;
                            parseParam(x, layer.patternHorzOverlap, PARAM_NONE);
                        } else if ("vert-overlap" == n) {
                            if (x.empty())
                                raiseError("Empty overlap value");
                            if (x.back() == '%')
                                x.remove_suffix(1); //erase ( x.length()-1, 1 ) ;
                            parseParam(x, layer.patternVertOverlap, PARAM_NONE);
                        }
                    }
				} else
				if ( "full-screen" == name_ ) parse(value_, &layer.full_screen) ; else
				if ( "static" == name_ ) layer.speed.clear(), layer.static_flag = true ; else
				if ( "align" == name_ ) parseAlign ( value_, layer.alignHorz, layer.alignVert ) ; else
				if ( "align-horz" == name_ || "align_horz" == name_ ) parseHorzAlign ( value_, layer.alignHorz ) ; else
				if ( "align-vert" == name_ || "align_vert" == name_ ) parseVertAlign ( value_, layer.alignVert ) ; else

				// parallax effects
				if ( "parallax-range" == name_ ) parseParam ( value_, layer.parallaxRange, PARAM_NONE ) ;  else 
				if ( "parallax-alpha" == name_ ) parseParam ( value_, layer.parallaxAlpha, PARAM_NONE ) ;  else 
				if ( "parallax-size" == name_ ) parseParam ( value_, layer.parallaxSize, PARAM_NONE ) ;  else 
				if ( "parallax-speed" == name_ ) parseParam ( value_, layer.parallaxSpeed, PARAM_NONE ) ;  else 
				if ( "parallax-fog" == name_ ) parseParam ( value_, layer.parallaxFog, PARAM_NONE ) ;  else 
				if ( "parallax-fog-color" == name_ ) layer.parallaxFogColor = parseColor ( value_ ) ;  else 
				if ( "parallax-period-k" == name_ ) parseParam ( value_, layer.parallaxPeriod, PARAM_NONE ) ;  else 

				if ( strictParse && !name_.empty() )
					raiseError ( "Invalid parameter: %s in %s", name_, sec.name ) ;
			}

			layer.index = (int)layers_.size() ;
			layers_.push_back(layer) ;
		}
	}

}


void MBackground::parse_point (StrView value, Point &p )
{
	using namespace apl ;
	String x, y ;

	str_split(value,',',x,y) ; 
	parse_real (x, p.x) ;
	parse_real (y, p.y) ;
} 

void MBackground::parseParam (StrView value, MBackgroundParameter &param, const ParseParam &opts )
{
	using namespace apl ;

	param.set = true ;

	if ( opts & PARAM_ALONG )	param.alongDirection = PARAM_ALONG ;
	if ( opts & PARAM_CROSS )	param.alongDirection = PARAM_CROSS ;
	if ( opts & PARAM_DIAGONAL ) param.alongDirection = PARAM_DIAGONAL ;

    //MString valueString, paramString;
	//t, sigmaString;
	//MStringView sigmaString;
    auto [valueString, paramString] = str_split(value, ':'); //, valueString, paramString ) ;

	if ( !paramString.empty() ) {
		auto [t, sigmaString] = str_split ( paramString, '~') ;
		paramString = t ;
	} else {
		auto [a, b] = str_split ( value, '~'); //, valueString, sigmaString ) ;
		valueString = a;
		//sigmaString = b;
	}

	if ( !parse_real(valueString, param.value) )
		raiseError ( "Invalid value: %s", valueString ) ;


	if ( !paramString.empty() ) {
		param.isRandom = true ;
		if ( !parse_real(paramString, param.maxValue) )
			raiseError ( "Invalid value: %s", paramString ) ;
		param.minValue = param.value ;
		param.isRandom = true ;
		param.randomUniform = true ;
	}

}

void MBackground::parseParamAlpha (StrView value, MBackgroundParameterAlpha &param )
{
    using namespace apl;

    auto [paramString, freqString] = str_split_trim(value, '@');

    parseParam(paramString, param, PARAM_NONE);

    if (!freqString.empty()) {
        if (!parse_real(freqString, param.frequency))
            raiseError("Invalid value: %s", freqString);

        param.flick = true;
        if (param.frequency != 0)
            param.period = 1. / param.frequency;
    } else {
        param.flick = false;
    }

}


void MBackground::parseParamPoint (StrView value, MBackgroundParameterPoint &point )
{
	using namespace apl ;

	auto [xs,ys] = str_split_trim ( value, ',' ) ;

	parseParam ( xs, point.x, PARAM_ALONG ) ;
	parseParam ( ys, point.y, PARAM_CROSS ) ;
}

void MBackground::parseParamRange (StrView value, MBackgroundParameterRange &range, const ParseParam &opts )
{
	using namespace apl ;

	auto [xs,ys] = str_split_trim ( value, ',' ) ;

	parseParam ( xs, range.mn, opts ) ;
	parseParam ( ys, range.mx, opts ) ;
}


void MBackground::parseHorzAlign (StrView value, int &alignHorz )
{
	auto s = apl::str_trim(value) ;

	if ( "left" == s || "LEFT" == s ) alignHorz = M::AlignLeft ; else
	if ( "center" == s || "CENTER" == s ) alignHorz = M::AlignCenter ; else
	if ( "right" == s || "RIGHT" == s ) alignHorz = M::AlignRight ; else
		raiseError ( "Invalid horizontal align value: %s", s ) ;
}

void MBackground::parseVertAlign (StrView value, int &alignVert )
{
	auto s = apl::str_trim ( value ) ;

	if ( "bottom" == s || "BOTTOM" == s ) alignVert = M::AlignBottom ; else
	if ( "center" == s || "middle" == s || "CENTER" == s || "MIDDLE" == s ) alignVert = M::AlignVCenter ; else
	if ( "top" == s || "TOP" == s ) alignVert = M::AlignTop ; else
		raiseError ( "Invalid vertical align value: %s", s ) ;
}

void MBackground::parseAlign ( StrView value, int &halign, int &valign )
{
	auto [hs,vs] = apl::str_split_trim ( value, ',' ) ;
	parseHorzAlign ( hs, halign ) ;
	parseVertAlign ( vs, valign ) ;
}

}  // namespace mangl

