/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_types.h"

#include "mangl_texture.h"
#include "mangl_font.h"
#include "mangl_error.h"

#include <apeal/file.hpp>
#include <apeal/noncopyable.hpp>


namespace mangl {

struct TextureContext {};

struct TextureAtlas;
using TextureAtlasP = TextureAtlas*;

struct TextureAtlas: public apl::NonCopyable {
    using Self = TextureAtlas;

    // Types
    //using TextureList = std::unordered_map<MString, MTexture>;
    //using FontList = std::unordered_map<MString, MFont>;
    using TextureList = StrValCont<Texture>;
    using FontList = StrValCont<Font>;


    //
    // Data
    //
    TextureId textureId{};  // User supplied id (or automatically generated)
    String atlasFile, imageFile;  // atlas and corresponding image file

private:
    TextureList textures_;
    FontList fonts_;

    apl::Buffer<RealGL> coords_;

    RealGL density_{1};


public:
    //
    // Initialization/Loading/Setup
    //
    ~TextureAtlas() noexcept override {
        try { release(); }
        catch (...) {}
    }

    // Checks if the texture with this file name already loaded
    [[nodiscard]] bool loaded(StrView filename) const;

    //Self& loadOld(StrView filename);
    virtual void load(StrView filename);
    void release();


    auto setDensity(Cord dens) noexcept {
        density_ = static_cast<RealGL>(dens);
        for (auto& tex: textures_)
            tex.second.density = density_;
        for (auto& font: fonts_)
            font.second.textureDensity = density_;
        return this;
    }


    //
    // Verification, access 
    //
    [[nodiscard]] bool valid() const noexcept { return textureId > 0; }
    [[nodiscard]] bool isValid() const noexcept { return valid(); }
    void setValid(bool st) noexcept {  textureId = Texture::invalidId(); }


    [[nodiscard]] auto atlasFileBaseName() const noexcept { return apl::extractFileName(atlasFile); }
    [[nodiscard]] auto imageFileBaseName() const noexcept { return apl::extractFileName(imageFile); }

    [[nodiscard]] const auto& get(StrView name) const {
        // TODO: Avoiding misaligned pointer warning
        // Need to come up with a better solution
        String names{name};
        if (auto it = textures_.find(names); it != textures_.end())
            return it->second;
        apl::throwError("Texture not found '%s'.'%s'", atlasFileBaseName(), name);
    }

    [[nodiscard]] const auto& getFont(StrView name) const {
        // TODO: Avoiding misaligned pointer warning
        // Need to come up with a better solution
        String names{name};
        if (auto it = fonts_.find(names); it != fonts_.end())
            return it->second;
        apl::throwError("Font not found in '%s'.'%s'", atlasFileBaseName(), name);
    }

    [[nodiscard]] TextureA texture(StrView name) const { return get(name); }

    [[nodiscard]] Texture get(StrView name, Index index) const {
        auto tex = get(name);
        tex.coords += index * 4;
        return tex;
    }

    [[nodiscard]] bool exists(StrView name) const noexcept {
        return textures_.find(name) != textures_.end(); 
    }

    TextureA operator[](StrView name) const { return get(name); }
    TextureA operator()(StrView name) const { return get(name); }
    Texture operator()(StrView name, int index) const { return get(name, index); }


    //
    // Fonts manipulation
    //
    //const auto& font(StrView name) const { return getFont(name); }
    [[nodiscard]] const auto& getFonts() const noexcept { return fonts_; }
    [[nodiscard]] bool fontExists(StrView name) const noexcept { return fonts_.find(name) != fonts_.end(); }


private:
    //void loadAtlasData(StrView filename);
    void loadYamlData(StrView filename);


    void clear() noexcept {
        imageFile.clear();
        textures_.clear();
        coords_.release();
        fonts_.clear();
        textureId = 0;
    }
};

}  // namespace mangl

