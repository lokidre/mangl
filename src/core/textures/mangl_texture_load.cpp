/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_texture_atlas.h"

#include "../mangl_common.h"
#include "../mangl_render.h"
#include "../render/mangl_pl_util_global.h"
#include "../mangl_resources.h"
#include "../mangl_parse.h"

#include <apeal/parse/yaml.hpp>
#include <apeal/file.hpp>
#include <apeal/hash.hpp>

namespace mangl {

void TextureAtlas::load(StrView filename) 
{
    using namespace apl;

    // Parse and load yaml file
    // Deduce file names (for now)
    // Obsolete
    auto [dir, fname] = splitFilePath(filename);
    auto [name, ext] = splitFileExt(fname.empty() ? dir : fname);

    String fileBase{makeFilePath(dir, name)};

    if (!Res::textureSuffix.empty()) {
        fileBase += Res::textureSuffix;
    }

    auto yamlFileName = makeFileName(fileBase, M::DefaultTextureAtlasExt);
    loadYamlData(yamlFileName);


    // Call the renderer and schedule texture loading
    textureId = render::sceneRender_->loadTexture(imageFile);

    for (auto& tex: textures_)
        tex.second.tid = textureId;

    for (auto& font: fonts_) {
        font.second.textureId = textureId;
        for (auto& glyph: font.second.charlist) {
            glyph.second.texture.tid = textureId;
        }
    }
}

static void
loadFont(const apl::YamlNode& fontNode, RealGL*& coordsPtr, Font* pfont, String* pfontName) {
    using namespace apl;

    auto& font = *pfont;
    auto& fontName = *pfontName;

    for (auto& fontEl: fontNode) {
        switch (pshHash(fontEl.key)) {
        case "div"_psh: fontEl.get(&font.fontDiv); break;
        default: break;
        }
    }


    for (auto& fontEl: fontNode) {

        switch (pshHash(fontEl.key)) {
        case "name"_psh: fontName = fontEl.getString(); break;
        // case "div"_psh: font.fontDiv = fontEl.getInt(); break;
        case "height"_psh: fontEl.get(&font.fontHeight); break;

        case "face"_psh:
            for (auto& el: fontEl) {
                switch (pshHash(el.key)) {
                case "units_per_em"_psh: el.get(&font.unitsPerEM); break;
                case "bbox"_psh:
                    font.bboxXMin = el.nodes[0].getReal<RealGL>();
                    font.bboxXMax = el.nodes[1].getReal<RealGL>();
                    font.bboxYMin = el.nodes[2].getReal<RealGL>();
                    font.bboxYMax = el.nodes[3].getReal<RealGL>();
                    break;
                case "ascent"_psh: el.get(&font.ascent); break;
                case "descent"_psh: el.get(&font.descent); break;
                case "height"_psh: el.get(&font.designHeight); break;
                case "max_advance"_psh: el.get(&font.designMaxAdvance); break;
                case "underline_position"_psh: el.get(&font.underlinePosition); break;
                case "underline_thickness"_psh: el.get(&font.underlineThickness); break;
                default: break;
                }
            }
            break;
        case "metrics"_psh:
            for (auto& el: fontEl) {
                switch (pshHash(el.key)) {
                case "ppem"_psh:
                    //                    font.ppem = {
                    //                        el.nodes[0].getReal<RealGL>(),
                    //                        el.nodes[1].getReal<RealGL>()
                    //                    };
                    el.nodes[0].get(&font.ppem.x);
                    el.nodes[1].get(&font.ppem.y);
                    break;
                case "scale"_psh:
                    el.nodes[0].get(&font.metricsScale.x);
                    el.nodes[1].get(&font.metricsScale.y);
                    // font.metricsScale = {el.nodes[0].getReal<RealGL>(),
                    // el.nodes[1].getReal<RealGL>()};
                    break;
                case "ascent"_psh: el.get(&font.metricsAscent); break;
                case "descent"_psh: el.get(&font.metricsDescent); break;
                case "height"_psh: el.get(&font.metricsHeight); break;
                case "max_advance"_psh: el.get(&font.metricsMaxAdvance); break;
                default: break;
                }
            }
            break;

        case "glyphs"_psh:
            for (auto& glyphNode: fontEl) {
                FontGlyph glyph;
                for (auto& gl: glyphNode) {
                    switch (pshHash(gl.key)) {
                    case "charcode"_psh: glyph.charcode = M::fontCharCodeParse(gl.value); break;
                    case "tex"_psh: {
                        glyph.texture.coords = coordsPtr;
                        coordsPtr[0] = gl.nodes[0].getReal<RealGL>();
                        coordsPtr[1] = gl.nodes[1].getReal<RealGL>();
                        coordsPtr[2] = gl.nodes[2].getReal<RealGL>();
                        coordsPtr[3] = gl.nodes[3].getReal<RealGL>();
                        coordsPtr += 4;
                        glyph.texture.count = 1; // parseReal<RealGL>(value);
                        break;
                    }
                    case "glyph_size"_psh: {
                        parseNode(gl, &glyph.glyphSize);
                        //glyph.glyphSize = {
                        //    gl.nodes[0].getReal<RealGL>(), gl.nodes[1].getReal<RealGL>()};
                        glyph.glyphSize /= font.fontDiv;
                        break;
                    }
                    case "bitmap_margin"_psh:
                        glyph.bitmapMargin = {
                            gl.nodes[0].getReal<RealGL>(), gl.nodes[1].getReal<RealGL>()};
                        break;
                    case "horz_bearing"_psh:
                        glyph.horzBearing = {
                            gl.nodes[0].getReal<RealGL>(), gl.nodes[1].getReal<RealGL>()};
                        glyph.horzBearing /= font.fontDiv;
                        break;
                    case "vert_bearing"_psh: {
                        glyph.vertBearing = {
                            gl.nodes[0].getReal<RealGL>(), gl.nodes[1].getReal<RealGL>()};
                        glyph.vertBearing /= font.fontDiv;
                        break;
                    }
                    case "advance"_psh: {
                        glyph.advance = {
                            gl.nodes[0].getReal<RealGL>(), gl.nodes[1].getReal<RealGL>()};
                        glyph.advance /= font.fontDiv;
                        break;
                    }
                    case "kerning"_psh:
                        glyph.kerning[M::fontCharCodeParse(gl.nodes[0].getString())] =
                            gl.nodes[1].getReal<RealGL>() / font.fontDiv;
                        break;
                    default: break;
                    }
                }
                font.insertGlyph(std::move(glyph));
            }
            break;

        default: break;
        }

    } // Font keys

    if (font.unitsPerEM != 0) {
        auto scale = font.ppem / font.unitsPerEM;

        font.bboxXMin *= scale.x;
        font.bboxXMax *= scale.x;
        font.bboxYMin *= scale.y;
        font.bboxYMax *= scale.y;
    }
}

void TextureAtlas::loadYamlData(StrView filename) {
    using namespace apl;
    using namespace apl::literals;

    clear();

    atlasFile = filename;

    auto yaml = loadYaml(filename);

    // Load file into memory
    String name, value;

    Index totalTexturesCount{}, totalFontsCount{}, totalRectsCount{};

    for (auto& docNode: yaml.front()) {
        if ("textures" == docNode.key) {
            for (auto& texNode: docNode) {
                for (auto& texEl: texNode) {
                    switch (pshHash(texEl.key)) {
                    case "file"_psh: imageFile = substFileName(atlasFile, texEl.getValue()); break;
                    case "density"_psh: texEl.get(&density_); break;
                    case "total_images"_psh: texEl.get(&totalTexturesCount); break;
                    case "images"_psh:
                        for (auto& imageNode: texEl) {
                            for (auto& imageEl: imageNode) {
                                if ("coords" == imageEl.key) {
                                    totalRectsCount += std::ssize(imageEl.nodes);
                                }
                            }
                        }
                        break;

                    case "fonts"_psh: ++totalFontsCount; break;
                    default: break;
                    }
                }
            }
        }
    }

    coords_.resize(totalTexturesCount * 4);
    RealGL* coordsPtr = coords_.data();

    for (auto& docNode: yaml.front()) {
        if ("textures" == docNode.key) {
            for (auto& texNode: docNode) {
                for (auto& texEl: texNode) {
                    switch (pshHash(texEl.key)) {
                    case "images"_psh: {
                        for (auto& imageEl: texEl) {
                            String imageName;
                            Texture image{};
                            image.density = density_;
                            image.count = 0;

                            for (auto& el: imageEl) {
                                switch (pshHash(el.key)) {
                                case "name"_psh: imageName = el.getString(); break;
                                case "coords"_psh: {
                                    image.coords = coordsPtr;
                                    for (auto& coordNode: el) {
                                        for (auto& rl: coordNode) {
                                            if (rl.key == "rect") {
                                                coordsPtr[0] = rl.nodes[0].getReal<RealGL>();
                                                coordsPtr[1] = rl.nodes[1].getReal<RealGL>();
                                                coordsPtr[2] = rl.nodes[2].getReal<RealGL>();
                                                coordsPtr[3] = rl.nodes[3].getReal<RealGL>();
                                                coordsPtr += 4;
                                                ++image.count;
                                            }
                                        }
                                    }
                                    break;
                                }
                                default: break;
                                }
                            }
                            textures_.emplace(imageName, image);
                        }
                        break;
                    }

                    case "fonts"_psh: {
                        for (auto& fontNode: texEl) {
                            Font font;
                            String fontName;

                            loadFont(fontNode, coordsPtr, &font, &fontName);

                            fonts_.emplace(fontName, font);
                        } // Font

                        break;
                    }
                    default: break;
                    }
                }
            }
        }
    }


    // Do some font maintenance
    for (auto&& fontIt: fonts_) {
        // determine the space advance
        auto&& font = fontIt.second;
        auto&& charlist = font.getGlyphs();

        // Check with the space char
        // If we have the space character then use it
        // If we don't just pick some random char (TODO, maybe pick average)
        if (auto chIt = charlist.find(' '); chIt != charlist.end()) {
            font.spaceAdvance = chIt->second.advance.x;
        } else {
            font.spaceAdvance = charlist.begin()->second.advance.x;
        }

        if (font.bboxXMin == 0 && font.bboxXMax == 0 && font.bboxYMin == 0 && font.bboxYMax == 0) {
            for (auto&& ch: charlist) {
                if (ch.second.glyphSize.w > font.bboxXMax)
                    font.bboxXMax = ch.second.glyphSize.w;
                if (ch.second.glyphSize.h > font.bboxYMax)
                    font.bboxYMax = ch.second.glyphSize.h;
            }
        }
    }

    setDensity(density_);
}

} // namespace mangl
