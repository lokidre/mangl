/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl/packet.h>

#include "pod_object.h"

#include "generated/pod_object_pack_gen_packet.hpp"
#include "generated/pod_object_pack_gen_json.hpp"
#include "generated/pod_object_pack_gen_yaml.hpp"

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
            PodObject obj{};
            obj.intVal = 1;
            packetTest(obj, "pod-1");
        }

        {
            PodObject obj{};
            obj.charVal = 'A';
            obj.ucharVal = 'B';
            setMaxValue(&obj.intVal);
            packetTest(obj, "pod-2");
        }

        {
            PodObject obj{};
            obj.charVal = 'a';
            obj.ucharVal = 'b';

            setLowestValue(&obj.intVal);
            setMaxValue1(&obj.uintVal);
            setLowestValue(&obj.longVal);
            setMaxValue(&obj.ulongVal);

            packetTest(obj, "pod-3");
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
