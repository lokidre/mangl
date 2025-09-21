/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "texmake.h"

#include <apeal/file.hpp>
#include <apeal/image.hpp>
#include <apeal/hash/sha256.hpp>

void TexMake::buildTextureImage() {
    using namespace apl;

    //
    // Prepare output image
    //
    outputImage_.zero();

    // Fill out alpha
    // outputImage_.fillRGBA(0x000000FF);

    ImageContainer currentImage;
    Image glyphImage;


    // For all the textures
    for (auto& tex: textures_) {
        if (tex.phonyImage)
            continue;

        loadSourceImage(tex, &currentImage);

        placeTextureImageSource(tex, currentImage.front());

        // Sha256 hash of tex
        tex.hash = file_SHA256(tex.filename);

        deps_.insert(tex.filename);
    }

    // for all the font glyphs
    for (auto& font: fonts_) {
        auto [name, ext] = splitFileExt(font.filename);

        if (font.type == FontSourceType::Ttf) {
            auto fontImageFile = makeFileName(name, "png");

            loadImage(fontImageFile, &currentImage);
            auto& fontImage = currentImage.front();

            deps_.insert(fontImageFile);

            //auto glyphsImage = loadImage(name);
            for (auto& glyph: font.glyphs) {
                if (glyph.phonyImage)
                    continue;

                // TODO: move this to the class member to avoid reallocating the memory
                glyphImage.create({
                    .cm = Image::Model::RGBA,
                    .size = glyph.targetRect.size()
                });

                getSubImage(fontImage, glyph.sourceRect, &glyphImage);

                if (params.bleed) {
                    auto rect = glyph.targetRect.shrank(glyph.margin * 2);
                    rect.x = rect.y = glyph.margin;
                    bleedImage(glyphImage, rect);
                }


                // auto psrc = fontImage.pixelPosData(glyph.sourceRect.position());
                // auto pdst = glyphImage.data();
                // for (int y = 0; y < glyph.sourceRect.h; ++y) {
                //
                //     std::memcpy(pdst, psrc, glyphImage.bytesPerRow);
                //
                //     psrc += fontImage.bytesPerRow;
                //     pdst += glyphImage.bytesPerRow;
                // }

                // We don't need bleeding for fonts. They never at the edges anyway
                placeTextureImageSource(glyph, glyphImage);
            }
        }

        if (font.type == FontSourceType::Image) {
            for (auto& glyph: font.glyphs) {
                if (glyph.phonyImage)
                    continue;

                loadSourceImage(glyph, &currentImage);

                deps_.insert(glyph.filename);

                placeTextureImageSource(glyph, currentImage.front());
            }
        }

    }

    //
    // Save the output image file
    //
    mkdirIfNotExistsParents(extractDirName(params.outputImageFile));
    saveImage(params.outputImageFile, outputImage_);
}


void TexMake::placeTextureImageSource(ImageSourceCR source, ImageCR inputImage) {

    apl::copySubImage(inputImage, source.targetRect.pos(), &outputImage_);

    // using namespace apl;
    //
    // verify(inputImage.cm == ColorModel::RGBA, "Input image is not RGBA");
    //
    // auto& targetRect = source.targetRect;
    //
    // for (int j = 0; j < targetRect.h; ++j) {
    //     auto dst = outputImage_.pixelPosData<>(targetRect.x, targetRect.y + j);
    //     auto src = inputImage.pixelRowData<>(j);
    //     std::memcpy(dst, src, inputImage.bytesPerRow);
    // }
}


