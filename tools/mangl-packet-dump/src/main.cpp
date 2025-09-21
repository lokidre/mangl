/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "packet_dump.h"

#include <memory>
#include <iostream>

int main(int argc, char* argv[])
{
    int retCode = 0;

    try {
        if (argc <= 1)
            throw std::runtime_error("Packet file missing");

        
        auto packetFile = argv[1];

        packetDump(packetFile);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        retCode = 1;
    }
    catch (...) {
        std::cout << "exception!" << std::endl;
        retCode = 1;
    }

    return retCode;
}
