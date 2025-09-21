/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
//#include <test_app.h>

#include <cognit/mtx/mtx_format.hpp>
#include <mangl/mangl.h>
#include <apeal/console.hpp>

int main() {
    using namespace mangl;
    using namespace apl;
    using namespace cognit;

    Matrix m;
    m.eye();

    println("M: %.0t", m);
    println("M: %s", mtx_fmt(m));


    // TODO: matrix format isn't working
    m.rotationX(1.3);
    //apl::println("RotX: %7.4t", m);

    m.rotationY(2.3);
    //apl::println("RotY: %7.4t", m);

    //apl::println("T: %7.4t", m.transposed());

    cognit::Mtx3<double> m3;
    m3.eye();
    //apl::println("M3: %.0t", m3);
}
