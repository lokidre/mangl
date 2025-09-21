/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_layout.h"

#include <apeal/time.hpp>
#include <apeal/parse.hpp>
#include <apeal/file.hpp>
#include <apeal/parse/geo.hpp>

namespace mangl {

void MLayout::preload(StrView filename)
{
	using namespace apl ;
	
	filename_ = filename ;

	AIniParser parser ;
	AIni ini ;
	Preprocessor preprocessor ;
	String filedir ;
	filedir = extractDirName(filename) ;
	preprocessor.search_dirs.push_back(filedir) ;
	parser.load(preprocessor,filename,ini) ;

	String serr, sfile ;

	clear() ;
	setupParameters() ;


	for ( auto &sec: ini.sec ) {
		if ( !processCondition ( sec.name, sec_name_ ) )
			continue ;
		if ( "LAYOUT" == sec.name ) {
			for ( auto &el: sec.el ) {
				line_ = el.line ;
				processExpression ( el.value, value_ ) ;

				if ( !processCondition ( el.name, name_ ) )
					continue ;
				if ( "texture" == name_ )	atlasFile = value_ ; else
				if ( "texture_dir" == name_ || "texture-dir" == name_ ) textureDir = value_ ; else
				if ( "var" == name_ ) {
                    char delim = ':';
                    if (value_.find_first_of(delim) == value_.npos)
                        delim = ',';
                    auto [varName, varValue] = str_split_trim(value_, delim);
                    variables[varName] = varValue;
				} else
					;
			}
		}
	}
}

void MLayout::load(StrView filename)
{
	clear() ;
	setupParameters() ;


	using namespace apl ;
	
	filename_ = filename ;

	auto [filedir, sfile] = splitFilePath ( filename ) ;

	String serr ;
	IniParser parser ;
	Ini cfg ;
	Preprocessor preprocessor ;
	preprocessor.search_dirs.push_back(String{filedir}) ;
	parser.load(preprocessor,filename,cfg) ;

    constexpr auto pi = cognit::constants::pi<Cord>;

	for (auto&& sec: cfg.sec) {
		if ( !processCondition ( sec.name, sec_name_ ) )
			continue ;

		if ( "LAYOUT" == sec_name_ ) {
			for ( const auto &el : sec.el ) {
				line_ = el.line ;
				processExpression ( el.value, value_ ) ;
				
				if ( !processCondition ( el.name, name_ ) )
					continue ;

				if ( "texture_dir" == name_ || "texture-dir" == name_ ) textureDir = value_ ; else
				if ( "texture" == name_ )	atlasFile = value_ ; else
				if ( "var" == name_ ) {
					char delim = ':' ;
					if ( value_.find_first_of(delim) == value_.npos )
						delim = ',' ;
					auto [varName,varValue] = str_split_trim ( value_, delim ) ;
					variables[varName] = varValue ;
				} else
				if ( "origin" == name_ ) defaultOrigin_ = parseLayoutPoint ( value_ ) ; else
				if ( "axis" == name_ ) parse(value_, &defaultAxis_) ; else
				if ( "align" == name_ ) defaultAlign_ = parseAlign ( value_ ) ; else
				if ( !value_.empty() )
					throw std::runtime_error(str_format ( serr, "%s : %d : Invalid Parameter '%s'", sfile, el.line, name_ )) ;
			}
		} else
		if ( "ITEM" == sec_name_ ) {
			items_.resize(items_.size()+1) ;
			auto &item = items_.back() ;

			for ( const auto &el : sec.el ) {
				line_ = el.line ;

				processExpression ( el.value, value_ ) ;

				if ( !processCondition ( el.name, name_ ) )
					continue ;

				if ( "id" == name_ ) item.id = value_ ; else
				if ( "size" == name_ ) item.size = parseLayoutSize(value_) ; else
				if ( "p" == name_ || "pos" == name_ || "position" == name_ ) item.position = parseLayoutPoint ( value_ ) ; else
				if ( "origin" == name_ ) item.origin = parseLayoutPoint ( value_ ) ; else 
				if ( "axis" == name_ ) {
					Point value;
					parse(value_, &value);
					item.axis = value;
				}else
				if ( "align" == name_ ) item.align = parseAlign ( value_ ) ; else 
				if ( "image" == name_ ) {
                    item.texture_flag = true;
                    item.textureName = value_;
                    auto [texture, index] = str_split(value_, ',');
                    if (!index.empty()) {
                        item.textureName = texture;
                        item.textureIndex = parseInt(index);
                    }
				} else
				if ( "rotation" == name_ ) item.rotation = parseFloat(value_) * pi / 180 ;  else
				if ( "alpha" == name_ ) item.alpha = parseFloat(value_) ;  else
				if ( !name_.empty() )
					throw std::runtime_error(str_format ( serr, "%s:%d:Invalid Parameter:%s", sfile, el.line, name_ )) ;
			}

			if ( !item.id.empty() )
				itemsIndex_[item.id] = (int)items_.size()-1 ;
		} else
		if ( !sec_name_.empty() )
			throw std::runtime_error(str_format ( serr, "%s:%d:Invalid Section:%s", sfile, sec.line, sec_name_ )) ;
	}


}


}  // namespace mangl

