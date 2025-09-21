/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
/*
    Mangl Config Processor
*/

#include <iostream>
#include "texmake.h"

int main(int argc, char* argv[]) {
    //using namespace std;
    using std::cerr, std::endl;

    int retCode = 1;

    try {
        auto texMake = std::make_unique<TexMake>();

        if (auto ret = texMake->params.parseCmdLine(argc, argv))
            return ret;

        retCode = texMake->process();
    } catch (const exception& e) {
        cerr << e.what() << endl;
    } catch (...) {
        cerr << "Unhandled Exception!" << endl;
    }

    return retCode;
}
