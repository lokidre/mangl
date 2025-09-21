/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "font_renderer.h"

#include <memory>
#include <iostream>

int main(int argc, char* argv[])
{
    int retCode = 1;

    try {
        auto renderer = std::make_unique<FontRenderer>();

        if (auto ret = renderer->params.parseCmdLine(argc, argv); ret != 0)
            return ret;

        renderer->process();

        retCode = 0;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "exception!" << std::endl;
    }

    return retCode;
}
