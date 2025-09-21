/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "flat_object.h"

#include "generated/flat_object_pack_gen.hpp"
#include "generated/flat_object_pack_gen_json.hpp"
#include "generated/flat_object_pack_gen_yaml.hpp"

#include "../shared/packet_test.hpp"


#include <apeal/file.hpp>

#include <limits>

#include <iostream>
#include <stdexcept>



int main(int argc, char* argv[])
{
    int retCode = 1;
    

    try {
        //{
        //    FlatObject obj{};
        //    obj.intVal = 1;
        //    testObject(obj);
        //}

        //{
        //    FlatObject obj{};
        //    obj.charVal = 'A';
        //    obj.intVal = std::numeric_limits<int>::max();
        //    testObject(obj);
        //}

        {
            FlatObject obj{};
            obj.charVal = 'a';
            obj.ucharVal = 'A';
            setLowestValue(&obj.intVal);
            setLowestValue(&obj.uintVal);
            obj.stringVal = "Sample String";
            obj.vectorVal = {"aaa", "bbb", "ccc"};

            obj.mapVal.insert({"key1", 1.5});
            obj.mapVal.insert({"key2", 101.5});

            obj.unorderedMapVal.insert({"unordered_key1", "1.5"});
            obj.unorderedMapVal.insert({"unordered_key2", "101.5"});

            obj.setVal.insert(10);
            obj.setVal.insert(20);

            obj.charEnum = SampleCharEnum::ValueA;
            obj.intEnum = SampleIntEnum::Value1;

            packetTest(obj, "flat-3");
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
