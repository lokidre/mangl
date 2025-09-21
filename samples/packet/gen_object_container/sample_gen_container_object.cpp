/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl/packet.h>

#include "container_object.h"

#include "generated/container_object_pack_gen_packet.hpp"
#include "generated/container_object_pack_gen_yaml.hpp"
#include "generated/container_object_pack_gen_json.hpp"

#include "../shared/packet_test.hpp"


#include <iostream>
#include <stdexcept>



int main(int argc, char* argv[])
{
    using namespace mangl;
    using namespace container_test;


    int retCode = 1;

    
    try {
        ContainerObject obj{};

        /*
        {
            ContainerObject obj{};
            obj.charVal = 'b';
            obj.ucharVal = 'A';
            setLowestValue(&obj.intVal);
            setMaxValue(&obj.uintVal);

            obj.podVector.push_back({.intVal = 1, .doubleVal = 3.4});
            obj.podVector.push_back({.intVal = 100, .doubleVal = 300.4});

            packetTest(obj, "container-1");
        }
        */

        {
            obj.charVal = 'b';
            obj.ucharVal = 'A';
            setLowestValue(&obj.intVal);
            setMaxValue(&obj.uintVal);

            obj.podVector.push_back({.intVal = 1, .doubleVal = 3.4});
            obj.podVector.push_back({.intVal = 100, .doubleVal = 300.4});

            ContainerVectorValue vecVal;

            vecVal.podVecVal.push_back({5});
            vecVal.podVecVal.push_back({6});

            obj.vecVector.push_back(vecVal);

            obj.vecVector.push_back({.podVecVal = {10, 30, 40}});
            obj.vecVector.push_back({.podVecVal = {100, 300, 400}});

            obj.podArray[0] = {.intVal = 40, .doubleVal = 63.4};


            packetTest(obj, "container-2");
        }


        // Testing maps
        {
            obj.charVal = 'c';
            obj.ucharVal = 'C';

            obj.mapIntInt.insert({1, 100});
            obj.mapIntInt.insert({2, 200});
            obj.mapIntInt.insert({3, 300});

            obj.mapIntStruct.insert({4, {.intVal = 40, .doubleVal = 63.4}});
            obj.mapIntStruct.insert({5, {.intVal = 50, .doubleVal = 73.4}});

            obj.mapStringStruct.insert({"4", {.intVal = 40, .doubleVal = 63.4}});
            obj.mapStringStruct.insert({"4", {.intVal = 440, .doubleVal = 663.4}});
            obj.mapStringStruct.insert({"5", {.intVal = 50, .doubleVal = 73.4}});

            obj.intArray[1] = 10;
            obj.intArray[2] = 20;
            obj.intArray[4] = 40;


            packetTest(obj, "container-3");
        }

        // Testing vector<vector<object>>
        {
            ContainerVectorObjectValue vecVal;

            vecVal.complexVecVal.push_back({.intVal = 30, .doubleVal = 3.4, .stringVal = "Hello World!"});
            obj.complexVector.push_back(vecVal);
            obj.complexVector.push_back(vecVal);

            packetTest(obj, "container-4");
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
