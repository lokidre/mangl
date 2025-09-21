/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "texmake.h"


void TexMake::loadSourceImageInfo(ImageSource& src, StrView filename, int margin) {
    auto imageInfoContainer = apl::ImageFile::loadInfo(src.filename);

    src.margin = margin;
    src.filename = filename;
    src.sourceRect = {};
    src.sourceRect.setSize(imageInfoContainer.front().size);

    src.targetRect = src.sourceRect;
    src.targetRect.expand(margin * 2);
}



void TexMake::loadSourceImage(ImageSourceCR src, apl::ImageContainer* ic) {
    using namespace apl;

    loadImage(src.filename, ic);

    auto& image = ic->front();

    if (image.cm != ColorModel::RGBA) {
        tempImage_.create({.cm = ColorModel::RGBA, .size = image.size});
        image.convert(tempImage_);
        image.swap(tempImage_);
    }

    if (src.margin == 0)
        return;

    auto& targetImage = tempImage_;
    auto& sourceImage = image;

    targetImage.create({.cm = ColorModel::RGBA, .size = src.targetRect.size()});
    targetImage.zero();


    // Copy image itself
    copySubImage(sourceImage, {src.margin, src.margin}, &targetImage);
    if (params.bleed) {
        auto targetImageRect = RectI::make({}, targetImage.size);
        bleedImage(targetImage, targetImageRect.shrank(src.margin * 2));
    }


    /*
    //
    // Implement the bleeding
    //
    auto targetSize = targetImage.size;

    auto bytesPerSample = targetImage.bytesPerSample;
    auto bytesPerRow = targetImage.bytesPerRow;
    auto pixelsPerRow = bytesPerRow / bytesPerSample;

    //
    // Vertical bleeding
    //
    auto vertSrcLeft = targetImage.pixelPosData<uint32_t>(src.bleeding, 0);
    auto vertDstLeft = targetImage.pixelPosData<uint32_t>(0, 0);

    auto vertSrcRight = targetImage.pixelPosData<uint32_t>(targetSize.w - src.bleeding - 1, 0);
    auto vertDstRight = vertSrcRight + 1;

    auto bleedingColor = bleedingDebugColors_[currentBleedingDebugColor_++];
    if (currentBleedingDebugColor_ >= std::size(bleedingDebugColors_))
        currentBleedingDebugColor_ = 0;

    // TODO: implement if bleeding more than 1

    // Vertical
    for (auto row = 0; row < targetSize.h; ++row) {
        *vertDstLeft = *vertSrcLeft;
        *vertDstRight = *vertSrcRight;

        if (params.bleedingDebug) {
            *vertDstLeft = bleedingColor;
            *vertDstRight = bleedingColor;
        }

        vertSrcLeft += pixelsPerRow, vertDstLeft += pixelsPerRow;
        vertSrcRight += pixelsPerRow, vertDstRight += pixelsPerRow;
    }

    // Horizontal
    if (params.bleedingDebug) {
        auto destBegin = targetImage.pixelPosData<uint32_t>(1, 0);
        auto destEnd = destBegin + sourceImage.size.w;
        std::fill(destBegin, destEnd, bleedingColor);

        destBegin = targetImage.pixelPosData<uint32_t>(1, targetImage.size.h - 1);
        destEnd = destBegin + sourceImage.size.w;
        std::fill(destBegin, destEnd, bleedingColor);
    } else {
        std::memcpy(targetImage.pixelPosData(1, 0), sourceImage.pixelRowData(1), sourceImage.bytesPerRow);
        std::memcpy(targetImage.pixelPosData(1, targetImage.size.h - 1), sourceImage.pixelRowData(sourceImage.size.h - 1), sourceImage.bytesPerRow);
    }


    //
    // Corners
    //
    if (params.bleedingDebug) {
        *targetImage.pixelPosData<uint32_t>(0, 0) = bleedingColor;
        *targetImage.pixelPosData<uint32_t>(targetSize.w - 1, 0) = bleedingColor;
        *targetImage.pixelPosData<uint32_t>(targetSize.w - 1, targetSize.h - 1) = bleedingColor;
        *targetImage.pixelPosData<uint32_t>(0, targetSize.h - 1) = bleedingColor;
    } else {
        struct Corner {
            PointI p;
            PointI n[3];
        };

        auto tw1 = targetSize.w - 1;
        auto th1 = targetSize.h - 1;

        std::array<Corner, 4> corners{
            {
                // Bottom left
                {
                    .p = {0, 0},
                    .n = {{0, 1}, {1, 1}, {1, 0}},
                },

                // Bottom right
                {
                    .p = {tw1, 0},
                    .n = {{tw1, 1}, {tw1, 1}, {tw1 - 1, 0}},
                },

                // Top left
                {
                    .p = {0, th1},
                    .n = {{1, th1}, {1, th1 - 1}, {1, th1 - 1}},
                },

                // Top right
                {
                    .p = {tw1, th1},
                    .n = {{tw1 - 1, th1}, {tw1 - 1, th1 - 1}, {tw1, th1 - 1}},
                }
            }
        };

        for (auto& corner: corners) {
            std::array<uint32_t, 4> avrRgba{};

            // Each neighbor pixel
            for (auto& n: corner.n) {
                // Each color component
                auto s = targetImage.pixelPosData(n);
                for (auto& avr: avrRgba)
                    avr += *s++;
            }

            // Target pixel
            auto p = targetImage.pixelPosData<uint8_t>(corner.p);
            // Each color component
            for (auto& avr: avrRgba)
                *p++ = static_cast<uint8_t>(avr / 3);
        }



    }
    */

    sourceImage.swap(targetImage);
}

void TexMake::loadImage(StrView filename, apl::ImageContainer* ic) {
    using namespace apl;

    auto [dir, name, ext] = parseFilePath(filename);

    if (ext.empty())
        ext = "png";

    auto imagePath = makeFilePathExt(dir, name, ext);
    ImageFile::load(filename, ic);
}


apl::Image TexMake::loadImage(StrView filename) {
    apl::ImageContainer ic;

    loadImage(filename, &ic);

    return std::move(ic.front());
}
