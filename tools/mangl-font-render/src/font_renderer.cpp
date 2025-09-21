/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "font_renderer.h"

#include <apeal/assert.hpp>
#include <apeal/endian.hpp>
#include <apeal/file.hpp>
#include <apeal/image.hpp>
#include <apeal/console.hpp>

#include <apeal/parse/yaml.hpp>


#include <numeric>

struct FontImageInfo {
    FT_ULong charcode;
    mangl::RectI rect;
    apl::Image image;
};


void FontRenderer::process() {
    using namespace apl;

    FT_Library ft;
    FT_Face face;
    FT_Error result;
    YamlBuilder yaml;

    auto assertError = [&result](StringA message) {
        if (result == 0)
            return;
        throw std::runtime_error("Error: " + message);
    };

    result = FT_Init_FreeType(&ft);
    assertError("FreeType Library cannot initialize");

    FT_Int verMajor, verMinor, verPatch;
    FT_Library_Version(ft, &verMajor, &verMinor, &verPatch);

    if (params.infoOnly) {
        Console::println("FT Library Ver: %d.%d.%d", verMajor, verMinor, verPatch);
    }


    result = FT_New_Face(ft, params.fontFile.c_str(), 0, &face);
    assertError("Cannot load font: " + params.fontFile);


    if (params.infoOnly) {
        Console::println("Face:");

        Console::printiln(1, "num_faces: %d", face->num_faces);
        Console::printiln(1, "num_glyphs: %d", face->num_glyphs);
        Console::printiln(1, "family_name: %s", face->family_name);
        Console::printiln(1, "style_name: %s", face->style_name);
        Console::printiln(1, "num_charmaps: %d", face->num_charmaps);

        char buff[5]{};
        for (auto i = 0; i < face->num_charmaps; ++i) {
            auto charmap = face->charmaps[i];

            Console::nl();
            Console::printiln(2, "encoding: %s", from_fourcc(charmap->encoding, buff));
            Console::printiln(2, "platform_id: %d", charmap->platform_id);
            Console::printiln(2, "encoding_id: %d", charmap->encoding_id);
        }

        FT_Done_Face(face);
        return;
    }


    int fontDiv = 64;


    result = FT_Set_Char_Size(face, 0, params.fontSize * fontDiv, 0, 72);
    assertError("Cannot set char size");

    // result = FT_Set_Pixel_Sizes ( face, 64, 64 ) ;
    // assertError("FT_Set_Pixel_Sizes") ;

    // ALibPng png;

    std::vector<FontImageInfo> images;

    if (params.characterRanges.empty()) {
        params.characterRanges.emplace_back(0x0020, 0x007E);
        params.characterRanges.emplace_back(0x00A0, 0x00AC);
        //params.characterRanges.emplace_back(0x00AE, 0x00FF);
    }

    std::vector<FT_ULong> charBuffer;

    auto totalChars = std::accumulate(
        params.characterRanges.begin(), params.characterRanges.end(), 0,
        [](auto val, auto el) { return val + el.second - el.first; }
    );
    charBuffer.reserve(totalChars);


    for (auto& range: params.characterRanges)
        for (auto code = range.first; code <= range.second; ++code)
            charBuffer.push_back(code);


    // auto outputComps = splitFileComponents ( params.fontFile ) ;
    //auto [outputDir, outputName, outputExt] = parseFilePath(params.outputFile);

    auto fontFileName = extractFileName(params.fontFile);
    //auto outputBasePath = makeFilePath(outputDir, outputName);

    // Check if the output directory exists. Require users to create it
//    verify(
//        outputDir.empty() || fileExists(outputDir), "Output directory doesn't exist: %s", outputDir
//    );

    //auto outputFontDescFile = makeFileName(outputBasePath, outputExt.empty() ? "ini" : outputExt);
    //auto outputFontDescFileYaml = makeFileName(outputBasePath, "yaml");

    yaml.obj("font");
    yaml.field("type", "ttf");
    yaml.field("file", fontFileName);
    yaml.field("div", fontDiv);
    yaml.field("height", params.fontSize);
    yaml.field("glyphs_count", charBuffer.size());

    yaml.obj("face");
    yaml.field("family", face->family_name);
    yaml.field("style", face->style_name);
    yaml.field("units_per_em", face->units_per_EM);
    yaml.flowSeq("bbox", face->bbox.xMin, face->bbox.xMax, face->bbox.yMin, face->bbox.yMax);
    yaml.field("ascent", face->ascender);
    yaml.field("descent", face->descender);
    yaml.field("height", face->height);
    yaml.field("max_advance", face->max_advance_width);
    yaml.field("underline_position", face->underline_position);
    yaml.field("underline_thickness", face->underline_thickness);
    yaml.pop();

    yaml.obj("metrics");
    yaml.flowSeq("ppem", face->size->metrics.x_ppem, face->size->metrics.y_ppem);
    yaml.flowSeq("scale", face->size->metrics.x_scale, face->size->metrics.y_scale);
    yaml.field("ascent", face->size->metrics.ascender);
    yaml.field("descent", face->size->metrics.descender);
    yaml.field("height", face->size->metrics.height);
    yaml.field("max_advance", face->size->metrics.max_advance);
    yaml.pop();


    for (auto charcode: charBuffer) {

        auto scharcode = mangl::M::fontCharCodeFormat(charcode);

        /*   if (charcode == 0x6e) {
               int a = 0;
           }*/


        auto glyphIndex = FT_Get_Char_Index(face, charcode);

        if (glyphIndex == 0) {
            Console::println("Character missing: %s", scharcode);
            continue;
        }

        result = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
        assertError("FT_Load_Glyph");


        result = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        assertError("Error rendering glyph: " + scharcode);


        auto glyph = face->glyph;
        auto bitmap = &glyph->bitmap;


        Image image;

        if (bitmap->rows) {
            auto imageSize = apl::Size<int>::make(bitmap->width, bitmap->rows);

            image.create({.cm = Image::Model::RGBA, .size = imageSize});

            int bitmapPitch = std::abs(bitmap->pitch);
            int pixelMode = bitmap->pixel_mode;

            verify(
                pixelMode == FT_PIXEL_MODE_GRAY, "FreeType::Unsupported pixelMode: %d", pixelMode
            );

            // Check for negative
            for (int row = 0; row < image.size.h; ++row) {
                //
                auto bitmapPtr = (uint8_t*)bitmap->buffer +
                                 (bitmap->pitch < 0 ? image.size.h - 1 - row : row) * bitmapPitch;

                auto bufferPtr = image.pixelRowData<uint32_t>(row);

                for (int col = 0; col < image.size.w; ++col, ++bufferPtr, ++bitmapPtr) {
                    uint8_t alpha = *bitmapPtr;

#if APEAL_BYTE_ORDER == APEAL_LITTLE_ENDIAN
                    uint32_t color = (uint32_t(alpha) << 24) | 0xFFFFFF;
#else
                    uint32_t color = (0xFFFFFF << uint32_t(alpha)) | value;
#endif
                    *bufferPtr = color;
                }
            }
        }

        FontImageInfo im{charcode, {0, 0, (int)image.size.w, (int)image.size.h}, std::move(image)};
        images.emplace_back(im);
    }

    mangl::SizeI outputImageSize{params.gap, params.gap};

    // Index outputImageW{};
    // Index outputImageH{};

    for (auto& el: images) {
        if (el.rect.w == 0)
            continue;

        el.rect.x = outputImageSize.w;
        el.rect.y = params.gap;
        outputImageSize.w += el.image.size.w;
        outputImageSize.h = std::max(outputImageSize.h, el.rect.h + params.gap * 2);

        outputImageSize.w += params.gap;  // Gap
    }

    yaml.obj("glyphs");

    for (auto& el: images) {
        auto scharcode = mangl::M::fontCharCodeFormat(el.charcode);

        yaml.seq("charcode", scharcode);

        auto glyphIndex = FT_Get_Char_Index(face, el.charcode);

        result = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
        assertError("FT_Load_Glyph");

        auto glyph = face->glyph;
        auto bitmap = &glyph->bitmap;
        yaml.flowSeq("bitmap_size", bitmap->width, bitmap->rows);
        yaml.flowSeq("bitmap_rect", el.rect.x, el.rect.y, el.rect.w, el.rect.h);

        yaml.flowSeq("bitmap_margin", glyph->bitmap_left, glyph->bitmap_top);
        yaml.flowSeq("glyph_size", glyph->metrics.width, glyph->metrics.height);
        yaml.flowSeq("horz_bearing", glyph->metrics.horiBearingX, glyph->metrics.horiBearingY);
        yaml.flowSeq("vert_bearing", glyph->metrics.vertBearingX, glyph->metrics.vertBearingY);
        yaml.flowSeq("advance", glyph->metrics.horiAdvance, glyph->metrics.vertAdvance);


        if (FT_HAS_KERNING(face)) {
            for (auto kernChar: charBuffer) {
                if (el.charcode == kernChar)
                    continue;

                auto kernGlyphIndex = FT_Get_Char_Index(face, kernChar);

                FT_Vector delta;
                result = FT_Get_Kerning(
                    face,
                    glyphIndex,
                    kernGlyphIndex,
                    FT_KERNING_DEFAULT,
                    &delta);
                assertError("Cannot Get Kerning");

                if (delta.x != 0) {
                    yaml.flowSeq(
                        "kerning", mangl::M::fontCharCodeFormat(kernChar), fmt("%d", delta.x)
                    );
                }
            }
        }

        yaml.pop();
    }

    Image outputImage;

    outputImage.create({.cm = Image::Model::RGBA, .size = outputImageSize});
    memset(outputImage.data(), 0, outputImage.dataSize());

    // TODO: remove
    // auto p = (uint32_t *)outputImage.data();
    // auto count = outputImage.dataSize()/4;
    // for (int i = 0; i < count; ++i) {
    //    *p++ = 0xff0000ff;
    //}


    for (auto& el: images) {
        if (el.rect.w == 0)
            continue;
        apl::copySubImage(el.image, el.rect.position(), &outputImage);
        // auto psrc = el.image.data();
        // auto pdest = outputImage.data() + el.rect.x * 4;
        // for (int y = 0; y < el.rect.h; ++y) {
        //
        //     std::memcpy(pdest, psrc, el.rect.w * 4);
        //
        //     psrc += el.rect.w * 4;
        //     pdest += outputImageSize.w * 4;
        // }
    }

    yaml.pop();
    yaml.end();

    saveFile(params.outputDescFile, yaml.str());

    ImageFile::save(params.outputImageFile, outputImage);
}
