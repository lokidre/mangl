/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <map>

#include <apeal/singleton.hpp>
#include <apeal/string.hpp>

#include "../mangl_types.h"

#include "../mangl_texture_atlas.h"
#include "../mangl_render.h"

namespace mangl {


struct TexturePool : public apl::Singleton<TexturePool> {
    /*
    struct TextureInfo {
        int texture_id ;
        string_type name ;
        string_type image_file, atlas_file ;
        std::shared_ptr<MTextureAtlas> textures ;
    } ;


    void setup ( SceneRenderer *render )
    {
        render_ = render ;
    }

    void add ( int texture_id, const string_type &image_file, const string_type &atlas_file )
    {
        TextureInfo ti ;
        ti.texture_id = texture_id ;
        ti.image_file = image_file ;
        ti.atlas_file = atlas_file ;
        textures_.insert ( std::pair<int,TextureInfo>(texture_id,ti) ) ;
    }

    void add ( const string_type &name, int texture_id, const string_type &image_file, const string_type &atlas_file )
    {
        add ( texture_id, image_file, atlas_file ) ;
        add ( texture_id, name ) ;
    }
    void add ( int texture_id, const string_type &name )
    {
        find(texture_id).name = name ;
    }

    void remove ( const MString &name )
    {
        auto it = textureNames_.find(name) ;
        if ( it != textureNames_.end() ) {
            textures_.erase(textures_.find(it->second)) ;
            textureNames_.erase(it) ;
        }
    }

    void load ( const string_type &name )
    {
        TextureInfo &ti = find(name) ;
        ti.textures = std::make_shared<MTextureAtlas>() ;
        render_->loadAtlas ( ti.texture_id, ti.image_file, ti.atlas_file, ti.textures.get() ) ;
    }

    void load ( MTexture::Id textureId )
    {
        TextureInfo &ti = find(textureId) ;
        ti.textures = std::make_shared<MTextureAtlas>() ;
        render_->loadAtlas ( ti.texture_id, ti.image_file, ti.atlas_file, ti.textures.get() ) ;
    }

    void load ( const MString &name, int texture_id, const MString &image_file, const MString &atlas_file )
    {
        add ( name, texture_id, image_file, atlas_file ) ;
        load ( texture_id ) ;
    }
    void load ( int texture_id, const MString &image_file, const MString &atlas_file )
    {
        add ( texture_id, image_file, atlas_file ) ;
        load ( texture_id ) ;
    }
    void release ( const MString &name ) ;


    TextureInfo &find ( MTexture::Id texture_id )
    {
        auto it = textures_.find(texture_id) ;
        if ( it == textures_.end() ) {
            string_type s ;
            apl::str_format ( s, "Texture not found in textures pool: %d", texture_id ) ;
            throw std::runtime_error(s) ;
        }
        return it->second ;
    }

    TextureInfo &find ( const MString &name )
    {
        auto it = textureNames_.find(name) ;
        if ( it == textureNames_.end() )
            throw std::runtime_error("Texture not found in textures pool: " + name) ;
        return find(it->second) ;
    }

    MTextureAtlas &atlas ( const MString &name )
    {
        return *find(name).textures ;
    }

    MTextureAtlas &atlas ( MTexture::Id texture_id )
    {
        return *find(texture_id).textures ;
    }


private :
    std::map<MTexture::Id,TextureInfo> textures_ ;
    std::map<MString,MTexture::Id> textureNames_ ;

    SceneRenderer *render_{} ;
    */
} ;

}  // namespace mangl
