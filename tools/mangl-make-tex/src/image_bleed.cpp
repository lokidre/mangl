/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "texmake.h"

static constexpr std::array bleedingDebugColors_{
    0xff000088ul, 0xff0000fful,
    0xff008800ul, 0xff008888ul, 0xff0088fful,
    0xff00ff00ul, 0xff00ff88ul, 0xff00fffful,
    0xff880000ul, 0xff880088ul, 0xff8800fful,
    0xff888800ul, 0xff888888ul, 0xff8888fful,
    0xff88ff00ul, 0xff88ff88ul, 0xff88fffful,
    0xffffff00ul, 0xffffff88ul,

    // 0x000088fful, 0x0000fffful,
    // 0x008800fful, 0x008888fful, 0x0088fffful,
    // 0x00ff00fful, 0x00ff88FFul, 0x00fffffful,
    // 0x880000fful, 0x880088FFul, 0x8800fffful,
    // 0x888800fful, 0x888888FFul, 0x8888fffful,
    // 0x88ff00fful, 0x88ff88FFul, 0x88fffffful,
    // 0xffff00fful, 0xffff88FFul,

};

static std::size_t currentBleedingDebugColor_ = 0;


void TexMake::bleedImage(Image& image, RectI rect) {
    //
    // Implement the bleeding
    //
    auto targetSize = image.size;

    auto bytesPerSample = image.bytesPerSample;
    auto bytesPerRow = image.bytesPerRow;
    auto pixelsPerRow = bytesPerRow / bytesPerSample;

    std::uint32_t bleedingColor{};

    if (params.bleedingDebug) {
        bleedingColor = bleedingDebugColors_[currentBleedingDebugColor_++];
        if (currentBleedingDebugColor_ >= std::size(bleedingDebugColors_))
            currentBleedingDebugColor_ = 0;
    }


    //
    // Vertical bleeding
    //
    auto vertDstLeft = image.pixelPosData<uint32_t>(rect.x - 1, rect.y);
    auto vertSrcLeft = vertDstLeft + 1;

    auto vertSrcRight = image.pixelPosData<uint32_t>(rect.right() - 1, rect.y);
    auto vertDstRight = vertSrcRight + 1;

    // Vertical
    for (auto row = 0; row < rect.h; ++row) {
        if (params.bleedingDebug) {
            *vertDstLeft = bleedingColor;
            *vertDstRight = bleedingColor;
        } else {
            *vertDstLeft = *vertSrcLeft;
            *vertDstRight = *vertSrcRight;
        }


        vertSrcLeft += pixelsPerRow;
        vertDstLeft += pixelsPerRow;

        vertSrcRight += pixelsPerRow;
        vertDstRight += pixelsPerRow;
    }

    // Horizontal
    if (params.bleedingDebug) {
        auto destBegin = image.pixelPosData<uint32_t>(rect.x, rect.y - 1);
        auto destEnd = destBegin + rect.w;
        std::fill(destBegin, destEnd, bleedingColor);

        destBegin = image.pixelPosData<uint32_t>(rect.x, rect.top());
        destEnd = destBegin + rect.w;
        std::fill(destBegin, destEnd, bleedingColor);
    } else {
        std::memcpy(image.pixelPosData(rect.x, rect.y - 1), image.pixelPosData(rect.x, rect.y), rect.w * image.bytesPerSample);
        std::memcpy(image.pixelPosData(rect.x, rect.top()), image.pixelPosData(rect.x, rect.top() - 1), rect.w * image.bytesPerSample);
    }

    //
    // Corners
    //
    if (params.bleedingDebug) {
        *image.pixelPosData<uint32_t>(rect.x - 1, rect.y - 1) = bleedingColor;
        *image.pixelPosData<uint32_t>(rect.right(), rect.y - 1) = bleedingColor;
        *image.pixelPosData<uint32_t>(rect.right(), rect.top()) = bleedingColor;
        *image.pixelPosData<uint32_t>(rect.x - 1, rect.top()) = bleedingColor;
    } else {
        struct Corner {
            PointI p;
            PointI n;
        };

        auto tw = rect.w;
        auto th = rect.h;

        std::array<Corner, 4> corners{
                {
                    // Bottom left
                    {
                        .p = {-1, -1},
                        .n = {0, 0},
                    },

                    // Bottom right
                    {
                        .p = {tw, -1},
                        .n = {tw - 1, 0},
                    },

                    // Top left
                    {
                        .p = {-1, th},
                        .n = {0, th - 1},
                    },

                    // Top right
                    {
                        .p = {tw, th},
                        .n = {tw - 1, th - 1},
                    }
                }
        };

        for (auto& corner: corners) {

            // Target pixel
            auto src = image.pixelPosData<uint32_t>(corner.n + rect.position());
            auto dst = image.pixelPosData<uint32_t>(corner.p + rect.position());

            *dst = *src;
        }



#if 0
        struct Corner {
            PointI p;
            PointI n[3];
        };

        auto tw = rect.w;
        auto th = rect.h;

        std::array<Corner, 4> corners{
            {
                // Bottom left
                {
                    .p = {-1, -1},
                    .n = {{-1, 0}, {0, 0}, {0, -1}},
                },

                // Bottom right
                {
                    .p = {tw, -1},
                    .n = {{tw, 0}, {tw - 1, 0}, {tw - 1, -1}},
                },

                // Top left
                {
                    .p = {-1, th},
                    .n = {{0, th}, {0, th - 1}, {-1, th - 1}},
                },

                // Top right
                {
                    .p = {tw, th},
                    .n = {{tw - 1, th}, {tw - 1, th - 1}, {tw, th - 1}},
                }
            }
        };

        for (auto& corner: corners) {
            std::array<uint32_t, 4> avrRgba{};

            // Each neighbor pixel
            for (auto& n: corner.n) {
                // Each color component
                auto pn = n + rect.position();
                auto s = image.pixelPosData(pn);
                for (auto& avr: avrRgba)
                    avr += *s++;
            }

            // Target pixel
            auto p = image.pixelPosData(corner.p + rect.position());

            // Each color component
            for (auto& avr: avrRgba)
                *p++ = static_cast<uint8_t>(avr / 3);
            // *p++ = 0xff;
            // *p++ = 0x00;
            // *p++ = 0x0;
            // *p++ = 0xff;

        }
#endif
    }
}
