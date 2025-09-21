/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl/packet.h>

#include "nested_object.h"

#include "generated/nested_object_pack_gen.hpp"
#include "generated/nested_object_pack_gen_yaml.hpp"
#include "generated/nested_object_pack_gen_json.hpp"

#include "../shared/packet_test.hpp"


#include <apeal/file.hpp>

#include <limits>

#include <iostream>
#include <stdexcept>

using namespace mangl;




int main(int argc, char* argv[])
{
    int retCode = 1;

    try {
        {
            NestedObject obj{};

            obj.charVal = 'b';
            obj.ucharVal = 'A';
            obj.intVal = -11;
            obj.uintVal = 11;

            obj.subVal.vectorVal = {"aaa", "bbb"};

            obj.subVal.subSubVal.mapVal = {
                {"key1", "val1"},
                {"key2", "val2"},
            };
            packetTest(obj, "nested-1");
        }


        retCode = 0;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Exception!" << std::endl;
    }

    return retCode;
}
