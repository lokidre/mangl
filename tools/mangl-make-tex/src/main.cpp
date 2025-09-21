/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
/*
    Texture generation utility
*/

#include <iostream>
#include "texmake.h"

int main(int argc, char* argv[]) {
    int retCode = 1;

    try {
        auto texMake = std::make_unique<TexMake>();

        if (auto ret = texMake->params.parseCmdLine(argc, argv))
            return ret;

        retCode = texMake->process();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unhandled Exception!" << std::endl;
    }

    return retCode;
}
