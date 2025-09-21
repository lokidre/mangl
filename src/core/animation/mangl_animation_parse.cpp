/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_animation.h"

#include "../mangl_environment.h"
#include "../mangl_resources.h"

#include "../audio/mangl_sound.h"

#include "../mangl_prim.h"

#include <apeal/time.hpp>
#include <apeal/parse.hpp>
#include <apeal/parse/geo.hpp>
#include <apeal/file.hpp>

namespace mangl {


void Animation::preload(StrView filename)
{
    using namespace apl;

    filename_ = filename;

    IniParser parser;
    Ini ini;
    Preprocessor preprocessor;
    auto filedir = extractDirName(filename);

    preprocessor.search_dirs.push_back(String{filedir});

    // Android bundle access
    //if ( filename[0] == ':' ) {
    //}


    parser.load(preprocessor, filename, ini);

    String serr, sfile;

    clear();
    setupParameters();


    loadParser();

    String nameBase, nameExt;

    for (auto& sec: ini.sec) {
        if (!processCondition(sec.name, sec_name_))
            continue;
        if ("ANIMATION" == sec.name) {
            for (auto& el: sec.el) {
                line_ = el.line;

                processElement(el);
                if (!processCondition(el.name, name_))
                    continue;
                processExpression(el.value, value_);

                if ("version" == name_) processVersion(value_);
                else if ("texture" == name_)	atlasFile = value_;
                else if ("texture_dir" == name_ || "texture-dir" == name_) textureDir = value_;
                else if ("upscale" == name_) {
                    //Cord val = 1 ;
                    auto [stype, sval] = str_split(value_, ',');
                    auto val = parseReal<Real>(sval);
                    if ("textures" == stype || "images" == stype)
                        scaleTextures_ = val;
                } else if ("screenLayout" == name_) {
                    Size size{};
                    parse(value_, &size);
                    layout.resize(size);
                    layoutFlag = true;
                }
                else if (parseVar(name_, value_)); else
                    ;
            }
        }
    }
}

void Animation::load(StrView filename)
{
    using namespace apl;

    filename_ = filename;

    current_sound_id_ = Sfx::maxId();
    if (current_sound_id_ == 0)
        current_sound_id_ = 1;

    //clear() ;

    loadParser();
    setupParameters();

    String serr;
    auto sfile = extractFileName(filename);
    auto filedir = extractDirName(filename);

    //debug_printf ( "animation dir: %s", filedir ) ;

    IniParser parser;
    Ini cfg;
    Preprocessor preprocessor;
    preprocessor.search_dirs.push_back(String{filedir});
    parser.load(preprocessor, filename, cfg);

    deps_ = preprocessor.deps;

    bool layer_skipped = false;

    for (auto&& sec: cfg.sec) {
        if (!processCondition(sec.name, sec_name_)) {
            if ("LAYER" == sec_name_)
                layer_skipped = true;
            continue;
        }

        if ("ANIMATION" == sec_name_) {

            String name_1, name_2;

            for (auto&& el : sec.el) {
                line_ = el.line;

                processElement(el);
                if (!processCondition(el.name, name_))
                    continue;
                processExpression(el.value, value_);

				if ( "id" == name_ )  id = value_ ;  else
				if ( "version" == name_ )  processVersion(value_) ;  else
				if ( "texture_dir" == name_ || "texture-dir" == name_ )  textureDir = value_ ;  else
				if ( "texture" == name_ )	atlasFile = value_ ;  else
				if ( "upscale" == name_ ) { 
					//Cord val = 1 ;
					auto [stype, sval] = str_split(value_, ',');
					//parse_real(sval, val) ;
					if ( "texture" == stype || "textures" == stype || "images" == stype || "image" == stype ) 
						scaleTextures_ = M::parseCoord(sval);
				} else
				if ( "length" == name_ || "time" == name_ )  length = parseFloat(value_) * speedK ;  else
				if ( "speed_k" == name_ || "speed-k" == name_ )  speedK = parseFloat(value_) ;  else
				if ( "repeat" == name_ )  repeat = parseBool ( value_ ) ;  else
				if ( "padded" == name_ )  padded = parseBool ( value_ ) ;  else
				if ( "screenLayout" == name_ ) {
				    Size size{};
				    parse(value_, &size);
					layout.resize(size);
					if ( layout.w == 0 )
						layout.w = viewport_.w / scale_ ;
					if ( layout.h == 0 )
						layout.h = viewport_.h / scale_ ;
					layoutFlag = true ; 
				} else
				if (parseVar(name_,value_)){}
                else
				if ( !value_.empty() )
					raiseError ( "Invalid Parameter: '%s'", name_ ) ;
			}

		} else if ( "LAYER" == sec_name_ ) {
            layer_skipped = false;

            auto& layer = layers.emplace_back();

            layer.clear();

            for (auto&& el: sec.el) {
                line_ = el.line;

                processElement(el);
                if (!processCondition(el.name, name_))
                    continue;
                processExpression(el.value, value_);

				if ( "id" == name_ ) layer.id = value_ ; 
                else if ( "visible" == name_ ) layer.visible = parseBool(value_) ; 
                else if ( "start" == name_ )  layer.start = M::parseCoord(value_) * speedK ; 
                else if ( "length" == name_ || "time" == name_ )  layer.length = M::parseCoord(value_) * speedK ;  
                else if ( "period" == name_ ) layer.period = M::parseCoord(value_) * speedK ; 
                else if ( "framestep" == name_ ) layer.framestep = M::parseCoord(value_) * speedK ; 
                else if ( "loop" == name_ ) {
					if ( "none" == value_ ) layer.loop = MAnimationLayer::Loop::NONE ;  
                    else if ( "cycle" == value_ ) layer.loop = MAnimationLayer::Loop::CYCLE ;  
                    else if ( "pingpong" == value_ ) layer.loop = MAnimationLayer::Loop::PINGPONG ;  
                    else throwError ( "%s:%d: Invalid loop value '%s'", sfile, el.line, value_) ;
				} else if ( "origin" == name_ ) layer.origin = parseLayoutPoint(value_) ;  
                else if ( "transition" == name_ ) {
					if ( "square" == value_ ) layer.transition = MAnimationLayer::Transition::SQUARE ;  
                    else if ( "linear" == value_ ) layer.transition = MAnimationLayer::Transition::LINEAR ;  
                    else mthrowError("%s:%d: Invalid transition value '%s'", sfile, el.line, value_) ;
				} else if ( "clone" == name_ ) {  // copy the layer
                    auto copyId = value_;
                    bool found = false;
                    for (auto& l: layers) {
                        if (l.id == copyId) {
                            layer.origin = l.origin;
                            layer.transition = l.transition;
                            layer.loop = l.loop;
                            layer.framestep = l.framestep;
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                        throw std::runtime_error(str_format(serr, "%s : %d : Cannot find layer '%s'", sfile, el.line, value_));
				} else if ( !name_.empty() )
					raiseError ( "Invalid Parameter: '%s'", name_ ) ;

			}
		} else if ( "KEYFRAME" == sec_name_ ) {
            if (layer_skipped)
                continue;

            auto& layer = layers.back();

            auto& keyframe = layer.keyframes.emplace_back();

            keyframe.clear();

            // initialize the keyframe from previous keyframe
            if (layer.keyframes.size() > 1) {
                const auto& pk = layer.keyframes[layer.keyframes.size()-2];
                keyframe = pk;

                // except timing
                keyframe.time = 0;
            }

            wasImage_ = false, wasText_ = false;

            for (auto& el : sec.el) {
                line_ = el.line;

                processElement(el);
                if (!processCondition(el.name, name_))
                    continue;
                processExpression(el.value, value_);

                if (name_.empty())
                    continue;

                auto parseIt = kfEls_.find(name_);
                if (parseIt == kfEls_.end())
                    raiseError("Invalid Parameter: '%s'", name_);

                parseIt->second(value_, keyframe);
            }

            // Keyframe post processing

            if (keyframe.type == MAnimationKeyframe::Type::NONE) {
                keyframe.type = MAnimationKeyframe::Type::BOX;
            }

        } else
            if (!sec_name_.empty())
                raiseError("Invalid Section: '%s'", sec_name_);
	}



    //
    // Post processing
    //
    started = false;
    finished = true;

    //
    // Calculate the times
    //
    for (auto& layer: layers) {

        // Handle the keyframe steps
        if (layer.framestep) {
            Timestamp currentTime = 0;
            for (auto& kf: layer.keyframes) {
                if (kf.time == 0)
                    kf.time = currentTime;
                else
                    currentTime = kf.time;
                currentTime += layer.framestep;
            }
        }

        // Another maintenance about keyframe times
        // Eliminate zeros except for the first
        Timestamp currentTime{};
        for (auto& kf: layer.keyframes) {
            if (kf.time == 0)
                kf.time = currentTime;
            else
                currentTime = kf.time;
        }

        // Handle total layer length
        layer.estimatedLength = 0;
        for (auto& kf : layer.keyframes) {
            if (layer.estimatedLength < kf.time)
                layer.estimatedLength = kf.time;
        }

        if (layer.loop == MAnimationLayer::Loop::NONE) {
            if (layer.length == 0)
                if (length != 0)
                    layer.length = length - layer.start;

        } else {  // if the layer is looped

            if (layer.keyframes.size() >= 2 && layer.loop == MAnimationLayer::Loop::PINGPONG)
                layer.estimatedLength += layer.keyframes[1].time - layer.keyframes[0].time;

            if (layer.estimatedLength > layer.length)
                layer.length = layer.estimatedLength;

            if (layer.period > layer.length)
                layer.length = layer.period;

            if (layer.length > layer.period)
                layer.period = layer.length;
        }

        //if ( layer.period == 0 && layer.loop != layer.LOOP_NONE )
        //	layer.period = layer.estimatedLength ;
    }

    // Handle the total animation length if not provided
    if (0 == length) {
        // By default select the largest period
        for (auto& layer : layers)
            if (layer.period > length)
                length = std::max(length, layer.length);

        // In the end default to 1 second
        if (0 == length)
            length = 1.;

        // Check the layers one more time
        for (auto& layer: layers) {
            if (layer.length == 0)
                layer.length = length - layer.start;
        }
    }

    // Now assume every layer without given provided length be of the total length
    for (auto& layer : layers) {
        if (layer.length == 0)
            layer.length = length - layer.start;
        if (layer.period == 0)
            layer.period = layer.length;
    }

}


void Animation::loadParser()
{
	using namespace apl ;
	
	using S = StrView;
	using K = Keyframe ;
	using T = MAnimationKeyframe::Type ;

	if ( parserLoaded_ )
		return  ;
	parserLoaded_ = true ;

	kfEls_["id"] = [](auto v, K &keyframe){  keyframe.id = v;  } ;


	kfEls_["clone"] = [this](auto v, K &keyframe) {
		auto found = false ;
		for ( auto &l: layers ) {
			for ( auto &kf: l.keyframes ) {
				if ( kf.id == v) {
					keyframe.time = kf.time ;
					if ( kf.alphaFlag ) {
						keyframe.alphaFlag = kf.alphaFlag ;
						keyframe.alphaValue = kf.alphaValue ;
					}
					if ( kf.colorFlag )
						keyframe.setColor(kf.color) ;
					if ( kf.fillColorFlag )
						keyframe.setFillColor(kf.fillColor) ;
					keyframe.alignVert = kf.alignVert ;
					keyframe.alignHorz = kf.alignHorz ;
					keyframe.tiledVert = kf.tiledVert ;
					keyframe.tiledHorz = kf.tiledHorz ;
					keyframe.fontName = kf.fontName ;
					if ( kf.textHeightFlag ) {
						keyframe.textHeight = kf.textHeight ;
						keyframe.textHeightFlag = kf.textHeightFlag ;
					}
					keyframe.position = kf.position ;

					found = true ;
					break ;
				}
			}
			if ( found )
				break ;
		}
		if ( !found )
			raiseError ( "Keyframe not found: %s", v) ;
	} ;

	kfEls_["time"] = [this](S v, K &keyframe){  keyframe.time = parseReal<Real>(v) * speedK ;  } ;

	kfEls_["visible"] = [](S v, K &keyframe){  keyframe.visibleFlag = parseBool(v) ; } ;

	//
	// Keyframe Types
	//

	kfEls_["texture"] = [this](S v, K &keyframe) {
        if (!wasImage_)
            keyframe.images.clear();
        wasImage_ = true;

        if (keyframe.type == T::NONE)
            keyframe.type = T::IMAGE;

        auto& image = keyframe.images.emplace_back();

        auto [imageName, imageNameRight] = str_split(v, '[');
		image.imageName = imageName;

		auto [imageIndexString, imageIndexRem] = str_split(imageNameRight, ']');

        if (!imageIndexString.empty()) {
            image.imageIndex = parseInt(imageIndexString) - 1;
			auto [temp, imageIndexRight] = str_split(imageIndexRem, ',');
            imageNameRight = imageIndexRight;
        } else {
            auto [indexImageName, imageIndexRight] = str_split(v, ',');
			image.imageName = indexImageName;
            imageNameRight = imageIndexRight;
        }

        if (!imageNameRight.empty()) {
            auto params = parse_std_vector_trim(imageNameRight, ',');

            if (params.size() >= 2) {
                image.position = parseLayoutPoint(params[0], params[1]);
            }

            if (params.size() >= 3) {
                auto os = str_trim(params[2]);
                if ("0" == os) image.orientation = M::Orientation0;
                else if ("90" == os) image.orientation = M::Orientation90;
                else if ("180" == os) image.orientation = M::Orientation180;
                else if ("270" == os) image.orientation = M::Orientation270;
                else raiseError("invalid orientation: %s", os);
                image.orientationFlag = true;
            }
        }
	};

	kfEls_["image"] = kfEls_["texture"] ;

    kfEls_["text"] = [this](S v, K& keyframe) {
        keyframe.type = T::TEXT;

        using namespace apl;

        if (!wasText_)
            keyframe.texts.clear();
        wasText_ = true;

        keyframe.texts.resize(keyframe.texts.size()+1);
        auto& text = keyframe.texts.back();

        text.text = v;

        if (auto lastComma = v.find_last_of('@'); lastComma != v.npos) {
            text.text.erase(lastComma);
            text.text = str_trim(text.text);
            text.position = parseLayoutPoint(v.substr(lastComma+1));
        }

        text.text = str_trim(text.text);
    };



    kfEls_["position"] = [this](S v, K& keyframe) {  keyframe.position = parseLayoutPoint(v);  };

    kfEls_["size"] = [this](S v, K& keyframe) {  keyframe.size = parseLayoutSize(v);  };
    kfEls_["elsize"] = [this](S v, K& keyframe) {  keyframe.elSize = parseLayoutSize(v);  };
    kfEls_["tile"] = [this](S v, K& keyframe) {  keyframe.tiledHorz = keyframe.tiledVert = parseBool(v);  };
    kfEls_["tiled"] = kfEls_["tile"];

    kfEls_["tiled-vert"] = [this](S v, K& keyframe) {	keyframe.tiledVert = parseBool(v);  };
    kfEls_["tiled-horz"] = [this](S v, K& keyframe) {	keyframe.tiledHorz = parseBool(v);  };

    kfEls_["text_height"] = [this](S v, K& keyframe) {  
        keyframe.textHeight = parseReal<Real>(v) * scaleTextures_; 
        keyframe.textHeightFlag = true;  
    };
    kfEls_["text-height"] = kfEls_["text_height"];

    kfEls_["font"] = [this](S v, K& keyframe) {  keyframe.fontName = v; };


    kfEls_["align"] = [this](S v, K& keyframe) {
        auto [horzAlign, vertAlign] = apl::str_split_trim(v, ',');
        if ("left" == horzAlign) keyframe.alignHorz = M::AlignLeft;
        else if ("right" == horzAlign) keyframe.alignHorz = M::AlignRight;
        else if ("center" == horzAlign) keyframe.alignHorz = M::AlignCenter;
        else raiseError("Invalid align value: %s", horzAlign);
        if ("top" == vertAlign) keyframe.alignVert = M::AlignTop;
        else if ("bottom" == vertAlign) keyframe.alignVert = M::AlignBottom;
        else if ("middle" == vertAlign || "center" == vertAlign) keyframe.alignVert = M::AlignVCenter;
        else raiseError("Invalid align value: %s", vertAlign);
    };

    kfEls_["align-horz"] = [this](S v, K& keyframe) {
        if ("left" == v || "LEFT" == v) keyframe.alignHorz = M::AlignLeft;
        else if ("right" == v || "RIGHT" == v) keyframe.alignHorz = M::AlignRight;
        else if ("center" == v || "CENTER" == v) keyframe.alignHorz = M::AlignCenter;
        else raiseError("Invalid align value: %s", v);
    };
	kfEls_["align-vert"] = [this](S v, K &keyframe){ 
		if ( "top" == v ) keyframe.alignVert = M::AlignTop ; else
		if ( "bottom" == v ) keyframe.alignVert =  M::AlignBottom ;  else
		if ( "middle" == v || "center" == v ) keyframe.alignVert = M::AlignVCenter ; else
			raiseError ( "Invalid Align Value: %s", v ) ;
	} ;

    kfEls_["color"] = [this](S v, K& keyframe) {  keyframe.setColor(parseColor(v));  };

    kfEls_["fill-color"] = [this](S v, K& keyframe) { keyframe.setFillColor(parseColor(v)); };

    kfEls_["rotation"] = [this](S v, K& keyframe) {
        keyframe.rotationFlag = true;
        keyframe.rotationValue = parseReal<Real>(v) * cognit::constants::pi<Cord> / 180;
    };

	kfEls_["alpha"] = [this](S v, K &keyframe) {  
        keyframe.alphaFlag = true;
        keyframe.alphaValue = parseReal<Real>(v);
    };

	kfEls_["transparency"] = kfEls_["alpha"] ;

	kfEls_["rect"] = [this](S v, K &keyframe){  
		keyframe.type = T::RECTANGLE ;
		parseLayoutRectPair ( v, keyframe.position, keyframe.size ) ;
		//raiseError ( "Invalid rectangle: %s", v ) ;
	} ;
	kfEls_["rectangle"] = kfEls_["rect"] ;

	kfEls_["radius"] = [this](S v, K &keyframe) {
		auto &param = keyframe.params[MAnimationKeyframe::ParameterTag::RADIUS] ;
		param.setFlag = true ;
		param.distanceValue = parseCoordinate(v) ;
	} ;

    kfEls_["line"] = [this](S v, K& keyframe) {
        keyframe.type = T::LINE;

        auto&& vs = parse_std_vector_view_trim(v, ':');
        keyframe.pts.clear();
        keyframe.pts.reserve(vs.size());
        for (auto&& s: vs)
			keyframe.pts.push_back({parseLayoutPoint(s), {}});

        if (keyframe.pts.size() < 2)
            raiseError("Invalid line coordinates: %s", v);
    };

    kfEls_["bezier"] = [this](S v, K& keyframe) {
        keyframe.type = T::BEZIER;

        auto&& vs = parse_std_vector_view_trim(v, ':');
        keyframe.pts.clear();

        for (auto&& s: vs)
            keyframe.pts.push_back({parseLayoutPoint(s), {}});

        if (keyframe.pts.size() < 4)
            raiseError("Invalid bezier coordinates: %s", v);
    };

    kfEls_["circle"] = [this](S v, K& keyframe) {
        keyframe.type = T::CIRCLE;
        auto [sposition, sradius] = str_split(v, ':');

        auto& pparam = keyframe.params[MAnimationKeyframe::ParameterTag::POSITION];
        pparam.setFlag = true;
        pparam.pointValue = parseLayoutPoint(sposition);

        auto& rparam = keyframe.params[MAnimationKeyframe::ParameterTag::RADIUS];
        rparam.setFlag = true;
        rparam.distanceValue = parseCoordinate(sradius);
    };



    kfEls_["width"] = [this](S v, K& keyframe) {
        keyframe.widthFlag = true;
        keyframe.widthValue = M::parseCoord(v);
    };

    kfEls_["effect"] = [this](S v, K& keyframe) {
        auto params = parse_std_vector_view_trim(v, ',');

        if (params[0] == "frames") {
            keyframe.framesEffectFlag = true;
            keyframe.framesEffectDuration = 1;
            keyframe.framesEffectRepeat = true;
            keyframe.framesEffectReverse = false;
            keyframe.framesEffectBounce = false;

            for (auto& param : params) {
                auto [pname, pvalue] = str_split_trim(param, ':');
                if ("duration" == pname) keyframe.framesEffectDuration = M::parseCoord(pvalue);
                else if ("reverse" == pname) keyframe.framesEffectReverse = parseBool(pvalue);
                else if ("bounce" == pname) keyframe.framesEffectBounce = parseBool(pvalue);
                else if ("frames" == pname) {}
                else raiseError("Invalid frames effect param: %s", pname);
            }
        }
    };


    kfEls_["sound"] = [this](S v, K& keyframe) {
        auto soundPath = Resources::bundleDirJoin(v);
        soundPath += Env::instance()->soundExt;
        keyframe.soundId = Sfx::findByFn(soundPath);
        if (!keyframe.soundId) {
            keyframe.soundId = current_sound_id_++;
            Sfx::load(keyframe.soundId, soundPath);
        }
    };

}


}  // namespace mangl
