/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "main_types.hpp"

namespace toolkit {

struct RotationAngle {
    Cord theta; // Rotation around Y axis (horizontal) (pan)
    Cord rho;   // Rotation around X axis (vertical) (tilt)


    // Math
    constexpr static auto pi_ = cognit::constants::pi<Cord>;
    constexpr static auto pi2_ = pi_ * 2;


    void normalize() noexcept {
        // normalize
        while (rho >= pi2_)
            rho -= pi2_;
        while (rho < -pi2_)
            rho += pi2_;

        while (theta >= pi2_)
            theta -= pi2_;
        while (theta < -pi2_)
            theta += pi2_;
    }

    void rotateMatrix(MatrixHashedGL* m) const {
        using namespace cognit;

        MatrixGL xM, yM, zM;
        xM.rotationX(-rho);
        yM.rotationY(-theta);

        mtx_mul(yM, *m, zM);
        mtx_mul(xM, zM, *m);

        m->updateHash();
    }

    [[nodiscard]] auto rotationMatrix() const {
        MatrixHashedGL m;
        m.eye();
        rotateMatrix(&m);
        return m;
    }

    auto& operator+=(const RotationAngle& o) {
        theta += o.theta;
        rho += o.rho;
        return *this;
    }
};

}  // namespace toolkit
