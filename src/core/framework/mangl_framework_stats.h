/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_setup.h"

namespace mangl {

struct MFrameworkStats {

    struct DebugStatsBuffer {
        constexpr static int BufferSize = 10;
        int currentIndex{};
        double data[BufferSize]{};

        DebugStatsBuffer() {
            currentIndex = 0;
            for (double& i: data)
                i = 0;
        }
    };

    DebugStatsBuffer fpsBuffer_, loadBuffer_, runtimeLoadBuffer_;
    DebugStatsBuffer sceneLoadBuffer_, renderLoadBuffer_;

    [[nodiscard]] auto fps() const noexcept { return fps_; }
    [[nodiscard]] auto load() const noexcept { return load_; }
    [[nodiscard]] auto runtimeLoad() const noexcept { return runtimeLoad_; }
    [[nodiscard]] auto sceneLoad() const noexcept { return sceneLoad_; }
    [[nodiscard]] auto renderLoad() const noexcept { return renderLoad_; }


    double fps_{}, load_{};
    double runtimeLoad_{}, sceneLoad_{}, renderLoad_{};

    static double addData(DebugStatsBuffer& buffer, double data) noexcept {
        auto bufferSize = MFrameworkStats::DebugStatsBuffer::BufferSize;
        buffer.data[buffer.currentIndex++] = data;
        if (buffer.currentIndex >= bufferSize)
            buffer.currentIndex = 0;

        double sum = 0;
        auto d = buffer.data;
        for (int i = 0; i < bufferSize; ++i, ++d)
            sum += *d;

        return sum / static_cast<double>(bufferSize);
    }
};


} // namespace mangl
